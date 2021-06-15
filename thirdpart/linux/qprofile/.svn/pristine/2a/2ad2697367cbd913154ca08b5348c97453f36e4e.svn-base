/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QPROFILE_EXPORT_H
#define QPROFILE_EXPORT_H

#include <QtCore/qglobal.h>

#ifndef QPROFILE_STATIC
# ifndef QPROFILE_EXPORT
#  if defined(QPROFILE_MAKEDLL)
/* We are building this library */
#   define QPROFILE_EXPORT Q_DECL_EXPORT
#  else
/* We are using this library */
#   define QPROFILE_EXPORT Q_DECL_IMPORT
#  endif
# endif
#endif
#ifndef QPROFILE_EXPORT
# define QPROFILE_EXPORT
#endif

#endif
