cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(storageManager main.cpp ddl.cpp ddl.h dml.cpp dml.h system_catalog.cpp system_catalog.h type.cpp type.h file.cpp file.h index.cpp index.h page.cpp page.h record.cpp record.h defs.h helpers.cpp helpers.h directory.cpp directory.h bucket.cpp bucket.h)
