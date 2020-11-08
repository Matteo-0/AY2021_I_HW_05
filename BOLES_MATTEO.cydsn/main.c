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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
