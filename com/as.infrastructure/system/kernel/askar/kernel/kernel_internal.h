/**
 * AS - the open source Automotive Software on https://github.com/parai
 *
 * Copyright (C) 2017  AS <parai@foxmail.com>
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
#ifndef KERNEL_INTERNAL_H_
#define KERNEL_INTERNAL_H_
/* ============================ [ INCLUDES  ] ====================================================== */
#include "Os.h"
#include "portable.h"
/* ============================ [ MACROS    ] ====================================================== */
/*
 * BCC1 (only basic tasks, limited to one activation request per task and one task per
 * priority, while all tasks have different priorities)
 * BCC2 (like BCC1, plus more than one task per priority possible and multiple requesting
 * of task activation allowed)
 * ECC1 (like BCC1, plus extended tasks)
 * ECC2 (like ECC1, plus more than one task per priority possible and multiple requesting
 * of task activation allowed for basic tasks)
 */
enum {
	BCC1,
	BCC2,
	ECC1,
	ECC2
};

enum {
	STANDARD,
	EXTENDED
};
/*
 *  kernel call level
 */
#define TCL_NULL		((unsigned int) 0x00)	/* invalid kernel call level */
#define TCL_TASK		((unsigned int) 0x01)	/* task level */
#define TCL_ISR2		((unsigned int) 0x02)	/* interrupt type 2 ISR */
#define TCL_ERROR		((unsigned int) 0x04)	/* ErrorHook */
#define TCL_PREPOST		((unsigned int) 0x08)	/* PreTaskHook & PostTaskHook */
#define TCL_STARTUP		((unsigned int) 0x10)	/* StartupHook */
#define TCL_SHUTDOWN	((unsigned int) 0x20)	/* ShutdownHook */

#ifdef OS_USE_ERROR_HOOK
/* OSErrorOne/OSErrorTwo/OSErrorThree
 * a. Each API MUST keep the parameter name the same with
 * the "param" of OSError_##api##_##param.
 * b. Each API block MUST have the "ercd" variable.
 */
/* for those API only with no parameter */
#define OSErrorNone(api) do { \
	if(ercd != E_OK) { \
		unsigned int savedLevel; \
		imask_t mask; \
		Irq_Save(mask); \
		_errorhook_svcid = OSServiceId_##api; \
		savedLevel = CallLevel; \
		CallLevel = TCL_ERROR; \
		ErrorHook(ercd); \
		CallLevel = savedLevel; \
		Irq_Restore(mask); \
	} \
} while(0)

/* for those API only with 1 parameter */
#define OSErrorOne(api, param) do { \
	if(ercd != E_OK) { \
		unsigned int savedLevel; \
		imask_t mask; \
		Irq_Save(mask); \
		_errorhook_svcid = OSServiceId_##api; \
		OSError_##api##_##param() = param; \
		savedLevel = CallLevel; \
		CallLevel = TCL_ERROR; \
		ErrorHook(ercd); \
		CallLevel = savedLevel; \
		Irq_Restore(mask); \
	} \
} while(0)

/* for those API with 2 parameters */
#define OSErrorTwo(api, param0, param1) do { \
	if(ercd != E_OK) { \
		unsigned int savedLevel; \
		imask_t mask; \
		Irq_Save(mask); \
		_errorhook_svcid = OSServiceId_##api; \
		OSError_##api##_##param0() = param0; \
		OSError_##api##_##param1() = param1; \
		savedLevel = CallLevel; \
		CallLevel = TCL_ERROR; \
		ErrorHook(ercd); \
		CallLevel = savedLevel; \
		Irq_Restore(mask); \
	} \
} while(0)

/* for those API with 3 parameters */
#define OSErrorThree(api, param0, param1, param2) do { \
	if(ercd != E_OK) { \
		unsigned int savedLevel; \
		imask_t mask; \
		Irq_Save(mask); \
		_errorhook_svcid = OSServiceId_##api; \
		OSError_##api##_##param0() = param0; \
		OSError_##api##_##param1() = param1; \
		OSError_##api##_##param2() = param2; \
		savedLevel = CallLevel; \
		CallLevel = TCL_ERROR; \
		ErrorHook(ercd); \
		CallLevel = savedLevel; \
		Irq_Restore(mask); \
	} \
} while(0)
#else
#define OSErrorNone(api)
#define OSErrorOne(api, param)
#define OSErrorTwo(api, param0, param1)
#define OSErrorThree(api, param0, param1, param2)
#endif
/* ============================ [ TYPES     ] ====================================================== */
typedef unsigned int			KeyType;
typedef KeyType					PriorityType;

typedef void	(*TaskMainEntryType)(void);
typedef void	(*AlarmMainEntryType)(void);

typedef struct
{
	void* pStack;
	uint32_t stackSize;
	TaskMainEntryType entry;
	PriorityType initPriority;
	PriorityType runPriority;
	const char* name;
	#ifdef MULTIPLY_TASK_ACTIVATION
	uint8 maxActivation;
	#endif
	boolean autoStart;
} TaskConstType;

typedef struct
{
	TaskContextType context;
	PriorityType priority;
	const TaskConstType* pConst;
	#ifdef MULTIPLY_TASK_ACTIVATION
	uint8 activation;
	#endif
	StatusType state;
} TaskVarType;
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
extern TaskVarType* RunningVar;
extern TaskVarType* ReadyVar;
extern unsigned int CallLevel;
extern const TaskConstType TaskConstArray[TASK_NUM];
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
extern void Os_TaskInit(void);

extern void Os_PortInit(void);
extern void Os_PortInitContext(TaskVarType* pTaskVar);
extern void Os_PortStartDispatch(void);
extern void Os_PortDispatch(void);

extern void Sched_Init(void);
extern void Sched_AddReady(TaskVarType* pTaskVar);
extern void Sched_GetReady(void);
extern void Sched_Preempt(void);

#endif /* KERNEL_INTERNAL_H_ */
