#include "Config.h"
#include <xc.h>
#include "I2C.h"

void I2C_Init(uint16_t Fscl){
    float Delay = 120*0.000000001;//120ns
    /*configurar*/
    float registro = (((FCY/Fscl)/1000)-FCY*Delay )-2;
    I2C1BRG = (uint8_t) registro;
    I2C1CONbits.I2CEN = 1;// habilitamos el bus de i2c
}

void I2C_Start(void){

    I2C1CONbits.SEN=1;
    while(I2C1CONbits.SEN==1);
    
}

void I2C_Restart(void){
 I2C1CONbits.RSEN=1;
    while(I2C1CONbits.RSEN==1);
}

void I2C_Stop(void){
 I2C1CONbits.PEN=1;
    while(I2C1CONbits.PEN==1);
}

void I2C_Write_Address_Slave(uint8_t address, uint8_t data_dir){

    I2C1TRN = (address & 0b11111110) | (0b00000001 & data_dir);
    while(I2C1STATbits.TRSTAT);
}

void I2C_Write_Data_Slave(uint8_t data){
    I2C1TRN = data;
    while(I2C1STATbits.TRSTAT);
}

void I2C_Read_data(uint8_t *ptr_data,uint8_t var_ack_nack){
    I2C1CONbits.ACKEN = 0;
    I2C1CONbits.RCEN = 1;
    
    if (var_ack_nack == I2C_ACK) {

        while ( I2C1CONbits.RCEN == 1);
        *ptr_data = I2C1RCV;
        I2C1CONbits.ACKDT = 0;//prearamos el ack
        I2C1CONbits.ACKEN =1;//hacemos el envio
        while( I2C1CONbits.ACKEN ==1);
    }
    else if (var_ack_nack == I2C_nACK) {
        while ( I2C1CONbits.RCEN == 1);
        *ptr_data = I2C1RCV;
        I2C1CONbits.ACKDT = 1;//prearamos el nack
        I2C1CONbits.ACKEN =1;//hacemos el envio
        while( I2C1CONbits.ACKEN ==1);
    }


    
    
    
    
}

