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

#ifndef PMC_LIBS_QXORM_EXPORT_H_
#define PMC_LIBS_QXORM_EXPORT_H_

#ifdef _BUILDING_PMC
#define PMC_DLL_EXPORT QX_DLL_EXPORT_HELPER
#else // _BUILDING_PMC
#define PMC_DLL_EXPORT QX_DLL_IMPORT_HELPER
#endif // _BUILDING_PMC

#ifdef _BUILDING_PMC
#define QX_REGISTER_HPP_PMC QX_REGISTER_HPP_EXPORT_DLL
#define QX_REGISTER_CPP_PMC QX_REGISTER_CPP_EXPORT_DLL
#define QX_REGISTER_COMPLEX_CLASS_NAME_HPP_PMC QX_REGISTER_COMPLEX_CLASS_NAME_HPP_EXPORT_DLL
#define QX_REGISTER_COMPLEX_CLASS_NAME_CPP_PMC QX_REGISTER_COMPLEX_CLASS_NAME_CPP_EXPORT_DLL
#else
#define QX_REGISTER_HPP_PMC QX_REGISTER_HPP_IMPORT_DLL
#define QX_REGISTER_CPP_PMC QX_REGISTER_CPP_IMPORT_DLL
#define QX_REGISTER_COMPLEX_CLASS_NAME_HPP_PMC QX_REGISTER_COMPLEX_CLASS_NAME_HPP_IMPORT_DLL
#define QX_REGISTER_COMPLEX_CLASS_NAME_CPP_PMC QX_REGISTER_COMPLEX_CLASS_NAME_CPP_IMPORT_DLL
#endif // _BUILDING_PMC

#endif // PMC_LIBS_QXORM_EXPORT_H_
