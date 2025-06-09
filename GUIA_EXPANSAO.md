# Guia de Expansão e Customização do CGAnimator no Windows

Este guia foi criado para ajudar você a expandir e customizar o CGAnimator em seu ambiente Windows, permitindo adicionar novas funcionalidades, modificar comportamentos existentes e experimentar com computação gráfica.

## Índice
1. [Configuração do Ambiente](#1-configuração-do-ambiente)
2. [Estrutura do Projeto](#2-estrutura-do-projeto)
3. [Compilação e Execução](#3-compilação-e-execução)
4. [Modo Interativo vs. Modo de Renderização](#4-modo-interativo-vs-modo-de-renderização)
5. [Customização do Cubo](#5-customização-do-cubo)
6. [Modificação de Shaders](#6-modificação-de-shaders)
7. [Adição de Novas Texturas](#7-adição-de-novas-texturas)
8. [Implementação de Novas Formas](#8-implementação-de-novas-formas)
9. [Controle de Câmera](#9-controle-de-câmera)
10. [Exportação de Vídeo](#10-exportação-de-vídeo)
11. [Dicas de Otimização](#11-dicas-de-otimização)
12. [Solução de Problemas Comuns](#12-solução-de-problemas-comuns)

## 1. Configuração do Ambiente

### Requisitos
- Visual Studio 2019/2022 ou JetBrains Rider
- CMake 3.16 ou superior
- Git (opcional, mas recomendado para controle de versão)

### Configuração da IDE
- **Visual Studio**:
  - Instale a extensão "CMake Tools for Visual Studio"
  - Abra o projeto selecionando "Abrir pasta" e navegue até a pasta do CGAnimator
  - O Visual Studio detectará automaticamente o arquivo CMakeLists.txt

- **Rider**:
  - Abra o projeto selecionando "Open" e navegue até a pasta do CGAnimator
  - Selecione "Open as Project" quando solicitado
  - Rider detectará automaticamente o arquivo CMakeLists.txt

## 2. Estrutura do Projeto

O projeto está organizado da seguinte forma:
```
CGAnimator/
├── include/              # Arquivos de cabeçalho (.h)
├── src/                  # Arquivos de implementação (.cpp)
├── external/             # Bibliotecas externas
│   ├── glad/             # OpenGL loader
│   ├── glfw/             # Gerenciamento de janelas
│   ├── glm/              # Matemática para OpenGL
│   └── stb_image/        # Carregamento de imagens
├── shaders/              # Shaders GLSL
├── textures/             # Texturas e imagens
├── CMakeLists.txt        # Configuração do CMake
└── config.h.in           # Template de configuração
```

## 3. Compilação e Execução

### Compilação via Terminal
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Execução
Para executar o programa corretamente, você deve estar no diretório onde o executável foi gerado:
```bash
cd build\bin\Release
.\CGAnimator.exe
```

### Opções de Linha de Comando
O CGAnimator suporta várias opções de linha de comando:
```
Uso: CGAnimator.exe [opções]
Opções:
  --width N     Largura da janela (padrão: 1280)
  --height N    Altura da janela (padrão: 720)
  --frames N    Número total de frames (padrão: 180)
  --output DIR  Diretório de saída (padrão: ./output)
  --mode MODE   Modo de visualização (interactive/render, padrão: interactive)
  --help        Exibir esta ajuda
```

### Configuração do Diretório de Trabalho na IDE
- **Visual Studio**:
  - Clique com o botão direito no projeto no Gerenciador de Soluções
  - Selecione "Propriedades"
  - Navegue até "Depuração"
  - Defina "Diretório de Trabalho" como `$(OutDir)` ou o caminho completo para `build\bin\Release`

- **Rider**:
  - Edite as configurações de execução (Run/Debug Configurations)
  - No campo "Working directory", defina o caminho para a pasta `build/bin/Release`

## 4. Modo Interativo vs. Modo de Renderização

O CGAnimator possui dois modos de operação:

### Modo Interativo (padrão)
- A janela permanece aberta após a renderização
- Você pode observar a animação em tempo real
- Pressione ESC para sair
- Execute com: `.\CGAnimator.exe --mode interactive`

### Modo de Renderização
- Renderiza todos os frames e salva no diretório de saída
- Combina os frames em um vídeo
- Fecha automaticamente após a conclusão
- Execute com: `.\CGAnimator.exe --mode render`

## 5. Customização do Cubo

Para modificar o cubo, você pode editar os seguintes arquivos:

### Propriedades do Cubo
Edite `include/cube.h` e `src/cube.cpp` para modificar:
- Tamanho do cubo
- Posição inicial
- Velocidade de rotação
- Propriedades do material

Exemplo de modificação da velocidade de rotação no `main.cpp`:
```cpp
// Modificar a velocidade de rotação
float rotationSpeed = 90.0f;  // 90 graus por segundo (dobro da velocidade original)
```

## 6. Modificação de Shaders

Os shaders estão localizados na pasta `shaders/`:
- `cube.vs`: Vertex shader (processamento de vértices)
- `cube.fs`: Fragment shader (processamento de pixels/cores)

### Exemplo: Modificar a Cor do Cubo
Edite `shaders/cube.fs` para alterar a cor base:
```glsl
// Encontre esta linha
vec3 objectColor = texture(material.diffuse, TexCoords).rgb;

// Modifique para
vec3 objectColor = texture(material.diffuse, TexCoords).rgb * vec3(1.0, 0.5, 0.5); // Tom avermelhado
```

### Exemplo: Adicionar Efeito de Pulso
Adicione ao `shaders/cube.fs`:
```glsl
// No início do arquivo, após as declarações existentes
uniform float time; // Tempo para animação

// No cálculo da cor final
vec3 result = ...;
// Adicione efeito de pulso
result *= 0.8 + 0.2 * sin(time * 2.0); // Pulsa entre 80% e 100% da intensidade
```

E atualize o shader no `renderer.cpp`:
```cpp
// Na função renderFrame
m_shader.use();
m_shader.setFloat("time", static_cast<float>(glfwGetTime())); // Passa o tempo atual para o shader
```

## 7. Adição de Novas Texturas

Para adicionar novas texturas:

1. Coloque o arquivo de imagem na pasta `textures/`
2. Modifique o código para carregar a nova textura:

```cpp
// Em cube.cpp, método initialize
if (!m_texture.load("textures/sua_nova_textura.jpg")) {
    std::cerr << "Falha ao carregar textura" << std::endl;
    return false;
}
```

### Suporte a Múltiplas Texturas
Para usar múltiplas texturas (como diffuse, specular, normal maps):

1. Modifique a classe `Texture` para suportar múltiplas texturas
2. Atualize os shaders para usar as texturas adicionais
3. Passe as texturas para o shader no método de renderização

## 8. Implementação de Novas Formas

Para adicionar novas formas geométricas:

1. Crie uma nova classe (ex: `Sphere.h` e `Sphere.cpp`) baseada na classe `Cube`
2. Defina os vértices, normais, coordenadas de textura e índices para a nova forma
3. Implemente os métodos de inicialização e renderização

Exemplo de implementação de uma esfera:
```cpp
// Em Sphere.h
class Sphere {
public:
    Sphere();
    ~Sphere();
    
    bool initialize(const std::string& texturePath);
    void render(Shader& shader, const glm::mat4& view, const glm::mat4& projection);
    
    // Getters e setters para posição, rotação, etc.
    
private:
    // Membros privados (VAO, VBO, EBO, textura, etc.)
    void setupMesh(float radius, int sectors, int stacks);
};
```

## 9. Controle de Câmera

Para implementar controle de câmera interativo:

1. Modifique a classe `Window` para capturar entrada do mouse e teclado
2. Atualize a classe `Camera` para responder a esses inputs

Exemplo de implementação no `window.cpp`:
```cpp
void Window::processInput(float deltaTime) {
    // Fechar janela com ESC
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    // Movimento da câmera com WASD
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    }
    // ... adicione mais controles conforme necessário
}
```

## 10. Exportação de Vídeo

O CGAnimator já suporta exportação de vídeo básica. Para implementar formatos ou configurações adicionais:

1. Modifique o método `combineFramesToVideo` na classe `Renderer`
2. Adicione suporte a bibliotecas como FFmpeg para mais opções de codificação

Para implementar a exportação real de vídeo (atualmente simulada):
```cpp
bool Renderer::combineFramesToVideo(const std::string& framesPath, const std::string& outputPath, 
                                   int frameCount, int fps) {
    // Usar FFmpeg via sistema para combinar frames
    std::string command = "ffmpeg -y -framerate " + std::to_string(fps) + 
                         " -i " + framesPath + "/frame_%05d.png" +
                         " -c:v libx264 -pix_fmt yuv420p " + outputPath;
    
    return (system(command.c_str()) == 0);
}
```

## 11. Dicas de Otimização

Para melhorar o desempenho do seu código:

1. **Batch Rendering**: Combine múltiplos objetos em um único draw call
2. **Frustum Culling**: Renderize apenas objetos visíveis pela câmera
3. **Level of Detail (LOD)**: Use modelos menos detalhados para objetos distantes
4. **Texture Compression**: Use texturas comprimidas para reduzir uso de memória
5. **Shader Optimization**: Simplifique cálculos em shaders para melhor desempenho

## 12. Solução de Problemas Comuns

### Problema: Janela fecha imediatamente
**Solução**: Execute o programa a partir do diretório correto ou configure o diretório de trabalho na IDE.

### Problema: Shaders não compilam
**Solução**: Verifique o console para mensagens de erro específicas. Corrija a sintaxe GLSL nos arquivos de shader.

### Problema: Texturas não carregam
**Solução**: Verifique se os caminhos das texturas estão corretos e se os arquivos existem no diretório esperado.

### Problema: Erros de linkagem
**Solução**: Certifique-se de que todas as dependências estão corretamente configuradas no CMakeLists.txt.

---

Este guia cobre os aspectos básicos para começar a expandir e customizar o CGAnimator. À medida que você se familiariza com o código, poderá implementar funcionalidades mais avançadas e experimentar com diferentes técnicas de computação gráfica.

Bom desenvolvimento!
