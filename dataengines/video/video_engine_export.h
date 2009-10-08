#ifndef VIDEO_ENGINE_EXPORT_H
#define VIDEO_ENGINE_EXPORT_H

/* needed for KDE_EXPORT macros */
#include <kdemacros.h>

#if defined _WIN32 || defined _WIN64
#ifndef VIDEO_ENGINE_EXPORT
# ifdef MAKE_PLASMACOMICPROVIDERCORE_LIB
#  define VIDEO_ENGINE_EXPORT KDE_EXPORT
# else
#  define VIDEO_ENGINE_EXPORT KDE_IMPORT
# endif
#endif

#else /* UNIX*/

/* export statements for unix */
#define VIDEO_ENGINE_EXPORT KDE_EXPORT
#endif

#endif
