cmake_minimum_required(VERSION 2.8.12)
project(pybind_example)

#if instal in system use
#find_package(pybind11 REQUIRED)

add_subdirectory(pybind11)

add_subdirectory(0_base_fun)
add_subdirectory(1_base_struct)
add_subdirectory(2_class_inherit)

