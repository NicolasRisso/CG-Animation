# Projeto CGAnimator

Este projeto implementa um sistema de animação 3D com OpenGL, permitindo a renderização de um cubo com textura metálica, iluminação dinâmica e exportação para vídeo.

## Estrutura do Projeto

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
├── Makefile              # Makefile para compilação direta
├── GUIA_EXPANSAO.md      # Guia detalhado para expansão e customização
└── config.h.in           # Template de configuração
```

## Compilação

### Windows (Visual Studio ou Rider)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Linux
```bash
mkdir build
cd build
cmake ..
make
```

## Execução

Para executar o programa corretamente, você deve estar no diretório onde o executável foi gerado:
```bash
cd build/bin/Release  # Windows
cd build              # Linux
./CGAnimator
```

## Opções de Linha de Comando

```
Uso: CGAnimator [opções]
Opções:
  --width N     Largura da janela (padrão: 1280)
  --height N    Altura da janela (padrão: 720)
  --frames N    Número total de frames (padrão: 180)
  --output DIR  Diretório de saída (padrão: ./output)
  --mode MODE   Modo de visualização (interactive/render, padrão: interactive)
  --help        Exibir esta ajuda
```

## Funcionalidades

- Renderização de um cubo 3D com textura metálica
- Iluminação dinâmica com luzes que mudam de cor
- Rotação automática do cubo
- Exportação de frames para imagens
- Combinação de frames em vídeo
- Modo interativo para visualização em tempo real
- Modo de renderização para geração de vídeo

## Customização

Consulte o arquivo `GUIA_EXPANSAO.md` para instruções detalhadas sobre como expandir e customizar o projeto.

## Requisitos

- OpenGL 4.5 ou superior
- Compilador com suporte a C++17
- CMake 3.16 ou superior
