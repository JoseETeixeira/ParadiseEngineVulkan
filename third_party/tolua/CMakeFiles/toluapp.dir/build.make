# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua

# Include any dependencies generated for this target.
include CMakeFiles/toluapp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/toluapp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/toluapp.dir/flags.make

CMakeFiles/toluapp.dir/src/bin/tolua.c.o: CMakeFiles/toluapp.dir/flags.make
CMakeFiles/toluapp.dir/src/bin/tolua.c.o: src/bin/tolua.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/toluapp.dir/src/bin/tolua.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/toluapp.dir/src/bin/tolua.c.o   -c /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/tolua.c

CMakeFiles/toluapp.dir/src/bin/tolua.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/toluapp.dir/src/bin/tolua.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/tolua.c > CMakeFiles/toluapp.dir/src/bin/tolua.c.i

CMakeFiles/toluapp.dir/src/bin/tolua.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/toluapp.dir/src/bin/tolua.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/tolua.c -o CMakeFiles/toluapp.dir/src/bin/tolua.c.s

CMakeFiles/toluapp.dir/src/bin/toluabind.c.o: CMakeFiles/toluapp.dir/flags.make
CMakeFiles/toluapp.dir/src/bin/toluabind.c.o: src/bin/toluabind.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/toluapp.dir/src/bin/toluabind.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/toluapp.dir/src/bin/toluabind.c.o   -c /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/toluabind.c

CMakeFiles/toluapp.dir/src/bin/toluabind.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/toluapp.dir/src/bin/toluabind.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/toluabind.c > CMakeFiles/toluapp.dir/src/bin/toluabind.c.i

CMakeFiles/toluapp.dir/src/bin/toluabind.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/toluapp.dir/src/bin/toluabind.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/src/bin/toluabind.c -o CMakeFiles/toluapp.dir/src/bin/toluabind.c.s

# Object files for target toluapp
toluapp_OBJECTS = \
"CMakeFiles/toluapp.dir/src/bin/tolua.c.o" \
"CMakeFiles/toluapp.dir/src/bin/toluabind.c.o"

# External object files for target toluapp
toluapp_EXTERNAL_OBJECTS =

toluapp: CMakeFiles/toluapp.dir/src/bin/tolua.c.o
toluapp: CMakeFiles/toluapp.dir/src/bin/toluabind.c.o
toluapp: CMakeFiles/toluapp.dir/build.make
toluapp: libtoluapp.so
toluapp: /usr/lib/x86_64-linux-gnu/liblua5.1.so
toluapp: /usr/lib/x86_64-linux-gnu/libm.so
toluapp: CMakeFiles/toluapp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable toluapp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/toluapp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/toluapp.dir/build: toluapp

.PHONY : CMakeFiles/toluapp.dir/build

CMakeFiles/toluapp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/toluapp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/toluapp.dir/clean

CMakeFiles/toluapp.dir/depend:
	cd /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua /media/lucas/29F419E21696543C/Projects/Pessoal/ParadiseEngine/ParadiseEngineVulkan/third_party/tolua/CMakeFiles/toluapp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/toluapp.dir/depend

