/*
*    \brief Main source file for the fifth assignment.
*
*    In this main file we start the components and set up the registers and make
*    the reading and reconstruction of the acceleration data that are then converted
*    in m/s^2 and sent via UART to the Bridge Control Panel.
*
*    \author Matteo Boles
*    \date November 14, 2020
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
// Control register 4 configuration (1000 1000). BDU = 1, High resolution and full scale range +-2g
#define LIS3DH_CTRL_REG4_CFG 0x88    
// Address of the accelerometer output on the three axis LSB register
#define LIS3DH_OUT_XL 0x28
#define LIS3DH_OUT_YL 0x2A
#define LIS3DH_OUT_ZL 0x2C
// Address of the accelerometer output on the three axis MSB register
#define LIS3DH_OUT_XH 0x29
#define LIS3DH_OUT_YH 0x2B
#define LIS3DH_OUT_ZH 0x2D

// Sampling frequencies
#define FREQUENCY_1HZ   0x17
#define FREQUENCY_10HZ  0x27
#define FREQUENCY_25HZ  0x37
#define FREQUENCY_50HZ  0x47
#define FREQUENCY_100HZ 0x57
#define FREQUENCY_200HZ 0x67

// Defining some variables
uint8_t eeprom_value;                                           // Variable where we save the data present in the EEPROM memory
uint8_t sampling_frequency[6]={FREQUENCY_1HZ,FREQUENCY_10HZ,FREQUENCY_25HZ,FREQUENCY_50HZ,FREQUENCY_100HZ,FREQUENCY_200HZ};  // Array where we save the values for different sampling frequencies
int i;                                                          // Variable used to move in the different position in the array sampling_frequency
int k;                                                          // Variable used to save the position of the sampling_frequency vector in which we are when we turn on the system
int flag;                                                       // Variable that is set to 1 each time we want to change the sampling frequency

int main(void)
{
    // Enable global interrupts.
    CyGlobalIntEnable; 

    // Start the components and the ISR.
    I2C_Peripheral_Start();
    UART_Start();
    EEPROM_Start();
    ISR_EEPROM_StartEx(eeprom_config);
    
    // When we turn on the system we check which value is saved in the EEPROM memory.
    eeprom_value = EEPROM_ReadByte(EEPROM_MEMORY_ADDRESS);
    
    // Checking if in EEPROM there is already data related to a certain sampling frequency 
    for(i=0;i<6;i++)
    {
        if (eeprom_value == sampling_frequency[i])
        {
            k=i;
            i=6;
        }
        else
        {
            k=0;
        }    
    }
    
    // We activate the interrupt via software at the beginning.
    ISR_EEPROM_SetPending();
    
    // Writing on control register 4
    uint8_t ctrl_reg4;
    ctrl_reg4 = LIS3DH_CTRL_REG4_CFG;
                
    I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                 LIS3DH_CTRL_REG4,
                                 ctrl_reg4);
    
    // Defining some variables
    uint8_t status_register;                     // Variable where we save the data read in the status register
    uint8_t header = 0xA0;                       // Packet header
    uint8_t footer = 0xC0;                       // Packet tail
    uint8_t OutArray [8];                        // Array where we save the bytes to be sent via UART
    OutArray[0] = header;                        // In the first position of the array we save the packet header
    OutArray[7] = footer;                        // In the last position we save the packet tail   
    
    int16 dirtytrick = 1000;                     // Trick used to be able to keep 3 digits in the acceleration data sent via UART because with function PutArray I cannot send float data
    uint8_t AccelerometerData[6];                // Array where we save acceleration data read from the registers
    float OutAccConv[3];                         // Array used to save acceleration data converted in m/s^2
    int16 OutAcc[3];                             // Array where we reconstruct and save the data ready to be sent via UART
    int t;                                       // Variable used to move in the different position in the arrays OutAcc, OutAccConv, OutArray and AccelerometerData     
    
    ErrorCode error;                             // Variable used to check if any error occured during reading or writing processes

    for(;;)
    {
        // Each time we press the button the variable 'flag' is set to 1, so that we can change the sampling frequency
        if(flag)
        {
            // Writing on control register 1 to change the sampling frequency
            I2C_Peripheral_WriteRegister(LIS3DH_DEVICE_ADDRESS,
                                         LIS3DH_CTRL_REG1,
                                         eeprom_value); 
            flag=0;
            k++;
        }
        
        // Reading the status register to check if new data are available 
        error = I2C_Peripheral_ReadRegister(LIS3DH_DEVICE_ADDRESS,
                                            LIS3DH_STATUS_REG,
                                            &status_register);
        
        // If no error occured and new data are available    
        if ((error == NO_ERROR) && (status_register &= LIS3DH_STATUS_REG_NEW_DATA))
        {
            // Reading the acceleration data
            error = I2C_Peripheral_ReadRegisterMulti(LIS3DH_DEVICE_ADDRESS,
                                                     LIS3DH_OUT_XL,
                                                     6,
                                                     AccelerometerData); 

            // Reconstructing the data if no error occured and converting them in m/s^2 for each axis
            if(error == NO_ERROR)
            {  
                for (t=0;t<3;t++)
                {
                    // Data reconstruction and conversion in m/s^2
                    OutAcc[t] = (int16)((AccelerometerData[2*t] | (AccelerometerData[2*t+1]<<8)))>>4;
                    OutAccConv[t] = OutAcc[t]*(2*9.81)/2048.0;
                    OutAcc[t] = (int16) (OutAccConv[t]*dirtytrick);
                    
                    OutArray[2*t+1] = (uint8_t)(OutAcc[t] & 0xFF);
                    OutArray[2*t+2] = (uint8_t)(OutAcc[t] >> 8);
                }
                // We send the data via UART
                UART_PutArray(OutArray, 8);                  
            }
            else
            {
                // We send a message via UART if any error occured during Acceleration data reading
                UART_PutString("Error occurred during I2C comm\r\n");   
            } 
        }    
    }
}

/* [] END OF FILE */
