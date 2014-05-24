/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
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

#include "filtermediamodeltest.h"
#include <mediacenter/filtermediamodel.h>

#include <qtest_kde.h>

#include <mockcpp/mockcpp.hpp>

QTEST_KDEMAIN(FilterMediaModelTest, NoGUI);

void FilterMediaModelTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void FilterMediaModelTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void FilterMediaModelTest::init()
{
    // Called before each testfunction is executed
}

void FilterMediaModelTest::cleanup()
{
    // Called after every testfunction
}

void FilterMediaModelTest::shouldFilterStringData()
{
    MockObject<QAbstractListModel> sourceModel;
    MOCK_METHOD(sourceModel, rowCount).stubs().will(returnValue(1));
    MOCK_METHOD(sourceModel, data).stubs().will(returnValue(QVariant("Plasma Media Center")));

    FilterMediaModel filterModel;
    connect(sourceModel, SIGNAL(layoutChanged()), SLOT(init()));
//     filterModel.setSourceModel(sourceModel);
//     filterModel.addFilter(Qt::DisplayRole, "Media");

//     QCOMPARE(filterModel.data(filterModel.index(0, 0)), QVariant("Plasma Media Center"));
}

#include "filtermediamodeltest.moc"
