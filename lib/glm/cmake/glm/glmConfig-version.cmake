# SPDX-FileCopyrightText: 2005 G-Truc Creation
#
# SPDX-License-Identifier: MIT

set(PACKAGE_VERSION 0.9.9)

if(${PACKAGE_FIND_VERSION_MAJOR} EQUAL 0)
    if (${PACKAGE_FIND_VERSION} VERSION_LESS ${GLM_VERSION})
        set(PACKAGE_VERSION_COMPATIBLE 1)  
    endif()
    if(${PACKAGE_FIND_VERSION} VERSION_EQUAL ${GLM_VERSION})
        set(PACKAGE_VERSION_EXACT 1)  
    endif()
else()
    set(PACKAGE_VERSION_UNSUITABLE 1)
endif()

