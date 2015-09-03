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
#include "vEcu.h"
/* ============================ [ MACROS    ] ====================================================== */
/* ============================ [ TYPES     ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
vEcu::vEcu ( QString dll, QObject *parent )
    : QThread(parent)
{
    bool bOK;
    hxDll = LoadLibrary(dll.toStdString().c_str());
    assert(hxDll);

    rsc_tbl_size = 16*1024;
    rsc_tbl_address = malloc(rsc_tbl_size);
    memset(rsc_tbl_address,0,rsc_tbl_size);
    r_lock = CreateMutex( NULL, FALSE, NULL );
    w_lock = CreateMutex( NULL, FALSE, NULL );
    r_event = CreateEvent( NULL, FALSE, FALSE, NULL );
    w_event = CreateEvent( NULL, FALSE, FALSE, NULL );

    pfMain = (PF_MAIN)GetProcAddress(hxDll,"main");
    assert(pfMain);
    pfRprocInit = (PF_RPROC_INIT)GetProcAddress(hxDll,"AsRproc_Init");
    assert(pfRprocInit);
    bOK = pfRprocInit(rsc_tbl_address,rsc_tbl_size,w_lock,r_lock,w_event,r_event,1024);
    assert(bOK);
}

vEcu::~vEcu ( )
{
    FreeLibrary( hxDll );
    CloseHandle(r_lock);
    CloseHandle(w_lock);
    CloseHandle(r_event);
    CloseHandle(w_event);
    TerminateThread(pvThread,0);
    CloseHandle(pvThread);
    exit(0);
}

void vEcu::run(void)
{
    HANDLE pvObjectList[ 2 ];

    pvObjectList[ 0 ] = r_lock;
    pvObjectList[ 1 ] = r_event;

    pvThread = CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE ) pfMain, NULL, 0, NULL );
    while(true)
    {
        WaitForMultipleObjects( sizeof( pvObjectList ) / sizeof( HANDLE ), pvObjectList, TRUE, INFINITE );
        ReleaseMutex( r_lock );
    }
}
