# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\JetBrains\CLion 2020.1.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\JetBrains\CLion 2020.1.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\projecy-sat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\projecy-sat\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/projecy_sat.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/projecy_sat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projecy_sat.dir/flags.make

CMakeFiles/projecy_sat.dir/main.cpp.obj: CMakeFiles/projecy_sat.dir/flags.make
CMakeFiles/projecy_sat.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\projecy-sat\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/projecy_sat.dir/main.cpp.obj"
	C:\MinGW\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\projecy_sat.dir\main.cpp.obj -c D:\projecy-sat\main.cpp

CMakeFiles/projecy_sat.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/projecy_sat.dir/main.cpp.i"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\projecy-sat\main.cpp > CMakeFiles\projecy_sat.dir\main.cpp.i

CMakeFiles/projecy_sat.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/projecy_sat.dir/main.cpp.s"
	C:\MinGW\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\projecy-sat\main.cpp -o CMakeFiles\projecy_sat.dir\main.cpp.s

# Object files for target projecy_sat
projecy_sat_OBJECTS = \
"CMakeFiles/projecy_sat.dir/main.cpp.obj"

# External object files for target projecy_sat
projecy_sat_EXTERNAL_OBJECTS =

projecy_sat.exe: CMakeFiles/projecy_sat.dir/main.cpp.obj
projecy_sat.exe: CMakeFiles/projecy_sat.dir/build.make
projecy_sat.exe: CMakeFiles/projecy_sat.dir/linklibs.rsp
projecy_sat.exe: CMakeFiles/projecy_sat.dir/objects1.rsp
projecy_sat.exe: CMakeFiles/projecy_sat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\projecy-sat\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable projecy_sat.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\projecy_sat.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projecy_sat.dir/build: projecy_sat.exe

.PHONY : CMakeFiles/projecy_sat.dir/build

CMakeFiles/projecy_sat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\projecy_sat.dir\cmake_clean.cmake
.PHONY : CMakeFiles/projecy_sat.dir/clean

CMakeFiles/projecy_sat.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\projecy-sat D:\projecy-sat D:\projecy-sat\cmake-build-debug D:\projecy-sat\cmake-build-debug D:\projecy-sat\cmake-build-debug\CMakeFiles\projecy_sat.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/projecy_sat.dir/depend

