/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 26 de enero de 2023, 10:50 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Config.h"
#include <libpic30.h>
#include <xc.h>


/*
 * 
 */
uint8_t Display_7SegA[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uint8_t i;
uint8_t unidades =0, decenas=0 , cuenta=0;
void GPIO_INIT(void);
void Ejemplo1(void);
int main(int argc, char** argv) {

    
    GPIO_INIT();
   
    for (;;) {
        
        unidades = cuenta%10;
        decenas = cuenta/10;

        for (i = 0; i < 255; i++) {
         
            LATB = Display_7SegA[unidades];
            LATCbits.LATC1 =0;
            __delay_ms(1);
            LATCbits.LATC1 =1;
            
            LATB = Display_7SegA[decenas];
            LATCbits.LATC0 =0;
            __delay_ms(1);
            LATCbits.LATC0 =1;
        }
        cuenta++;

    }

    return (EXIT_SUCCESS);
}

void GPIO_INIT(void){

    AD1PCFGL = 0xFF;//PINES ANALOGICOS COMO DIGITALES
    TRISC = 0X0000;
    TRISB = 0X0000;
    
}
void Ejemplo1(void){
for (i = 0; i < 10; i++) {
            LATC= Display_7SegA[i];
            LATB= Display_7SegA[i];
            __delay_ms(400);
        }

}
