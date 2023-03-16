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

#include "heartrate.h"
#include "max30100.h"

/**
  Section: Variable Definitions
 */

static uint8_t heartrate_initialized = 0;

/**
  Section: Private function prototypes
 */

static void HeartRate_initializeClick(void);

/**
  Section: Driver APIs
 */

void HeartRate_readSensors(void) 
{
    if (!heartrate_initialized) {
        HeartRate_initializeClick();
    }
    MAX30100_readSensors();
}

uint16_t HeartRate_getIRdata(void) 
{
    return MAX30100_getIRdata();
}

uint16_t HeartRate_getREDdata(void) 
{
    return MAX30100_getREDdata();
}

float HeartRate_getTemperature(void) 
{
    if (!heartrate_initialized) 
	{
        HeartRate_initializeClick();
    }

    MAX30100_readTemp();
    return MAX30100_getTemp();
}

static void HeartRate_initializeClick(void) 
{
    MAX30100_setSpo2RdyInterrupt(SPO2_INTERRUPT_EN);
    MAX30100_setHrRdyInterrupt(HR_INTERRUPT_EN);
    MAX30100_setTempRdyInterrupt(TEMP_INTERRUPT_EN);
    MAX30100_setFifoAfullInterrupt(FIFO_INTERRUPT_EN);

    MAX30100_setMode(DEFAULT_MODE);
    MAX30100_setHiResEnabled(DEFAULT_HIRES_SET);
    MAX30100_setSampleRate(DEFAULT_SAMP_RATE);
    MAX30100_setPulseWidth(DEFAULT_PWIDTH);
    MAX30100_setIRLEDCurrent(DEFAULT_IR_CURRENT);
    MAX30100_setREDLEDCurrent(DEFAULT_RED_CURRENT);

    MAX30100_initializeSensor();

    heartrate_initialized = 1;
}
