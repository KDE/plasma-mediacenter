/***********************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef MEDIACENTER_MODELMETADATA_H
#define MEDIACENTER_MODELMETADATA_H

#include <QObject>
#include <QMetaType>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT ModelMetadata : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool supportsSearch READ supportsSearch NOTIFY supportsSearchChanged)
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString headerText READ headerText NOTIFY headerTextChanged)
public:
    explicit ModelMetadata(QObject *model = 0, QObject* parent = 0);

    QString name() const;
    void setName(const QString &name);

    bool supportsSearch() const;
    void setSupportsSearch(bool supports);

    QObject *model();
    void setModel(QObject* model);

    QString headerText() const;
    void setHeaderText(const QString &text);

Q_SIGNALS:
    void nameChanged();
    void supportsSearchChanged();
    void modelChanged();
    void headerTextChanged();

private:
    class Private;
    Private * const d;
};

#endif // MEDIACENTER_MODELMETADATA_H
