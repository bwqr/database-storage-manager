# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = /home/fmk/Projects/lessons/cmpe321/project2/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fmk/Projects/lessons/cmpe321/project2/src

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fmk/Projects/lessons/cmpe321/project2/src/CMakeFiles /home/fmk/Projects/lessons/cmpe321/project2/src/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/fmk/Projects/lessons/cmpe321/project2/src/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named storageManager

# Build rule for target.
storageManager: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 storageManager
.PHONY : storageManager

# fast build rule for target.
storageManager/fast:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/build
.PHONY : storageManager/fast

ddl.o: ddl.cpp.o

.PHONY : ddl.o

# target to build an object file
ddl.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/ddl.cpp.o
.PHONY : ddl.cpp.o

ddl.i: ddl.cpp.i

.PHONY : ddl.i

# target to preprocess a source file
ddl.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/ddl.cpp.i
.PHONY : ddl.cpp.i

ddl.s: ddl.cpp.s

.PHONY : ddl.s

# target to generate assembly for a file
ddl.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/ddl.cpp.s
.PHONY : ddl.cpp.s

dml.o: dml.cpp.o

.PHONY : dml.o

# target to build an object file
dml.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/dml.cpp.o
.PHONY : dml.cpp.o

dml.i: dml.cpp.i

.PHONY : dml.i

# target to preprocess a source file
dml.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/dml.cpp.i
.PHONY : dml.cpp.i

dml.s: dml.cpp.s

.PHONY : dml.s

# target to generate assembly for a file
dml.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/dml.cpp.s
.PHONY : dml.cpp.s

file.o: file.cpp.o

.PHONY : file.o

# target to build an object file
file.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/file.cpp.o
.PHONY : file.cpp.o

file.i: file.cpp.i

.PHONY : file.i

# target to preprocess a source file
file.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/file.cpp.i
.PHONY : file.cpp.i

file.s: file.cpp.s

.PHONY : file.s

# target to generate assembly for a file
file.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/file.cpp.s
.PHONY : file.cpp.s

index.o: index.cpp.o

.PHONY : index.o

# target to build an object file
index.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/index.cpp.o
.PHONY : index.cpp.o

index.i: index.cpp.i

.PHONY : index.i

# target to preprocess a source file
index.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/index.cpp.i
.PHONY : index.cpp.i

index.s: index.cpp.s

.PHONY : index.s

# target to generate assembly for a file
index.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/index.cpp.s
.PHONY : index.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/main.cpp.s
.PHONY : main.cpp.s

page.o: page.cpp.o

.PHONY : page.o

# target to build an object file
page.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/page.cpp.o
.PHONY : page.cpp.o

page.i: page.cpp.i

.PHONY : page.i

# target to preprocess a source file
page.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/page.cpp.i
.PHONY : page.cpp.i

page.s: page.cpp.s

.PHONY : page.s

# target to generate assembly for a file
page.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/page.cpp.s
.PHONY : page.cpp.s

record.o: record.cpp.o

.PHONY : record.o

# target to build an object file
record.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/record.cpp.o
.PHONY : record.cpp.o

record.i: record.cpp.i

.PHONY : record.i

# target to preprocess a source file
record.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/record.cpp.i
.PHONY : record.cpp.i

record.s: record.cpp.s

.PHONY : record.s

# target to generate assembly for a file
record.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/record.cpp.s
.PHONY : record.cpp.s

system_catalog.o: system_catalog.cpp.o

.PHONY : system_catalog.o

# target to build an object file
system_catalog.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/system_catalog.cpp.o
.PHONY : system_catalog.cpp.o

system_catalog.i: system_catalog.cpp.i

.PHONY : system_catalog.i

# target to preprocess a source file
system_catalog.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/system_catalog.cpp.i
.PHONY : system_catalog.cpp.i

system_catalog.s: system_catalog.cpp.s

.PHONY : system_catalog.s

# target to generate assembly for a file
system_catalog.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/system_catalog.cpp.s
.PHONY : system_catalog.cpp.s

type.o: type.cpp.o

.PHONY : type.o

# target to build an object file
type.cpp.o:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/type.cpp.o
.PHONY : type.cpp.o

type.i: type.cpp.i

.PHONY : type.i

# target to preprocess a source file
type.cpp.i:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/type.cpp.i
.PHONY : type.cpp.i

type.s: type.cpp.s

.PHONY : type.s

# target to generate assembly for a file
type.cpp.s:
	$(MAKE) -f CMakeFiles/storageManager.dir/build.make CMakeFiles/storageManager.dir/type.cpp.s
.PHONY : type.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... storageManager"
	@echo "... edit_cache"
	@echo "... ddl.o"
	@echo "... ddl.i"
	@echo "... ddl.s"
	@echo "... dml.o"
	@echo "... dml.i"
	@echo "... dml.s"
	@echo "... file.o"
	@echo "... file.i"
	@echo "... file.s"
	@echo "... index.o"
	@echo "... index.i"
	@echo "... index.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... page.o"
	@echo "... page.i"
	@echo "... page.s"
	@echo "... record.o"
	@echo "... record.i"
	@echo "... record.s"
	@echo "... system_catalog.o"
	@echo "... system_catalog.i"
	@echo "... system_catalog.s"
	@echo "... type.o"
	@echo "... type.i"
	@echo "... type.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

