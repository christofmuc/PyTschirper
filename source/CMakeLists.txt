#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

#
# TODO
# 
# Automatic import of pytschirp Rev2, Rev2Patch symbols
# Header Textbox with explanation which objects are in
# Headers for StdOut, StdErr
# Retain rev2 object in globals once detected
# New Button row on top - Load, Save, Save As, Run, Close
# Load and save script
# Vertical Button strip with assignable scripts, named buttons
# Hooks to react on certain messages received, e.g. program change
# Somehow get Midi Log filled? 
# About Box with licenses, like SourceTree does it
# Installer?
# README

cmake_minimum_required(VERSION 3.14)

project(PyTschirperUI)

IF (WIN32)
	# All setup for UI software on Windows
ELSEIF(UNIX)
	# Include useful scripts for CMake
	find_package(PkgConfig REQUIRED) 
	find_package(OpenGL)

	# These calls create special `PkgConfig::<MODULE>` variables
	pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0)
	pkg_check_modules(WEBKIT REQUIRED IMPORTED_TARGET webkit2gtk-4.0)
ENDIF()

find_package(JUCE REQUIRED 
	COMPONENTS 
		juce_core		
		juce_events
		juce_audio_basics
		juce_audio_devices
		juce_data_structures
		juce_audio_utils # AudioAppComponent
		juce_dsp
		juce_opengl
)

set(SOURCES
	MainComponent.h MainComponent.cpp	
	PyTschirpEditor.h PyTschirpEditor.cpp
	PythonUtils.h PythonUtils.cpp
	Main.cpp
)

add_executable(PyTschirp WIN32 ${SOURCES})
target_include_directories(PyTschirp INTERFACE ${CMAKE_CURRENT_LIST_DIR})
IF(WIN32)
	target_link_libraries(PyTschirp PRIVATE ${JUCE_LIBRARIES} pybind11::embed juce-widgets)
ELSEIF(UNIX)
	target_link_libraries(PyTschirp PRIVATE 
		${JUCE_LIBRARIES} 
		PkgConfig::WEBKIT 
		PkgConfig::GTK 
		Xext 
		X11 
		pthread 
		${CMAKE_DL_LIBS} 
		freetype 
		curl 
		pybind11::embed
		juce-widgets
		)
	target_compile_options(PyTschirp PRIVATE -pthread -I/usr/include/webkitgtk-4.0 -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng16 -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/webkitgtk-4.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include)
ENDIF()

IF(WIN32)
	# Post build commands - copy the DLLs (Windows only I guess)
	get_filename_component(PYTHONPATH ${PYTHON_EXECUTABLE} DIRECTORY)
	get_filename_component(PYTHONNAME ${PYTHON_EXECUTABLE} NAME_WE)
	add_custom_command(
			TARGET PyTschirp POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					"${PYTHONPATH}/python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}.dll"
					${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>)
ENDIF()

# Use all cores
IF (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()
