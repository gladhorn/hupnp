/*
 *  Copyright (C) 2010 Tuomo Penttinen, all rights reserved.
 *
 *  Author: Tuomo Penttinen <tp@herqq.org>
 *
 *  This file is part of Herqq UPnP (HUPnP) library.
 *
 *  Herqq UPnP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Herqq UPnP is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with Herqq UPnP. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HACTIONS_SETUPDATA_H_
#define HACTIONS_SETUPDATA_H_

#include <HUpnpCore/HActionInvoke>

#include <QtCore/QHash>
#include <QtCore/QString>

namespace Herqq
{

namespace Upnp
{

class HActionSetupPrivate;

/*!
 * This class is used to specify information that is required to setup an
 * HAction.
 *
 * \headerfile hactions_setupdata.h HActionSetup
 *
 * \ingroup hupnp_devicemodel
 *
 * \sa HActionsSetupData, HAction
 *
 * \remarks this class is not thread-safe.
 */
class H_UPNP_CORE_EXPORT HActionSetup
{
private:

    HActionSetupPrivate* h_ptr;

public:

    /*!
     * Creates a new, invalid instance.
     *
     * \sa isValid()
     */
    HActionSetup();

    /*!
     * Creates a new instance.
     *
     * \param name specifies the name of the action.
     *
     * \param incReq specifies the \e inclusion \e requirement of the action.
     *
     * \sa isValid()
     *
     * \remarks the version() is set to 1.
     */
    explicit HActionSetup(
        const QString& name,
        HInclusionRequirement incReq = InclusionMandatory);

    /*!
     * Creates a new instance.
     *
     * \param name specifies the name of the action.
     *
     * \param version specifies the UPnP service version in which the action
     * was first specified.
     *
     * \param incReq specifies the \e inclusion \e requirement of the action.
     *
     * \sa isValid()
     */
    HActionSetup(
        const QString& name,
        qint32 version,
        HInclusionRequirement incReq = InclusionMandatory);

    /*!
     * Creates a new instance.
     *
     * \param name specifies the name of the action.
     *
     * \param invoke specifies the callable entity that is called when the
     * action is invoked. This is used only at server side.
     *
     * \param incReq specifies the \e inclusion \e requirement of the action.
     *
     * \sa isValid()
     *
     * \remarks the version() is set to 1.
     */
    HActionSetup(
        const QString& name,
        const HActionInvoke& invoke,
        HInclusionRequirement incReq = InclusionMandatory);

    /*!
     * Creates a new instance.
     *
     * \param name specifies the name of the action.
     *
     * \param invoke specifies the callable entity that is called when the
     * action is invoked. This is used only at server side.
     *
     * \param version specifies the UPnP service version in which the action
     * was first specified.
     *
     * \param incReq specifies the \e inclusion \e requirement of the action.
     *
     * \sa isValid()
     */
    HActionSetup(
        const QString& name,
        const HActionInvoke& invoke,
        qint32 version,
        HInclusionRequirement incReq = InclusionMandatory);

    /*!
     * Copy constructor.
     *
     * Creates a copy of \a other.
     */
    HActionSetup(const HActionSetup&);

    /*!
     * Assignment operator.
     *
     * Copies the contents of \a other to this.
     */
    HActionSetup& operator=(const HActionSetup&);

    /*!
     * Destroys the instance.
     *
     * Destroys the instance.
     */
    ~HActionSetup();

    /*!
     * Returns the setup information of the action's input arguments.
     *
     * \return the setup information of the action's input arguments.
     *
     * \sa setInputArguments()
     */
    const HActionArguments& inputArguments() const;

    /*!
     * Returns the setup information of the action's output arguments.
     *
     * \return the setup information of the action's output arguments.
     *
     * \sa setOutputArguments()
     */
    const HActionArguments& outputArguments() const;

    /*!
     * Returns the callable entity that is called when the
     * action is invoked.
     *
     * \remarks This is used only at server side.
     *
     * \sa setActionInvoke()
     */
    HActionInvoke actionInvoke() const;

    /*!
     * Returns the <em>inclusion requirement</em> of the action.
     *
     * \return the <em>inclusion requirement</em> of the action.
     *
     * \sa setInclusionRequirement()
     */
    HInclusionRequirement inclusionRequirement() const;

    /*!
     * Indicates if the object is valid.
     *
     * \return \e true in case the object is valid, that is,
     * the name(), version() and the inclusionRequirement() are properly defined.
     */
    bool isValid() const;

    /*!
     * Returns the name of the action.
     *
     * \return the name of the action.
     *
     * \sa setName()
     */
    QString name() const;

    /*!
     * Returns the UPnP service version in which the action
     * was first specified.
     *
     * \return the UPnP service version in which the action
     * was first specified.
     *
     * \sa setVersion()
     */
    qint32 version() const;

    /*!
     * Specifies the action's input arguments.
     *
     * \param args specifies the setup information for the action's input arguments.
     *
     * \sa inputArguments()
     */
    void setInputArguments(const HActionArguments& args);

    /*!
     * Specifies the action's output arguments.
     *
     * \param args specifies the setup information for the action's output arguments.
     *
     * \sa outputArguments()
     */
    void setOutputArguments(const HActionArguments& args);

    /*!
     * Specifies the callable entity that is called when the
     * action is invoked.
     *
     * \param arg specifies the callable entity that is called when the
     * action is invoked.
     *
     * \remarks This is used only at server side.
     *
     * \sa actionInvoke()
     */
    void setActionInvoke(const HActionInvoke& arg);

    /*!
     * Sets the name of the action.
     *
     * \param name specifies the name of the action.
     *
     * \param err is a pointer to a \c QString that contains an error description
     * in case the name could not be set. This is an optional parameter.
     *
     * \return \e true in case the specified name was successfully set.
     *
     * \sa name()
     */
    bool setName(const QString& name, QString* err = 0);

     /*!
     * Sets the <em>inclusion requirement</em> of the action.
     *
     * \param arg specifies the <em>inclusion requirement</em> of the action.
     *
     * \sa inclusionRequirement()
     */
    void setInclusionRequirement(HInclusionRequirement arg);

    /*!
     * Specifies the UPnP service version in which the action
     * was first specified.
     *
     * \param version specifies the UPnP service version in which the action
     * was first specified.
     *
     * \sa version()
     */
    void setVersion(qint32 version);
};

/*!
 * This class is used to specify information that is required to setup the
 * \c HActions of an HService.
 *
 * \headerfile hactions_setupdata.h HActionsSetupData
 *
 * \ingroup hupnp_devicemodel
 *
 * \remarks this class is not thread-safe.
 *
 * \sa HActionSetup
 */
class H_UPNP_CORE_EXPORT HActionsSetupData
{

private:

    QHash<QString, HActionSetup> m_actionSetupInfos;

public:

    /*!
     * Creates a new, empty instance.
     *
     * \sa isEmpty()
     */
    HActionsSetupData();

    /*!
     * Inserts a new item.
     *
     * \param newItem specifies the item to be added.
     *
     * \return \e true in case the item was added. The item will not be added
     * if the instance already contains an item with the
     * same name as \c newItem.
     *
     * \sa remove()
     */
    bool insert(const HActionSetup& newItem);

    /*!
     * Removes an existing item.
     *
     * \param name specifies the name of the item to be removed.
     *
     * \return \e true in case the item was found and removed.
     *
     * \sa insert()
     */
    bool remove(const QString& name);

    /*!
     * Retrieves an action setup object.
     *
     * \param name specifies the name of the item to be retrieved.
     *
     * \return the item with the specified name. Note that the returned item
     * is invalid, i.e. HActionSetup::isValid() returns false in case no item
     * with the specified name was found.
     *
     * \sa contains()
     */
    HActionSetup get(const QString& name) const;

    /*!
     * This is a convenience method for setting the callable entity that is
     * called when the action is invoked.
     *
     * \param name specifies the name of the item.
     *
     * \param actionInvoke specifies the callable entity.
     *
     * \return \e true in case the item was found and its callable entity was set.
     *
     * \remark
     * HActionInvoke is a server-side concept.
     */
    bool setInvoke(
        const QString& name, const HActionInvoke& actionInvoke);

    /*!
     * This is a convenience method for setting the inclusion requirement
     * element of an item.
     *
     * \param name specifies the name of the item.
     *
     * \param incReq specifies the inclusion requirement value.
     *
     * \return \e true when the item was found and the inclusion requirement
     * element was set.
     */
    bool setInclusionRequirement(
        const QString& name, HInclusionRequirement incReq);

    /*!
     * Indicates if the instance contains an item with the specified name.
     *
     * \param name specifies the name of the item.
     *
     * \return \e true when the instance contains an item with the specified name.
     *
     * \sa get()
     */
    inline bool contains(const QString& name) const
    {
        return m_actionSetupInfos.contains(name);
    }

    /*!
     * Returns the names of the contained items.
     *
     * \return the names of the contained items.
     */
    QSet<QString> names() const;

    /*!
     * Returns the number of contained items.
     *
     * \return the number of contained items.
     */
    inline qint32 size() const
    {
        return m_actionSetupInfos.size();
    }

    /*!
     * Indicates if the object is empty.
     *
     * \return \e true in case the instance has no items.
     */
    inline bool isEmpty() const
    {
        return m_actionSetupInfos.isEmpty();
    }

    /*!
     * Removes every contained object.
     */
    inline void clear()
    {
        m_actionSetupInfos.clear();
    }
};

}
}

#endif /* HACTIONS_SETUPDATA_H_ */