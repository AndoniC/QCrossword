#---------------------------------------------------#
#          CMake Project Configuration              #
#---------------------------------------------------#
# CMake will issue a warning if you don't set this
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


if(COMMAND cmake_policy)
     cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#@ Carpeta de modulos
set (MODULES_folder "./modules" CACHE PATH "Path to folder with modules (ext, openssl, exif, matlablib, ... )" ) 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MODULES_folder}/CMakeModules/")

#@ check architecture
include("${MODULES_folder}/CMakeModules/TargetArch.cmake")
target_architecture (TARGET_COMPILATION_ARCH)
#check : IF(TARGET_COMPILATION_ARCH MATCHES x86_64)
message(STATUS "** Compilation Target is ${TARGET_COMPILATION_ARCH} bits **")
PROJECT( qcrossword )

set_property( GLOBAL PROPERTY USE_FOLDERS On)


#---------------------------------------------------#
#              Control version                 		#
#---------------------------------------------------#
SET(QCROSSWORD_VERSION_MAJOR 1)	
SET(QCROSSWORD_VERSION_MINOR 2)	
SET(QCROSSWORD_VERSION_PATCH 8)  	
SET(QCROSSWORD_VERSION_STATUS "rc")

ADD_DEFINITIONS(-DQCROSSWORD_VERSION_MAJOR=${QCROSSWORD_VERSION_MAJOR} 
				-DQCROSSWORD_VERSION_MINOR=${QCROSSWORD_VERSION_MINOR}
				-DQCROSSWORD_VERSION_PATCH=${QCROSSWORD_VERSION_PATCH}
				-DQCROSSWORD_VERSION_STATUS="${QCROSSWORD_VERSION_STATUS}" )


#---------------------------------------------------#
#              OpenCV Configuration                 #
#---------------------------------------------------#
# Automatically find OpenCV files
FIND_PACKAGE( OpenCV REQUIRED )


#@ Set additional functions library files
add_subdirectory(${MODULES_folder}/ext ${PROJECT_BINARY_DIR}/ext)

#---------------------------------------------------#
#              QT Configuration                     #
#---------------------------------------------------#
FIND_PACKAGE( Qt5Core QUIET)

if(  NOT Qt5Core_FOUND )
	message(STATUS "Using Qt4...")

	SET( QT_USE_QTOPENGL 1 )
	FIND_PACKAGE( Qt4 REQUIRED )

	IF ( QT_FOUND)
		INCLUDE( ${QT_USE_FILE} )
		SET(QT_VERSION "Qt4")
	ELSE ( QT_FOUND)
		MESSAGE( FATAL_ERROR "This application requires Qt. One of these components is missing. Please verify configuration")
	ENDIF ( QT_FOUND)
else(   NOT Qt5Core_FOUND )
	message(STATUS "Using Qt5... Qt5OpenGL, Qt5Widgets, Qt5Gui, Qt5Charts")
	FIND_PACKAGE( Qt5OpenGL )
	FIND_PACKAGE( Qt5Widgets )
	FIND_PACKAGE( Qt5Gui )
	FIND_PACKAGE( Qt5Charts )
ENDIF(   NOT Qt5Core_FOUND )


#---------------------------------------------------#
#              OpenCV Configuration                 #
#---------------------------------------------------#
# Automatically find OpenCV files
find_package( OpenCV REQUIRED )

SET (include_dirs  ${include_dirs} ${OPENCV_INCLUDE_DIR} ./include)

#---------------------------------------------------#
#              Variables configuration              #
#---------------------------------------------------#

SET ( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules )
SET ( CMAKE_DEBUG_POSTFIX d )
SET ( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all libraries." FORCE )
SET ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Single output directory for building all executables." FORCE )

SET (include_dirs ${QT_INCLUDES} "src"  ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} "systemLayer/include")


#@ Set include dirs	
include_directories( ${OpenCV_INCLUDE_DIRS}  ${CAPTURE_INCLUDE_DIR} ${MODULES_folder} "./include" "./include_qt" ${LOGURU_DIR} "./src" ${EIGEN_INCLUDE_DIRS} ${MODULES_folder}/lmmin ${QT_INCLUDE_DIR})

#@ Set project files
FILE (GLOB sources "src/*.cpp" "./*.cpp" "src/*.c" "./*.c" "src/*.cxx" "./*.cxx" )
FILE (GLOB headers "include/*.hpp" "include/*.hxx" "include/*.h")
FILE (GLOB headers_qt "include_qt/*.hpp" "include_qt/*.hxx" "include_qt/*.h")
FILE (GLOB UIS  "include_qt/*.ui" )
set(RC_FILES src/imagenes.qrc)




if(Qt5Core_FOUND)
	
	message(STATUS "Creating MOC Qt5 Files ....")
	# Configure Qt
	SET( QT_USE_QTOPENGL TRUE )
	
	#it's neccesary to find Qt5Widgets to use this macros
	qt5_wrap_ui( SOURCES_UIS_H ${UIS} )
	qt5_wrap_cpp( MOC_SOURCE_FILES ${headers_qt} )
	qt5_add_resources( RC_SRC_FILES ${RC_FILES} )

	# Use the compile definitions defined in the Qt 5 Widgets module
	add_definitions(${Qt5Widgets_DEFINITIONS})
	add_definitions(${Qt5Gui_DEFINITIONS})
	add_definitions(${Qt5OpenGL_DEFINITIONS})
	add_definitions(${Qt5Core_DEFINITIONS})
	
	# Add the include directories for the Qt 5 Widgets module to
	# the compile lines.
	SET( QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${Qt5OpenGL_INCLUDE_DIRS} )
	SET( QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${Qt5Gui_INCLUDE_DIRS} )
	SET( QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${Qt5Core_INCLUDE_DIRS} )
	SET( QT_INCLUDE_DIR ${QT_INCLUDE_DIR} ${Qt5Widgets_INCLUDE_DIRS} )
	
	# Add compiler flags for building executables (-fPIE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
	
ELSE(Qt5Core_FOUND)
	message(STATUS "Creating MOC Qt4 Files ....")
	QT4_WRAP_UI( SOURCES_UIS_H ${UIS} )
	QT4_WRAP_CPP(MOC_SOURCE_FILES ${MOC_FILES}  )
	QT4_ADD_RESOURCES( RC_SRC_FILES ${RC_FILES} )
	SET( QT_INCLUDE_DIR ${QT_INCLUDE_DIRS} )
ENDIF(Qt5Core_FOUND)


#---------------------------------------------------#
#              		GROUPS FILES		            #
#---------------------------------------------------#
SOURCE_GROUP( "Generated Files" FILES ${MOC_SOURCE_FILES} ${RC_SRC_FILES} )
SOURCE_GROUP("Generated Files\\UIs" FILES ${UIS})
SOURCE_GROUP("ext" FILES ${EXT_SRCS})


#---------------------------------------------------#
#               Project Configuration               #
#---------------------------------------------------#
# Add all sources defined above to the program
INCLUDE_DIRECTORIES( ${include_dirs} )
LINK_DIRECTORIES (  ${QT_LIBRARY_DIR} "./bin/debug" "./bin/release")

#---------------------------------------------------#
#	               EXECUTABLE		            	#
#---------------------------------------------------#
ADD_EXECUTABLE( qcrossword ${sources} ${headers} ${headers_qt} ${EXT_SRCS} ${MOC_SOURCE_FILES} ${UIS} ${RC_SRC_FILES} )

#---------------------------------------------------#
#	               LIBRERIAS		            	#
#---------------------------------------------------#
# Nota: han de estar después de la creación del ejecutable
TARGET_LINK_LIBRARIES( qcrossword ${OpenCV_LIBS}   )
if(Qt5Core_FOUND)
	SET (QT_LIBRARIES ${Qt5Core_QTMAIN_LIBRARIES} ${Qt5Charts_QTMAIN_LIBRARIES})# <--  Qt5::WinMain ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
	target_link_libraries(qcrossword ${Qt5Core_QTMAIN_LIBRARIES} ${Qt5Charts_QTMAIN_LIBRARIES} )# <--  Qt5::WinMain ${Qt5Widgets_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
	qt5_use_modules( qcrossword Widgets OpenGL Core  Charts) 
else (Qt5Core_FOUND)
	target_link_libraries( qcrossword  ${QT_LIBRARIES})
	#Para que el integration del VS funcione
	SET_TARGET_PROPERTIES( qcrossword PROPERTIES  VS_KEYWORD Qt4VSv1.0 )
endif(Qt5Core_FOUND)

INSTALL( TARGETS qcrossword RUNTIME DESTINATION "." COMPONENT app)

