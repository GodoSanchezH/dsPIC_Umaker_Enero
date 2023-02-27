/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 23 de febrero de 2023, 08:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p33FJ32MC202.h>
#include "Config.h"
#include <libpic30.h>
#include "ADC.h"

void __attribute__ ((interrupt,no_auto_psv)) _OC1Interrupt(void){

   IFS0bits.OC1IF = 0;
}



void GPIO_Init(void);
void Timer_Init(void);
void Active_Low_One_Shot(void);
void Active_High_One_Shot(void);
void Toggle_mode(void);
void Delayed_One_Shot(void);
void Continuous_Pulse(void);
void PWM_mode(void);

float var;
uint16_t var1;
int main(int argc, char** argv) {

    GPIO_Init();
    ADC_Init(bits12);
    Timer_Init();
    PWM_mode();
    
    for(;;){
    
        var= AnalogRead(An0)*12.20703125;
        var1 = (uint16_t)var;
        OC1RS = var1;
        __delay_ms(10);
    }
    
    return (EXIT_SUCCESS);
}


void GPIO_Init(void){

    TRISB &= ~(1<<7);
    RPOR3bits.RP7R =  0b10010;//RB7

}
void Timer_Init(void){

    T2CONbits.TON=0;//Timer 1 off    
    T2CONbits.TCS=0;//FCY
    T2CONbits.TGATE =0;//gate off
    T2CONbits.TCKPS=0;//preescaler8
    
    
    
    
}

void Active_Low_One_Shot(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=25000;
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b001;//
    T2CONbits.TON=1;//TimerOn
}
void Active_High_One_Shot(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=25000;
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b010;//desabilitamos el modo OC1
    T2CONbits.TON=1;//TimerOn
}
void Toggle_mode(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=25000;
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b011;//togle mode
    T2CONbits.TON=1;//TimerOn
}
void Delayed_One_Shot(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=25000;//5ms
    OC1RS = 40000;//8ms
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b100;//togle mode
    T2CONbits.TON=1;//TimerOn

}
void Continuous_Pulse(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=25000;//5ms
    OC1RS = 40000;//8ms
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b101;//continus
    T2CONbits.TON=1;//TimerOn
}
void PWM_mode(void){
    OC1CONbits.OCM = 0b000;//desabilitamos el modo OC1
    OC1CONbits.OCTSEL = 0;//TIMER2
    TMR2 =0;
    PR2 =49999;
    OC1R=1;//5ms
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    IPC0bits.OC1IP = 7;
    OC1CONbits.OCM = 0b110;//pwm
    T2CONbits.TON=1;//TimerOn

}

