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
#ifndef RELEASE_ASCORE_VIRTUAL_INCLUDE_VECU_H_
#define RELEASE_ASCORE_VIRTUAL_INCLUDE_VECU_H_
/* ============================ [ INCLUDES  ] ====================================================== */
#include <stdint.h>
#include <QThread>
#include <QString>
#include <QDebug>
#include <assert.h>
#include "Virtio.h"
/* ============================ [ MACROS    ] ====================================================== */

/* ============================ [ TYPES     ] ====================================================== */
typedef void* (*PF_MAIN)(void);

/* ============================ [ CLASS     ] ====================================================== */
class vEcu: public QThread
{
Q_OBJECT
private:
    void* hxDll;
    PF_MAIN pfMain;
    QString name;
    Virtio* virtio;

public:
    explicit vEcu ( QString dll, QObject *parent = 0);
    ~vEcu ( );
private:
    void run(void);
signals:

protected:

private slots:

};
/* ============================ [ DATAS     ] ====================================================== */
/* ============================ [ DECLARES  ] ====================================================== */
/* ============================ [ LOCALS    ] ====================================================== */
/* ============================ [ FUNCTIONS ] ====================================================== */
#endif /* RELEASE_ASCORE_VIRTUAL_INCLUDE_VECU_H_ */
