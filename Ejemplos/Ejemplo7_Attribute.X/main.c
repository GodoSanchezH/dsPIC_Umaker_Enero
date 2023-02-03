/* 
 * File:   main.c
 * Author: gssan
 *
 * Created on February 2, 2023, 10:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/*
 * 
 */

/*atributos (space)
 -> DMA
 -> Datos
 -> Programa
 -> Xmemory
 -> Ymemory
 ->EEPROM
 ->PSV
 */

char var1 __attribute__ ((space(xmemory))) = 'b';
uint8_t var2 __attribute__ ((space(ymemory)))= 86;
float var3 __attribute__ ((space(xmemory)))=  3.1416;
char var4[] __attribute__ ((space(ymemory))) ="hola mundo";

uint8_t  i;
void Ejemplo(void);

int X_memory_buffer[5] __attribute__ ((space(xmemory))) ={0,1,2,3,4};
int Y_memory_buffer[5] __attribute__ ((space(ymemory)))={5,6,7,8,9};

int main(int argc, char** argv) {

    asm("nop");
 
    printf("Simulacion X16\n\r");
    printf("******X MEMORY*******\n\r");
    
    
    for (i = 0; i < 5; i++) {
        printf("La direccion %u de la memoria x es igual 0x%X y su valor es %u\n\r",i,&(X_memory_buffer[i]),X_memory_buffer[i]);

    }

       printf("\n\r\n\r******Y MEMORY*******\n\r");
    
    
    for (i = 0; i < 5; i++) {
        printf("La direccion %u de la memoria y es igual 0x%X y su valor es %u\n\r",i,&(Y_memory_buffer[i]),Y_memory_buffer[i]);

    }
    for(;;){
    
    
    }
    
    
    return (EXIT_SUCCESS);
}

void Ejemplo(void){
  //var4 = "hola mundo";
    
    printf("dsPIC33FJ32MC202\n\r");
    printf("Var1:%c\n\r",var1);
    printf("Var2:%u\n\r",var2);
    printf("Var3:%0.3f\n\r",var3);
    printf("Var4:%s\n\r",var4);
    asm("nop");
    
}


