/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
 */

#include <math.h>
#include "max30100.h"
#include "delay.h"
#include "drivers/i2c_simple_master.h"

/**
  Section: Macro Declarations
 */

// MAX30100 Slave Address
#define MAX30100_ADDR   0x57

// Registers
#define MAX30100_INTERRUPT_STAT_REG     0x00
#define MAX30100_INTERRUPT_EN_REG       0x01
#define MAX30100_FIFO_WR_PTR_REG        0x02
#define MAX30100_OVF_CTR_REG            0x03
#define MAX30100_FIFO_RD_PTR_REG        0x04
#define MAX30100_FIFO_DATA_REG          0x05
#define MAX30100_MODE_CONFIG_REG        0x06
#define MAX30100_SPO2_CONFIG_REG        0x07
#define MAX30100_LED_CONFIG_REG         0x09
#define MAX30100_TEMP_INT_REG           0x16
#define MAX30100_TEMP_FRAC_REG          0x17
#define MAX30100_REV_ID_REG             0xFE
#define MAX30100_PART_ID_REG            0xFF

/**
  Section: Variable Definitions
 */

typedef union {

    struct {
        uint8_t pwr_rdy : 1;
        uint8_t : 3;
        uint8_t spo2_rdy : 1;
        uint8_t hr_rdy : 1;
        uint8_t temp_rdy : 1;
        uint8_t fifo_afull : 1;
    };
    uint8_t interruptStat;
} hr_interruptStatbits_t;
hr_interruptStatbits_t interrupt_stat_bits;

typedef union {

    struct {
        uint8_t : 4;
        uint8_t en_spo2_rdy : 1;
        uint8_t en_hr_rdy : 1;
        uint8_t en_temp_rdy : 1;
        uint8_t en_fifo_afull : 1;
    };
    uint8_t interruptEn;
} hr_interruptEnbits_t;
hr_interruptEnbits_t interrupt_en_bits;

typedef union {

    struct {
        uint8_t mode : 3;
        uint8_t temp_en : 1;
        uint8_t : 2;
        uint8_t reset : 1;
        uint8_t shdn : 1;
    };
    uint8_t modeConfig;
} hr_modeConfigbits_t;
hr_modeConfigbits_t mode_config_bits;

typedef union {

    struct {
        uint8_t led_pw : 2;
        uint8_t spo2_sr : 3;
        uint8_t : 1;
        uint8_t spo2_hires_en : 1;
        uint8_t : 1;
    };
    uint8_t spo2Config;
} hr_spo2Configbits_t;
hr_spo2Configbits_t spo2_config_bits;

typedef union {

    struct {
        uint8_t ir_pa : 4;
        uint8_t red_pa : 4;
    };
    uint8_t ledConfig;
} hr_ledConfigbits_t;
hr_ledConfigbits_t led_config_bits;

static uint8_t fifo_wr_ptr;
static uint8_t fifo_rd_ptr;
static uint8_t ovf_ctr;
static uint16_t ir_data;
static uint16_t red_data;
static int8_t temp_int;
static float temp_frac;

static uint8_t fifo_buff[16];
static uint8_t temp_buff[2];

/**
  Section: Private function prototypes
 */

// Internal Functions
static void MAX30100_startTemp(void);
static uint16_t Shift_Bits(uint16_t read_res);

// I2C Functions
static void MAX30100_writeByte(uint8_t reg_addr, uint8_t data);
static void MAX30100_writeBlock(uint8_t *write_buff, uint8_t length);
static uint8_t MAX30100_readByte(uint8_t reg_addr);
static void MAX30100_readBlock(uint8_t reg_addr, uint8_t *read_buff, uint8_t length);

/**
  Section: Driver APIs
 */

/* Get Measurements */

void MAX30100_readSensors(void) 
{
    MAX30100_clearCounters();

    if (mode_config_bits.mode == MAX30100_HR_ONLY) 
	{
        while (!MAX30100_isHrRdy())
		{
		}
    } 
	else if (mode_config_bits.mode == MAX30100_SPO2_EN) 
	{
        while (!MAX30100_isSpo2Rdy())
		{
		}
    }

    MAX30100_readFifoData();
}

void MAX30100_readTemp(void) 
{
    MAX30100_startTemp();
    DELAY_milliseconds(29);

    while (!MAX30100_isTempRdy())
	{
	}
    MAX30100_readBlock(MAX30100_TEMP_INT_REG, temp_buff, 2);

    temp_int = temp_buff[0];
    temp_frac = temp_buff[1]*0.0625;
}

uint16_t MAX30100_getIRdata(void) 
{
    return ir_data;
}

uint16_t MAX30100_getREDdata(void) 
{
    return red_data;
}

float MAX30100_getTemp(void) 
{
    return (temp_int + temp_frac);
}

/* Setup the Sensor */

void MAX30100_setMode(uint8_t mode) 
{
    switch (mode) {
        case MAX30100_HR_ONLY:
            mode_config_bits.mode = MAX30100_HR_ONLY;
            break;
        case MAX30100_SPO2_EN:
            mode_config_bits.mode = MAX30100_SPO2_EN;
            break;
        default: break;
    }
}

void MAX30100_setHiResEnabled(uint8_t hiResEnable) 
{
    spo2_config_bits.spo2_hires_en = hiResEnable;
}

void MAX30100_setSampleRate(uint8_t sampRate) 
{
    spo2_config_bits.spo2_sr = sampRate;
}

void MAX30100_setPulseWidth(uint8_t pWidth) 
{
    spo2_config_bits.led_pw = pWidth;
}

void MAX30100_setIRLEDCurrent(uint8_t irCurrent) 
{
    led_config_bits.ir_pa = irCurrent;
}

void MAX30100_setREDLEDCurrent(uint8_t redCurrent) 
{
    led_config_bits.red_pa = redCurrent;
}

/* Interrupts */

void MAX30100_setSpo2RdyInterrupt(uint8_t interruptEnabled) 
{
    interrupt_en_bits.en_spo2_rdy = interruptEnabled;
}

void MAX30100_setHrRdyInterrupt(uint8_t interruptEnabled) 
{
    interrupt_en_bits.en_hr_rdy = interruptEnabled;
}

void MAX30100_setTempRdyInterrupt(uint8_t interruptEnabled) 
{
    interrupt_en_bits.en_temp_rdy = interruptEnabled;
}

void MAX30100_setFifoAfullInterrupt(uint8_t interruptEnabled) 
{
    interrupt_en_bits.en_fifo_afull = interruptEnabled;
}

bool MAX30100_isPowerRdy(void) 
{
    interrupt_stat_bits.interruptStat = MAX30100_readByte(MAX30100_INTERRUPT_STAT_REG);
    return interrupt_stat_bits.pwr_rdy;
}

bool MAX30100_isSpo2Rdy(void) 
{
    interrupt_stat_bits.interruptStat = MAX30100_readByte(MAX30100_INTERRUPT_STAT_REG);
    return interrupt_stat_bits.spo2_rdy;
}

bool MAX30100_isHrRdy(void) 
{
    interrupt_stat_bits.interruptStat = MAX30100_readByte(MAX30100_INTERRUPT_STAT_REG);
    return interrupt_stat_bits.hr_rdy;
}

bool MAX30100_isTempRdy(void) 
{
    interrupt_stat_bits.temp_rdy = MAX30100_readByte(MAX30100_INTERRUPT_STAT_REG);
    return interrupt_stat_bits.temp_rdy;
}

bool MAX30100_isFifoAfull(void) 
{
    interrupt_stat_bits.interruptStat = MAX30100_readByte(MAX30100_INTERRUPT_STAT_REG);
    return interrupt_stat_bits.fifo_afull;
}

/* Initialize MAX30100 */

void MAX30100_initializeSensor(void) 
{
    uint8_t I2CM_dataBuffer[5];

    MAX30100_reset();

    // Initialize Settings
    interrupt_stat_bits.interruptStat = 0x00;
    mode_config_bits.shdn = DISABLED;
    mode_config_bits.reset = DISABLED;

    // Interrupts
    I2CM_dataBuffer[0] = MAX30100_INTERRUPT_STAT_REG;
    I2CM_dataBuffer[1] = interrupt_stat_bits.interruptStat;
    I2CM_dataBuffer[2] = interrupt_en_bits.interruptEn;
    MAX30100_writeBlock(I2CM_dataBuffer, 3);

    // Configurations
    I2CM_dataBuffer[0] = MAX30100_MODE_CONFIG_REG;
    I2CM_dataBuffer[1] = mode_config_bits.modeConfig;
    I2CM_dataBuffer[2] = spo2_config_bits.spo2Config;
    I2CM_dataBuffer[3] = RESERVED;
    I2CM_dataBuffer[4] = led_config_bits.ledConfig;
    MAX30100_writeBlock(I2CM_dataBuffer, 5);
}

/* Update Sensor Set-up */

void MAX30100_updateSensorConfig(void) 
{
    uint8_t I2CM_dataBuffer[3];

    I2CM_dataBuffer[0] = MAX30100_MODE_CONFIG_REG;
    I2CM_dataBuffer[1] = mode_config_bits.modeConfig;
    I2CM_dataBuffer[2] = spo2_config_bits.spo2Config;
    MAX30100_writeBlock(I2CM_dataBuffer, 3);
}

void MAX30100_updateLEDCurrent(void) 
{
    MAX30100_writeByte(MAX30100_LED_CONFIG_REG, led_config_bits.ledConfig);
}

void MAX30100_updateEnabledInterrupts(void) 
{
    MAX30100_writeByte(MAX30100_INTERRUPT_EN_REG, interrupt_en_bits.interruptEn);
}

/* FIFO Operations */

uint8_t MAX30100_getSampleCount(void) 
{
    fifo_wr_ptr = MAX30100_readByte(MAX30100_FIFO_WR_PTR_REG);
    fifo_rd_ptr = MAX30100_readByte(MAX30100_FIFO_RD_PTR_REG);
    return ((16 + fifo_wr_ptr - fifo_rd_ptr) % 16);
}

uint8_t MAX30100_getFifoOverflow(void) 
{
    return MAX30100_readByte(MAX30100_OVF_CTR_REG);
}

void MAX30100_clearCounters(void)
{
    uint8_t I2CM_dataBuffer[4];

    fifo_wr_ptr = 0x00;
    ovf_ctr = 0x00;
    fifo_rd_ptr = 0x00;

    I2CM_dataBuffer[0] = MAX30100_FIFO_WR_PTR_REG;
    I2CM_dataBuffer[1] = fifo_wr_ptr;
    I2CM_dataBuffer[2] = ovf_ctr;
    I2CM_dataBuffer[3] = fifo_rd_ptr;

    MAX30100_writeBlock(I2CM_dataBuffer, 4);
}

void MAX30100_readFifoData(void) 
{
    MAX30100_readBlock(MAX30100_FIFO_DATA_REG, fifo_buff, 4);

    ir_data = (fifo_buff[0] << 8) | fifo_buff[1];
    red_data = (fifo_buff[2] << 8) | fifo_buff[3];
    ir_data = Shift_Bits(ir_data);
    red_data = Shift_Bits(red_data);
}

/* Misc. Operations */

void MAX30100_reset(void) 
{
    mode_config_bits.reset = ENABLED;
    MAX30100_writeByte(MAX30100_MODE_CONFIG_REG, mode_config_bits.modeConfig);
}

void MAX30100_wakeup(void) 
{
    mode_config_bits.shdn = DISABLED;
    MAX30100_writeByte(MAX30100_MODE_CONFIG_REG, mode_config_bits.modeConfig);
}

void MAX30100_shutdown(void) 
{
    mode_config_bits.shdn = ENABLED;
    MAX30100_writeByte(MAX30100_MODE_CONFIG_REG, mode_config_bits.modeConfig);
}

uint8_t MAX30100_getRevID(void) 
{
    return MAX30100_readByte(MAX30100_REV_ID_REG);
}

uint8_t MAX30100_getPartID(void) 
{
    return MAX30100_readByte(MAX30100_PART_ID_REG);
}


static void MAX30100_startTemp(void) 
{
    mode_config_bits.temp_en = ENABLED;
    MAX30100_writeByte(MAX30100_MODE_CONFIG_REG, mode_config_bits.modeConfig);
}

static uint16_t Shift_Bits(uint16_t read_res) 
{
    uint16_t shift_res;
    switch (spo2_config_bits.led_pw) {
        case MAX30100_PW200: shift_res = read_res >> 3;
            break;
        case MAX30100_PW400: shift_res = read_res >> 2;
            break;
        case MAX30100_PW800: shift_res = read_res >> 1;
            break;
        default: shift_res = read_res;
            break;
    }
    return shift_res;
}

static void MAX30100_writeByte(uint8_t reg_addr, uint8_t data) 
{
    i2c_write1ByteRegister(MAX30100_ADDR, reg_addr, data);
}

static void MAX30100_writeBlock(uint8_t *write_buff, uint8_t length) 
{
    i2c_writeNBytes(MAX30100_ADDR, write_buff, length);
}

static uint8_t MAX30100_readByte(uint8_t reg_addr) 
{
    return i2c_read1ByteRegister(MAX30100_ADDR, reg_addr);
}

static void MAX30100_readBlock(uint8_t reg_addr, uint8_t *read_buff, uint8_t length) 
{
    i2c_readDataBlock(MAX30100_ADDR, reg_addr, read_buff, length);
}
