FILE(GLOB SRCS *.cpp)
FILE(GLOB HDRS *.h)

SOURCE_GROUP("" FILES ${SRCS} ${HDRS})

INCLUDE_DIRECTORIES(${assimp_INCLUDE_DIRS})

NL_TARGET_LIB(mesh_utils ${SRCS} ${HDRS})

TARGET_LINK_LIBRARIES(mesh_utils ${assimp_LIBRARIES} nelmisc nel3d)
NL_DEFAULT_PROPS(mesh_utils "NeL, Tools, 3D: Mesh Utils")
NL_ADD_RUNTIME_FLAGS(mesh_utils)

IF((WITH_INSTALL_LIBRARIES AND WITH_STATIC) OR NOT WITH_STATIC)
  INSTALL(TARGETS mesh_utils LIBRARY DESTINATION ${NL_LIB_PREFIX} ARCHIVE DESTINATION ${NL_LIB_PREFIX} COMPONENT tools3d)
ENDIF((WITH_INSTALL_LIBRARIES AND WITH_STATIC) OR NOT WITH_STATIC)
