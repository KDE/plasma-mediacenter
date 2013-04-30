/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AGENTMANAGER_H
#define AGENTMANAGER_H

#include <QtCore/QObject>


namespace Akonadi {
	class EntityTreeModel;
	class Session;
	class ChangeRecorder;
	class AgentManager;
}

class KJob;

class AgentManager : public QObject
{
	Q_OBJECT
public:
    AgentManager ( QObject* parent = 0 );
public:
	void requestAgent();
private:
	void setup();
	bool requestNewAgent();
signals:
	void agentRdy(const QString* agentid);
private Q_SLOTS:
	void agentCreated( KJob* job );
private:
	Akonadi::AgentManager* m_agentmanager;
	QString* m_agentid;
};

#endif // AGENTMANAGER_H
