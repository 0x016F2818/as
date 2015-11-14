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
#ifndef ENTRY_H
#define ENTRY_H
#include <QtWidgets>
#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include "ardevice.h"
#include <QMap>
#include <vEcu.h>

typedef QMap<QString, arDevice*> map_device_t;
typedef QMap<QString, vEcu*> map_ecu_t;
// ====================== [ ICON RESOURCES ] =======================
class Entry: public QMainWindow
{
Q_OBJECT
private:
	map_device_t map_device;
    map_ecu_t map_ecu;
	QMenu* menuBSW /* Basic Software Modules*/;
	QMenu* menuVD /* Virtual Device */;

    QToolBar* toolbar;
public:
	explicit Entry ( QWidget *parent = 0 );
	~Entry ( );
	static class Entry* Self ( void );

	void registerDevice ( arDevice* device );
	void deleteDevice ( QString name );
	arDevice* getDevice ( QString name );

    void registerEcu ( vEcu* ecu );
    void deleteEcu ( QString name );
    vEcu* getEcu ( QString name );

    void Can_Write(uint8_t busid,uint32_t canid,uint8_t dlc,uint8_t* data);
    void Can_RxIndication(uint8_t busid,uint32_t canid,uint8_t dlc,uint8_t* data);
signals:

protected:

private slots:
	void save ( void );
	void open ( void );
private:
	void createMenuAndToolbar ( void );

};

#endif // ENTRY_H
