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
#include "Os.h"

/* ============================ [ MACROS    ] ====================================================== */

/* ============================ [ TYPES     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
extern	TickType				OsTickCounter;
#if (KSM_NUM > 0)
static KSMState_Type ksm_state[KSM_NUM];
#endif
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */

#if (KSM_NUM > 0)
void KsmInit(void)
{
	KsmID_Type i;
	for(i=0;i<KSM_NUM;i++)
	{
		ksm_state[i] = KSM_INVALID;
		KSM_Config[i].Ksm[KSM_INIT]();
	}
}
void KsmStart(void)
{
	KsmID_Type i;
	for(i=0;i<KSM_NUM;i++)
	{
		KSM_Config[i].Ksm[KSM_START]();
	}
}
void KsmStop(void)
{
	KsmID_Type i;
	for(i=0;i<KSM_NUM;i++)
	{
		KSM_Config[i].Ksm[KSM_STOP]();
	}
}
void KsmExecute(void)
{
	KsmID_Type i;
	for(i=0;i<KSM_NUM;i++)
	{
		if(ksm_state[i]<KSM_Config[i].KsLimit)
		{
			KSM_Config[i].Ksm[ksm_state[i]]();
		}
	}
}
void KsmSetState(KsmID_Type Ksm,KSMState_Type state)
{
	if(Ksm < KSM_NUM)
	{
		ksm_state[Ksm] = state;
	}
	else
	{
		assert(0);
	}
}
KSMState_Type KsmGetState(KsmID_Type Ksm)
{
	KSMState_Type state;
	if(Ksm < KSM_NUM)
	{
		state = ksm_state[Ksm];
	}
	else
	{
		state = KSM_INVALID;
		assert(0);
	}
	return state;
}
#endif /* KSM_NUM  */
void StartTimer(TimerType* timer)
{
	assert(timer!=NULL);
	*timer = OsTickCounter;
}
void StopTimer(TimerType* timer)
{
	assert(timer!=NULL);
	*timer = 0;
}
TimerType GetTimer(TimerType* timer)
{
	TimerType time;
	assert(timer!=NULL);

	if(0 == *timer)
	{
		time = 0;
	}
	else
	{
		if (OsTickCounter >= *timer)
		{
			 time = (OsTickCounter - *timer);
		}
		else
		{
			time = (TICK_MAX - *timer + OsTickCounter);
		}
	}
	return time;
}
