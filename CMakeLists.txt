cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
project(CGAnimator)

# Definir variáveis para organização do projeto
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(EXTERNAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(TEXTURES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/textures)

# Adicionar diretórios de include
include_directories(
    ${INCLUDE_DIR}
    ${EXTERNAL_DIR}
    ${EXTERNAL_DIR}/glad
    ${EXTERNAL_DIR}/glm
    ${EXTERNAL_DIR}/stb_image
    ${EXTERNAL_DIR}/glfw/include
)

# Configuração específica para Windows
if(WIN32)
    set(GLFW_LIB_DIR ${EXTERNAL_DIR}/glfw/lib/windows)
    set(GLFW_LIBS ${GLFW_LIB_DIR}/glfw3.lib)
    set(OPENGL_LIBS opengl32)
    set(ADDITIONAL_LIBS "")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    # Copiar DLLs para o diretório de saída
    file(COPY ${GLFW_LIB_DIR}/glfw3.dll DESTINATION ${CMAKE_BINARY_DIR})
    
# Configuração específica para Linux
else()
    set(GLFW_LIBS glfw)
    set(OPENGL_LIBS GL)
    set(ADDITIONAL_LIBS dl)
endif()

# Criar diretórios de saída
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adicionar arquivos fonte
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/cube.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/renderer.cpp
    ${EXTERNAL_DIR}/glad/glad.c
    ${EXTERNAL_DIR}/stb_image/stb_image.cpp
)

# Adicionar executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Vincular bibliotecas
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBS} ${GLFW_LIBS} ${ADDITIONAL_LIBS})

# Copiar shaders e texturas para o diretório de saída
file(COPY ${SHADERS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${TEXTURES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Definir variáveis de configuração
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${INCLUDE_DIR}/config.h
)

# Copia a pasta shaders e textures para o output
add_custom_command(TARGET CGAnimator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:CGAnimator>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures
        $<TARGET_FILE_DIR:CGAnimator>/textures
)

# Mensagem de configuração concluída
message(STATUS "Configuração do CMake concluída para ${CMAKE_SYSTEM_NAME}")
