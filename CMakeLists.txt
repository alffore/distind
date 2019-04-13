cmake_minimum_required(VERSION 3.13)
project(distind C)

set(CMAKE_C_STANDARD 99)

add_executable(distind main.c distloc.h algoritmo.c cargadatos.c salida.c)
target_link_libraries(distind m)