/* 
 * File:   I2C.h
 * Author: gssan
 *
 * Created on 23 de febrero de 2023, 10:44 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <stdint.h>

#define PCF_Unidades  0b0100000
#define PCF_Decenas   0b0100110
#define PCF_Centenas  0b0100011


#define I2C_ACK 0b00000000
#define I2C_nACK 0b00000001

#define I2C_Write 0b00000000
#define I2C_Read  0b00000001
//funciones
void I2C_Init(uint16_t Fscl);
void I2C_Start(void);
void I2C_Restart(void);
void I2C_Stop(void);
void I2C_Write_Address_Slave(uint8_t address, uint8_t data_dir);
void I2C_Write_Data_Slave(uint8_t data);
void I2C_Read_data(uint8_t *ptr_data,uint8_t var_ack_nack);



#endif	/* I2C_H */

