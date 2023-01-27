/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 26 de enero de 2023, 10:27 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Config.h"
#include <libpic30.h>
#include <xc.h>
#include "LCD_dsPIC.h"
/*
 * 
 */
int main(int argc, char** argv) {

    LCD_dsPIC_Init();
    LCD_dsPIC_New_Char(1, 0x0A,
                                0x1F,
                                0x1F,
                                0x1F,
                                0x0E,
                                0x04,
                                0x00,
                                0x00);
    LCD_dsPIC_SetCursor(0,Fila1);
    LCD_dsPIC_Print_String("Bienvenidos a:");
    LCD_dsPIC_SetCursor(0,Fila2);
    LCD_dsPIC_Print_String("dsPIC :3");
    LCD_dsPIC_SetCursor(0,Fila3);
    LCD_dsPIC_Print_String("orientado a PDS");
    LCD_dsPIC_SetCursor(0,Fila4);
    LCD_dsPIC_Print_String("Instr. Godo");
    LCD_dsPIC_Write_New_Char(1);
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

