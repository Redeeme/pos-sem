# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.mffmxVgwxA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.mffmxVgwxA/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tcp_client2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tcp_client2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tcp_client2.dir/flags.make

CMakeFiles/tcp_client2.dir/tcp_client2.c.o: CMakeFiles/tcp_client2.dir/flags.make
CMakeFiles/tcp_client2.dir/tcp_client2.c.o: ../tcp_client2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tcp_client2.dir/tcp_client2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tcp_client2.dir/tcp_client2.c.o -c /tmp/tmp.mffmxVgwxA/tcp_client2.c

CMakeFiles/tcp_client2.dir/tcp_client2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tcp_client2.dir/tcp_client2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.mffmxVgwxA/tcp_client2.c > CMakeFiles/tcp_client2.dir/tcp_client2.c.i

CMakeFiles/tcp_client2.dir/tcp_client2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tcp_client2.dir/tcp_client2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.mffmxVgwxA/tcp_client2.c -o CMakeFiles/tcp_client2.dir/tcp_client2.c.s

# Object files for target tcp_client2
tcp_client2_OBJECTS = \
"CMakeFiles/tcp_client2.dir/tcp_client2.c.o"

# External object files for target tcp_client2
tcp_client2_EXTERNAL_OBJECTS =

tcp_client2: CMakeFiles/tcp_client2.dir/tcp_client2.c.o
tcp_client2: CMakeFiles/tcp_client2.dir/build.make
tcp_client2: CMakeFiles/tcp_client2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tcp_client2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tcp_client2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tcp_client2.dir/build: tcp_client2

.PHONY : CMakeFiles/tcp_client2.dir/build

CMakeFiles/tcp_client2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tcp_client2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tcp_client2.dir/clean

CMakeFiles/tcp_client2.dir/depend:
	cd /tmp/tmp.mffmxVgwxA/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.mffmxVgwxA /tmp/tmp.mffmxVgwxA /tmp/tmp.mffmxVgwxA/cmake-build-debug /tmp/tmp.mffmxVgwxA/cmake-build-debug /tmp/tmp.mffmxVgwxA/cmake-build-debug/CMakeFiles/tcp_client2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tcp_client2.dir/depend

