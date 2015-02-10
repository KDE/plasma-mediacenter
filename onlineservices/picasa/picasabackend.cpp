/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "picasabackend.h"
#include "picasamodel.h"

#include <QTimer>
#include <KLocalizedString>

MEDIACENTER_EXPORT_BROWSINGBACKEND(PicasaBackend, "picasabackend.json")

namespace {
    static const QString loginButtonText = i18n("Select Account");
}

PicasaBackend::PicasaBackend(QObject* parent, const QVariantList& args):
                             MediaCenter::AbstractBrowsingBackend(parent, args)
{
    setButtons(QStringList() << loginButtonText);
    QTimer::singleShot(2000, this, SLOT(showLoginScreen()));
}

bool PicasaBackend::initImpl()
{
    return true;
}

bool PicasaBackend::goOneLevelUp()
{
    PicasaModel *picasaModel = qobject_cast<PicasaModel*>(model());
    if(picasaModel != NULL) {
        return picasaModel->goBack();
    }
    return MediaCenter::AbstractBrowsingBackend::goOneLevelUp();
}

void PicasaBackend::login(const QString& username, const QString& password)
{
    PicasaModel * picasaModel = new PicasaModel(this, username, password);
    setModel(picasaModel);
    connect (picasaModel, SIGNAL (loginComplete(bool)), this, SLOT (updateLoginStatus(bool)));
}

bool PicasaBackend::expand(int row)
{
    PicasaModel *picasaModel = qobject_cast<PicasaModel*>(model());
    return picasaModel->browseToAlbum(row);
}

void PicasaBackend::updateLoginStatus(bool status)
{
    if (status) {
        emit loginSuccessful();
        m_loginText = "";
    } else {
        emit loginFailed();
        setModel((PmcModel*)(0));
        sender()->deleteLater();
    }
}

void PicasaBackend::showLoginScreen()
{
    emit showCustomUi(constructQmlSource("picasacomponents", "0.1", "PicasaSidePanel"));
}

void PicasaBackend::handleButtonClick(const QString& buttonName)
{
    if (buttonName == loginButtonText) {
        showLoginScreen();
    }
}

#include "picasabackend.moc"
