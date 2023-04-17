cmake_minimum_required(VERSION 3.0.0)
project(render VERSION 0.1.0)

include(CTest)
enable_testing()



set(CMAKE_EXPORT_COMPILE_COMMANDS True)






file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE GLAD_SRC "vendor/glad/src/*.c")

message("source fiels ${SRC}")

add_executable(render ${SRC} ${GLAD_SRC})
include("vendor.cmake")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
