/*
 *  Copyright (C) 2010 Tuomo Penttinen, all rights reserved.
 *
 *  Author: Tuomo Penttinen <tp@herqq.org>
 *
 *  This file is part of Herqq UPnP (HUPnP) library.
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

#ifndef HCONTROL_POINT_P_H_
#define HCONTROL_POINT_P_H_

//
// !! Warning !!
//
// This file is not part of public API and it should
// never be included in client code. The contents of this file may
// change or the file may be removed without of notice.
//

#include "hcontrolpoint.h"
#include "hdevicebuild_p.h"

#include "./../habstracthost_p.h"
#include "./../../devicemodel/hdevice.h"
#include "./../../devicemodel/hservice.h"

#include "./../../ssdp/hssdp.h"
#include "./../../ssdp/hssdp_p.h"
#include "./../../http/hhttp_server_p.h"
#include "./../../ssdp/hdiscovery_messages.h"

#include <QHash>
#include <QUuid>
#include <QMutex>
#include <QScopedPointer>

class QString;
class QtSoapMessage;
class QHttpRequestHeader;

//
// !! Warning !!
//
// This file is not part of public API and it should
// never be included in client code. The contents of this file may
// change or the file may be removed without of notice.
//

namespace Herqq
{

namespace Upnp
{

class HServiceSubscribtion;
class HControlPointPrivate;

//
// The HTTP server the control point uses to receive event notifications.
//
class ControlPointHttpServer :
    public HHttpServer
{
Q_OBJECT
H_DISABLE_COPY(ControlPointHttpServer)

private:

    HControlPointPrivate* m_owner;

protected:

    virtual void incomingNotifyMessage(MessagingInfo&, const NotifyRequest&);

public:

    explicit ControlPointHttpServer(HControlPointPrivate*);
    virtual ~ControlPointHttpServer();
};

//
//
//
class HControlPointSsdpHandler :
    public HSsdp
{
H_DISABLE_COPY(HControlPointSsdpHandler)

private:

    HControlPointPrivate* m_owner;

protected:

    virtual bool incomingDiscoveryResponse(
        const HDiscoveryResponse& msg,
        const HEndpoint& source);

    virtual bool incomingDeviceAvailableAnnouncement(
        const HResourceAvailable& msg);

    virtual bool incomingDeviceUnavailableAnnouncement(
        const HResourceUnavailable& msg);

public:

    HControlPointSsdpHandler(HControlPointPrivate*);
    virtual ~HControlPointSsdpHandler();
};

//
// Implementation details of HControlPoint
//
class H_UPNP_CORE_EXPORT HControlPointPrivate :
    public HAbstractHostPrivate
{
Q_OBJECT
H_DECLARE_PUBLIC(HControlPoint)
H_DISABLE_COPY(HControlPointPrivate)
friend class DeviceBuildTask;
friend class HControlPointSsdpHandler;

private:

    DeviceBuildTasks m_deviceBuildTasks;
    // this is accessed only from the thread in which all the HUpnp objects live.

private Q_SLOTS:

    void deviceModelBuildDone(const Herqq::Upnp::HUdn&);

private:

    void subscribeToEvents(HDeviceController*);

    void removeRootDeviceAndSubscriptions(
        HDeviceController* rootDevice, bool unsubscribe);

    void removeRootDeviceSubscriptions(
        HDeviceController* rootDevice, bool unsubscribe);

    HActionInvoke createActionInvoker(HAction*);

    virtual void doClear();

    template<class Msg>
    bool processDeviceDiscovery(
        const Msg& msg, const HEndpoint& source = HEndpoint());

    template<class Msg>
    bool shouldFetch(const Msg& msg);

private Q_SLOTS:

    void deviceExpired(HDeviceController* source);
    void addRootDevice_(HDeviceController* device);

public:

    QScopedPointer<HControlPointConfiguration> m_initParams;
    HControlPointSsdpHandler* m_ssdp;

    ControlPointHttpServer* m_server;
    QHash<QUuid, QSharedPointer<HServiceSubscribtion> > m_serviceSubscribtions;
    QMutex m_serviceSubscribtionsMutex;

    QMutex m_deviceCreationMutex;
    //

    HControlPointPrivate();
    virtual ~HControlPointPrivate();

    HDeviceController* buildDevice(QUrl deviceLocation, qint32 maxAge);
};

}
}

#endif /* HCONTROL_POINT_P_H_ */
