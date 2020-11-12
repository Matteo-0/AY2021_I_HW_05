/*
*    \brief header file for the fifth assignment.
*
*    This file contains the definition of the ISR activated
*    by pressing the button
*
*    \author Matteo Boles
*    \date November 14, 2020
*/

#ifndef ISR_EEPROM_H
    #define ISR_EEPROM_H
    
    #include "project.h"
    
    #define EEPROM_MEMORY_ADDRESS 0x00    // Address of EEPROM where we save the data for changing the sampling frequency
    CY_ISR_PROTO(eeprom_config);
    
#endif
/* [] END OF FILE */
