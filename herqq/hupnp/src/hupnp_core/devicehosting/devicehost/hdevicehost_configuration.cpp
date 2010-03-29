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

#include "hdevicehost_configuration.h"
#include "hdevicehost_configuration_p.h"

#include <QFile>

namespace Herqq
{

namespace Upnp
{

/*******************************************************************************
 * HDeviceConfigurationPrivate
 ******************************************************************************/
HDeviceConfigurationPrivate::HDeviceConfigurationPrivate() :
    m_pathToDeviceDescriptor(), m_cacheControlMaxAgeInSecs(1800),
    m_deviceCreator()
{
}

/*******************************************************************************
 * HDeviceConfiguration
 ******************************************************************************/
HDeviceConfiguration::HDeviceConfiguration() :
    h_ptr(new HDeviceConfigurationPrivate())
{
}

HDeviceConfiguration::~HDeviceConfiguration()
{
    delete h_ptr;
}

HDeviceConfiguration* HDeviceConfiguration::doClone() const
{
    return new HDeviceConfiguration();
}

HDeviceConfiguration* HDeviceConfiguration::clone() const
{
    HDeviceConfiguration* newClone = doClone();
    if (!newClone) { return 0; }

    *newClone->h_ptr = *h_ptr;

    return newClone;
}

QString HDeviceConfiguration::pathToDeviceDescription() const
{
    return h_ptr->m_pathToDeviceDescriptor;
}

bool HDeviceConfiguration::setPathToDeviceDescription(
    const QString& pathToDeviceDescriptor)
{
    if (!QFile::exists(pathToDeviceDescriptor))
    {
        return false;
    }

    h_ptr->m_pathToDeviceDescriptor = pathToDeviceDescriptor;
    return true;
}

void HDeviceConfiguration::setCacheControlMaxAge(quint32 maxAgeInSecs)
{
    if (maxAgeInSecs < 5)
    {
        maxAgeInSecs = 5;
    }
    else if (maxAgeInSecs > 60*60*24)
    {
        maxAgeInSecs = 60*60*24; // a day
    }

    h_ptr->m_cacheControlMaxAgeInSecs = maxAgeInSecs;
}

quint32 HDeviceConfiguration::cacheControlMaxAge() const
{
    return h_ptr->m_cacheControlMaxAgeInSecs;
}

HDeviceCreator HDeviceConfiguration::deviceCreator() const
{
    return h_ptr->m_deviceCreator;
}

void HDeviceConfiguration::setDeviceCreator(
    HDeviceCreator deviceCreator)
{
    h_ptr->m_deviceCreator = deviceCreator;
}

bool HDeviceConfiguration::isValid() const
{
    return !h_ptr->m_pathToDeviceDescriptor.isEmpty() && h_ptr->m_deviceCreator;
}

/*******************************************************************************
 * HDeviceHostConfigurationPrivate
 ******************************************************************************/
HDeviceHostConfigurationPrivate::HDeviceHostConfigurationPrivate() :
    m_collection(), m_individualAdvertisementCount(2)
{
}

/*******************************************************************************
 * HDeviceHostConfiguration
 ******************************************************************************/
HDeviceHostConfiguration::HDeviceHostConfiguration() :
    h_ptr(new HDeviceHostConfigurationPrivate())
{
}

HDeviceHostConfiguration::HDeviceHostConfiguration(
    const HDeviceConfiguration& arg) :
        h_ptr(new HDeviceHostConfigurationPrivate())
{
    add(arg);
}

HDeviceHostConfiguration* HDeviceHostConfiguration::doClone() const
{
    return new HDeviceHostConfiguration();
}

HDeviceHostConfiguration* HDeviceHostConfiguration::clone() const
{
    HDeviceHostConfiguration* newClone = doClone();
    if (!newClone) { return 0; }

    foreach(HDeviceConfiguration* arg, h_ptr->m_collection)
    {
        newClone->add(*arg);
    }

    newClone->h_ptr->m_individualAdvertisementCount =
        h_ptr->m_individualAdvertisementCount;

    return newClone;
}

HDeviceHostConfiguration::~HDeviceHostConfiguration()
{
    qDeleteAll(h_ptr->m_collection);
    delete h_ptr;
}

bool HDeviceHostConfiguration::add(const HDeviceConfiguration& arg)
{
    if (arg.isValid())
    {
        h_ptr->m_collection.push_back(arg.clone());
        return true;
    }

    return false;
}

QList<HDeviceConfiguration*> HDeviceHostConfiguration::deviceConfigurations() const
{
    return h_ptr->m_collection;
}

quint32 HDeviceHostConfiguration::individualAdvertisementCount() const
{
    return h_ptr->m_individualAdvertisementCount;
}

void HDeviceHostConfiguration::setIndividualAdvertisementCount(quint32 arg)
{
    h_ptr->m_individualAdvertisementCount = arg;
}

bool HDeviceHostConfiguration::isEmpty() const
{
    return h_ptr->m_collection.isEmpty();
}

}
}