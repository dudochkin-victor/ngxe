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
include src/CMakeFiles/jazzd.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/jazzd.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/jazzd.dir/flags.make

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o: ../src/AppSurfing.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/AppSurfing.cpp.o -c /mnt/work/Ohyo/jazz/src/AppSurfing.cpp

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/AppSurfing.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/AppSurfing.cpp > CMakeFiles/jazzd.dir/AppSurfing.cpp.i

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/AppSurfing.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/AppSurfing.cpp -o CMakeFiles/jazzd.dir/AppSurfing.cpp.s

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.requires

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.provides: src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.provides

src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o

src/CMakeFiles/jazzd.dir/debug.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/debug.cpp.o: ../src/debug.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/debug.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/debug.cpp.o -c /mnt/work/Ohyo/jazz/src/debug.cpp

src/CMakeFiles/jazzd.dir/debug.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/debug.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/debug.cpp > CMakeFiles/jazzd.dir/debug.cpp.i

src/CMakeFiles/jazzd.dir/debug.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/debug.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/debug.cpp -o CMakeFiles/jazzd.dir/debug.cpp.s

src/CMakeFiles/jazzd.dir/debug.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/debug.cpp.o.requires

src/CMakeFiles/jazzd.dir/debug.cpp.o.provides: src/CMakeFiles/jazzd.dir/debug.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/debug.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/debug.cpp.o.provides

src/CMakeFiles/jazzd.dir/debug.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/debug.cpp.o

src/CMakeFiles/jazzd.dir/jazz.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/jazz.cpp.o: ../src/jazz.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/jazz.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/jazz.cpp.o -c /mnt/work/Ohyo/jazz/src/jazz.cpp

src/CMakeFiles/jazzd.dir/jazz.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/jazz.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/jazz.cpp > CMakeFiles/jazzd.dir/jazz.cpp.i

src/CMakeFiles/jazzd.dir/jazz.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/jazz.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/jazz.cpp -o CMakeFiles/jazzd.dir/jazz.cpp.s

src/CMakeFiles/jazzd.dir/jazz.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/jazz.cpp.o.requires

src/CMakeFiles/jazzd.dir/jazz.cpp.o.provides: src/CMakeFiles/jazzd.dir/jazz.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/jazz.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/jazz.cpp.o.provides

src/CMakeFiles/jazzd.dir/jazz.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/jazz.cpp.o

src/CMakeFiles/jazzd.dir/Response.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/Response.cpp.o: ../src/Response.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/Response.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/Response.cpp.o -c /mnt/work/Ohyo/jazz/src/Response.cpp

src/CMakeFiles/jazzd.dir/Response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/Response.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/Response.cpp > CMakeFiles/jazzd.dir/Response.cpp.i

src/CMakeFiles/jazzd.dir/Response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/Response.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/Response.cpp -o CMakeFiles/jazzd.dir/Response.cpp.s

src/CMakeFiles/jazzd.dir/Response.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/Response.cpp.o.requires

src/CMakeFiles/jazzd.dir/Response.cpp.o.provides: src/CMakeFiles/jazzd.dir/Response.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/Response.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/Response.cpp.o.provides

src/CMakeFiles/jazzd.dir/Response.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/Response.cpp.o

src/CMakeFiles/jazzd.dir/Session.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/Session.cpp.o: ../src/Session.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/Session.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/Session.cpp.o -c /mnt/work/Ohyo/jazz/src/Session.cpp

src/CMakeFiles/jazzd.dir/Session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/Session.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/Session.cpp > CMakeFiles/jazzd.dir/Session.cpp.i

src/CMakeFiles/jazzd.dir/Session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/Session.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/Session.cpp -o CMakeFiles/jazzd.dir/Session.cpp.s

src/CMakeFiles/jazzd.dir/Session.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/Session.cpp.o.requires

src/CMakeFiles/jazzd.dir/Session.cpp.o.provides: src/CMakeFiles/jazzd.dir/Session.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/Session.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/Session.cpp.o.provides

src/CMakeFiles/jazzd.dir/Session.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/Session.cpp.o

src/CMakeFiles/jazzd.dir/Url.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/Url.cpp.o: ../src/Url.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/Url.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/Url.cpp.o -c /mnt/work/Ohyo/jazz/src/Url.cpp

src/CMakeFiles/jazzd.dir/Url.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/Url.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/Url.cpp > CMakeFiles/jazzd.dir/Url.cpp.i

src/CMakeFiles/jazzd.dir/Url.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/Url.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/Url.cpp -o CMakeFiles/jazzd.dir/Url.cpp.s

src/CMakeFiles/jazzd.dir/Url.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/Url.cpp.o.requires

src/CMakeFiles/jazzd.dir/Url.cpp.o.provides: src/CMakeFiles/jazzd.dir/Url.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/Url.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/Url.cpp.o.provides

src/CMakeFiles/jazzd.dir/Url.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/Url.cpp.o

src/CMakeFiles/jazzd.dir/User.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/User.cpp.o: ../src/User.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/User.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/User.cpp.o -c /mnt/work/Ohyo/jazz/src/User.cpp

src/CMakeFiles/jazzd.dir/User.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/User.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/User.cpp > CMakeFiles/jazzd.dir/User.cpp.i

src/CMakeFiles/jazzd.dir/User.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/User.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/User.cpp -o CMakeFiles/jazzd.dir/User.cpp.s

src/CMakeFiles/jazzd.dir/User.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/User.cpp.o.requires

src/CMakeFiles/jazzd.dir/User.cpp.o.provides: src/CMakeFiles/jazzd.dir/User.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/User.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/User.cpp.o.provides

src/CMakeFiles/jazzd.dir/User.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/User.cpp.o

src/CMakeFiles/jazzd.dir/Variant.cpp.o: src/CMakeFiles/jazzd.dir/flags.make
src/CMakeFiles/jazzd.dir/Variant.cpp.o: ../src/Variant.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /mnt/work/Ohyo/jazz/CMake/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/jazzd.dir/Variant.cpp.o"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/jazzd.dir/Variant.cpp.o -c /mnt/work/Ohyo/jazz/src/Variant.cpp

src/CMakeFiles/jazzd.dir/Variant.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jazzd.dir/Variant.cpp.i"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /mnt/work/Ohyo/jazz/src/Variant.cpp > CMakeFiles/jazzd.dir/Variant.cpp.i

src/CMakeFiles/jazzd.dir/Variant.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jazzd.dir/Variant.cpp.s"
	cd /mnt/work/Ohyo/jazz/CMake/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /mnt/work/Ohyo/jazz/src/Variant.cpp -o CMakeFiles/jazzd.dir/Variant.cpp.s

src/CMakeFiles/jazzd.dir/Variant.cpp.o.requires:
.PHONY : src/CMakeFiles/jazzd.dir/Variant.cpp.o.requires

src/CMakeFiles/jazzd.dir/Variant.cpp.o.provides: src/CMakeFiles/jazzd.dir/Variant.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/jazzd.dir/build.make src/CMakeFiles/jazzd.dir/Variant.cpp.o.provides.build
.PHONY : src/CMakeFiles/jazzd.dir/Variant.cpp.o.provides

src/CMakeFiles/jazzd.dir/Variant.cpp.o.provides.build: src/CMakeFiles/jazzd.dir/Variant.cpp.o

# Object files for target jazzd
jazzd_OBJECTS = \
"CMakeFiles/jazzd.dir/AppSurfing.cpp.o" \
"CMakeFiles/jazzd.dir/debug.cpp.o" \
"CMakeFiles/jazzd.dir/jazz.cpp.o" \
"CMakeFiles/jazzd.dir/Response.cpp.o" \
"CMakeFiles/jazzd.dir/Session.cpp.o" \
"CMakeFiles/jazzd.dir/Url.cpp.o" \
"CMakeFiles/jazzd.dir/User.cpp.o" \
"CMakeFiles/jazzd.dir/Variant.cpp.o"

# External object files for target jazzd
jazzd_EXTERNAL_OBJECTS =

jazzd: src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/debug.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/jazz.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/Response.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/Session.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/Url.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/User.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/Variant.cpp.o
jazzd: src/CMakeFiles/jazzd.dir/build.make
jazzd: /usr/lib64/libmysqlclient_r.so
jazzd: /usr/lib64/libmysqlpp.so
jazzd: /usr/lib64/libcrypto++.so
jazzd: /usr/lib64/libevent.so
jazzd: /usr/lib64/libxml2.so
jazzd: /usr/lib64/libboost_regex-mt-1_49.so
jazzd: src/CMakeFiles/jazzd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../jazzd"
	cd /mnt/work/Ohyo/jazz/CMake/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jazzd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/jazzd.dir/build: jazzd
.PHONY : src/CMakeFiles/jazzd.dir/build

src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/AppSurfing.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/debug.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/jazz.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/Response.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/Session.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/Url.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/User.cpp.o.requires
src/CMakeFiles/jazzd.dir/requires: src/CMakeFiles/jazzd.dir/Variant.cpp.o.requires
.PHONY : src/CMakeFiles/jazzd.dir/requires

src/CMakeFiles/jazzd.dir/clean:
	cd /mnt/work/Ohyo/jazz/CMake/src && $(CMAKE_COMMAND) -P CMakeFiles/jazzd.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/jazzd.dir/clean

src/CMakeFiles/jazzd.dir/depend:
	cd /mnt/work/Ohyo/jazz/CMake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/work/Ohyo/jazz /mnt/work/Ohyo/jazz/src /mnt/work/Ohyo/jazz/CMake /mnt/work/Ohyo/jazz/CMake/src /mnt/work/Ohyo/jazz/CMake/src/CMakeFiles/jazzd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/jazzd.dir/depend

