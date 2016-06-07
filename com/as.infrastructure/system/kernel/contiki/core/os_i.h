/**
 * AS - the open source Automotive Software on https://github.com/parai
 *
 * Copyright (C) 2016  AS <parai@foxmail.com>
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
#ifndef OS_I_H
#define OS_I_H
/* ============================ [ INCLUDES  ] ====================================================== */
#include "Std_Types.h"
/* ============================ [ MACROS    ] ====================================================== */
#define    E_OS_ACCESS             	  (StatusType)1
#define    E_OS_CALLEVEL              (StatusType)2
#define    E_OS_ID                    (StatusType)3
#define    E_OS_LIMIT                 (StatusType)4
#define    E_OS_NOFUNC                (StatusType)5
#define    E_OS_RESOURCE              (StatusType)6
#define    E_OS_STATE                 (StatusType)7
#define    E_OS_VALUE                 (StatusType)8

#define OSDEFAULTAPPMODE        (AppModeType)1

#define SUSPENDED                  ((StatusType) 0)
#define RUNNING                    ((StatusType) 1)
#define READY                      ((StatusType) 2)
#define WAITING                    ((StatusType) 3)

#define INVALID_TASK            (TaskType)-1

/*
 *  Macro for declare Task/Alarm/ISR Entry
 */
#define TASK(TaskName)        	 void TaskMain##TaskName(void)
#define ISR(ISRName)        	 void ISRMain##ISRName(void)
#define ALARM(AlarmCallBackName) void AlarmMain##AlarmCallBackName(void)

#define RES_SCHEDULER           (ResourceType)0 /* default resources for OS */

/* ============================ [ TYPES     ] ====================================================== */
typedef uint8 					StatusType;
typedef uint32   				EventMaskType;
typedef EventMaskType *			EventMaskRefType;
typedef uint8  			    	TaskType;
typedef TaskType *				TaskRefType;
typedef uint8					TaskStateType;
typedef TaskStateType *			TaskStateRefType;
typedef uint32                  AppModeType;	/*! each bit is a mode */

typedef uint32					TickType;
typedef TickType*				TickRefType;
typedef uint8			        IsrType;			/* ISR ID */
typedef uint8			        CounterType;		/* Counter ID */

typedef uint8					AlarmType;
typedef struct
{
	TickType maxallowedvalue;
	TickType ticksperbase;
	TickType mincycle;
} 								AlarmBaseType;
typedef AlarmBaseType *			AlarmBaseRefType;

typedef uint8                   ResourceType;

/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */

#endif /* OS_I_H */
