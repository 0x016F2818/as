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
#include "Ipc.h"
/* ============================ [ MACROS    ] ====================================================== */

/* ============================ [ TYPES     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */

/* ============================ [ DATAS     ] ====================================================== */
static Ipc_FifoType r_fifo;
static Ipc_FifoType w_fifo;
static const Ipc_Idx2VirtQChannleMappingType mapping[2] =
{
	{ .idx = VIRTQ_IDX_RPMSG_RX, .chl= VIRTQ_CHL_RPMSG_RX },
	{ .idx = VIRTQ_IDX_RPMSG_TX, .chl= VIRTQ_CHL_RPMSG_TX },
};
static Ipc_ChannelConfigType Ipc_ChlConfig[IPC_CHL_NUM] =
{
	{
		.r_fifo = &r_fifo,
		.w_fifo = &w_fifo,

		.mapping = mapping,
		.map_size = sizeof(mapping)/sizeof(Ipc_Idx2VirtQChannleMappingType),

		.rxNotification = VirtQ_RxNotificatin,
		.txConfirmation = VirtQ_TxConfirmation,

	}
};
const Ipc_ConfigType Ipc_Config =
{
	.channelConfig = Ipc_ChlConfig
};
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
void Qt_SetIpcParam(Ipc_ChannelType chl, void* r_lock, void* r_event, void* w_lock, void* w_event)
{
	assert(chl < IPC_CHL_NUM);
	Ipc_ChlConfig[chl].r_lock = w_lock;
	Ipc_ChlConfig[chl].w_lock = r_lock;
	Ipc_ChlConfig[chl].r_event = w_event;
	Ipc_ChlConfig[chl].w_event = r_event;
}
void Qt_GetIpcFifo(Ipc_ChannelType chl, Ipc_FifoType** r_fifo, Ipc_FifoType** w_fifo)
{
	assert(chl < IPC_CHL_NUM);
	*r_fifo = Ipc_ChlConfig[chl].w_fifo;
	*w_fifo = Ipc_ChlConfig[chl].r_fifo;
}
