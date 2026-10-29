# Batalha Naval em C

Jogo de Batalha Naval desenvolvido em linguagem C como trabalho prático da disciplina de Programação (ano letivo 2025/2026).

**Autores:** Ashlley Pena e Caio Affonso

## Sobre o projeto

Implementação do clássico jogo de estratégia Batalha Naval no terminal, cumprindo os requisitos definidos: sem uso de ponteiros ou structs, utilizando matrizes, funções, constantes e variáveis globais.

## Modos de jogo

- **1 vs 1** — dois jogadores no mesmo computador
- **Jogador vs Computador** — o PC posiciona os navios e joga automaticamente

## Funcionalidades

- Menu inicial com ecrã de regras e menu de fim de jogo
- Posicionamento manual dos navios ou automático (aleatório no modo PC)
- Validação de jogadas (coordenadas e direção)
- Ocultação do tabuleiro adversário entre turnos
- Regra personalizada: acertar num navio permite jogar novamente
- Opção de recomeçar após o fim do jogo

## Conceitos de programação praticados

- Matrizes (tabuleiro 10×10)
- Arrays globais paralelos para os dados dos navios
- Funções com parâmetros e retorno
- Constantes com `#define`
- Ciclos e estruturas de decisão
- Validação de entrada do utilizador
- Geração de posições aleatórias com `rand()`

## Como compilar e executar

```bash
gcc batalha_naval.c -o batalha_naval
./batalha_naval
