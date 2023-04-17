cmake_minimum_required(VERSION 3.0.0)
project(render VERSION 0.1.0)

include(CTest)
enable_testing()



set(CMAKE_EXPORT_COMPILE_COMMANDS True)


include_directories("${PROJECT_SOURCE_DIR}/vendor/glad/include" "${PROJECT_SOURCE_DIR}/vendor/glfw/include")

aux_source_directory("vendor/glad/src" GLAD_SRC)
aux_source_directory("src/" SRC)
aux_source_directory("src/Shader" SHADER_SRC)
add_executable(render ${SHADER_SRC} ${SRC} ${GLAD_SRC})

include("vendor.cmake")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
