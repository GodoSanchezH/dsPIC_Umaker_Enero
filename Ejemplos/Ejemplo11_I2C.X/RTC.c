#include <xc.h>
#include "Config.h"
#include "RTC.h"
#include "I2C.h"

void DS1307_Init(void){
    I2C_Start();
    I2C_Write_Data_Slave(0XD0);
    
}
void DS1307_Write_Hours_24(uint8_t Hora,uint8_t Minuto,uint8_t Segundo){
            Segundo = ((Segundo/10)<<4) | (Segundo%10)  ;
            Minuto = ((Minuto/10)<<4) | (Minuto%10)  ;
            Hora = ((Hora/10)<<4) | (Hora%10)  ;
            
            I2C_Write_Data_Slave(0X00);
            I2C_Write_Data_Slave(Segundo);
            I2C_Write_Data_Slave(Minuto);
            I2C_Write_Data_Slave(Hora);
            I2C_Stop();
}
void DS1307_Write_Date(uint8_t Date,uint8_t Month , uint8_t Year,uint8_t Day){

            Day = ((Day/10)<<4) | (Day%10)  ;
            Date = ((Date/10)<<4) | (Date%10)  ;
            Month = ((Month/10)<<4) | (Month%10)  ;
            Year = ((Year/10)<<4) | (Year%10)  ;
    
            I2C_Start();
            I2C_Write_Data_Slave(0XD0);
                    
            I2C_Write_Data_Slave(0X03);
            
            I2C_Write_Data_Slave(Day);
            I2C_Write_Data_Slave(Date);
            I2C_Write_Data_Slave(Month);
            I2C_Write_Data_Slave(Year);
            I2C_Stop();
    
}
void DS1307_Read_H_24(uint8_t *Hora,uint8_t *Minuto,uint8_t *Segundo){

    I2C_Start();
    I2C_Write_Data_Slave(0xD0);
    I2C_Write_Data_Slave(0X00);
    I2C_Restart();
    I2C_Write_Data_Slave(0xD1);
    I2C_Read_data(Segundo,I2C_ACK);
    I2C_Read_data(Minuto,I2C_ACK);
    I2C_Read_data(Hora,I2C_nACK);
    I2C_Stop();
    
    *Segundo = (((*Segundo & 0xF0)>>4)*10)+(*Segundo & 0x0F);
    *Minuto = (((*Minuto & 0xF0)>>4)*10)+(*Minuto & 0x0F);
    *Hora = (((*Hora & 0xF0)>>4)*10)+(*Hora & 0x0F);
    
}
void DS1307_Read_Date(uint8_t *Date,uint8_t *Month , uint8_t *Year,uint8_t *Day){

    I2C_Start();
    I2C_Write_Data_Slave(0xD0);
    I2C_Write_Data_Slave(0X03);
    I2C_Restart();
    I2C_Write_Data_Slave(0xD1);
    I2C_Read_data(Day,I2C_ACK);
    I2C_Read_data(Date,I2C_ACK);
    I2C_Read_data(Month,I2C_ACK);
    I2C_Read_data(Year,I2C_nACK);
    I2C_Stop();
    
    *Day = (((*Day & 0xF0)>>4)*10)+(*Day & 0x0F);
    *Date = (((*Date & 0xF0)>>4)*10)+(*Date & 0x0F);
    *Month = (((*Month & 0xF0)>>4)*10)+(*Month & 0x0F);
    *Year = (((*Year & 0xF0)>>4)*10)+(*Year & 0x0F);
}
