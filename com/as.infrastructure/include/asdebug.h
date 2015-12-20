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
#ifndef COM_AS_INFRASTRUCTURE_INCLUDE_ASDEBUG_H_
#define COM_AS_INFRASTRUCTURE_INCLUDE_ASDEBUG_H_
/* ============================ [ INCLUDES  ] ====================================================== */
#include "Std_Types.h"
/* ============================ [ MACROS    ] ====================================================== */
/* levels for log output */
#define AS_LOG_IPC   	0
#define AS_LOG_VIRTQ    0
#define AS_LOG_RPMSG    0
#define AS_LOG_CAN      0
#define AS_LOG_CANIF    0
#define AS_LOG_LUA      0
#define AS_LOG_OS       0
#define AS_LOG_FLS      1
#define AS_LOG_EEP      1
#define AS_LOG_NVM      1
#define AS_LOG_KSM      1
#define AS_LOG_EA       1
#define AS_LOG_FEE      1
#define AS_LOG_MEMIF    1
#define AS_LOG_SHELL    0
/* and so on ... */
#define AS_LOG_DEFAULT  1
#define AS_LOG_STDOUT  	AS_LOG_DEFAULT
#define AS_LOG_STDERR  	AS_LOG_DEFAULT
#define AS_LOG_ON       AS_LOG_DEFAULT
#define AS_LOG_OFF      0

#if defined(__LINUX__) || defined(__WINDOWS__)
#define ASLOG(level,fmt,...) 								\
	do {													\
		if((AS_LOG_##level) >= AS_LOG_DEFAULT) {			\
			aslog(#level,fmt,##__VA_ARGS__);				\
		} 													\
	}while(0)

#define ASWARNING(fmt,...) 									\
	do {													\
			aslog("WARING",fmt,##__VA_ARGS__);				\
	}while(0)
#else
#define ASLOG(level,fmt,...) 								\
	do {													\
		if((AS_LOG_##level) >= AS_LOG_DEFAULT) {			\
			printf("%-16s:",#level);						\
			printf(fmt,##__VA_ARGS__);						\
		} 													\
	}while(0)

#define ASWARNING(fmt,...) 									\
	do {													\
			printf("%-16s:","WARNING");						\
			printf(fmt,##__VA_ARGS__);						\
	}while(0)
#endif

#define PRINTF(fmt,...) ASLOG(STDOUT,fmt,##__VA_ARGS__)
#define ASHEX(a)	ashex((unsigned long)(a))

#define asAssert(e)  																					\
	do {																								\
		if(FALSE==(e))																					\
		{																								\
			ASLOG(STDERR,"assert error on condition<%s> at line %d of %s\n,",#e, __LINE__, __FILE__);	\
			asAssertErrorHook();																		\
		}																								\
	}while(0)

/* ============================ [ TYPES     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
extern void  aslog(char* module,char* format,...);
extern void  asmem(void* address,size_t size);
extern char* ashex(unsigned long a);
extern char* aswho(void);
extern void  asAssertErrorHook(void);

#endif /* COM_AS_INFRASTRUCTURE_INCLUDE_ASDEBUG_H_ */
