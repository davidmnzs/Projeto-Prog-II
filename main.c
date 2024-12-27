#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#define TAM 4

int tabuleiro[TAM][TAM];
int linhas = TAM, colunas = TAM;
int linhaVazia, colunaVazia; // Para rastrear a posição do espaço vazio


void verificarVitoria(int tabuleiro[linhas][colunas]){
    int count =1, verif =0;

    for (int i = 0; i<linhas; i++) {
        for (int j = 0; j<colunas; j++) {
            if (tabuleiro[i][j]==count) {
                verif = verif + 1;

            }
            count = count+1;

        }
    }
    if (verif==15) {
        printf("Voce venceu");
    }
    //so pra verificar se funciona
    else {
        printf("deu bom nao");
    }
}

// Inicializar o tabuleiro com números de 1 a 15 e espaço vazio
void inicializarTabuleiro() {
    int valor = 1;
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (valor < TAM * TAM) {
                tabuleiro[i][j] = valor++;
            } else {
                tabuleiro[i][j] = 0; // Espaço vazio
                linhaVazia = i; // Salvar posição inicial do espaço vazio
                colunaVazia = j;
            }
        }
    }
}

// Função para exibir o tabuleiro
void exibirTabuleiro() {
    printf("\nTabuleiro:\n\n");
    for (int i = 0; i < TAM; i++) {
        printf("+----+----+----+----+\n");
        for (int j = 0; j < TAM; j++) {
            if (tabuleiro[i][j] == 0) {
                printf("|    ");
            } else {
                printf("| %2d ", tabuleiro[i][j]);
            }
        }
        printf("|\n");
    }
    printf("+----+----+----+----+\n");
}

// Embaralhar o tabuleiro
void embaralharPeca() {
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) { // Realizar múltiplas trocas para um bom embaralhamento
        int x1 = rand() % TAM, y1 = rand() % TAM;
        int x2 = rand() % TAM, y2 = rand() % TAM;
        int temp = tabuleiro[x1][y1];
        tabuleiro[x1][y1] = tabuleiro[x2][y2];
        tabuleiro[x2][y2] = temp;

        // Atualizar posição do espaço vazio se for trocado
        if (tabuleiro[x1][y1] == 0) {
            linhaVazia = x1;
            colunaVazia = y1;
        } else if (tabuleiro[x2][y2] == 0) {
            linhaVazia = x2;
            colunaVazia = y2;
        }
    }
}

// Exibir regras do jogo
void exibirRegras() {
    printf("\nRegras do Jogo dos 15:\n");
    printf("1. O tabuleiro é composto por números de 1 a 15 e um espaço vazio.\n");
    printf("2. O objetivo é organizar os números em ordem crescente, deixando o espaço vazio no final.\n");
    printf("3. Você pode mover peças para o espaço vazio adjacente usando W (cima), A (esquerda), S (baixo) e D (direita).\n");
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();
}

// Movimentar espaço vazio com WASD
void movimentarEspaco(char direcao) {
    int novaLinha = linhaVazia, novaColuna = colunaVazia;

    if (direcao == 'w' || direcao == 'W') {
        novaLinha--;
    } else if (direcao == 'a' || direcao == 'A') {
        novaColuna--;
    } else if (direcao == 's' || direcao == 'S') {
        novaLinha++;
    } else if (direcao == 'd' || direcao == 'D') {
        novaColuna++;
    }

    // Verificar se o movimento é válido
    if (novaLinha >= 0 && novaLinha < TAM && novaColuna >= 0 && novaColuna < TAM) {
        tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
        tabuleiro[novaLinha][novaColuna] = 0;
        linhaVazia = novaLinha;
        colunaVazia = novaColuna;

        //aqui
        verificarVitoria(tabuleiro);
    } else {
        printf("Movimento inválido!\n");
    }
}

// Jogar o jogo
void jogar() {
    inicializarTabuleiro();
    embaralharPeca();
    char movimento;

    do {
        exibirTabuleiro();
        printf("\nUse W, A, S, D para mover o espaço vazio. Pressione Q para sair.\n");
        printf("Movimento: ");
        scanf(" %c", &movimento);

        if (movimento == 'q' || movimento == 'Q') {
            printf("\nVocê saiu do jogo.\n");
            break;
        }

        movimentarEspaco(movimento);
    } while (1);
}


// Menu principal
void menu() {
    char opcao;
    do {
        printf("\nBem-vindo ao Jogo dos 15!\n\n");
        printf("A. Jogar\n");
        printf("B. Regras do jogo\n");
        printf("C. Sair\n");
        printf("\nOpção: ");
        scanf(" %c", &opcao);

        switch (opcao) {
        case 'a':
        case 'A':
            jogar();
            break;

        case 'b':
        case 'B':
            exibirRegras();
            break;

        case 'c':
        case 'C':
            printf("\nObrigado por jogar!\n");
            break;

        default:
            printf("\nOpção inválida!\n");
            sleep(1);
            break;
        }
    } while (opcao != 'c' && opcao != 'C');
}



int main() {
    menu();
    return 0;
}
