# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bkaradzic/Private/projects/_github/glslang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bkaradzic/Private/projects/_github/glslang/build

# Include any dependencies generated for this target.
include StandAlone/CMakeFiles/spirv-remap.dir/depend.make

# Include the progress variables for this target.
include StandAlone/CMakeFiles/spirv-remap.dir/progress.make

# Include the compile flags for this target's objects.
include StandAlone/CMakeFiles/spirv-remap.dir/flags.make

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o: StandAlone/CMakeFiles/spirv-remap.dir/flags.make
StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o: ../StandAlone/spirv-remap.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bkaradzic/Private/projects/_github/glslang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o"
	cd /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o -c /home/bkaradzic/Private/projects/_github/glslang/StandAlone/spirv-remap.cpp

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spirv-remap.dir/spirv-remap.cpp.i"
	cd /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bkaradzic/Private/projects/_github/glslang/StandAlone/spirv-remap.cpp > CMakeFiles/spirv-remap.dir/spirv-remap.cpp.i

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spirv-remap.dir/spirv-remap.cpp.s"
	cd /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bkaradzic/Private/projects/_github/glslang/StandAlone/spirv-remap.cpp -o CMakeFiles/spirv-remap.dir/spirv-remap.cpp.s

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.requires:

.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.requires

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.provides: StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.requires
	$(MAKE) -f StandAlone/CMakeFiles/spirv-remap.dir/build.make StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.provides.build
.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.provides

StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.provides.build: StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o


# Object files for target spirv-remap
spirv__remap_OBJECTS = \
"CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o"

# External object files for target spirv-remap
spirv__remap_EXTERNAL_OBJECTS =

StandAlone/spirv-remap: StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o
StandAlone/spirv-remap: StandAlone/CMakeFiles/spirv-remap.dir/build.make
StandAlone/spirv-remap: glslang/libglslang.a
StandAlone/spirv-remap: SPIRV/libSPIRV.a
StandAlone/spirv-remap: StandAlone/libglslang-default-resource-limits.a
StandAlone/spirv-remap: SPIRV/libSPVRemapper.a
StandAlone/spirv-remap: glslang/libglslang.a
StandAlone/spirv-remap: OGLCompilersDLL/libOGLCompiler.a
StandAlone/spirv-remap: glslang/OSDependent/Unix/libOSDependent.a
StandAlone/spirv-remap: StandAlone/CMakeFiles/spirv-remap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bkaradzic/Private/projects/_github/glslang/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable spirv-remap"
	cd /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spirv-remap.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
StandAlone/CMakeFiles/spirv-remap.dir/build: StandAlone/spirv-remap

.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/build

StandAlone/CMakeFiles/spirv-remap.dir/requires: StandAlone/CMakeFiles/spirv-remap.dir/spirv-remap.cpp.o.requires

.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/requires

StandAlone/CMakeFiles/spirv-remap.dir/clean:
	cd /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone && $(CMAKE_COMMAND) -P CMakeFiles/spirv-remap.dir/cmake_clean.cmake
.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/clean

StandAlone/CMakeFiles/spirv-remap.dir/depend:
	cd /home/bkaradzic/Private/projects/_github/glslang/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bkaradzic/Private/projects/_github/glslang /home/bkaradzic/Private/projects/_github/glslang/StandAlone /home/bkaradzic/Private/projects/_github/glslang/build /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone /home/bkaradzic/Private/projects/_github/glslang/build/StandAlone/CMakeFiles/spirv-remap.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : StandAlone/CMakeFiles/spirv-remap.dir/depend

