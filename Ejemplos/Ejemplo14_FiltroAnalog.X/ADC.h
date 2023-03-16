/* 
 * File:   ADC.h
 * Author: gssan
 *
 * Created on February 2, 2023, 8:45 PM
 */

#ifndef ADC_H
#define	ADC_H

#include <stdint.h>

typedef enum{
    bits_10=0,
            bits12
}Resolucion;

typedef enum{
    An0=0,
            An1,
            An2,
            An3,
            An4 ,         
            An5
}Canal;


void ADC_Init(Resolucion adc);
uint16_t AnalogRead(Canal Pin);


#endif	/* ADC_H */

