#include <xc.h>
#include "Config.h"
#include <libpic30.h>
#include "ADC.h"



void ADC_Init(Resolucion adc){
    
    //Definimos los pines como analogicos
    AD1PCFGL &= ~((1<<0) |(1<<1) |(1<<2)|(1<<3)|(1<<4)|(1<<5));
  
    //Definimos como entrada
    TRISA |= (1<<0) |(1<<1);//AN0 AN1
    TRISB |= (1<<0) |(1<<1) |(1<<2)|(1<<3);//AN2 AN3 AN4 AN5
    
    /*Configuracion ADC*/
    switch(adc){
    
        case bits_10:
            AD1CON1bits.ADON = 0;//apagamos el adc
            AD1CON1bits.AD12B = 0;//modo 10 bits
            AD1CON1bits.FORM = 0b00;//unsigned integer
            AD1CON1bits.SSRC = 0b000;//trigger por software
            AD1CON1bits.SAMP = 0;//habilitador de conversion
            AD1CON2bits.VCFG = 0b000;//AVDD AVSS
            AD1CON2bits.CHPS = 0b00;// CH0
            AD1CON3bits.ADRC = 0;//FCY 0.2uS
            AD1CON3bits.ADCS = 0b100;//TCY*5 = 1uS
            AD1CON1bits.ADON = 1;//Encendemos el adc
            AD1CHS0bits.CH0SA =0;
            break;
            
        case bits12:
            AD1CON1bits.ADON = 0;//apagamos el adc
            AD1CON1bits.AD12B = 1;//modo 1 bits
            AD1CON1bits.FORM = 0b00;//unsigned integer
            AD1CON1bits.SSRC = 0b010;//trigger por software
            AD1CON1bits.SAMP = 0;//habilitador de conversion
            AD1CON2bits.VCFG = 0b000;//AVDD AVSS
            AD1CON2bits.CHPS = 0b00;// CH0
            AD1CON3bits.ADRC = 0;//FCY 0.2uS
            AD1CON3bits.ADCS = 0b100;//TCY*5 = 1uS
            AD1CON1bits.ADON = 1;//Encendemos el adc
            AD1CHS0bits.CH0SA =0;
            break;
    

    }
   
    
}




uint16_t AnalogRead(Canal Pin){

AD1CHS0bits.CH0SA = (uint8_t)Pin;
//AD1CON1bits.SAMP = 1;
//__delay_ms(1);
//AD1CON1bits.SAMP = 0;

while(!AD1CON1bits.DONE);//esperamos que acabe la conversion

return ADC1BUF0;

}