# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/work/Ohyo/jazz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/work/Ohyo/jazz/CMake

# Include any dependencies generated for this target.
include src/service/CMakeFiles/jazzsvc.dir/depend.make

# Include the progress variables for this target.
include src/service/CMakeFiles/jazzsvc.dir/progress.make

# Include the compile flags for this target's objects.
include src/service/CMakeFiles/jazzsvc.dir/flags.make

src/service/CMakeFiles/jazzsvc.dir/main.cpp.o: src/service/CMakeFiles/jazzsvc.dir/flags.make
src/service/CMakeFiles/jazzsvc.dir/main.cpp.o: ../src/service/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/service/CMakeFiles/jazzsvc.dir/main.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzsvc.dir/main.cpp.o -c /mnt/work/Ohyo/jazz/src/service/main.cpp

src/service/CMakeFiles/jazzsvc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzsvc.dir/main.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/service/main.cpp > CMakeFiles/jazzsvc.dir/main.cpp.i

src/service/CMakeFiles/jazzsvc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzsvc.dir/main.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/service/main.cpp -o CMakeFiles/jazzsvc.dir/main.cpp.s

src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.requires:
.PHONY : src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.requires

src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.provides: src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.requires
	$(MAKE) -f src/service/CMakeFiles/jazzsvc.dir/build.make src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.provides.build
.PHONY : src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.provides

src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.provides.build: src/service/CMakeFiles/jazzsvc.dir/main.cpp.o

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o: src/service/CMakeFiles/jazzsvc.dir/flags.make
src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o: ../src/service/lli.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzsvc.dir/lli.cpp.o -c /mnt/work/Ohyo/jazz/src/service/lli.cpp

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzsvc.dir/lli.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/service/lli.cpp > CMakeFiles/jazzsvc.dir/lli.cpp.i

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzsvc.dir/lli.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/service/lli.cpp -o CMakeFiles/jazzsvc.dir/lli.cpp.s

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.requires:
.PHONY : src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.requires

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.provides: src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.requires
	$(MAKE) -f src/service/CMakeFiles/jazzsvc.dir/build.make src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.provides.build
.PHONY : src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.provides

src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.provides.build: src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o

# Object files for target jazzsvc
jazzsvc_OBJECTS = \
"CMakeFiles/jazzsvc.dir/main.cpp.o" \
"CMakeFiles/jazzsvc.dir/lli.cpp.o"

# External object files for target jazzsvc
jazzsvc_EXTERNAL_OBJECTS =

jazzsvc: src/service/CMakeFiles/jazzsvc.dir/main.cpp.o
jazzsvc: src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o
jazzsvc: src/service/CMakeFiles/jazzsvc.dir/build.make
jazzsvc: src/service/CMakeFiles/jazzsvc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../../jazzsvc"
	cd /mnt/work/Ohyo/jazz/CMake/src/service && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jazzsvc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/service/CMakeFiles/jazzsvc.dir/build: jazzsvc
.PHONY : src/service/CMakeFiles/jazzsvc.dir/build

src/service/CMakeFiles/jazzsvc.dir/requires: src/service/CMakeFiles/jazzsvc.dir/main.cpp.o.requires
src/service/CMakeFiles/jazzsvc.dir/requires: src/service/CMakeFiles/jazzsvc.dir/lli.cpp.o.requires
.PHONY : src/service/CMakeFiles/jazzsvc.dir/requires

src/service/CMakeFiles/jazzsvc.dir/clean:
	cd /mnt/work/Ohyo/jazz/CMake/src/service && $(CMAKE_COMMAND) -P CMakeFiles/jazzsvc.dir/cmake_clean.cmake
.PHONY : src/service/CMakeFiles/jazzsvc.dir/clean

src/service/CMakeFiles/jazzsvc.dir/depend:
	cd /mnt/work/Ohyo/jazz/CMake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/work/Ohyo/jazz /mnt/work/Ohyo/jazz/src/service /mnt/work/Ohyo/jazz/CMake /mnt/work/Ohyo/jazz/CMake/src/service /mnt/work/Ohyo/jazz/CMake/src/service/CMakeFiles/jazzsvc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/service/CMakeFiles/jazzsvc.dir/depend

