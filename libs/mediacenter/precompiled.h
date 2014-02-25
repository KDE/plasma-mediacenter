#ifndef PMC_LIBS_PRECOMPILED_HEADER_H_
#define PMC_LIBS_PRECOMPILED_HEADER_H_

#define fromStdString(s) fromAscii(s.data(), s.size())
#define toStdString() toLatin1().constData()
#define fromStdWString(s) fromWCharArray(s.c_str())

#include <QxOrm.h>

#include "qxorm_export.h"

#endif // PMC_LIBS_PRECOMPILED_HEADER_H_
