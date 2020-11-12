/*
*    \brief header file for the fifth assignment.
*
*    This file contains the declaration of a new 
*    variable type called ErrorCode
*
*    \author Matteo Boles
*    \credits Davide Marzorati
*    \date November 14, 2020
*/

#ifndef __ERRORCODES_H
    #define __ERRORCODES_H
    
    typedef enum {
        NO_ERROR,           // No error generated during reading or writing processes
        ERROR               // Error generated during reading or writing processes
    } ErrorCode;

#endif
/* [] END OF FILE */
