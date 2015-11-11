/**
 * WhatsApp - the open source AUTOSAR platform https://github.com/parai
 *
 * Copyright (C) 2014  WhatsApp <parai@foxmail.com>
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
#include "entry.h"
#include "arcan.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
static class Entry* self = NULL;

Entry::Entry ( QWidget *parent )
		: QMainWindow(parent)
{
	self = this;
	this->setWindowTitle(tr("WhatsApp ( parai@foxmail.com )"));
	this->createMenuAndToolbar();

    this->setGeometry(50,50,600,20);

    registerDevice(new arCan(CAN_DEVICE_NAME,CAN_CTRL_NUM));
}

class Entry* Entry::Self ( void )
{
	return self;
}
Entry::~Entry ( )
{
	map_device.clear(); // this in charge to destroy VirtualDevice
}

void Entry::registerDevice ( arDevice* device )
{
	if ( map_device.contains(device->Name()) )
	{
		qDebug() << "System error: Device " << device->Name() << " re-registeration!\n";
		delete device;
	}
	else
	{
		map_device[device->Name()] = device;
		QAction * action = new QAction(device->Name(),this);
		this->connect(action,SIGNAL(triggered()),device,SLOT(wakeup()));
		menuVD->addAction(action);
	}
}
void Entry::deleteDevice ( QString name )
{
	if ( map_device.contains(name) )
	{
		arDevice* device = map_device.take(name);
		delete device;
		/* menu virtual device re-build */
		menuVD->clear();
		QList<arDevice*> devices = 	map_device.values();
		for(int i=0;i<devices.size();i++)
		{
			device = devices[i];
			QAction * action = new QAction(device->Name(),this);
			this->connect(action,SIGNAL(triggered()),device,SLOT(wakeup()));
			menuVD->addAction(action);
		}
	}
	else
	{
		qDebug() << "System error: Virtual device " << name  << " delete!\n";
	}
}

arDevice* Entry::getDevice ( QString name )
{
	if ( map_device.contains(name) )
	{
		return map_device[name];
	}
	else
	{
		return NULL;
	}
}

void Entry::registerEcu ( vEcu* ecu )
{
    if ( map_ecu.contains(ecu->Name()) )
    {
        qDebug() << "System error: ECU " << ecu->Name() << " re-registeration!\n";
        delete ecu;
    }
    else
    {
        map_ecu[ecu->Name()] = ecu;
        QAction * action = new QAction(ecu->Name(),this);
        this->connect(action,SIGNAL(triggered()),ecu,SLOT(start()));
        toolbar->addAction(action);
    }
}

void Entry::deleteEcu ( QString name )
{
    if ( map_device.contains(name) )
    {
        vEcu* ecu = map_ecu.take(name);
        delete ecu;
        /* menu virtual device re-build */
        toolbar->clear();
        QList<vEcu*> ecus = 	map_ecu.values();
        for(int i=0;i<ecus.size();i++)
        {
            ecu = ecus[i];
            QAction * action = new QAction(ecu->Name(),this);
            this->connect(action,SIGNAL(triggered()),ecu,SLOT(start()));
            toolbar->addAction(action);
        }
    }
    else
    {
        qDebug() << "System error: Virtual ECU " << name  << " delete!\n";
    }
}

vEcu* Entry::getEcu ( QString name )
{
    if ( map_ecu.contains(name) )
    {
        return map_ecu[name];
    }
    else
    {
        return NULL;
    }
}
void Entry::Can_Write(uint8_t busid,uint32_t canid,uint8_t dlc,uint8_t* data)
{
    vEcu* ecu;
    QList<vEcu*> ecus = 	map_ecu.values();
    for(int i=0;i<ecus.size();i++)
    {
        ecu = ecus[i];
        ecu->Can_Write(busid,canid,dlc,data);
    }
}

// ==================== [ SIGNALS       ] =====================================

// ==================== [ PRIVATE SLOTS ] ======================================
void Entry::open ( void )
{

}

void Entry::save ( void )
{

}

// ==================== [ PRIVATE FUNCTIONS ] ==================================
void Entry::createMenuAndToolbar ( void )
{
    toolbar = this->addToolBar("virtual machine");

    char* cwd = getcwd(NULL,0);
    ASLOG(OFF,cwd);
    chdir("../../out");
    char* workpath = getcwd(NULL,0);

    free(cwd);
    free(workpath);

    DIR* d = opendir(".");
    struct dirent *file;
    while((file = readdir(d)) != NULL)
    {
        if(strstr(file->d_name,".dll"))
        {
            ASLOG(OFF,"load %s\n",file->d_name);
            registerEcu(new vEcu(file->d_name));
        }
    }
    closedir(d);

	this->menuBSW = this->menuBar()->addMenu(tr("BSW"));

	this->menuVD = menuBSW->addMenu(tr("Device"));
}
