/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 23 de febrero de 2023, 10:45 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include <libpic30.h>
#include "I2C.h" 
#include <xc.h>
/*
 * 
 */
uint8_t i= 0;
int main(int argc, char** argv) {

    
    I2C_Init(100);
    

    for (;;) {
        
        I2C_Start();
        I2C_Write_Address_Slave((0x27)<<1,I2C_Write);
        I2C_Write_Data_Slave(i);
        I2C_Stop();
        
        i++;
        if (i == 256)i=0;
        __delay_ms(200);

    }

    
    
    return (EXIT_SUCCESS);
}

