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
// Status register new data
#define LIS3DH_STATUS_REG_NEW_DATA 0x08
// Control register 1 address
#define LIS3DH_CTRL_REG1 0x20
// Control register 4 address
#define LIS3DH_CTRL_REG4 0x23
// Control register 4 configuration (1010 1000). BDU = 1, High resolution and full scale range +-4g
#define LIS3DH_CTRL_REG4_CFG 0x88    
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
int b=1;
int flag=0;

int main(void)
{
    // Enable global interrupts.
    CyGlobalIntEnable; 

    // Start the components and the ISR.
    I2C_Peripheral_Start();
    UART_Start();
    EEPROM_Start();
    ISR_EEPROM_StartEx(eeprom_config);
    
    // Check if any device is connected over I2C.
    I2C_Peripheral_IsDeviceConnected(LIS3DH_DEVICE_ADDRESS);
    
    eeprom_value = EEPROM_ReadByte(0x00);
    
    for(i=0;i<6;i++)
    {
        if (b==1)
        {
            if (eeprom_value == sampling_frequency[i])
            {
                k=i;
                b=0;
            }
            else
            {
                k=0;
            }
        }
    }
    
    // Activate the interrupt
    ISR_EEPROM_SetPending();
    
    uint8_t ctrl_reg4;
    ctrl_reg4 = LIS3DH_CTRL_REG4_CFG;
                
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                 LIS3DH_CTRL_REG4,
                                 ctrl_reg4);
    
    uint8_t header = 0xA0;
    uint8_t footer = 0xC0;
    uint8_t OutArray [8]; 
    OutArray[0] = header;
    OutArray[7] = footer; 
    int16 dirtytrick = 1000; 
    float OutAccXConv,OutAccYConv,OutAccZConv;
    
    uint8_t AccelerometerData[6];
    int16 OutAccX,OutAccY,OutAccZ; 
    ErrorCode error;  

    for(;;)
    {
        if(flag)
        {
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG1,
                                         eeprom_value); 
            flag=0;
            k++;
        }
        
        // Reading the status register
        uint8_t status_register; 
        I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                    LIS3DH_STATUS_REG,
                                    &status_register);
        
            if (status_register &= LIS3DH_STATUS_REG_NEW_DATA)
            {
                // Reading the acceleration data
                error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                         LIS3DH_OUT_XL,
                                                         6,
                                                         AccelerometerData); 

                // Reconstructing the data if no error occured
                if(error == NO_ERROR)
                {  
                    OutAccX = (int16)((AccelerometerData[0] | (AccelerometerData[1]<<8)))>>4;
                    OutAccY = (int16)((AccelerometerData[2] | (AccelerometerData[3]<<8)))>>4;
                    OutAccZ = (int16)((AccelerometerData[4] | (AccelerometerData[5]<<8)))>>4;
                    
                    OutAccXConv=OutAccX*(2*9.81)/2048.0;
                    OutAccYConv=OutAccY*(2*9.81)/2048.0;
                    OutAccZConv=OutAccZ*(2*9.81)/2048.0;
                    
                    OutAccX = (int16) (OutAccXConv*dirtytrick); 
                    OutAccY = (int16) (OutAccYConv*dirtytrick);
                    OutAccZ = (int16) (OutAccZConv*dirtytrick);
                    
                    /*OutAccXConv = OutAccX * conversion;
                    OutAccYConv = OutAccY * conversion;
                    OutAccZConv = OutAccZ * conversion;
                    
                    OutAccX = (int16) (OutAccXConv * dirtytrick);
                    OutAccY = (int16) (OutAccYConv * dirtytrick);
                    OutAccZ = (int16) (OutAccZConv * dirtytrick);*/
                    
                    OutArray[1] = (uint8_t)(OutAccX & 0xFF);
                    OutArray[2] = (uint8_t)(OutAccX >> 8);
                    OutArray[3] = (uint8_t)(OutAccY & 0xFF);
                    OutArray[4] = (uint8_t)(OutAccY >> 8);
                    OutArray[5] = (uint8_t)(OutAccZ & 0xFF);
                    OutArray[6] = (uint8_t)(OutAccZ >> 8);
                    
                    UART_PutArray(OutArray, 8);
                }
                else
                {
                    UART_PutString("Error occurred during I2C comm\r\n");   
                } 
            }    
    }
}

/* [] END OF FILE */
