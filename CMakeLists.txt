#
#  Copyright (c) 2019 Christof Ruch. All rights reserved.
#
#  Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
#

cmake_minimum_required(VERSION 3.14)

project(PyTschirper)

# Include useful scripts for CMake    
include(FetchContent REQUIRED)

# On Windows, we need to download external dependencies
IF (WIN32)
    FetchContent_Declare(
	  boost
	  URL "https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.zip"
	)
	FetchContent_MakeAvailable(boost)

	FetchContent_Declare(
		vcredist
		URL https://aka.ms/vs/16/release/vc_redist.x64.exe
		DOWNLOAD_NO_EXTRACT true
	)
	FetchContent_MakeAvailable(vcredist)
	set(VCREDIST_PATH "${CMAKE_CURRENT_BINARY_DIR}/_deps/vcredist-subbuild/vcredist-populate-prefix/src")
ENDIF()

# We need a little bit support for the JUCE library, and also want to put our own CMake helpers on the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third_party/juce-cmake/cmake" "${CMAKE_CURRENT_LIST_DIR}/cmake")

# RapidJson is an include-only library, so instead of bothering with their CMake files, just add the include path
set(MANUALLY_RAPID_JSON "${CMAKE_CURRENT_LIST_DIR}/third_party/rapidjson/include")

# Adding JUCE 6.0
add_subdirectory("third_party/JUCE")

# Build a static library from juce 
add_library(juce-static STATIC)
target_link_libraries(juce-static
	PRIVATE
	    juce::juce_audio_basics juce::juce_audio_devices juce::juce_audio_formats
	    juce::juce_audio_processors juce::juce_audio_utils juce::juce_core
	    juce::juce_cryptography juce::juce_data_structures juce::juce_dsp
	    juce::juce_events juce::juce_graphics juce::juce_gui_basics
	    juce::juce_gui_extra juce::juce_opengl juce::juce_video
	PUBLIC
		juce::juce_recommended_config_flags
		juce::juce_recommended_lto_flags
		juce::juce_recommended_warning_flags)

target_compile_definitions(juce-static
	PUBLIC
		JUCE_PLUGINHOST_VST=0
		JUCE_PLUGINHOST_AU=0
		DONT_SET_USING_JUCE_NAMESPACE=1
		JUCE_DISPLAY_SPLASH_SCREEN=0
		JUCE_REPORT_APP_USAGE=0
		JUCE_CHECK_MEMORY_LEAKS=0
		JUCE_QUICKTIME=0
		JUCE_USE_DIRECTWRITE=1
		JUCE_CATCH_UNHANDLED_EXCEPTIONS=0
		JUCE_COREGRAPHICS_DRAW_ASYNC=1
		JUCE_WIN_PER_MONITOR_DPI_AWARE=1
		JUCE_USE_FLAC=1
	INTERFACE
		$<TARGET_PROPERTY:juce-static,COMPILE_DEFINITIONS>)

target_include_directories(juce-static
	INTERFACE
		$<TARGET_PROPERTY:juce-static,INCLUDE_DIRECTORIES>)

set_target_properties(juce-static PROPERTIES
	POSITION_INDEPENDENT_CODE TRUE
	VISIBILITY_INLINES_HIDDEN TRUE
	C_VISIBILITY_PRESET hidden
	CXX_VISIBILITY_PRESET hidden)

# Adding my static libraries
add_subdirectory(juce-utils)
add_subdirectory(juce-widgets)
add_subdirectory(MidiKraft-base)
add_subdirectory(MidiKraft-librarian)
add_subdirectory(MidiKraft-BCR2000)
add_subdirectory(MidiKraft-sequential-rev2)
add_subdirectory(MidiKraft-kawai-k3)

# Setup pybind11
IF(WIN32)
	IF(DEFINED ENV{APPVEYOR})
		# Fixed path in Appveyor Windows build machine
		set(PYTHON_EXECUTABLE "C:/Python37-x64/python.exe")
	ELSE()
		set(PYTHON_EXECUTABLE "c:/python/python37/python.exe")
	ENDIF()
ENDIF()
set(PYBIND11_PYTHON_VERSION 3.7)
add_subdirectory(third_party/pybind11)

add_subdirectory(pytschirp)

add_subdirectory(source)

