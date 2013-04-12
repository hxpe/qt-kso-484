/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the test suite of the Qt Toolkit.
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
#ifndef TESTTYPES_H
#define TESTTYPES_H

#include <QtCore/qobject.h>
#include <QtDeclarative/qdeclarative.h>

class MyQmlObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int result READ result WRITE setResult)
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(MyQmlObject *object READ object WRITE setObject NOTIFY objectChanged)
    Q_PROPERTY(QDeclarativeListProperty<QObject> data READ data)
    Q_CLASSINFO("DefaultProperty", "data")
public:
    MyQmlObject() : m_result(0), m_value(0), m_object(0) {}

    int result() const { return m_result; }
    void setResult(int r) { m_result = r; }

    int value() const { return m_value; }
    void setValue(int v) { m_value = v; emit valueChanged(); }

    QDeclarativeListProperty<QObject> data() { return QDeclarativeListProperty<QObject>(this, m_data); }

    MyQmlObject *object() const { return m_object; }
    void setObject(MyQmlObject *o) { m_object = o; emit objectChanged(); }

signals:
    void valueChanged();
    void objectChanged();

private:
    QList<QObject *> m_data;
    int m_result;
    int m_value;
    MyQmlObject *m_object;
};
QML_DECLARE_TYPE(MyQmlObject);

void registerTypes();

#endif // TESTTYPES_H
