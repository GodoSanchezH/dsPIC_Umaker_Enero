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

#ifndef _MAX30100_H
#define _MAX30100_H

/**
  Section: Included Files
 */

#include <stdint.h>
#include <stdbool.h>

/**
  Section: Macro Declarations
 */

// Interrupt Flags
#define MAX30100_POWER_RDY       0x01
#define MAX30100_SPO2_RDY        0x10
#define MAX30100_HR_RDY          0x20
#define MAX30100_TEMP_RDY        0x40
#define MAX30100_FIFO_FULL       0x80

// Status
#define READY           0x01
#define N_READY         0x00
#define ENABLED         0x01
#define DISABLED        0x00
#define RESERVED        0x00

// Mode Select
#define MAX30100_HR_ONLY         0x02
#define MAX30100_SPO2_EN         0x03

// SPO2 Sample Rate (SPS)
#define MAX30100_SR50            0x00
#define MAX30100_SR100           0x01
#define MAX30100_SR167           0x02
#define MAX30100_SR200           0x03
#define MAX30100_SR400           0x04
#define MAX30100_SR600           0x05
#define MAX30100_SR800           0x06
#define MAX30100_SR1000          0x07

// LED Pulse Width
#define MAX30100_PW200           0x00
#define MAX30100_PW400           0x01
#define MAX30100_PW800           0x02
#define MAX30100_PW1600          0x03

// LED Current (mA)
#define MAX30100_I0              0x00 
#define MAX30100_I4              0x01
#define MAX30100_I7              0x02
#define MAX30100_I11             0x03
#define MAX30100_I14             0x04 
#define MAX30100_I17             0x05
#define MAX30100_I21             0x06 
#define MAX30100_I24             0x07
#define MAX30100_I27             0x08 
#define MAX30100_I31             0x09 
#define MAX30100_I34             0x0A 
#define MAX30100_I37             0x0B
#define MAX30100_I40             0x0C 
#define MAX30100_I44             0x0D 
#define MAX30100_I47             0x0E 
#define MAX30100_I50             0x0F

/**
  Section: MAX30100 Driver APIs
 */

/* Get Measurements */
void MAX30100_readSensors(void);
void MAX30100_readTemp(void);
uint16_t MAX30100_getIRdata(void);
uint16_t MAX30100_getREDdata(void);
float MAX30100_getTemp(void);

/* Setup the Sensor */
void MAX30100_setMode(uint8_t mode);
void MAX30100_setHiResEnabled(uint8_t hiResEnable);
void MAX30100_setSampleRate(uint8_t sampRate);
void MAX30100_setPulseWidth(uint8_t pWidth);
void MAX30100_setIRLEDCurrent(uint8_t irCurrent);
void MAX30100_setREDLEDCurrent(uint8_t redCurrent);
void MAX30100_updateLEDCurrent(void);
void MAX30100_updateSensorConfig(void);

/* Interrupts */
void MAX30100_setSpo2RdyInterrupt(uint8_t interruptEnabled);
void MAX30100_setHrRdyInterrupt(uint8_t interruptEnabled);
void MAX30100_setTempRdyInterrupt(uint8_t interruptEnabled);
void MAX30100_setFifoAfullInterrupt(uint8_t interruptEnabled);
bool MAX30100_isPowerRdy(void);
bool MAX30100_isSpo2Rdy(void);
bool MAX30100_isHrRdy(void);
bool MAX30100_isTempRdy(void);
bool MAX30100_isFifoAfull(void);

/* Initialize MAX30100 */
void MAX30100_initializeSensor(void);

/* Update Sensor Set-up */
void MAX30100_updateSensorConfig(void);
void MAX30100_updateLEDCurrent(void);
void MAX30100_updateEnabledInterrupts(void);

/* FIFO Operations */
void MAX30100_clearCounters(void);
uint8_t MAX30100_getFifoOverflow(void);
uint8_t MAX30100_getSampleCount(void);
void MAX30100_readFifoData(void);

/* Misc. Operations */
void MAX30100_reset(void);
void MAX30100_wakeup(void);
void MAX30100_shutdown(void);
uint8_t MAX30100_getRevID(void);
uint8_t MAX30100_getPartID(void);

#endif // _MAX30100_H
