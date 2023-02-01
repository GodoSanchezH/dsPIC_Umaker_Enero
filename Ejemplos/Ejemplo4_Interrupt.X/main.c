/* 
 * File:   main.c
 * Author: HP
 *
 * Created on 31 de enero de 2023, 08:41 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Config.h"
#include <libpic30.h>
#include <xc.h>
#include "Interrupt.h"
#include "LCD_dsPIC.h"
/*
 * 
 */

extern volatile bool flag1,flag2,flag3;
uint16_t personas,productos,accidentes;
char str_personas[10],str_productos[10],str_accidentes[10];
int main(int argc, char** argv) {
    LCD_dsPIC_Init();
    Interrupt_Extern(Intx_0,Bajada,7);
    Interrupt_Extern(Intx_1,Bajada,5);
    Interrupt_Extern(Intx_2,Subida,2);
    TRISC = 0x00;
    
    LCD_dsPIC_SetCursor(0,Fila1);
    LCD_dsPIC_Print_String("Clase Interrupciones");
    
    LCD_dsPIC_SetCursor(0,Fila2);
    LCD_dsPIC_Print_String("Productos:");
    LCD_dsPIC_SetCursor(0,Fila3);
    LCD_dsPIC_Print_String("Personas:");   
    LCD_dsPIC_SetCursor(0,Fila4);
    LCD_dsPIC_Print_String("Accidentes:");
    for(;;){
    
        
        if (flag1) {
            productos++;
            if (productos==1000) productos=0;
            sprintf(str_productos,"%03u",productos);
            LCD_dsPIC_SetCursor(16,Fila2);
            LCD_dsPIC_Print_String(str_productos);
            flag1=false;
            
        }
        else if (flag2) {
            personas++;
            if (personas==1000) personas=0;
            sprintf(str_personas,"%03u",personas);
            LCD_dsPIC_SetCursor(16,Fila3);
            LCD_dsPIC_Print_String(str_personas);
            flag2=false;
        }
        else if  (flag3) {
            accidentes++;
            if (accidentes==1000) accidentes=0;
            sprintf(str_accidentes,"%03u",accidentes);
            LCD_dsPIC_SetCursor(16,Fila4);
            LCD_dsPIC_Print_String(str_accidentes);
            flag3=false;
        }
    
    }

    return (EXIT_SUCCESS);
}

