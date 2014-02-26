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

#ifndef PMC_LIBS_PRECOMPILED_HEADER_H_
#define PMC_LIBS_PRECOMPILED_HEADER_H_

#define fromStdString(s) fromAscii(s.data(), s.size())
#define toStdString() toLatin1().constData()
#define fromStdWString(s) fromWCharArray(s.c_str())

#include <QxOrm.h>

#include "qxorm_export.h"

#endif // PMC_LIBS_PRECOMPILED_HEADER_H_
