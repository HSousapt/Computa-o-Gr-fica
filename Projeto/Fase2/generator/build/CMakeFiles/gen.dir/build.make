# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build"

# Include any dependencies generated for this target.
include CMakeFiles/gen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/gen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gen.dir/flags.make

CMakeFiles/gen.dir/main.cpp.o: CMakeFiles/gen.dir/flags.make
CMakeFiles/gen.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gen.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gen.dir/main.cpp.o -c "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/main.cpp"

CMakeFiles/gen.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gen.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/main.cpp" > CMakeFiles/gen.dir/main.cpp.i

CMakeFiles/gen.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gen.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/main.cpp" -o CMakeFiles/gen.dir/main.cpp.s

CMakeFiles/gen.dir/generator.cpp.o: CMakeFiles/gen.dir/flags.make
CMakeFiles/gen.dir/generator.cpp.o: ../generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gen.dir/generator.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gen.dir/generator.cpp.o -c "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/generator.cpp"

CMakeFiles/gen.dir/generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gen.dir/generator.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/generator.cpp" > CMakeFiles/gen.dir/generator.cpp.i

CMakeFiles/gen.dir/generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gen.dir/generator.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/generator.cpp" -o CMakeFiles/gen.dir/generator.cpp.s

# Object files for target gen
gen_OBJECTS = \
"CMakeFiles/gen.dir/main.cpp.o" \
"CMakeFiles/gen.dir/generator.cpp.o"

# External object files for target gen
gen_EXTERNAL_OBJECTS =

gen: CMakeFiles/gen.dir/main.cpp.o
gen: CMakeFiles/gen.dir/generator.cpp.o
gen: CMakeFiles/gen.dir/build.make
gen: CMakeFiles/gen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable gen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gen.dir/build: gen

.PHONY : CMakeFiles/gen.dir/build

CMakeFiles/gen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gen.dir/clean

CMakeFiles/gen.dir/depend:
	cd "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator" "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator" "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build" "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build" "/home/eu/Universidade/3º ano/2º Semestre/CG/Fase1/generator/build/CMakeFiles/gen.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/gen.dir/depend

