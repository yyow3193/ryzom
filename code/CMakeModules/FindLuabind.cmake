# - Locate Luabind library
# This module defines
#  LUABIND_LIBRARIES, the libraries to link against
#  LUABIND_FOUND, if false, do not try to link to LUABIND
#  LUABIND_INCLUDE_DIR, where to find headers.

MACRO(FIND_CORRECT_LUA_VERSION)
  # Check Lua version linked to Luabind under Linux
  IF(LUABIND_LIBRARY_RELEASE MATCHES "\\.so")
    INCLUDE(CheckDepends)

    SET(LUA52_LIBRARY "liblua5.2")
    CHECK_LINKED_LIBRARY(LUABIND_LIBRARY_RELEASE LUA52_LIBRARY LUALIB_FOUND)
    
    IF(NOT LUALIB_FOUND)
      # fedora (v20)
      SET(LUA52_LIBRARY "liblua-5.2")
      CHECK_LINKED_LIBRARY(LUABIND_LIBRARY_RELEASE LUA52_LIBRARY LUALIB_FOUND)
    ENDIF(NOT LUALIB_FOUND)

    IF(LUALIB_FOUND)
      MESSAGE(STATUS "Luabind is using Lua 5.2")
      FIND_PACKAGE(Lua52 REQUIRED)
    ELSE(LUALIB_FOUND)
      SET(LUA51_LIBRARY "liblua5.1")
      CHECK_LINKED_LIBRARY(LUABIND_LIBRARY_RELEASE LUA51_LIBRARY LUALIB_FOUND)

      IF(LUALIB_FOUND)
        MESSAGE(STATUS "Luabind is using Lua 5.1")
        FIND_PACKAGE(Lua51 REQUIRED)
      ELSE(LUALIB_FOUND)
        SET(LUA50_LIBRARY "liblua5.0")
        CHECK_LINKED_LIBRARY(LUABIND_LIBRARY_RELEASE LUA50_LIBRARY LUALIB_FOUND)

        IF(LUALIB_FOUND)
          MESSAGE(STATUS "Luabind is using Lua 5.0")
          FIND_PACKAGE(Lua50 REQUIRED)
        ELSE(LUALIB_FOUND)
          MESSAGE(FATAL_ERROR "Can't determine Lua version used by Luabind")
        ENDIF(LUALIB_FOUND)
      ENDIF(LUALIB_FOUND)
    ENDIF(LUALIB_FOUND)
  ELSE(LUABIND_LIBRARY_RELEASE MATCHES "\\.so")
    # TODO: find a way to detect Lua version
    IF(WITH_LUA53)
      FIND_PACKAGE(Lua53 REQUIRED)
    ELSEIF(WITH_LUA52)
      FIND_PACKAGE(Lua52 REQUIRED)
    ELSEIF(WITH_LUA51)
      FIND_PACKAGE(Lua51 REQUIRED)
    ELSE()
      FIND_PACKAGE(Lua50 REQUIRED)
    ENDIF()
  ENDIF()
ENDMACRO()

IF(LUABIND_LIBRARIES AND LUABIND_INCLUDE_DIR)
  # in cache already
  SET(Luabind_FIND_QUIETLY TRUE)
ENDIF()

FIND_PATH(LUABIND_INCLUDE_DIR
  luabind/luabind.hpp
  PATHS
  $ENV{LUABIND_DIR}/include
  /usr/local/include
  /usr/include
  /sw/include
  /opt/local/include
  /opt/csw/include
  /opt/include
)

SET(LIBRARY_NAME_RELEASE)
SET(LIBRARY_NAME_DEBUG)

IF(WITH_LUA53)
  IF(WITH_STLPORT)
    LIST(APPEND LIBRARY_NAME_RELEASE luabind_stlport_lua53)
    LIST(APPEND LIBRARY_NAME_DEBUG luabind_stlport_lua53d)
  ENDIF()

  LIST(APPEND LIBRARY_NAME_RELEASE luabind_lua53)
  LIST(APPEND LIBRARY_NAME_DEBUG luabind_lua53d)
ENDIF()

IF(WITH_LUA52)
  IF(WITH_STLPORT)
    LIST(APPEND LIBRARY_NAME_RELEASE luabind_stlport_lua52)
    LIST(APPEND LIBRARY_NAME_DEBUG luabind_stlport_lua52d)
  ENDIF()

  LIST(APPEND LIBRARY_NAME_RELEASE luabind_lua52)
  LIST(APPEND LIBRARY_NAME_DEBUG luabind_lua52d)
ENDIF()

IF(WITH_LUA51)
  IF(WITH_STLPORT)
    LIST(APPEND LIBRARY_NAME_RELEASE luabind_stlport_lua51)
    LIST(APPEND LIBRARY_NAME_DEBUG luabind_stlport_lua51d)
  ENDIF(WITH_STLPORT)

  LIST(APPEND LIBRARY_NAME_RELEASE luabind_lua51)
  LIST(APPEND LIBRARY_NAME_DEBUG luabind_lua51d)
ENDIF()

IF(WITH_LUA50)
  IF(WITH_STLPORT)
    LIST(APPEND LIBRARY_NAME_RELEASE luabind_stlport_lua50)
    LIST(APPEND LIBRARY_NAME_DEBUG luabind_stlport_lua50d)
  ENDIF(WITH_STLPORT)

  LIST(APPEND LIBRARY_NAME_RELEASE luabind_lua50)
  LIST(APPEND LIBRARY_NAME_DEBUG luabind_lua50d)
ENDIF()

IF(WITH_STLPORT)
  LIST(APPEND LIBRARY_NAME_RELEASE luabind_stlport)
  LIST(APPEND LIBRARY_NAME_DEBUG luabind_stlportd)
ENDIF(WITH_STLPORT)

# generic libraries names
LIST(APPEND LIBRARY_NAME_RELEASE luabind libluabind)
LIST(APPEND LIBRARY_NAME_DEBUG luabind_d luabindd libluabind_d libluabindd)

FIND_LIBRARY(LUABIND_LIBRARY_RELEASE
  NAMES ${LIBRARY_NAME_RELEASE}
  PATHS
  $ENV{LUABIND_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

FIND_LIBRARY(LUABIND_LIBRARY_DEBUG
  NAMES ${LIBRARY_NAME_DEBUG}
  PATHS
  $ENV{LUABIND_DIR}/lib
  /usr/local/lib
  /usr/lib
  /usr/local/X11R6/lib
  /usr/X11R6/lib
  /sw/lib
  /opt/local/lib
  /opt/csw/lib
  /opt/lib
  /usr/freeware/lib64
)

FIND_PACKAGE(Boost REQUIRED)
MESSAGE(WARNING "FIND_PACKAGE(Boost REQUIRED)")

IF(LUABIND_INCLUDE_DIR AND Boost_INCLUDE_DIR)
  IF(LUABIND_LIBRARY_RELEASE AND LUABIND_LIBRARY_DEBUG)
    # Case where both Release and Debug versions are provided
    SET(LUABIND_FOUND TRUE)
    SET(LUABIND_LIBRARIES optimized ${LUABIND_LIBRARY_RELEASE} debug ${LUABIND_LIBRARY_DEBUG})
  ELSEIF(LUABIND_LIBRARY_RELEASE)
    # Normal case
    SET(LUABIND_FOUND TRUE)
    SET(LUABIND_LIBRARIES ${LUABIND_LIBRARY_RELEASE})
  ELSEIF(LUABIND_LIBRARY_DEBUG)
    # Case where Luabind is compiled from sources (debug version is compiled by default)
    SET(LUABIND_FOUND TRUE)
    SET(LUABIND_LIBRARIES ${LUABIND_LIBRARY_DEBUG})
  ENDIF(LUABIND_LIBRARY_RELEASE AND LUABIND_LIBRARY_DEBUG)
ENDIF(LUABIND_INCLUDE_DIR AND Boost_INCLUDE_DIR)

IF(LUABIND_FOUND)
  SET(LUABIND_INCLUDE_DIR ${LUABIND_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
  # Check if luabind/version.hpp exists
  FIND_FILE(LUABIND_VERSION_FILE luabind/version.hpp PATHS ${LUABIND_INCLUDE_DIR})
  IF(LUABIND_VERSION_FILE)
    SET(LUABIND_DEFINITIONS "-DHAVE_LUABIND_VERSION")
  ENDIF(LUABIND_VERSION_FILE)

  FIND_CORRECT_LUA_VERSION()

  IF(NOT Luabind_FIND_QUIETLY)
    MESSAGE(STATUS "Found Luabind: ${LUABIND_LIBRARIES}")
  ENDIF(NOT Luabind_FIND_QUIETLY)
ELSE(LUABIND_FOUND)
  IF(NOT Luabind_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find Luabind!")
  ENDIF(NOT Luabind_FIND_QUIETLY)
ENDIF(LUABIND_FOUND)

MARK_AS_ADVANCED(LUABIND_LIBRARY_RELEASE LUABIND_LIBRARY_DEBUG Boost_LIB_DIAGNOSTIC_DEFINITIONS)
