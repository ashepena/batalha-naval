#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
#define TAM    10
#define AGUA   '~'
#define ACERTO 'X'
#define FALHA  'O'
#define NTIPOS  4
 
/* Dados dos navios */
char nomes[NTIPOS][16]   = {"Porta-avioes","Cruzador","Submarino","Destroyer"};
int  tamanhos[NTIPOS]    = {5, 4, 3, 2};
int  quantidades[NTIPOS] = {1, 2, 3, 4};
char simbolos[NTIPOS]    = {'P','C','S','D'};
 
/* Variaveis globais que guardam a ultima coordenada lida */
int coordL = 0;
int coordC = 0;
 
/* ---- Utilitarios ---- */
 
void limpar() {
    system("cls");
}
 
void pausar() {
    printf("\n  [ENTER para continuar]");
    while (getchar() != '\n');
    getchar();
    limpar();
}
 
/* ---- Tabuleiro ---- */
 /*Função que recebe o tabuleiro (matriz 10x10 de chars)
  e o preenche com AGUA ('~') */
int inicializar(char t[TAM][TAM]) {
    int i, j;
    for (i = 0; i < TAM; i++) 
        for (j = 0; j < TAM; j++)
            t[i][j] = AGUA;
}

//Função que imprime o tabuleiro no ecrã
int mostrar(char t[TAM][TAM], int esconder) {
    int i, j;
    printf("\n    ");
    for (j = 0; j < TAM; j++) printf(" %c ", 'A'+j);  /*Imprime o cabeçalho de letras A a J
	                                                 'A'+j soma o valor ASCII de 'A' com j
	                                              obtendo as letras em sequência (A, B, C...) */
    printf("\n   +");
    for (j = 0; j < TAM; j++) printf("---");
    printf("+\n");

    for (i = 0; i < TAM; i++) {
        printf("%2d |", i+1);
        
        for (j = 0; j < TAM; j++) {
            char c = t[i][j];
            if (esconder && c != ACERTO && c != FALHA) c = AGUA; 
			/*Se esconder=1 E a célula não for acerto nem falha, 
			substitui por AGUA antes de imprimir
			os navios do adversário ficam ocultos. */
            printf(" %c ", c);
        }
        printf("|\n");
    }
    printf("   +");
    for (j = 0; j < TAM; j++) printf("---");
    printf("+\n");
}
 
/* ---- Posicionamento ---- */
 
 /*Verifica se um navio de tamanho 'tam' cabe em (l,c) 
 na direção dir (0=horizontal, 1=vertical)
 Devolve 1 se sim, 0 se não */
int cabe(char t[TAM][TAM], int l, int c, int tam, int dir) {
    int i;
    if (dir == 0) {
        if (c + tam > TAM) return 0;
        for (i = 0; i < tam; i++) if (t[l][c+i] != AGUA) return 0;
    } else {
        if (l + tam > TAM) return 0;
        for (i = 0; i < tam; i++) if (t[l+i][c] != AGUA) return 0;
    }
    return 1;
}
 
 /*Coloca o símbolo 's' do navio nas células corretas do tabuleiro na direção indicada*/
void inserir(char t[TAM][TAM], int l, int c, int tam, int dir, char s) {
    int i;
    if (dir == 0) for (i = 0; i < tam; i++) t[l][c+i] = s;
    else          for (i = 0; i < tam; i++) t[l+i][c] = s;
}
 
/* ---- Entrada do jogador ---- */
 
/* Le a coordenada e guarda em coordL e coordC (variaveis globais) */
void lerCoord() {
    char buf[10]; //buf: guarda o texto digitado (ex: "B4")
    char letra;
    int  num;
    while (1) {
        printf("  Coordenada (ex: B4): ");
        scanf("%s", buf);
        letra = buf[0]; //Primeiro caractere do que foi escrito
        if (letra >= 'a' && letra <= 'j') letra -= 32; //Converte letra minúscula para maiúscula
        num = atoi(buf + 1); 
		/*atoi converte texto em número inteiro
		buf+1 aponta para o segundo caractere em diante */
        if (letra >= 'A' && letra <= 'J' && num >= 1 && num <= 10) {
            coordC = letra - 'A'; //'A' (0 a 9)
            coordL = num - 1; //(0 a 9)
            return;
        }
        printf("  Invalido! Letra A-J e numero 1-10.\n");
    }
}
 
 /*Pede ao jogador a direção do navio
 Devolve 0 para horizontal, 1 para vertical*/
int lerDir() {
    char op;
    while (1) {
        printf("  (H)orizontal ou (V)ertical? ");
        scanf(" %c", &op);
        if (op=='H'||op=='h') return 0;
        if (op=='V'||op=='v') return 1;
        printf("  Digite H ou V.\n");
    }
}
 
/* ---- Colocar navios ---- */
 
void posicionarHumano(char t[TAM][TAM], int jogador) {
    int tp, k, dir; //tp = tipo de navio (0 a 3); k = qual navio do mesmo tipo
    printf("\n=== JOGADOR %d: posicione os navios ===\n", jogador);
    for (tp = 0; tp < NTIPOS; tp++) {
        for (k = 0; k < quantidades[tp]; k++) {
            mostrar(t, 0); //Mostra o tabuleiro atual e informa qual navio está sendo colocado
            printf("\n  Colocando: %s %d/%d (tamanho %d)\n",
                   nomes[tp], k+1, quantidades[tp], tamanhos[tp]);
            while (1) {
                lerCoord();
                dir = lerDir();
                if (cabe(t, coordL, coordC, tamanhos[tp], dir)) {
                    inserir(t, coordL, coordC, tamanhos[tp], dir, simbolos[tp]);
                    //insere o navio e sai do ciclo
                    break;
                }
                printf("  Posicao invalida! O navio sai do tabuleiro ou sobrepoe outro.\n");
            }
        }
    }
    mostrar(t, 0); //Mostra o tabuleiro final com todos os navios posicionados.
}
 
 //O computador posiciona os navios de forma aleatória
void posicionarPC(char t[TAM][TAM]) {
    int tp, k, l, c, dir;
    for (tp = 0; tp < NTIPOS; tp++)
        for (k = 0; k < quantidades[tp]; k++) {
            do { //Gera posição e direção aleatórias com rand()
                l = rand() % TAM; c = rand() % TAM; dir = rand() % 2;
            } while (!cabe(t, l, c, tamanhos[tp], dir)); 
            //Repete até encontrar uma posição válida
            inserir(t, l, c, tamanhos[tp], dir, simbolos[tp]);
            //Coloca o navio na posição encontrada.
        }
}
 
/* ---- Jogo ---- */
 
 //Conta quantas células de navios ainda não foram atingidas
int naviosRestantes(char t[TAM][TAM]) {
    int i, j, n = 0;
    for (i = 0; i < TAM; i++)
        for (j = 0; j < TAM; j++)
            if (t[i][j] != AGUA && t[i][j] != ACERTO && t[i][j] != FALHA)
            /*Percorre todo o tabuleiro. Se uma célula não for ~, X ou O, 
			é um navio intacto */
                n++; //incrementa n.
    return n; //Devolve esse número
}
 
/* Turno humano
   mostra so o tabuleiro do adversario.
   Acerto = joga de novo. Falha = troca (return 0).
   Vitoria = return 1. */
int turnoHumano(char dele[TAM][TAM], int numJog) {
    printf("\n=== JOGADOR %d ataca ===\n", numJog);
    while (1) { // enquanto o jogador acertar, continua dentro deste ciclo
        printf("\n  Tabuleiro do adversario (%d navios restantes):\n",
               naviosRestantes(dele));
        mostrar(dele, 1);
        do { //Loop que garante que o jogador não repete uma posição já atacada
            lerCoord();
            if (dele[coordL][coordC] == ACERTO || dele[coordL][coordC] == FALHA)
                printf("  Ja atacou aqui! Escolha outra posicao.\n");
        } while (dele[coordL][coordC] == ACERTO || dele[coordL][coordC] == FALHA);
 
        if (dele[coordL][coordC] != AGUA) {
            dele[coordL][coordC] = ACERTO;
            printf("  *** ACERTO! Voce joga de novo! ***\n");
            if (naviosRestantes(dele) == 0) return 1; 
			/*Se não restam navios, o jogador venceu. 
			Devolve 1 para sinalizar vitória */
        } else {
            dele[coordL][coordC] = FALHA;
            printf("  --- Agua! Troca de turno. ---\n");
            return 0; //o turno passou para o adversário
        }
    }
}
 
/* Turno do computador */
int turnoPC(char humano[TAM][TAM]) {
    int l, c;
    printf("\n=== TURNO DO COMPUTADOR ===\n");
    while (1) {
        do { //Gera posição aleatória, repetindo se já foi atacada anteriormente.
            l = rand() % TAM; c = rand() % TAM;
        } while (humano[l][c] == ACERTO || humano[l][c] == FALHA);
 
        printf("  PC ataca: %c%d\n", 'A'+c, l+1); //Mostra onde o PC atacou
 
        if (humano[l][c] != AGUA) {
            humano[l][c] = ACERTO;
            printf("  *** PC ACERTOU num dos seus navios! Joga de novo! ***\n");
            if (naviosRestantes(humano) == 0) return 1; 
            //Se destruiu tudo, devolve 1 (vitória do PC).
        } else {
            humano[l][c] = FALHA;
            printf("  --- PC errou! Sua vez. ---\n");
            return 0; //turno do jogador
        }
    }
}
 
/* ---- Menu de fim de jogo ---- */
 
int menuFim(int venceu, int modoVsPC) {
    char op;
    limpar();
    if (venceu) {
        printf("  +------------------------------------------+\n");
        printf("  |                                          |\n");
        printf("  |      PARABENS! VOCE GANHOU!              |\n");
        printf("  |                                          |\n");
        printf("  |  Todos os navios do adversario foram     |\n");
        printf("  |  destruidos. Excelente estrategia!       |\n");
        printf("  |                                          |\n");
        printf("  +------------------------------------------+\n");
    } else {
        printf("  +------------------------------------------+\n");
        printf("  |                                          |\n");
        printf("  |      QUE PENA, VOCE PERDEU...            |\n");
        printf("  |                                          |\n");
        if (modoVsPC)
        printf("  |  O computador destruiu todos os seus     |\n");
        else
        printf("  |  O adversario destruiu todos os seus     |\n");
        printf("  |  navios. Mais sorte na proxima vez!      |\n");
        printf("  |                                          |\n");
        printf("  +------------------------------------------+\n");
    }
    printf("\n  Quer jogar de novo?\n");
    printf("  (S) Sim   (N) Nao\n");
    printf("  Opcao: ");
    while (1) {
        scanf(" %c", &op);
        if (op=='S'||op=='s') return 1;
        if (op=='N'||op=='n') return 0;
        printf("  Digite S ou N: ");
    }
}
 
/* ---- Modos de jogo ---- */
 
 /*Função que gere o modo 1 vs 1. 
 Cria os dois tabuleiros e controla o fluxo do jogo. */
void jogo1v1() {
    char t1[TAM][TAM], t2[TAM][TAM]; 
	//Cria os dois tabuleiros (matrizes locais) e inicializa ambos com AGUA.
    int resultado;
    inicializar(t1); inicializar(t2);
    limpar();
    posicionarHumano(t1, 1); pausar(); 
    posicionarHumano(t2, 2); pausar();
    //Cada jogador posiciona os navios
    //pausar() limpa o ecrã entre os dois para não se verem os navios um do outro
 
    while (1) { //Ciclo principal do jogo, repete até alguém ganhar.
        if (turnoHumano(t2, 1)) { resultado = menuFim(1, 0); break; }
        //Executa o turno do Jogador 1. Se devolver 1 (vitória), anuncia o vencedor e sai do ciclo
        pausar();
        if (turnoHumano(t1, 2)) { resultado = menuFim(0, 0); break; }
        //Turno do Jogador 2. Se vencer, anuncia e termina
        pausar(); //pausar() antes de voltar ao J1
    }
    if (resultado) jogo1v1();
}

 //Função que gere o modo Jogador vs Computador.
void jogoVsPC() {
    char humano[TAM][TAM], pc[TAM][TAM]; //Cria e inicializa os dois tabuleiros (humano e PC)
    int resultado;
    inicializar(humano); inicializar(pc); 
    //O humano posiciona os navios Depois o PC posiciona os dele de forma aleatória (oculta)
    limpar();
    posicionarHumano(humano, 1);
    posicionarPC(pc);
    printf("\n  [O computador posicionou os navios secretamente]\n");
    pausar();
 
    while (1) {
        if (turnoHumano(pc, 1)) { resultado = menuFim(1, 1); break; }
        //Turno do humano. Se vencer, anuncia e sai
        pausar(); //pausar para dar visibilidade ao resultado
        if (turnoPC(humano))    { resultado = menuFim(0, 1); break; }
        //Turno do PC. Se vencer, anuncia, revela os navios do PC e termina
        pausar();
    }
    if (resultado) jogoVsPC();
}
 
/* ---- Regras ---- */
 
 //Função que imprime o ecrã de regras
void regras() {
    limpar();
    printf("============================================================\n");
    printf("             COMO JOGAR - BATALHA NAVAL\n");
    printf("============================================================\n\n");
 
    printf("[ O QUE E A BATALHA NAVAL? ]\n");
    printf("  E um jogo de estrategia entre dois adversarios.\n");
    printf("  Cada um esconde uma frota de navios num tabuleiro\n");
    printf("  e tenta descobrir e afundar os navios do outro\n");
    printf("  antes que o adversario afunde os seus.\n\n");
 
    printf("[ O TABULEIRO ]\n");
    printf("  Cada jogador tem uma grelha de 10x10 celulas.\n");
    printf("  As COLUNAS sao letras de A ate J (esquerda p/ direita).\n");
    printf("  As LINHAS sao numeros de 1 ate 10 (cima p/ baixo).\n");
    printf("  Para indicar uma celula, escreva letra + numero.\n");
    printf("  Exemplos validos: A1  B7  J10  C3  F5\n\n");
 
    printf("[ OS NAVIOS ]\n");
    printf("  Cada jogador tem 10 navios para esconder:\n\n");
    printf("  Simbolo | Nome          | Qtd | Tamanho\n");
    printf("  --------+---------------+-----+-------------------\n");
    printf("    [P]   | Porta-avioes  |  1  | PPPPP  (5 celulas)\n");
    printf("    [C]   | Cruzador      |  2  | CCCC   (4 celulas)\n");
    printf("    [S]   | Submarino     |  3  | SSS    (3 celulas)\n");
    printf("    [D]   | Destroyer     |  4  | DD     (2 celulas)\n\n");
 
    printf("[ FASE 1 - POSICIONAMENTO ]\n");
    printf("  Antes de comecar, cada jogador coloca os navios.\n");
    printf("  Para cada navio voce escolhe:\n");
    printf("    1. A coordenada inicial (ex: D3)\n");
    printf("    2. A direcao: H=horizontal  V=vertical\n\n");
    printf("  Exemplo - Submarino (3 celulas) em D3 horizontal:\n");
    printf("    Ocupa D3, E3, F3\n\n");
    printf("  Exemplo - Submarino (3 celulas) em D3 vertical:\n");
    printf("    Ocupa D3, D4, D5\n\n");
    printf("  Regras: o navio nao pode sair do tabuleiro nem\n");
    printf("  ficar em cima de outro navio ja colocado.\n\n");
 
    printf("[ FASE 2 - COMBATE ]\n");
    printf("  Os jogadores atacam em turnos, escolhendo uma\n");
    printf("  coordenada do tabuleiro do adversario.\n\n");
    printf("  [X] ACERTO - havia um navio nessa celula!\n");
    printf("      --> Voce joga DE NOVO! Continue atacando.\n");
    printf("  [O] FALHA  - era agua, nao havia navio ali.\n");
    printf("      --> O turno passa para o adversario.\n\n");
    printf("  Nao e possivel atacar a mesma celula duas vezes.\n\n");
 
    printf("[ SIMBOLOS NO TABULEIRO ]\n");
    printf("  ~        = Agua (ainda nao atacada)\n");
    printf("  X        = Acerto (havia navio aqui)\n");
    printf("  O        = Falha (era agua)\n");
    printf("  P/C/S/D  = Seus navios (so visiveis no seu ecra)\n\n");
 
    printf("[ COMO VENCER ]\n");
    printf("  Vence quem destruir TODOS os navios do adversario.\n");
    printf("  Um navio afunda quando todas as suas celulas\n");
    printf("  forem marcadas com X.\n\n");
 
    printf("============================================================\n");
    pausar();
}
 
/* ---- Menu principal ---- */
 
int main() {
    int op;
    srand((unsigned int)time(NULL));
    while (1) {
    	//Ciclo infinito do menu, o jogo fica no menu até o utilizador escolher 0 (Sair)
        limpar();
        printf("  ==========================================\n");
        printf("              BATALHA NAVAL\n");
        printf("  ==========================================\n\n");
        printf("    1. 1 vs 1  (dois jogadores)\n");
        printf("    2. Jogador vs Computador\n");
        printf("    3. Como Jogar (Regras)\n");
        printf("    0. Sair\n\n");
        printf("  ==========================================\n");
        printf("  Opcao: ");
        scanf("%d", &op);
        if      (op == 1) jogo1v1();
        else if (op == 2) jogoVsPC();
        else if (op == 3) regras();
        else if (op == 0) { printf("\n  Ate logo!\n\n"); break; }
        else printf("  Opcao invalida! Escolha 0, 1, 2 ou 3.\n");
    }
    return 0;
}
