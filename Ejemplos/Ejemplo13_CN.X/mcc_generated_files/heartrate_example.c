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

#include <stdio.h>
#include <stdint.h>
#include "heartrate.h"

/**
  Section: Macro Declarations
 */

#define DC_REMOVER_ALPHA                            0.95
#define BEATDETECTOR_INIT_HOLDOFF                   2000    // in ms, how long to wait before counting
#define BEATDETECTOR_MASKING_HOLDOFF                200     // in ms, non-retriggerable window after beat detection
#define BEATDETECTOR_BPFILTER_ALPHA                 0.6     // EMA factor for the beat period value
#define BEATDETECTOR_MIN_THRESHOLD                  20      // minimum threshold (filtered) value
#define BEATDETECTOR_MAX_THRESHOLD                  800     // maximum threshold (filtered) value
#define BEATDETECTOR_STEP_RESILIENCY                30      // maximum negative jump that triggers the beat edge
#define BEATDETECTOR_THRESHOLD_FALLOFF_TARGET       0.3     // thr chasing factor of the max value when beat
#define BEATDETECTOR_THRESHOLD_DECAY_FACTOR         0.99    // thr chasing factor when no beat
#define BEATDETECTOR_INVALID_READOUT_DELAY          2000    // in ms, no-beat time to cause a reset
#define BEATDETECTOR_SAMPLES_PERIOD                 10      // in ms, 1/Fs
#define FALSE                                       0
#define TRUE                                        1
#define REPORTING_PERIOD_MS                         1000

/**
  Section: Variable Definitions
 */

typedef struct {
    float v_ctr[2];
    float dcW;
} filter_t;

typedef enum {
    BEATDETECTOR_STATE_INIT,
    BEATDETECTOR_STATE_WAITING,
    BEATDETECTOR_STATE_FOLLOWING_SLOPE,
    BEATDETECTOR_STATE_MAYBE_DETECTED,
    BEATDETECTOR_STATE_MASKING
} BeatMonitorState_t;

static uint32_t timer_ms;
static uint16_t sampRate;

static BeatMonitorState_t state;
static float bpmRate;
static uint16_t irData;
static filter_t irFilters;
static float threshold;
static float beatPeriod;
static float lastMaxValue;
static uint32_t tsLastBeat;
static uint32_t tsLastSample;
static uint32_t tsLastReport;

/**
  Section: Private function prototypes
 */

static void TMR0MS_ISR(void);
static uint32_t checkTimeDiff(uint32_t time);
static uint16_t getSampRate(void);
static void checkSample(void);
static float dcRemoval(float x);
static float filterBuLp1(uint16_t x);
static bool checkForBeat(float value);
static void decreaseThreshold(void);
static float getBPMRate(void);

/**
  Section: Example Code
 */

void HeartRate_example(void) 
{
    irFilters.dcW = 0;
    irFilters.v_ctr[0] = 0;
    irFilters.v_ctr[1] = 0;
    tsLastSample = 0;
    irData = 0;
    state = BEATDETECTOR_STATE_INIT;
    threshold = BEATDETECTOR_MIN_THRESHOLD;
    beatPeriod = 0;
    lastMaxValue = 0;
    tsLastBeat = 0;
    tsLastReport = 0;
    bpmRate = 0;

    sampRate = getSampRate();
    
    while (1) {
        TMR0MS_ISR();
        checkSample();
        if (checkTimeDiff(tsLastReport) > REPORTING_PERIOD_MS) {
            bpmRate = getBPMRate();
            tsLastReport = timer_ms;
        }
    }
}

// TMR0 Interrupt Handler (millisecond counter)
static void TMR0MS_ISR(void) 
{
    if (timer_ms < 0x7FFFFFFF) 
	{
        timer_ms++;
    } 
	else 
	{
        timer_ms = 0;
    }
}

// Period-checker bet. time-stamps
static uint32_t checkTimeDiff(uint32_t time) 
{
    if (time < timer_ms) 
	{
        return (timer_ms - time);
    } 
	else 
	{
        return ((0x7FFFFFFF - time) + timer_ms);
    }
}

// Sample Rate from HeartRate Click Set-up
static uint16_t getSampRate(void) 
{
    uint16_t sRate;
    switch (DEFAULT_SAMP_RATE) 
	{
        case MAX30100_SR50:
            sRate = 50;
            break;
        case MAX30100_SR100:
            sRate = 100;
            break;
        case MAX30100_SR167:
            sRate = 167;
            break;
        case MAX30100_SR200:
            sRate = 200;
            break;
        case MAX30100_SR400:
            sRate = 400;
            break;
        case MAX30100_SR600:
            sRate = 600;
            break;
        case MAX30100_SR800:
            sRate = 800;
            break;
        case MAX30100_SR1000:
            sRate = 1000;
            break;
        default:
            sRate = 100;
            break;
    }
    return sRate;
}

// HELPER FUNCTIONS (https://github.com/oxullo/Arduino-MAX30100)

static void checkSample(void) 
{
    if (checkTimeDiff(tsLastSample) > 1.0 / sampRate * 1000.0) 
	{
        tsLastSample = timer_ms;

        HeartRate_readSensors();
        irData = HeartRate_getIRdata();
        
        float irACValue = dcRemoval(irData);

        // The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
        float filteredPulseValue = filterBuLp1((uint16_t) -irACValue);

        bool beatDetected = checkForBeat(filteredPulseValue);

        if (beatDetected) 
		{
            bpmRate = getBPMRate();
            printf("HR:%u bpm\t", (uint16_t) bpmRate);
        } 
        printf("EKG:%i\r\n", (int16_t) filteredPulseValue);
    }
}

// FILTERS

// http://sam-koblenski.blogspot.co.uk/2015/11/everyday-dsp-for-programmers-dc-and.html
static float dcRemoval(float x) 
{
    float olddcW = irFilters.dcW;
    irFilters.dcW = (float) x + (DC_REMOVER_ALPHA * irFilters.dcW);

    return (irFilters.dcW - olddcW);
}

// http://www.schwietering.com/jayduino/filtuino/
static float filterBuLp1(uint16_t x) 
{
    irFilters.v_ctr[0] = irFilters.v_ctr[1];

    // FS = 100Hz, Fc = 10Hz, 1st order
    irFilters.v_ctr[1] = (2.452372752527856026e-1 * x) + (0.50952544949442879485 * irFilters.v_ctr[0]);
    return (irFilters.v_ctr[0] + irFilters.v_ctr[1]);
}

// PULSE CHECKER (https://github.com/oxullo/Arduino-MAX30100)

static bool checkForBeat(float sample) 
{
    static bool beatDetected = FALSE;

    switch (state) 
	{
        case BEATDETECTOR_STATE_INIT:
            if (timer_ms > BEATDETECTOR_INIT_HOLDOFF) 
			{
                state = BEATDETECTOR_STATE_WAITING;
            }
            break;

        case BEATDETECTOR_STATE_WAITING:
            if (sample > threshold) 
			{
                threshold = sample < BEATDETECTOR_MAX_THRESHOLD ? sample : BEATDETECTOR_MAX_THRESHOLD;
                state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
            }

            // Tracking lost, resetting
            if (checkTimeDiff(tsLastBeat) > BEATDETECTOR_INVALID_READOUT_DELAY) 
			{
                beatPeriod = 0;
                lastMaxValue = 0;
            }

            decreaseThreshold();
            break;

        case BEATDETECTOR_STATE_FOLLOWING_SLOPE:
            if (sample < threshold) 
			{
                state = BEATDETECTOR_STATE_MAYBE_DETECTED;
            } 
			else 
			{
                threshold = sample < BEATDETECTOR_MAX_THRESHOLD ? sample : BEATDETECTOR_MAX_THRESHOLD;
            }
            break;

        case BEATDETECTOR_STATE_MAYBE_DETECTED:
            if ((sample + BEATDETECTOR_STEP_RESILIENCY) < threshold) 
			{
                // Found a beat
                beatDetected = TRUE;
                lastMaxValue = sample;
                state = BEATDETECTOR_STATE_MASKING;
                float delta = checkTimeDiff(tsLastBeat);
                if (delta) 
				{
                    beatPeriod = BEATDETECTOR_BPFILTER_ALPHA * delta +
                            (1 - BEATDETECTOR_BPFILTER_ALPHA) * beatPeriod;
                }
                tsLastBeat = timer_ms;
            } 
			else 
			{
                state = BEATDETECTOR_STATE_FOLLOWING_SLOPE;
            }
            break;

        case BEATDETECTOR_STATE_MASKING:
            if (checkTimeDiff(tsLastBeat) > BEATDETECTOR_MASKING_HOLDOFF) 
			{
                state = BEATDETECTOR_STATE_WAITING;
            }
            decreaseThreshold();
            break;
    }

    return beatDetected;
}

static void decreaseThreshold(void) 
{
    // When a valid beat rate readout is present
    if (lastMaxValue > 0 && beatPeriod > 0) 
	{
        threshold -= lastMaxValue * (1 - BEATDETECTOR_THRESHOLD_FALLOFF_TARGET) /
                (beatPeriod / BEATDETECTOR_SAMPLES_PERIOD);
    } 
	else 
	{
        // Asymptotic decay
        threshold *= BEATDETECTOR_THRESHOLD_DECAY_FACTOR;
    }

    if (threshold < BEATDETECTOR_MIN_THRESHOLD) 
	{
        threshold = BEATDETECTOR_MIN_THRESHOLD;
    }
}

static float getBPMRate(void) 
{
    if (beatPeriod != 0) 
	{
        return ((1 / beatPeriod) * 1000 * 60);
    } 
	else 
	{
        return 0;
    }
}
