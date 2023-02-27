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
#include "LCD_dsPIC.h"


//void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt(void){
//    LATB ^= (1<<7);

 // IFS0bits.T1IF=0;
//}

int main(int argc, char** argv) {
    OSCTUNbits.TUN = 21;
    TRISBbits.TRISB7 = 0;
     LCD_dsPIC_Init();
    
    /*T1CONbits.TON=0;//Timer 1 off    
    T1CONbits.TCS=0;//FCY
    T1CONbits.TGATE =0;//gate off
    T1CONbits.TCKPS=0;//preescaler 1
    TMR1 =0;
    PR1 = 39999;//10 ms a 8MHZ
    IEC0bits.T1IE=1;
    IFS0bits.T1IF=0;
    IPC0bits.T1IP=7;
    T1CONbits.TON=1;//TimerOn**/
   
    LCD_dsPIC_New_Char(1, 0x0A,
                                0x1F,
                                0x1F,
                                0x1F,
                                0x0E,
                                0x04,
                                0x00,
                                0x00);
    LCD_dsPIC_SetCursor(0,Fila1);
    LCD_dsPIC_Print_String("dsPIC - PDS");
    LCD_dsPIC_Write_New_Char(1);
    LCD_dsPIC_SetCursor(0,Fila2);
    LCD_dsPIC_Print_String("Instr. Godo");
    
    uint8_t i;
    char str_i[10];
    for(;;){
    
        
        for (i = 0; i < 100; i++) {
            
            sprintf(str_i,"%02u",i);
            
            LCD_dsPIC_SetCursor(10,Fila2);
            LCD_dsPIC_Print_String(str_i);
            __delay_ms(500);
        }

    }
    
    return (EXIT_SUCCESS);
}

