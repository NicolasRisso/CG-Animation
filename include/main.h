#ifndef MAIN_H
#define MAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <thread>

#include "window.h"
#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "renderer.h"

// Constantes padrão
const int DEFAULT_WIDTH = 1280;
const int DEFAULT_HEIGHT = 720;
const int TOTAL_FRAMES = 180;  // 3 segundos a 60 FPS
const std::string OUTPUT_DIR = "./output";

// Modos de visualização
enum class ViewMode {
    INTERACTIVE,  // Modo interativo (janela permanece aberta)
    RENDER_ONLY   // Apenas renderiza e salva frames
};

// Função principal para renderização da animação
bool renderAnimation(const std::string& outputDir, int totalFrames, ViewMode viewMode = ViewMode::RENDER_ONLY);

#endif // MAIN_H
