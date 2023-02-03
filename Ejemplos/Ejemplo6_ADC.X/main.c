/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on 2 de febrero de 2023, 08:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Config.h"
#include "LCD_dsPIC.h"
#include "ADC.h"
#include <libpic30.h>
/*
 * 
 */
uint16_t AdcRead[6];
char str_Convert[10];
int main(int argc, char** argv) {

    LCD_dsPIC_Init();
    ADC_Init(bits12);
    
    LCD_dsPIC_SetCursor(0,Fila1);
    LCD_dsPIC_Print_String("ADC0");
    LCD_dsPIC_SetCursor(0,Fila2);
    LCD_dsPIC_Print_String("ADC1");
    LCD_dsPIC_SetCursor(0,Fila3);
    LCD_dsPIC_Print_String("ADC2");
    LCD_dsPIC_SetCursor(0,Fila4);
    LCD_dsPIC_Print_String("ADC3");
    LCD_dsPIC_SetCursor(10,Fila1);
    LCD_dsPIC_Print_String("ADC4");
    LCD_dsPIC_SetCursor(10,Fila2);
    LCD_dsPIC_Print_String("ADC5");
    for(;;){
    
        /**capturas*/
        AdcRead[0] = AnalogRead(An0);
        AdcRead[1] = AnalogRead(An1);
        AdcRead[2] = AnalogRead(An2);
        AdcRead[3] = AnalogRead(An3);
        AdcRead[4] = AnalogRead(An4);
        AdcRead[5] = AnalogRead(An5);
        
        sprintf(str_Convert,"%04u",AdcRead[0]);
        LCD_dsPIC_SetCursor(5,Fila1);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
        
        sprintf(str_Convert,"%04u",AdcRead[1]);
        LCD_dsPIC_SetCursor(5,Fila2);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
        
        
        sprintf(str_Convert,"%04u",AdcRead[2]);
        LCD_dsPIC_SetCursor(5,Fila3);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
        
        sprintf(str_Convert,"%04u",AdcRead[3]);
        LCD_dsPIC_SetCursor(5,Fila4);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
        
        sprintf(str_Convert,"%04u",AdcRead[4]);
        LCD_dsPIC_SetCursor(15,Fila1);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
        
        
        sprintf(str_Convert,"%04u",AdcRead[5]);
        LCD_dsPIC_SetCursor(15,Fila2);
        LCD_dsPIC_Print_String(str_Convert);
        memset(str_Convert,0,strlen(str_Convert));
    
    }
    
    return (EXIT_SUCCESS);
}

