/*
*    \brief header file for the fifth assignment.
*
*    This file contains the definition of some usefull functions
*
*    \author Matteo Boles
*    \credits Davide Marzorati
*    \date November 14, 2020
*/

#ifndef I2C_Interface_H
    #define I2C_Interface_H
    
    #include "cytypes.h"
    #include "ErrorCodes.h"
    
    // This function starts the I2C peripheral so that it is ready to work.
    ErrorCode I2C_Peripheral_Start(void);
    // This function stops the I2C peripheral from working.
    ErrorCode I2C_Peripheral_Stop(void);
    
    // This function performs a complete writing operation over I2C to a single register.
    ErrorCode I2C_Peripheral_WriteRegister(uint8_t device_address,
                                           uint8_t register_address,
                                           uint8_t data);
    
    // This function performs a complete reading operation over I2C from a single register.
    ErrorCode I2C_Peripheral_ReadRegister(uint8_t device_address, 
                                          uint8_t register_address,
                                          uint8_t* data);
    
    // This function performs a complete reading operation over I2C from multiple registers.
    ErrorCode I2C_Peripheral_ReadRegisterMulti(uint8_t device_address,
                                               uint8_t register_address,
                                               uint8_t register_count,
                                               uint8_t* data);
#endif
    
/* [] END OF FILE */
