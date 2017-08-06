/**
 * AS - the open source Automotive Software on https://github.com/parai
 *
 * Copyright (C) 2015  AS <parai@foxmail.com>
 *
 * This source code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation; See <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */
/* ============================ [ INCLUDES  ] ====================================================== */

#include <assert.h>
#include <string.h>
#include "Std_Types.h"
#include "Mcu.h"
#if defined(USE_DET)
#include "Det.h"
#endif
#if defined(USE_DEM)
#include "Dem.h"
#endif
#include "Os.h"
#include "serial.h"
#include "irq.h"

/* ============================ [ MACROS    ] ====================================================== */
#define RESET() ((reset_t)(0x8000))()
/* ============================ [ TYPES     ] ====================================================== */
typedef void (*reset_t)(void);
/* ============================ [ DECLARES  ] ====================================================== */
extern void timer_init();
/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
void __putchar(char ch)
{
	serial_send_char(ch);
}

#ifndef __GNUC__
int putchar( int ch )	/* for printf */
{
	__putchar(ch);
  return ch;
}
#endif
void Mcu_Init(const Mcu_ConfigType *configPtr)
{
	(void)configPtr;
}

void Mcu_SetMode( Mcu_ModeType McuMode ) {
	(void)McuMode;
}

/**
 * Get the system clock in Hz. It calculates the clock from the
 * different register settings in HW.
 */
uint32_t McuE_GetSystemClock(void)
{
  return 0u;
}
Std_ReturnType Mcu_InitClock( const Mcu_ClockType ClockSetting ) {
	(void)ClockSetting;

    return E_OK;
}

void Mcu_PerformReset( void ) {
    DisableInterrupts();
    RESET();
}

Mcu_PllStatusType Mcu_GetPllStatus( void ) {
	return MCU_PLL_LOCKED;
}


Mcu_ResetType Mcu_GetResetReason( void )
{
	return MCU_POWER_ON_RESET;
}

#ifdef __AS_BOOTLOADER__
void StartOsTick(void)
{
	timer_init();
}
#endif

void tpl_shutdown(void)
{
	DisableInterrupts();
	while(1);
}
extern unsigned int _start;
void Mcu_DistributePllClock( void )
{
	serial_init();
	vic_setup();
	irq_init();
	DisableInterrupts();
#ifndef __AS_BOOTLOADER__
	/* for application bcm2835, need to reset the handler array*/
	memcpy((void*)(0x8000+4*24),(void*)(((unsigned int)(&_start))+4*8),32);
	timer_init();
#else
	memcpy((void*)(0x8000+4*24),(void*)(((unsigned int)(&_start))+4*32),32);
#endif

	printf(" >> versatilepb startup done,start @%p\n",&_start);
}
