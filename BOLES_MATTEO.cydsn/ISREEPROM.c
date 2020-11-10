/*
*    \brief source file for the fifth assignment.
*
*    This file contains the body of the ISR activated by pressing the button.
*    In this ISR we save the data corresponding to a certain sampling frequency 
*    in the EEPROM memory and in a variable called eeprom_value and then we set
*    a flag to 1 so that when we return in the main we save the correct data for
*    the sampling frequency in control register 1. 
*
*    \author Matteo Boles
*    \date November 14, 2020
*/

#include "project.h"
#include "ISREEPROM.h"

extern uint8_t eeprom_value;
extern uint8_t sampling_frequency[6];
extern int k;
extern int flag;

CY_ISR(eeprom_config)
{
    if(k==6)
    {
        k=0;
        EEPROM_UpdateTemperature();     
        EEPROM_WriteByte(sampling_frequency[k],0x00);
        eeprom_value = EEPROM_ReadByte(0x00);
        flag=1;
    }
    else
    {
        EEPROM_UpdateTemperature();     
        EEPROM_WriteByte(sampling_frequency[k],0x00);
        eeprom_value = EEPROM_ReadByte(0x00); 
        flag=1;
    }
}
  
/* [] END OF FILE */
