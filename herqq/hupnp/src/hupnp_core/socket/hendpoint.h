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


#ifndef HENDPOINT_H
#define HENDPOINT_H

#include "./../general/hdefs_p.h"

class QUrl;
class QHostAddress;

namespace Herqq
{

namespace Upnp
{

class HEndpointPrivate;

/*!
 * Class that represents a network endpoint, which is a combination of
 * a host address and a port number.
 *
 * \headerfile hendpoint.h HEndpoint
 *
 * \remark this class provides an assignment operator that is not thread-safe.
 */
class H_UPNP_CORE_EXPORT HEndpoint
{
friend H_UPNP_CORE_EXPORT bool operator==(const HEndpoint& ep1, const HEndpoint& ep2);

private:

    HEndpointPrivate* h_ptr;

public:

    /*!
     * Creates a new instance with host address set to "Null" and port set to "0".
     */
    HEndpoint();

    /*!
     * Creates a new instance with the specified host address and port set to "0".
     *
     * \param hostAddress specifies the host address.
     */
    HEndpoint(const QHostAddress& hostAddress);

    /*!
     * Creates a new instance with the specified host address and port.
     *
     * \param hostAddress specifies the host address.
     * \param portNumber specifies the port number.
     */
    HEndpoint(const QHostAddress& hostAddress, quint16 portNumber);

    /*!
     * Creates a new instance from the specified url.
     *
     * \param url specifies the url from which the endpoint and port information
     * is extracted (if present).
     */
    HEndpoint(const QUrl& url);

    /*!
     * Creates a new instance from the specified string following format
     * "hostAddress:portNumber", where everything after hostAddress is optional.
     *
     * \param arg specifies the string following format "hostAddress:portNumber",
     * from which the contents are extracted.
     */
    HEndpoint(const QString& arg);

    /*!
     * Copy constructor.
     */
    HEndpoint(const HEndpoint&);

    /*!
     * Assignment operator.
     */
    HEndpoint& operator=(const HEndpoint&);

    /*!
     * Destroys the instance.
     */
    ~HEndpoint();

    /*!
     * Indicates whether or not the end point is properly defined.
     *
     * \return true in case the end point is not defined.
     */
    bool isNull() const;

    /*!
     * Returns the host address of the endpoint.
     *
     * \return the host address of the endpoint.
     */
    QHostAddress hostAddress() const;

    /*!
     * Returns the port number of the endpoint.
     *
     * \return the port number of the endpoint.
     */
    quint16 portNumber () const;

    /*!
     * Indicates whether or not the end point refers to a multicast address.
     *
     * \retval true in case the end point refers to a multicast address.
     */
    bool isMulticast() const;

    /*!
     * Returns a string representation of the endpoint.
     *
     * \return the address and port number together separated by a ":". E.g
     * \c "192.168.0.1:80"
     */
    QString toString() const;
};

/*!
 * Compares the two objects for equality.
 *
 * \return \e true in case the object are logically equivalent.
 *
 * \relates HEndpoint
 */
H_UPNP_CORE_EXPORT bool operator==(const HEndpoint&, const HEndpoint&);

/*!
 * Compares the two objects for inequality.
 *
 * \return \e true in case the object are not logically equivalent.
 *
 * \relates HEndpoint
 */
H_UPNP_CORE_EXPORT bool operator!=(const HEndpoint&, const HEndpoint&);

}
}

#endif // HENDPOINT_H