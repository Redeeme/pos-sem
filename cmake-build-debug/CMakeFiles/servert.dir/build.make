# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = /tmp/tmp.mffmxVgwxA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.mffmxVgwxA/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/servert.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/servert.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/servert.dir/flags.make

CMakeFiles/servert.dir/server.c.o: CMakeFiles/servert.dir/flags.make
CMakeFiles/servert.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/servert.dir/server.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/servert.dir/server.c.o   -c /tmp/tmp.mffmxVgwxA/server.c

CMakeFiles/servert.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/servert.dir/server.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.mffmxVgwxA/server.c > CMakeFiles/servert.dir/server.c.i

CMakeFiles/servert.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/servert.dir/server.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.mffmxVgwxA/server.c -o CMakeFiles/servert.dir/server.c.s

CMakeFiles/servert.dir/server.c.o.requires:

.PHONY : CMakeFiles/servert.dir/server.c.o.requires

CMakeFiles/servert.dir/server.c.o.provides: CMakeFiles/servert.dir/server.c.o.requires
	$(MAKE) -f CMakeFiles/servert.dir/build.make CMakeFiles/servert.dir/server.c.o.provides.build
.PHONY : CMakeFiles/servert.dir/server.c.o.provides

CMakeFiles/servert.dir/server.c.o.provides.build: CMakeFiles/servert.dir/server.c.o


# Object files for target servert
servert_OBJECTS = \
"CMakeFiles/servert.dir/server.c.o"

# External object files for target servert
servert_EXTERNAL_OBJECTS =

servert: CMakeFiles/servert.dir/server.c.o
servert: CMakeFiles/servert.dir/build.make
servert: CMakeFiles/servert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable servert"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/servert.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/servert.dir/build: servert

.PHONY : CMakeFiles/servert.dir/build

CMakeFiles/servert.dir/requires: CMakeFiles/servert.dir/server.c.o.requires

.PHONY : CMakeFiles/servert.dir/requires

CMakeFiles/servert.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/servert.dir/cmake_clean.cmake
.PHONY : CMakeFiles/servert.dir/clean

CMakeFiles/servert.dir/depend:
	cd /tmp/tmp.mffmxVgwxA/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.mffmxVgwxA /tmp/tmp.mffmxVgwxA /tmp/tmp.mffmxVgwxA/cmake-build-debug /tmp/tmp.mffmxVgwxA/cmake-build-debug /tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles/servert.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/servert.dir/depend

