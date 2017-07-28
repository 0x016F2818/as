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
#ifdef USE_FATFS
/* ============================ [ INCLUDES  ] ====================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "diskio.h"
#include "asdebug.h"
#include "ext4.h"
#ifdef __LINUX__
#include "file_dev.h"
#else
#include "file_windows.h"
#endif

/* ============================ [ MACROS    ] ====================================================== */
#define AS_LOG_FATFS 0
#define AS_LOG_EXTFS 0
/* Definitions of physical drive number for each drive */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 0 : default */
#define DEV_RAM		1	/* Example: Map Ramdisk to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define FATFS_IMG	"FatFs.img"
#define EXTFS_IMG	"ExtFs.img"
/* ============================ [ TYPES     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	ASLOG(FATFS,"%s %d\n",__func__,pdrv);

	switch (pdrv) {
	case DEV_RAM :
		break;

	case DEV_MMC :
		stat = RES_OK;
		break;

	case DEV_USB :
		break;
	}
	return stat;
}

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
	ASLOG(FATFS,"%s %d\n",__func__,pdrv);
	switch (pdrv) {
	case DEV_RAM :
		break;

	case DEV_MMC :
	{
		FILE* fp = fopen(FATFS_IMG,"r");
		if(NULL == fp)
		{
			system("dd if=/dev/zero of=" FATFS_IMG " bs=1M count=32");
			system("sudo mkfs.fat " FATFS_IMG);
			ASLOG(FATFS,"simulation on new created 32Mb " FATFS_IMG "\n");
		}
		else
		{
			ASLOG(FATFS,"simulation on old " FATFS_IMG "\n");
			fclose(fp);
		}
		stat = 0;
		break;
	}
	case DEV_USB :
		break;
	}
	return stat;
}

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	ASLOG(FATFS,"%s %d %d %d\n",__func__,pdrv,sector,count);
	switch (pdrv) {
	case DEV_RAM :
		break;

	case DEV_MMC :
	{
		FILE* fp=fopen(FATFS_IMG,"r");
		asAssert(fp);
		fseek(fp,512*sector,SEEK_SET);
		int len=fread(buff,sizeof(char),count*512,fp);

		if(len!=count*512)
		{
			res=RES_ERROR;
		}
		else
		{
			res = RES_OK;
		}
		fclose(fp);
		break;
	}

	case DEV_USB :
		break;
	}

	return res;
}

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_PARERR;
	ASLOG(FATFS,"%s %d %d %d\n",__func__,pdrv,sector,count);
	switch (pdrv) {
	case DEV_RAM :
		break;

	case DEV_MMC :
	{
		FILE* fp=fopen(FATFS_IMG,"r+");
		asAssert(fp);
		fseek(fp,512*sector,SEEK_SET);
		int len=fwrite(buff,sizeof(char),count*512,fp);

		if(len!=count*512)
		{
			res=RES_ERROR;
		}
		else
		{
			res = RES_OK;
		}
		fclose(fp);
		break;
	}
	case DEV_USB :
		break;
	}

	return res;
}

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;
	ASLOG(FATFS,"%s %d %d\n",__func__,pdrv,cmd);
	switch (pdrv) {
	case DEV_RAM :
		break;

	case DEV_MMC :
	{
		switch (cmd) {
		case CTRL_SYNC:
			res = RES_OK;
			break;

		case GET_SECTOR_COUNT:
		{
			FILE* fp = fopen(FATFS_IMG,"r");
			asAssert(fp);
			fseek(fp, 0L, SEEK_END);
			*(DWORD*)buff = ftell(fp)/512;
			res = RES_OK;
			fclose(fp);
			break;
		}
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = 512;
			res = RES_OK;
			break;

		case GET_BLOCK_SIZE:
			*(DWORD*)buff = 1024*1024; /* 1M */
			res = RES_OK;
			break;

		}
		break;
	}
	case DEV_USB :
		break;
	}

	return res;
}

DWORD get_fattime (void)
{
	return time(0);
}

void ext_mount(void)
{
    int rc;
    struct ext4_blockdev * bd;
    FILE* fp = fopen(EXTFS_IMG,"r");
    if(NULL == fp)
    {
        system("dd if=/dev/zero of=" EXTFS_IMG " bs=1M count=32");
        system("sudo mkfs.ext4 " EXTFS_IMG);
        ASLOG(EXTFS,"simulation on new created 32Mb " EXTFS_IMG "\n");
    }
    else
    {
        ASLOG(EXTFS,"simulation on old " EXTFS_IMG "\n");
        fclose(fp);
    }

#ifdef __WINDOWS__
    file_windows_name_set(EXTFS_IMG);
    bd = file_windows_dev_get();

#else
    file_dev_name_set(EXTFS_IMG);
    bd = file_dev_get();
#endif
    rc = ext4_device_register(bd, EXTFS_IMG);
    if(rc != EOK)
    {
        ASLOG(EXTFS, "register ext4 device failed\n");
    }

	rc = ext4_mount(EXTFS_IMG, "/", false);
	if (rc != EOK)
    {
        ASLOG(EXTFS, "mount ext4 device failed\n");
    }

    ASLOG(EXTFS, "mount ext4 device " EXTFS_IMG " on '/‘ OK\n");
}
#endif /* USE_FATFS */
