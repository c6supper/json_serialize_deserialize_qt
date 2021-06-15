/* This file is part of qprofile
 *
 * Copyright (C) 2018 Calvin <c6supper@hotmail.com>
 */

#ifndef QPROFILE_DEBUG_H
#define QPROFILE_DEBUG_H

#include <QtCore/QDebug>

#define MAX_QPROFILE_DEBUG_LENGTH	(2048)

// define qprofileDebug(QtDebugMsg)
#ifdef QPROFILE_VERBOSE_DEBUG_OUTPUT
#define qprofileDebug(level) \
    (QDebug(level) << __FILE__ << ":" << __LINE__ << ":" << __PRETTY_FUNCTION__ << ":")
#else
#define qprofileDebug(level) if(false) QDebug(level)
#endif

#endif
