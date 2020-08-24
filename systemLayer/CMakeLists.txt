#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
cmake_minimum_required( VERSION 2.8 )

# If you don't name the project, it will take the default name "Project"
# Note: this must be set in the beginning of the CMakeLists file
project( systemlayer )

#---------------------------------------------------#
#              Project Files                        #
#---------------------------------------------------#
# List source files in a variable so we can use it later more conveniently
set( SL_SRCS
	src/linuxSystem.cpp
	src/windowsSystem.cpp
	src/system.cpp

)

set( SL_HEADERS
	include/linuxSystem.h
	include/linuxSystemFunctions.h
	include/systemLayer.h
	include/windowsSystem.h
	include/systemLayer_exp.h
	include/system.h
)
SET( SL_MOC_FILES
	include/linuxSystem.h
	include/linuxSystemFunctions.h
	include/systemLayer.h
	include/windowsSystem.h
	include/system.h
)


#---------------------------------------------------#
#              OpenCV Configuration                 #
#---------------------------------------------------#
# Automatically find OpenCV files
find_package( OpenCV REQUIRED )

SET (include_dirs  ${include_dirs} ${OPENCV_INCLUDE_DIR} ./include)

#---------------------------------------------------#
#              QT Configuration                     #
#---------------------------------------------------#
# Automatically find QT files
message("qtdir "$ENV{QT_DIR})

IF (NOT $ENV{QT_DIR} STREQUAL "")
FIND_PACKAGE( QT REQUIRED )

IF( QT_FOUND )
	
	ADD_DEFINITIONS(-DUSE_QT)
 	
 	INCLUDE( ${QT_USE_FILE} )
	SET (include_dirs  ${include_dirs} ${QT_INCLUDES})


	QT4_WRAP_CPP(SL_MOC_SOURCE_FILES ${SL_MOC_FILES}  )
	SOURCE_GROUP( "Generated Files" FILES ${SL_MOC_SOURCE_FILES} )
ENDIF( QT_FOUND )
ENDIF( )

#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
# Add all sources defined above to the program
include_directories( ${include_dirs} "./include")

# Add _d to the library in debug
SET(CMAKE_DEBUG_POSTFIX _d)

IF(QT_FOUND)
	add_library( systemlayer SHARED ${SL_SRCS} ${SL_HEADERS}  ${SL_MOC_SOURCE_FILES})
ELSE(QT_FOUND)
	add_library( systemlayer SHARED ${SL_SRCS} ${SL_HEADERS})
ENDIF(QT_FOUND)

IF(UNIX)
TARGET_LINK_LIBRARIES( systemlayer  ${QT_LIBRARIES} ${OpenCV_LIBS} )
ELSE(UNIX)
TARGET_LINK_LIBRARIES( systemlayer  ${QT_LIBRARIES} ${OpenCV_LIBS} Psapi.lib)
ENDIF(UNIX)


SET_TARGET_PROPERTIES(systemlayer PROPERTIES FOLDER "modules")
