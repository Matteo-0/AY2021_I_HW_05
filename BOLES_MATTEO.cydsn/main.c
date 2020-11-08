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
#include "I2C_Interface.h"
#include "ErrorCodes.h"
#include "ISREEPROM.h"

// Slave device address
#define LIS3DH_DEVICE_ADDRESS 0x18
// Status register address
#define LIS3DH_STATUS_REG 0x27
// Control register 1 address
#define LIS3DH_CTRL_REG1 0x20
// Control register 4 address
#define LIS3DH_CTRL_REG4 0x23
// Address of the accelerometer output on the three axis LSB register
#define LIS3DH_OUT_XL 0x28
#define LIS3DH_OUT_YL 0x2A
#define LIS3DH_OUT_ZL 0x2C
// Address of the accelerometer output on the three axis MSB register
#define LIS3DH_OUT_XH 0x29
#define LIS3DH_OUT_YH 0x2B
#define LIS3DH_OUT_ZH 0x2D

uint8_t eeprom_value;
uint8_t sampling_frequency[6]={0x17,0x27,0x37,0x47,0x57,0x67};
int i;
int k;

int main(void)
{
    // Enable global interrupts.
    CyGlobalIntEnable; 

    // Start the components and the ISR.
    I2C_Peripheral_Start();
    UART_Start();
    EEPROM_Start();
    ISR_EEPROM_StartEx(eeprom_config);
    
    eeprom_value = EEPROM_ReadByte(0x00);
    
    for(i=0;i<6;i++)
    {
        if (eeprom_value == sampling_frequency[i])
        {
            // sovrascrivo valore nel control reg 1
            k=i;
        }
        else
        {
            k=0;
            EEPROM_UpdateTemperature();     
            EEPROM_WriteByte(sampling_frequency[k],0x00);
            eeprom_value = EEPROM_ReadByte(0x00);
            // scrivo nel control reg 1
    }
    
       
    

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
