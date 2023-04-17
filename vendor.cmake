add_subdirectory(vendor/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(vendor/spdlog)
include_directories("${PROJECT_SOURCE_DIR}/vendor/glfw/include" "${PROJECT_SOURCE_DIR}/vendor/glad/include" "${PROJECT_SOURCE_DIR}/vendor/spdlog/include", "${PROJECT_SOURCE_DIR}/vendor/stb_image")