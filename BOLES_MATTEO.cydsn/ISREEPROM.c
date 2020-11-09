/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "ISREEPROM.h"

extern uint8_t eeprom_value;
extern uint8_t sampling_frequency[6];
extern int i,k,flag;

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
