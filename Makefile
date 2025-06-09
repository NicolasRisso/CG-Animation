# Makefile para compilação multiplataforma do projeto CubeAnimation
# Suporta Windows e Linux

# Variáveis de configuração
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -Iinclude -Iexternal -Iexternal/glad -Iexternal/glm -Iexternal/stb_image
OUTPUT_DIR = bin
OBJ_DIR = obj

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    # Configurações para Windows
    LIBS = -lopengl32 -lglfw3 -lgdi32
    EXECUTABLE = $(OUTPUT_DIR)/CubeAnimation.exe
    MKDIR = mkdir -p
    RM = del /Q /F
    RMDIR = rmdir /Q /S
    SEP = \\
else
    # Configurações para Linux
    LIBS = -lGL -lglfw -ldl -lpthread
    EXECUTABLE = $(OUTPUT_DIR)/CubeAnimation
    MKDIR = mkdir -p
    RM = rm -f
    RMDIR = rm -rf
    SEP = /
endif

# Arquivos fonte
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES)) $(OBJ_DIR)/glad.o

# Regras
all: directories $(EXECUTABLE)

directories:
	$(MKDIR) $(OUTPUT_DIR)
	$(MKDIR) $(OBJ_DIR)
	$(MKDIR) $(OUTPUT_DIR)$(SEP)shaders
	$(MKDIR) $(OUTPUT_DIR)$(SEP)textures
	$(MKDIR) $(OUTPUT_DIR)$(SEP)output

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
	cp -r shaders/* $(OUTPUT_DIR)$(SEP)shaders
	cp -r textures/* $(OUTPUT_DIR)$(SEP)textures

$(OBJ_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/glad.o: external/glad/glad.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJECTS)
	$(RM) $(EXECUTABLE)

.PHONY: all clean directories
