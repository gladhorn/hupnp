/*
 *  Copyright (C) 2010 Tuomo Penttinen, all rights reserved.
 *
 *  Author: Tuomo Penttinen <tp@herqq.org>
 *
 *  This file is part of an application named HUpnpSimpleTestApp
 *  used for demonstrating how to use the Herqq UPnP (HUPnP) library.
 *
 *  Herqq UPnP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Herqq UPnP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Herqq UPnP. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CONTROLPOINT_NAVIGATORITEM_H_
#define CONTROLPOINT_NAVIGATORITEM_H_

#include <HUpnp>
#include <HDevice>

#include <QString>

template<typename T>
class QList;

class QVariant;
class ActionItem;
class DeviceItem;
class ServiceItem;
class StateVariableItem;

//
//
//
class ControlPointNavigatorItemVisitor
{
public:

    virtual void visit(ActionItem*) = 0;
    virtual void visit(ServiceItem*) = 0;
    virtual void visit(DeviceItem*) = 0;
    virtual void visit(StateVariableItem*) = 0;
};

//
//
//
class ControlPointNavigatorItem
{
protected:

    QList<ControlPointNavigatorItem*> m_childItems;
    ControlPointNavigatorItem*        m_parentItem;

public:

    explicit ControlPointNavigatorItem(ControlPointNavigatorItem* parent = 0);
    virtual ~ControlPointNavigatorItem();

    virtual QVariant data(int column) const = 0;

    void appendChild(ControlPointNavigatorItem* child);
    void removeChild(qint32 row);
    ControlPointNavigatorItem* child (int row);

    int childCount () const;
    int columnCount() const;

    int row        () const;
    ControlPointNavigatorItem* parent();

    int rowCount   () const;

    virtual void accept(ControlPointNavigatorItemVisitor*) = 0;
};

//
//
//
class RootItem :
    public ControlPointNavigatorItem
{
private:

public:

    explicit RootItem(ControlPointNavigatorItem* parent = 0);
    virtual ~RootItem();

    virtual QVariant data (int column) const;

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

//
//
//
class ContainerItem :
    public ControlPointNavigatorItem
{
private:

    QString m_name;

public:

    explicit ContainerItem(
        const QString& name, ControlPointNavigatorItem* parent = 0);

    virtual ~ContainerItem();

    virtual QVariant data (int column) const;

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

//
//
//
class DeviceItem :
    public ControlPointNavigatorItem
{
private:

    Herqq::Upnp::HDevice* m_device;

public:

    explicit DeviceItem(
        Herqq::Upnp::HDevice* device,
        ControlPointNavigatorItem* parent = 0);

    virtual ~DeviceItem();

    virtual QVariant data (int column) const;

    inline Herqq::Upnp::HDevice* device() const { return m_device; }

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

//
//
//
class ServiceItem :
    public ControlPointNavigatorItem
{
private:

    Herqq::Upnp::HService* m_service;

public:

    explicit ServiceItem(
        Herqq::Upnp::HService* service, ControlPointNavigatorItem* parent = 0);

    virtual ~ServiceItem();

    virtual QVariant data (int column) const;

    inline Herqq::Upnp::HService* service() const { return m_service; }

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

//
//
//
class ActionItem :
    public ControlPointNavigatorItem
{
private:

    Herqq::Upnp::HAction* m_action;

public:

    explicit ActionItem(
        Herqq::Upnp::HAction* action,
        ControlPointNavigatorItem* parent = 0);

    virtual ~ActionItem();

    virtual QVariant data (int column) const;

    inline Herqq::Upnp::HAction* action() const { return m_action; }

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

//
//
//
class StateVariableItem :
    public ControlPointNavigatorItem
{
private:

    Herqq::Upnp::HStateVariable* m_stateVar;

public:

    explicit StateVariableItem(
        Herqq::Upnp::HStateVariable* stateVar,
        ControlPointNavigatorItem* parent = 0);

    virtual ~StateVariableItem();

    virtual QVariant data (int column) const;

    inline Herqq::Upnp::HStateVariable* stateVariable() const
    {
        return m_stateVar;
    }

    virtual void accept(ControlPointNavigatorItemVisitor*);
};

#endif /* CONTROLPOINT_NAVIGATORITEM_H_ */
