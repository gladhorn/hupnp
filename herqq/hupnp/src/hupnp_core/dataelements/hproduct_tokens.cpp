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

#include "hproduct_tokens.h"

#include "./../../utils/hlogger_p.h"

#include <QRegExp>
#include <QVector>

namespace Herqq
{

namespace Upnp
{

/*******************************************************************************
 * HProductToken
 ******************************************************************************/
HProductToken::HProductToken() :
    m_token(), m_productVersion()
{
}

HProductToken::HProductToken(const QString& token, const QString& productVersion) :
    m_token(), m_productVersion()
{
    HLOG(H_AT, H_FUN);

    QString tokenTmp(token.simplified());
    QString productVersionTmp(productVersion.simplified());
    if (tokenTmp.isEmpty() || productVersionTmp.isEmpty())
    {
        HLOG_WARN(QString(
            "Invalid product token. Token: %1, Product Version: %2").arg(
                token, productVersion));

        return;
    }

    m_token = tokenTmp;
    m_productVersion = productVersionTmp;
}

HProductToken::~HProductToken()
{
}

QString HProductToken::toString() const
{
    if (!isValid())
    {
        return QString();
    }

    return QString("%1/%2").arg(m_token, m_productVersion);
}

bool HProductToken::isValidUpnpToken(const HProductToken& token)
{
    if (!token.isValid())
    {
        return false;
    }

    if (token.token().compare("UPnP", Qt::CaseInsensitive) != 0)
    {
        return false;
    }

    QString version = token.version();

    return (version.size() == 3    &&
           (version[0]     == '1') && 
            version[1]     == '.'  && 
           (version[2] == '0' || version[2] == '1'));
}

qint32 HProductToken::minorVersion(const HProductToken& token)
{
    if (!token.isValid())
    {
        return -1;
    }

    QString tokenVersion = token.version();

    qint32 separatorIndex = tokenVersion.indexOf('.');
    if (separatorIndex < 0)
    {
        return -1;
    }

    bool ok = false;

    qint32 minTmp = tokenVersion.mid(
        separatorIndex+1, tokenVersion.indexOf('.', separatorIndex+1)).toInt(&ok);

    return ok ? minTmp : -1;
}

qint32 HProductToken::majorVersion(const HProductToken& token)
{
    if (!token.isValid())
    {
        return -1;
    }

    QString tokenVersion = token.version();

    bool ok = false;
    qint32 majTmp = -1;
    qint32 separatorIndex = tokenVersion.indexOf('.');
    if (separatorIndex < 0)
    {
        majTmp = tokenVersion.toInt(&ok);
        return ok ? majTmp : -1;
    }

    majTmp = tokenVersion.left(separatorIndex).toInt(&ok);
    return ok ? majTmp : -1;
}

bool operator==(const HProductToken& obj1, const HProductToken& obj2)
{
    return obj1.toString() == obj2.toString();
}

bool operator!=(const HProductToken& obj1, const HProductToken& obj2)
{
    return !(obj1 == obj2);
}

/*******************************************************************************
 * HProductTokensPrivate
 ******************************************************************************/
class HProductTokensPrivate
{
private:

    // tries to parse the string into "token/version" pairs
    // the pairs have to be delimited with white-space or commas
    // a pair can contain "trailing" data until the last delimiter after which
    // the token of a new pair is started. for instance, this is valid:
    // token/version (some data; some more data) otherToken/otherVersion finalToken/finalVersion (data)
    bool parse(const QString& tokens)
    {
        HLOG(H_AT, H_FUN);

        QVector<HProductToken> productTokens;

        QString token, version, buf;

        qint32 i = tokens.indexOf('/'), j = 0, lastDelim = 0;
        if (i < 0)
        {
            return false;
        }

        token = tokens.left(i);
        // the first special case "token/version token/version token/version"
        //                         ^^^^^

        for(i = i + 1; i < tokens.size(); ++i, ++j)
        {
            if (tokens[i] == '/')
            {
                if (lastDelim <= 0)
                {
                    // there must have been at least one space between the previous '/'
                    // and this one. it is an error otherwise.
                    return false;
                }

                HProductToken newToken(token, buf.left(lastDelim));
                if (newToken.isValid()) { productTokens.append(newToken); }
                else                    { return false;                   }

                token = buf.mid(lastDelim+1);
                version.clear(); buf.clear(); j = -1;

                continue;
            }
            else if (tokens[i] == ' ')
            {
                lastDelim = j;
            }

            buf.append(tokens[i]);
        }

        HProductToken newToken(token, buf);
        if (newToken.isValid()) { productTokens.append(newToken); }
        else                    { return false;                   }

        // at this point the provided token string is parsed into
        // valid token/version pairs, but it is not known if the tokens string
        // contained the UPnP token + we should inform the user if
        // non-std input was given.

        if (productTokens.size() < 3 ||
           !HProductToken::isValidUpnpToken(productTokens[1]))
        {
            HLOG_WARN_NONSTD(QString(
                "The specified token string [%1] is not formed according "
                "to the UDA specification").arg(tokens));
            return false;
        }

        m_productTokens = productTokens;
        return true;
    }

public:

    QString m_originalTokenString;
    QVector<HProductToken> m_productTokens;

public:

    HProductTokensPrivate() :
        m_originalTokenString(), m_productTokens()
    {
    }

    HProductTokensPrivate(const QString& tokens) :
        m_originalTokenString(tokens.simplified()), m_productTokens()
    {
        HLOG(H_AT, H_FUN);

        bool ok = parse(m_originalTokenString);
        if (ok)
        {
            // the string followed the UDA closely (rare, unfortunately)
            return;
        }

        if (m_originalTokenString.contains(','))
        {
            // some sloppy UPnP implementations uses the comma as the delimiter.
            // technically, comma could be part of the "version" part of the token,
            // but in practice, it if is present it is used as the delimiter.

            ok = parse(QString(m_originalTokenString).remove(','));
            if (ok)
            {
                HLOG_WARN_NONSTD(QString(
                    "Comma should not be used as a delimiter in "
                    "product tokens: [%1]").arg(tokens));

                return;
            }
        }

        if (!ok)
        {
            // tokenization failed.
            // fall back for scanning the UPnP/version only
            QRegExp rexp("(\\b|\\s+)UPnP/");
            qint32 index = m_originalTokenString.indexOf(
                rexp, Qt::CaseInsensitive);

            if (index >= 0)
            {
                qint32 matchedLength = rexp.matchedLength();
                qint32 slash = index + matchedLength;
                qint32 nextDelim =
                    m_originalTokenString.indexOf(QRegExp("\\s|,"), slash);

                HProductToken token(
                    m_originalTokenString.mid(index, matchedLength-1),
                    m_originalTokenString.mid(slash,
                        nextDelim < 0 ? -1 : nextDelim-slash));

                if (HProductToken::isValidUpnpToken(token))
                {
                    m_productTokens.push_back(token);
                }
            }
            else
            {
                HLOG_WARN_NONSTD(QString(
                    "Missing the mandatory UPnP token [%1]: ").arg(
                        m_originalTokenString));
            }
        }
    }
};


/*******************************************************************************
 * HProductTokens
 ******************************************************************************/
HProductTokens::HProductTokens() :
    h_ptr(new HProductTokensPrivate())
{
}

HProductTokens::HProductTokens(const QString& tokens) :
    h_ptr(new HProductTokensPrivate(tokens))
{
}

HProductTokens::HProductTokens(const HProductTokens& other) :
    h_ptr(new HProductTokensPrivate(*other.h_ptr))
{
}

HProductTokens& HProductTokens::operator=(const HProductTokens& other)
{
    HProductTokensPrivate* newHptr = new HProductTokensPrivate(*other.h_ptr);

    delete h_ptr;
    h_ptr = newHptr;

    return *this;
}

HProductTokens::~HProductTokens()
{
    delete h_ptr;
}

bool HProductTokens::isValid() const
{
    return h_ptr->m_productTokens.size() > 0;
}

bool HProductTokens::isEmpty() const
{
    return h_ptr->m_originalTokenString.isEmpty();
}

HProductToken HProductTokens::osToken() const
{
    if (h_ptr->m_productTokens.size() < 3)
    {
        return HProductToken();
    }

    return h_ptr->m_productTokens[0];
}

HProductToken HProductTokens::upnpToken() const
{
    qint32 size = h_ptr->m_productTokens.size();
    if (size <= 0)
    {
        return HProductToken();
    }
    else if (size == 1)
    {
        return h_ptr->m_productTokens[0];
    }

    return h_ptr->m_productTokens[1];
}

HProductToken HProductTokens::productToken() const
{
    if (h_ptr->m_productTokens.size() < 3)
    {
        return HProductToken();
    }

    return h_ptr->m_productTokens[2];
}

QVector<HProductToken> HProductTokens::extraTokens() const
{
    return h_ptr->m_productTokens.size() > 3 ?
        h_ptr->m_productTokens.mid(3) : QVector<HProductToken>();
}

bool HProductTokens::hasExtraTokens() const
{
    return h_ptr->m_productTokens.size() > 3;
}

QVector<HProductToken> HProductTokens::tokens() const
{
    return h_ptr->m_productTokens;
}

QString HProductTokens::toString() const
{
    return h_ptr->m_originalTokenString;
}

bool operator==(const HProductTokens& ht1, const HProductTokens& ht2)
{
    return ht1.toString() == ht2.toString();
}

bool operator!=(const HProductTokens& ht1, const HProductTokens& ht2)
{
    return !(ht1 == ht2);
}

}
}
