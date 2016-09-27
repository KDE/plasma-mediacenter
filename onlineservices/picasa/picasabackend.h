/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>
 *   Copyright 2013 (Documentation) by Akshay Ratan <akshayratan@gmail.com>
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

#ifndef PICASABACKEND_H
#define PICASABACKEND_H

#include <mediacenter/abstractbrowsingbackend.h>

/**
 * @class PicasaBackend
 * @brief A backend that manages queries to Picasa.
 *
 * This class direclty inherit from AbstractBrowsingBackend which is an interface between an applet
 * and a custom provided model
 */
class PicasaBackend : public KMediaCollection::AbstractBrowsingBackend
{
    Q_OBJECT
public:

    /**
     * Constructor
     * Used for initialization purposes.
     * Uses constructQmlSource for convenience
     *
     * @param parent is a QObject pointer
     * @param args type is of QVariantList
     */
    PicasaBackend(QObject *parent, const QVariantList &args);

    /**
     * Sets the picasaModel and updates the login status
     * @param username is the user name of the user's gmail account
     * @param password is the password of user's gmail account
     */
    Q_INVOKABLE void login(const QString& username, const QString& password);

    /**
     * Opens up the album
     * @param row is the model row number
     */
    bool expand (int row);

    /**
     * @return true
     */
    virtual bool initImpl();

    virtual void handleButtonClick(const QString& buttonName);

signals:
    void loginSuccessful();
    void loginFailed();

public Q_SLOTS:

   /**
     * Used to go one level up or back in the model
     */
    virtual bool goOneLevelUp();

    /**
     * Updates the login status as successful or failed based on the parameters
     * @param status determines the login status
     */
    void updateLoginStatus(bool status);

    void showLoginScreen();

private:
    QString m_loginText;
    void setLoginText(const QString &loginText);
};

#endif
