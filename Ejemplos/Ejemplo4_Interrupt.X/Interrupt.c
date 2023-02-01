
#include "Interrupt.h"
#include <xc.h>
#include "Config.h"
#include <libpic30.h>
#include <math.h>

void Interrupt_Extern(Interrupt INTX,bool Flanco,uint8_t prioridad){

    switch(INTX){
        case Intx_0:
            IFS0bits.INT0IF =0;//COLOCAMOS EL FLAG A 0
            IEC0bits.INT0IE = 1;//HABILITAMOS LA INTERRUPCION EXTERNA INT0
            IPC0bits.INT0IP = prioridad;//LE ASIGNO PRIODIDAD
            INTCON2bits.INT0EP = Flanco;
            TRISB |= (1<<7);//DEFINIMOS COMO ENTRADA     
            break;
        /********************************************************/     
        case Intx_1:
            IFS1bits.INT1IF =0;//COLOCAMOS EL FLAG A 0
            IEC1bits.INT1IE = 1;//HABILITAMOS LA INTERRUPCION EXTERNA INT0
            IPC5bits.INT1IP = prioridad;//LE ASIGNO PRIODIDAD
            INTCON2bits.INT1EP = Flanco;
            TRISB |= (1<<15);//DEFINIMOS COMO ENTRADA    
            RPINR0bits.INT1R = 0XF;//AL PIN RB15 LO DEFINIMOS COMO INT1
            break;
        /********************************************************/           
        case Intx_2:
            IFS1bits.INT2IF =0;//COLOCAMOS EL FLAG A 0
            IEC1bits.INT2IE = 1;//HABILITAMOS LA INTERRUPCION EXTERNA INT0
            IPC7bits.INT2IP = prioridad;//LE ASIGNO PRIODIDAD
            INTCON2bits.INT2EP = Flanco;
            TRISB |= (1<<14);//DEFINIMOS COMO ENTRADA    
            RPINR1bits.INT2R = 0XE;//AL PIN RB14 LO DEFINIMOS COMO INT1
            
            break;
        /********************************************************/   
    
    }
}

volatile bool flag1=false,flag2=false,flag3=false;
uint8_t i,j,k;
void __attribute__ ((interrupt,no_auto_psv)) _INT0Interrupt(void){

    
//    for (i = 0; i < 7; i++) {
//        LATC = 0x0F;
//        __delay_ms(200);
//        LATC = 0xF0;
//        __delay_ms(200);
//    }
//    LATC = 0X00;
flag1 =true;
    
  IFS0bits.INT0IF =0;//COLOCAMOS EL FLAG A 0
}

void __attribute__ ((interrupt,no_auto_psv)) _INT1Interrupt(void){

//        for (j = 0; j <=8; j++) {
//        LATC = pow(2,j)-1;
//        __delay_ms(100);
//        }
//        for (j = 7; j>0; j--) {
//        LATC = pow(2,j)-1;
//        __delay_ms(100);
//        }
//        LATC = 0X00;

flag2 =true;
  IFS1bits.INT1IF =0;//COLOCAMOS EL FLAG A 0
}

void __attribute__ ((interrupt,no_auto_psv)) _INT2Interrupt(void){

//    for (k = 0; k < 7; k++) {
//        LATC = 0b10101010;
//        __delay_ms(200);
//        LATC = 0b01010101;
//        __delay_ms(200);
//    }
//    LATC = 0X00;
//    
    flag3 =true;
  IFS1bits.INT2IF =0;//COLOCAMOS EL FLAG A 0
}











