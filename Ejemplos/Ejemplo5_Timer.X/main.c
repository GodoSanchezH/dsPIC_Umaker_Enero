/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 31 de enero de 2023, 10:24 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Config.h"
#include <libpic30.h>
#include <xc.h>
#include "LCD_dsPIC.h"
volatile bool flag=false;
void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt(void){
    LATC ^= (1<<0);
    flag=true;
  IFS0bits.T1IF=0;
}
void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt(void){
    LATC ^= (1<<1);
  IFS0bits.T2IF=0;
}
void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void){
    LATC ^= (1<<2);
  IFS0bits.T3IF=0;
}
void Timer1_Temp(void);
void Timer2_Temp(void);
void Timer3_Temp(void);
void Timer3_contador(void);
uint16_t cuenta_T3;
char str_cuenta_T3[10];
int main(int argc, char** argv) {
    TRISCbits.TRISC0=0;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC2=0;
    LCD_dsPIC_Init();
    Timer1_Temp();
    Timer2_Temp();
    Timer3_contador();
    //Timer3_Temp();
       LCD_dsPIC_SetCursor(0,Fila1);
    LCD_dsPIC_Print_String("Timer Contador");
    
    
    for (;;) {
        
        if (flag) {
            cuenta_T3 = TMR3;
            sprintf(str_cuenta_T3,"%03u",cuenta_T3);
            LCD_dsPIC_SetCursor(16,Fila2);
            LCD_dsPIC_Print_String(str_cuenta_T3);
            
            flag = false;
        }

            
            
                    
            
    }

    return (EXIT_SUCCESS);
}
void Timer3_contador(void){
    T3CONbits.TON=0;//Timer 1 off    
    T3CONbits.TCS=1;//Pin externo
    TRISBbits.TRISB15=1;
    RPINR3bits.T3CKR=15;
    T3CONbits.TGATE =0;//gate off
    T3CONbits.TCKPS=0;//preescaler8
    TMR3 =0;
    PR3 = 8;
    IEC0bits.T3IE=1;
    IFS0bits.T3IF=0;
    IPC2bits.T3IP=5;
    T3CONbits.TON=1;//TimerOn
    
}
void Timer1_Temp(void){
    T1CONbits.TON=0;//Timer 1 off    
    T1CONbits.TCS=0;//FCY
    T1CONbits.TGATE =0;//gate off
    T1CONbits.TCKPS=1;//preescaler8
    TMR1 =0;
    PR1 = 31250;
    IEC0bits.T1IE=1;
    IFS0bits.T1IF=0;
    IPC0bits.T1IP=7;
    T1CONbits.TON=1;//TimerOn
            
}
void Timer2_Temp(void){
    T2CONbits.TON=0;//Timer 1 off    
    T2CONbits.TCS=0;//FCY
    T2CONbits.TGATE =0;//gate off
    T2CONbits.TCKPS=0;//preescaler8
    TMR2 =0;
    PR2 =49999;
    IEC0bits.T2IE=1;
    IFS0bits.T2IF=0;
    IPC1bits.T2IP=6;
    T2CONbits.TON=1;//TimerOn
}
void Timer3_Temp(void){
    T3CONbits.TON=0;//Timer 1 off    
    T3CONbits.TCS=0;//FCY
    T3CONbits.TGATE =0;//gate off
    T3CONbits.TCKPS=0;//preescaler8
    TMR3 =0;
    PR3 = 4999;
    IEC0bits.T3IE=1;
    IFS0bits.T3IF=0;
    IPC2bits.T3IP=5;
    T3CONbits.TON=1;//TimerOn
    
}