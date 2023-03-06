/* 
 * File:   RTC.h
 * Author: HP
 *
 * Created on 29 de octubre de 2022, 03:47 PM
 */

#ifndef RTC_H
#define	RTC_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>



void DS1307_Init(void);
void DS1307_Write_Hours_24(uint8_t Hora,uint8_t Minuto,uint8_t Segundo);
void DS1307_Write_Date(uint8_t Date,uint8_t Month , uint8_t Year,uint8_t Day);
void DS1307_Read_H_24(uint8_t *Hora,uint8_t *Minuto,uint8_t *Segundo);
void DS1307_Read_Date(uint8_t *Date,uint8_t *Month , uint8_t *Year,uint8_t *Day);

#endif	/* RTC_H */

