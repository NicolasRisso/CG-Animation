# Projeto CGAnimator

Este projeto implementa um sistema de animação 3D com OpenGL e C++, permitindo a renderização do texto "EACH 20 ANOS" animado.

## Desenvolvido Por

Nicolas Pereira Risso Vieira

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

### Linux (NÃO TESTADO, O PROJETO FOI FEITO EM WINDOWS)
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
  --help        Exibir esta ajuda
```

## Funcionalidades

- Renderização de modelos 3D
- Iluminação dinâmica com luzes que mudam de cor e de posição
- Operações Geométricas feitas por Composição (Adicione  componentes de animação á objetos de cena para animá-los)
- Modelagem de formas por meio de Marching Cubes
- Operação Boleana para Modelagem (Letras foram feitas assim)

## Requisitos

- OpenGL 4.5 ou superior
- Compilador com suporte a C++17
- CMake 3.16 ou superior
