cmake_minimum_required(VERSION 3.16)
project(CGAnimator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1) Busca e configura o GLM (header-only)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# 2) Busca e adiciona o GLAD como um sub-módulo CMake
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.34
)
FetchContent_MakeAvailable(glad)
# → isso cria o target `glad` para linkarmos

# Diretórios do seu projeto
set(SRC_DIR       "${CMAKE_SOURCE_DIR}/src")
set(EXTERNAL_DIR  "${CMAKE_SOURCE_DIR}/external")
set(INCLUDE_DIR   "${CMAKE_SOURCE_DIR}/include")
set(SHADERS_DIR   "${CMAKE_SOURCE_DIR}/Shaders")
set(TEXTURES_DIR  "${CMAKE_SOURCE_DIR}/textures")

# Coleta fontes e headers da sua aplicação
file(GLOB_RECURSE APP_SOURCES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.c"
    "${EXTERNAL_DIR}/stb_image/stb_image.cpp"
)
file(GLOB_RECURSE APP_HEADERS
    "${INCLUDE_DIR}/*.h"
)
file(GLOB_RECURSE SHADER_FILES
    "${SHADERS_DIR}/*.vert"
    "${SHADERS_DIR}/*.frag"
    "${SHADERS_DIR}/*.vs"
    "${SHADERS_DIR}/*.fs"
)

add_executable(${PROJECT_NAME}
    ${APP_SOURCES}
    ${APP_HEADERS}
)

target_sources(${PROJECT_NAME} PRIVATE ${SHADER_FILES})

# Coloca o .exe em build/bin/
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Inclui seus headers e o include que o GLAD e o GLFW expõem
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDE_DIR}
        ${EXTERNAL_DIR}
        # o sub-diretório include do GLFW (quando você o clonou)
        "${CMAKE_SOURCE_DIR}/external/glfw/include"
)
# GLM já vem com target glm::glm; GLAD gera target glad
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glm::glm
        glad
)

# No Windows linka opengl32 e glfw3.lib, e copia o DLL
if (WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            opengl32
            "${CMAKE_SOURCE_DIR}/external/glfw/lib/windows/glfw3.lib"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/external/glfw/lib/windows/glfw3.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
            GL
            glfw
            dl
    )
endif()

# Copia shaders e texturas ao lado do executável
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADERS_DIR}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TEXTURES_DIR}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
)

message(STATUS "CMake configurado para ${CMAKE_SYSTEM_NAME}")
