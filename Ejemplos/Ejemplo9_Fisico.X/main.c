/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 21 de febrero de 2023, 08:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include <libpic30.h>
#include <xc.h>



void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt(void){
    LATB ^= (1<<7);

  IFS0bits.T1IF=0;
}

int main(int argc, char** argv) {
    OSCTUNbits.TUN = 21;
    TRISBbits.TRISB7 = 0;
    
    
    T1CONbits.TON=0;//Timer 1 off    
    T1CONbits.TCS=0;//FCY
    T1CONbits.TGATE =0;//gate off
    T1CONbits.TCKPS=0;//preescaler 1
    TMR1 =0;
    PR1 = 39999;//10 ms a 8MHZ
    IEC0bits.T1IE=1;
    IFS0bits.T1IF=0;
    IPC0bits.T1IP=7;
    T1CONbits.TON=1;//TimerOn
    for(;;){
    
        //LATB ^= (1<<7);
        //__delay_ms(200);
    
    
    }
    
    return (EXIT_SUCCESS);
}

