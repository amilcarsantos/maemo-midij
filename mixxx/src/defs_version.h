/***************************************************************************
                          defs_version.h  -  description
                          --------------
    copyright            : (C) 2010 by Sean Pappalardo
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// Doing this in its own file avoids needlessly rebuilding everything when just the version number changes

// We define the version to be the branch name in Sconscript.env, unless it's a release branch
#ifndef VERSION
    // In that case, this value is used
    #define VERSION "1.10.0"
#endif

#ifndef TITLE_VERSION
#ifdef VERSION_MOD
    // In that case, this value combined
    #define TITLE_VERSION VERSION " " VERSION_MOD
#else
    // In that case, this value is used
    #define TITLE_VERSION VERSION
#endif
#endif
