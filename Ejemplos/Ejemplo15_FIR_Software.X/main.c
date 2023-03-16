/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 9 de marzo de 2023, 08:47 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Config.h"
#include <libpic30.h>
#include "ADC.h"
#include "I2C.h"
#include <dsp.h>
#include <xc.h>





#define FIR 1

#ifdef PasaBajo
//
// 0.3859 z + 0.3859
//  -----------------
//     z - 0.2283
 

#define a0 0.3859
#define a1 0.3859
#define b0 0.2283

#elif FIR

//hacerlo con arreglo
#define a0 0
#define a1 0.005478675859511
#define a2 0.042763636162145
#define a3 0.121518892697392
#define a4 0.207579050378653
#define a5 0.245319489804598
#define a6 0.207579050378653
#define a7 0.121518892697392
#define a8 0.042763636162145
#define a9 0.005478675859511
#define a10 0

#endif

uint16_t Temp,ValH,ValL;
bool flag = false;



#define N_Coef 11

typedef struct{

   uint16_t n[N_Coef];
   uint16_t adc;
    
}Entradas;

typedef struct{
   float n[N_Coef];
   
}Salidas;

Entradas X;
Salidas Y;

void __attribute__ ((interrupt,no_auto_psv)) _T3Interrupt(void){
 X.adc = AnalogRead(An0);
 flag = true;
 IFS0bits.T3IF=0;
}

 int i;
void Ts(void);

int main(int argc, char** argv) {

    
   
    for (i = 0; i < N_Coef; i++) {
        X.n[i] = 0;
    }
    for (i = 0; i < N_Coef; i++) {
       Y.n[i] = 0;
    }
    
    ADC_Init(bits12);
    Ts();
    I2C_Init(100);
    
    for(;;){
    
        if (flag) {
#ifdef PasaBajo            
            X.n[1] =  X.n[0];
            X.n[0] = X.adc;
            
            Y.n[1] = Y.n[0];
            
            Y.n[0] = a0*X.n[0] + a1*X.n[1] + b0*Y.n[1];
            
            
#elif FIR
            
            X.n[10] = X.n[9];
            X.n[9] =  X.n[8];
            X.n[8] =  X.n[7];
            X.n[7] =  X.n[6];       
            X.n[6] =  X.n[5];
            X.n[5] =  X.n[4];        
            X.n[4] =  X.n[3];
            X.n[3] =  X.n[2];
            X.n[2] =  X.n[1];
            X.n[1] =  X.n[0];
            X.n[0] =  X.adc ;
            
            Y.n[0] = a0*X.n[0]+a1*X.n[1]+a2*X.n[2]+a3*X.n[3]+a4*X.n[4]+a5*X.n[5]+a6*X.n[6]+a7*X.n[7]+a8*X.n[8]+a9*X.n[9]+a10*X.n[10];
                    
#endif             
            Temp = (uint16_t) Y.n[0];
            I2C_Start();
            I2C_Write_Address_Slave(0X60<<1,I2C_Write);
            I2C_Write_Data_Slave((Temp>>8)& 0x0F);
            I2C_Write_Data_Slave(Temp & 0xFF);
            I2C_Stop();
            
            flag = false;
        }

        
    
    }
    
    
    return (EXIT_SUCCESS);
}

void Ts(void){
    T3CONbits.TON=0;//Timer 1 off    
    T3CONbits.TCS=0;//FCY
    T3CONbits.TGATE =0;//gate off
    T3CONbits.TCKPS=1;//preescaler8
    TMR3 =0;
    PR3 = 12500;
    IEC0bits.T3IE=1;
    IFS0bits.T3IF=0;
    IPC2bits.T3IP=5;
    T3CONbits.TON=1;//TimerOn

}