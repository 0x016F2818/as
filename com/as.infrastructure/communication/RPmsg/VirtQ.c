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
#include "VirtQ.h"
#include "Ipc.h"
/* ============================ [ MACROS    ] ====================================================== */

/* ============================ [ TYPES     ] ====================================================== */
typedef struct
{
	const VirtQ_ConfigType* config;
	VirtQ_QueueType vq[VIRTQ_CHL_NUM];
	boolean initialized;
}virtq_t;
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
static virtq_t virtq =
{
	.initialized = FALSE
};
/* ============================ [ LOCALS    ] ====================================================== */
static void *virtqueue_get_avail_buf(VirtQ_QueueType *vq, VirtQ_IdxType *idx, uint16 *len)
{
	void* buf;
	ASLOG(VIRTQ,"VirtQ get buf last_avail_idx=%d vring.avail->idx=%d\n",vq->last_avail_idx, vq->vring.avail->idx);
    /* There's nothing available? */
    if (vq->last_avail_idx == vq->vring.avail->idx) {
        /* We need to know about added buffers */
        vq->vring.used->flags &= ~VRING_USED_F_NO_NOTIFY;

        buf = NULL;
    }
    else
    {
		/*
		 * Grab the next descriptor number they're advertising, and increment
		 * the index we've seen.
		 */
    	*idx = vq->vring.avail->ring[vq->last_avail_idx++ % vq->vring.num];

		buf = IPC_MAP_PA_TO_VA(vq->vring.desc[*idx].addr);
		*len = (uint16)vq->vring.desc[*idx].len;
    }

    return buf;
}

static void virtqueue_set_used_buf(VirtQ_QueueType *vq, VirtQ_IdxType idx, uint32 len)
{
   Vring_UsedElemType *used;

   ASLOG(OFF,"VirtQ set used buf idx=%d vq->vring.used->idx=%d\n",idx, vq->vring.used->idx);

   if (idx > vq->vring.num) {
       assert(0);
   }
   else
   {
	   /*
	   * The virtqueue contains a ring of used buffers.  Get a pointer to the
	   * next entry in that used ring.
	   */
	   used = &vq->vring.used->ring[vq->vring.used->idx % vq->vring.num];
	   used->id = idx;
	   used->len = len;

	   vq->vring.used->idx++;
   }
}
/* ============================ [ FUNCTIONS ] ====================================================== */
void VirtQ_InitVq(VirtQ_ChannerlType chl)
{
	vring_init(	&virtq.vq[chl].vring,
				virtq.config->queueConfig[chl].vring->num,
				IPC_MAP_PA_TO_VA(virtq.config->queueConfig[chl].vring->da),
				virtq.config->queueConfig[chl].vring->align
			);

	virtq.vq[chl].last_avail_idx = 0;

    ASLOG(OFF,"sizeof(Vring_DescType)=%d,sizeof(Vring_UsedElemType)=%d,sizeof(uint16_t)=%d,num=%d,align=%d\n",
          sizeof(Vring_DescType),sizeof(Vring_UsedElemType),sizeof(uint16_t),
		  virtq.config->queueConfig[chl].vring->num,virtq.config->queueConfig[chl].vring->align);

	ASLOG(OFF,"vring[idx=%Xh,size=%d]: num=%d, desc=%s, avail=%s, used=%s, da=%Xh\n",
			virtq.config->queueConfig[chl].vring->notifyid,
			vring_size(virtq.config->queueConfig[chl].vring->num,virtq.config->queueConfig[chl].vring->align),
			virtq.vq[chl].vring.num,ASHEX(virtq.vq[chl].vring.desc),
			ASHEX(virtq.vq[chl].vring.avail),ASHEX(virtq.vq[chl].vring.used),
			virtq.config->queueConfig[chl].vring->da);
}
Std_ReturnType VirtQ_GetAvailiableBuffer(VirtQ_ChannerlType chl,VirtQ_IdxType* idx,void** buf,uint16* len)
{
	Std_ReturnType ercd;

	assert(chl < VIRTQ_CHL_NUM);
	assert(virtq.initialized);

	ASLOG(OFF,"VirtQ_GetAvailiableBuffer(chl=%d)\n",chl);

	*buf = virtqueue_get_avail_buf(&virtq.vq[chl],idx,len);

	if(*buf)
	{
		ercd = E_OK;
	}
	else
	{
		ercd = E_NOT_OK;
	}

	return ercd;
}

void VirtQ_AddUsedBuffer(VirtQ_ChannerlType chl,VirtQ_IdxType idx,uint16 len)
{
	virtqueue_set_used_buf(&virtq.vq[chl],idx,len);
}

void VirtQ_Kick(VirtQ_ChannerlType chl)
{
	if (virtq.vq[chl].vring.avail->flags & VRING_AVAIL_F_NO_INTERRUPT) {
		/* do nothing */
	}
	else
	{
		/* trigger IPC interrupt */
		Ipc_WriteIdx(virtq.config->queueConfig[chl].chl,virtq.config->queueConfig[chl].idx);
	}
}
void VirtQ_Init(const VirtQ_ConfigType *config)
{
	if(FALSE == virtq.initialized)
	{
		virtq.config = config;
		virtq.initialized = TRUE;
	}
	else
	{
		assert(0);
	}
}
void VirtQ_RxNotificatin(VirtQ_ChannerlType chl)
{
	assert(virtq.initialized);
	if(virtq.config->queueConfig[chl].rxNotification)
	{
		virtq.config->queueConfig[chl].rxNotification(chl);
	}
}
void VirtQ_TxConfirmation(VirtQ_ChannerlType chl)
{

}
