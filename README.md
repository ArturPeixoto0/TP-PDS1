# TP-PDS1

## Trabalho Final — Programação e Desenvolvimento de Software 1

Projeto desenvolvido para a disciplina de **Programação e Desenvolvimento de Software 1 (PDS1)**, utilizando a linguagem **C** e a biblioteca gráfica **Allegro 5**.

O trabalho consiste no desenvolvimento de um **jogo de nave em 2D**, no qual o jogador deve controlar uma nave, enfrentar inimigos, acumular pontos e utilizar melhorias durante a partida.

## 🎮 Sobre o jogo

O jogador controla uma nave que deve sobreviver ao avanço de diferentes inimigos.

Durante a partida, é possível:

- Movimentar a nave pelo cenário;
- Disparar tiros contra os inimigos;
- Destruir inimigos e acumular pontos;
- Coletar **Power-Ups**;
- Utilizar diferentes cenários e sprites;
- Enfrentar inimigos com diferentes características;
- Acompanhar a pontuação durante a partida;
- Registrar e atualizar o recorde obtido.

A dificuldade visual do jogo também evolui conforme a pontuação aumenta, alterando os cenários e os inimigos apresentados.

## 🕹️ Controles

| Tecla | Ação |
|---|---|
| `W` | Mover para cima |
| `A` | Mover para a esquerda |
| `S` | Mover para baixo |
| `D` | Mover para a direita |
| `SPACE` | Atirar |

## ⚙️ Tecnologias utilizadas

- **C**
- **Allegro 5**
  - Allegro
  - Allegro Primitives
  - Allegro Font
  - Allegro TTF
  - Allegro Image
  - Allegro Audio
  - Allegro Audio Codec
- **GCC**
- **Makefile**

## 📁 Estrutura do projeto

```text
TP-PDS1/
│
├── chain.c
├── Makefile
├── README.md
├── Enunciado TP Allegro 2026_01.pdf
│
├── imagens/
│   ├── PowerUp.png
│   ├── fundo0.jpeg
│   ├── fundo1.jpeg
│   ├── fundo2.jpeg
│   ├── fundo3.jpeg
│   ├── fundoMorte.jpeg
│   ├── jogador0.png
│   ├── jogador1.png
│   ├── inimigo0.png
│   ├── inimigo1.png
│   ├── inimigo2.png
│   ├── inimigo3.png
│   ├── tiro0.png
│   └── tiro1.png
│
├── sons/
│   └── somAmbiente.wav
│
├── include/
│   └── arquivos de suporte da Allegro
│
├── lib/
│   └── bibliotecas da Allegro
│
├── arial.ttf
├── allegro-5.0.10-monolith-mt.dll
├── historico.txt
└── recorde.dat
```

## 🧩 Principais funcionalidades

### Movimentação

A nave possui movimentação horizontal e vertical, limitada aos limites da tela.

### Sistema de tiros

O jogo possui diferentes estados para os tiros, permitindo controlar:

- Tiro inativo;
- Tiro ativo;
- Tiro aprimorado através do Power-Up.

### Inimigos

Os inimigos são gerados continuamente e possuem:

- Posição e velocidade aleatórias;
- Tamanho variável;
- Diferentes sprites conforme a pontuação;
- Sistema de colisão;
- Comportamento próprio após serem atingidos.

### Power-Up

Durante a partida, Power-Ups podem aparecer no cenário.

Ao coletá-los, o jogador recebe um tiro aprimorado temporariamente, aumentando o alcance do disparo.

### Sistema de pontuação

A pontuação é atualizada durante a partida e aumenta conforme os inimigos são destruídos.

Além disso, a pontuação sofre uma penalidade progressiva ao longo do tempo, aumentando a dificuldade de manter uma pontuação elevada.

### Sistema de recorde

Ao final da partida, a pontuação é comparada com o recorde armazenado em `historico.txt`.

Caso o jogador supere o recorde anterior, um novo recorde é registrado.

## 🔨 Compilação

O projeto possui um **Makefile** configurado para realizar a compilação utilizando o GCC e as bibliotecas da Allegro.

Para compilar o projeto, utilize:

```bash
make
```

Para remover os arquivos gerados pela compilação:

```bash
make clean
```

O executável gerado pelo processo de compilação é:

```text
chain.exe
```

## ▶️ Execução

Após a compilação, execute o arquivo:

```text
chain.exe
```

É importante manter os arquivos de recursos do projeto nas posições esperadas, principalmente:

- `imagens/`
- `sons/`
- `arial.ttf`
- bibliotecas da Allegro

Esses arquivos são utilizados durante a execução para carregar os gráficos, fontes e áudio do jogo.

## 📚 Objetivos acadêmicos

O desenvolvimento do projeto permitiu aplicar conceitos trabalhados ao longo da disciplina, incluindo:

- Programação estruturada em C;
- Funções;
- Structs;
- Ponteiros;
- Vetores;
- Manipulação de arquivos;
- Entrada de dados;
- Gerenciamento de memória;
- Compilação e linkedição;
- Utilização de bibliotecas externas;
- Manipulação de eventos;
- Desenvolvimento de aplicações gráficas;
- Detecção de colisões;
- Controle de tempo e atualização de estados.

## 📄 Enunciado

O enunciado do trabalho está disponível no próprio repositório:

**Enunciado TP Allegro 2026/01**

## 👨‍💻 Autor

**Artur Peixoto**

Estudante de Ciência da Computação — UFMG.

---

Desenvolvido como trabalho acadêmico para a disciplina de **Programação e Desenvolvimento de Software 1 — PDS1**.
