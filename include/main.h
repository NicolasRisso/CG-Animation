#ifndef MAIN_H
#define MAIN_H

#include <string>

// Constantes padrão
const int DEFAULT_WIDTH = 1920;
const int DEFAULT_HEIGHT = 1080;
const int TOTAL_FRAMES = 180;  // 3 segundos a 60 FPS
const std::string OUTPUT_DIR = "./output";

// Modos de visualização
enum class ViewMode {
    INTERACTIVE,  // Modo interativo (janela permanece aberta)
    RENDER_ONLY   // Apenas renderiza e salva frames
};

// Função principal para renderização da animação
bool RenderAnimation(const std::string& outputDir, int totalFrames, ViewMode viewMode = ViewMode::RENDER_ONLY);

#endif // MAIN_H
