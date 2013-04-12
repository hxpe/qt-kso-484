/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include "qmeegooverlaywidget.h"
#include "qmeegographicssystemhelper.h"
#include "qmeegoruntime.h"

QMeeGoOverlayWidget::QMeeGoOverlayWidget(int surfaceWidth, int surfaceHeight, QWidget *parent) : QWidget(parent, 0), 
    sw(surfaceWidth), 
    sh(surfaceHeight)
{
    if (! QMeeGoGraphicsSystemHelper::isRunningMeeGo())
        qFatal("QMeeGoOverlayWidget can only be used when running with 'meego' graphics system!");

    const QRect desktop = QApplication::desktop()->screenGeometry(parent);

    QMeeGoRuntime::setSurfaceFixedSize(surfaceWidth, surfaceHeight);

    scaleW = sw / desktop.width();
    scaleH = sh / desktop.height();
    installEventFilter(this);
}

QPoint QMeeGoOverlayWidget::convertPoint(const QPoint &p)
{
    int x = p.x() * scaleW;
    int y = p.y() * scaleH;
    return QPoint(x, y);
}

void QMeeGoOverlayWidget::showEvent(QShowEvent *)
{
    QMeeGoRuntime::setSurfaceScaling(0, 0, width(), height());
}

bool QMeeGoOverlayWidget::eventFilter(QObject *, QEvent *event)
{
    if (event->spontaneous() == false)
        return false;

    switch(event->type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        {
            QMouseEvent *e = static_cast <QMouseEvent *>(event);
            QMouseEvent newEvent = QMouseEvent(e->type(), 
                                               convertPoint(e->pos()), 
                                               convertPoint(e->globalPos()), 
                                               e->button(), 
                                               e->buttons(), 
                                               e->modifiers());
            QCoreApplication::sendEvent(this, &newEvent);
            return true;
        }

        default:
            return false;
    }
}
