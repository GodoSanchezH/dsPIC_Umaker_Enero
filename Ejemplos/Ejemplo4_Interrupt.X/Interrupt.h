/* 
 * File:   Interrupt.h
 * Author: HP
 *
 * Created on 31 de enero de 2023, 08:45 PM
 */

#ifndef INTERRUPT_H
#define	INTERRUPT_H


#include <stdint.h>
#include <stdbool.h>

#define Subida 0
#define Bajada 1


typedef enum{

    Intx_0=0,
            Intx_1,
            Intx_2

}Interrupt;

void Interrupt_Extern(Interrupt INTX,bool Flanco,uint8_t prioridad);

#endif	/* INTERRUPT_H */

