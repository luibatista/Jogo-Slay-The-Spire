DOCUMENTACAO TECNICA: PROJETO SLAY THE SPIRE (CLONE)

1. VISAO GERAL DO PROJETO

Este projeto e uma implementacao simplificada de um jogo de cartas "deck-builder" roguelike, inspirado em Slay the Spire. O objetivo e escalar uma torre (vencer 10 combates consecutivos) utilizando um baralho de cartas para atacar e defender contra inimigos gerados aleatoriamente.

Tecnologias Utilizadas:
- Linguagem: C
- Biblioteca Grafica: Allegro 5 (Primitivas, Imagens, Fontes, Teclado)
- Compilacao: GCC via Makefile

2. ARQUITETURA DO SISTEMA

O codigo esta organizado de forma modular, separando a logica do jogo, a renderizacao grafica e as estruturas de dados.

Estrutura de Ficheiros:
- main.c: Contem o loop principal do jogo, gestao de estados, input do utilizador e logica de combate.
- renderer.c / .h: Responsavel por todo o desenho na tela (jogador, inimigos, cartas, UI).
- baralho.c / .h: Gere as pilhas de cartas (compra, descarte, mao, exilio) e operacoes de embaralhar.
- carta.c / .h: Define a estrutura de uma carta e a sua criacao aleatoria.
- jogador.h: Define a estrutura do jogador (vida, energia, baralho).
- inimigo.h: Define a estrutura dos inimigos (vida, intencoes).
- utils.c / .h: Funcoes utilitarias (inicializacao segura, ferramentas matematicas).
- constants.h: Constantes globais para configuracao (tamanhos, posicoes, teclas).

---

3. ESTRUTURAS DE DADOS PRINCIPAIS

3.1. Carta (Card)
A unidade basica de acao do jogo.
- Tipo: Ataque, Defesa, Especial, Coringa (se implementado)
- Custo: Energia necessaria para jogar
- Efeito: Valor numerico do dano ou escudo

3.2. Baralho (Baralho)
Gere as colecoes de cartas do jogador usando alocacao dinamica.
- compra: Pilha de onde o jogador saca cartas.
- mao: Cartas disponiveis para jogar no turno atual.
- descarte: Para onde vao as cartas jogadas.
- exiladas: Cartas removidas permanentemente do combate (se a logica for ativada).

3.3. Jogador (Jogador)
Mantem o estado do heroi.
- vida / vida_max: Saude do personagem.
- escudo: Protecao temporaria que zera a cada turno.
- energia / energia_max: Recurso gasto para jogar cartas.

3.4. Inimigo (Inimigo)
Define os oponentes e a sua IA simples.
- vida / vida_max
- escudo
- intencao_atual: O que o inimigo fara (ATACAR ou DEFENDER).
- valor_acao: Quanto de dano ou escudo aplicara.

---

4. FLUXO PRINCIPAL (main.c)

O jogo opera numa Maquina de Estados simples controlada pela variavel 'estado_atual'.

Estados do Jogo:
1. TURN_PLAYER: O jogador usa o teclado para selecionar e jogar cartas.
2. TURN_ENEMY: Os inimigos executam as suas intencoes.
3. GAME_OVER: O jogador perdeu toda a vida.
4. VICTORY: O jogador venceu 10 combates.

Ciclo de Jogo (Game Loop):
O loop executa a cada frame (60 FPS):

1. Captura de Eventos:
   - Verifica inputs do teclado (Setas, Enter, Espaco, E).
   - Verifica o Timer para redesenhar a tela.

2. Logica de Combate:
   - Jogador Joga Carta: Verifica energia -> Aplica efeito -> Move carta para descarte/exilio -> Reorganiza a mao.
   - Fim de Turno: Passa o estado para TURN_ENEMY.

3. Inteligencia Artificial (IA):
   - Executa no inicio do TURN_ENEMY.
   - Inimigos atacam ou ganham escudo.
   - Reset: Energia restaura, escudo do player zera, mao e descartada e novas cartas sao compradas.
   - Previsao: Novas intencoes sao sorteadas.

4. Renderizacao:
   - Chama Render() para desenhar o estado atual.


5. MECANICAS IMPLEMENTADAS

Sistema de Alvo:
- O jogador usa as setas Cima/Baixo para alternar entre os inimigos.
- Verificacao de seguranca: se o alvo morre, a mira pula para o proximo vivo.

Tipos de Inimigos:
- Fracos: Vida entre 10-35.
- Fortes: Vida > 40 (visualmente distintos).
- Intencoes: O jogo mostra "ATK" ou "DEF" acima do inimigo.

6. INTERFACE GRAFICA (renderer.c)

- Player: Circulo Azul a esquerda.
- Inimigos: Circulos Vermelhos a direita.
- Barras de Vida: Retangulos proporcionais.
- Cartas: Retangulos coloridos por tipo.
- Energia: Texto no topo da tela.

---

7. COMO COMPILAR E EXECUTAR

1. Compile: make game
2. Execute: ./game.out