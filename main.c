#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define linhas 4
#define colunas 4
#define false 0
#define true 1
#define TAMaNHO_NOME 50

int tabuleiro[linhas][colunas];

typedef int BOOL;

BOOL verificacaoJogada(int linha, int coluna);
void movimentacao(int linha, int coluna);
BOOL movimentacaoValida(int linha, int coluna);
void iniciarTabuleiro();
void embaralharPeca();
void exibirTabuleiro();
void lerJogador(char jogador[]);
void introDoJogo();
void menu();
void exibirRegras();

int main()
{
    introDoJogo();
    menu();
    return 0;
}

BOOL verificacaoJogada(int linha, int coluna)
{
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas)
    {
        if ((linha > 0 && tabuleiro[linha - 1][coluna] == 0) || (linha < linhas - 1 && tabuleiro[linha + 1][coluna] == 0) || (coluna > 0 && tabuleiro[linha][coluna - 1] == 0) || (coluna < colunas - 1 && tabuleiro[linha][coluna + 1] == 0))
        {
            return true;
        }
    }
    printf("Movimentação inválida!\n");
    Sleep(2000);
    return false;
}
void movimentacao(int linha, int coluna)
{
    if (verificacaoJogada(linha, coluna))
    {
        if (linha > 0 && tabuleiro[linha - 1][coluna] == 0)
        {
            tabuleiro[linha - 1][coluna] = tabuleiro[linha][coluna];
            tabuleiro[linha][coluna] = 0;
        }
        else if (linha < linhas - 1 && tabuleiro[linha + 1][coluna] == 0)
        {
            tabuleiro[linha + 1][coluna] = tabuleiro[linha][coluna];
            tabuleiro[linha][coluna] = 0;
        }
        else if (coluna > 0 && tabuleiro[linha][coluna - 1] == 0)
        {
            tabuleiro[linha][coluna - 1] = tabuleiro[linha][coluna];
            tabuleiro[linha][coluna] = 0;
        }
        else if (coluna < colunas - 1 && tabuleiro[linha][coluna + 1] == 0)
        {
            tabuleiro[linha][coluna + 1] = tabuleiro[linha][coluna];
            tabuleiro[linha][coluna] = 0;
        }
    }
}

BOOL movimentacaoValida(int linha, int coluna)
{
    return (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas && tabuleiro[linha][coluna] != 0);
}

void iniciarTabuleiro()
{
    int tamanho = 1, i, j;
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            tabuleiro[i][j] = tamanho++;
        }
    }
    tabuleiro[linhas - 1][colunas - 1] = 0;
}

void embaralharPeca()
{
    srand(time(NULL));
    int i, j, ip, jp, embaralhar;
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            if (i == linhas - 1 && j == colunas - 1)
                continue;
            ip = rand() % linhas;
            jp = rand() % colunas;

            while (ip == linhas - 1 && jp == colunas - 1)
            {
                ip = rand() % linhas;
                jp = rand() % colunas;
            }
            embaralhar = tabuleiro[i][j];
            tabuleiro[i][j] = tabuleiro[ip][jp];
            tabuleiro[ip][jp] = embaralhar;
        }
    }

    embaralhar = tabuleiro[linhas - 1][colunas - 1];
    tabuleiro[linhas - 1][colunas - 1] = 0;
    for (i = 0; i < linhas; i++)
    {
        for (j = 0; j < colunas; j++)
        {
            if (tabuleiro[i][j] == 0 && !(i == linhas - 1 && j == colunas - 1))
            {
                tabuleiro[i][j] = embaralhar;
                return;
            }
        }
    }
}

void exibirTabuleiro()
{
    int i, j;
    system("cls");
    for (i = 0; i < linhas; i++)
    {
        printf("+-----+-----+-----+-----+\n");
        for (j = 0; j < colunas; j++)
        {
            if (tabuleiro[i][j] == 0)
            {
                printf("|     ");
            }
            if (tabuleiro[i][j] >= 1 && tabuleiro[i][j] < 10)
            {
                printf("|  %d  ", tabuleiro[i][j]);
            }
            if (tabuleiro[i][j] >= 10)
            {
                printf("|  %d ", tabuleiro[i][j]);
            }
        }
        printf("|\n");
    }
    printf("+-----+-----+-----+-----+\n");
}

void lerJogador(char jogador[])
{
    printf("\n\n\t\tComo é o seu nome: \n\t\t");
    scanf("%s", jogador);
}

void introDoJogo()
{
    printf("\n\n\t\t JOGO DO 15 \n\n");
    system("pause");
    system("cls");
}

void exibirRegras()
{
    system("cls");
    printf("\nRegras do Jogo dos 15:\n");
    printf("1. O tabuleiro e composto por numeros de 1 a 15 e um espaco vazio.\n");
    printf("2. O objetivo e organizar os numeros em ordem crescente, deixando o espaco vazio no final.\n");
    printf("3. Voce pode mover pecas para o espaco vazio adjacente.\n");
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();
}

void menu()
{
    char opcao;
    char jogador[TAMaNHO_NOME];
    int linha, coluna;
    ;
    do
    {

        printf("\nBem vindo ao Jogo dos 15!\n\n");
        printf("\nA. Jogar");
        printf("\nB. Regras do jogo");
        printf("\nC. Sair\n");
        printf("\nOpcao: ");
        scanf(" %c", &opcao);
        switch (opcao)
        {
        case 'a':
        case 'A':
            system("cls");
            lerJogador(jogador);
            iniciarTabuleiro(tabuleiro);
            embaralharPeca(tabuleiro);

            while (1)
            {
                system("cls");

                exibirTabuleiro(tabuleiro);
                
                printf("Selecione a peça ao lado do espaço vazio (Use 'w' (cima), 's' (baixo), 'a' (esquerda), 'd' (direita)) para fazer os movimentos: \n");

                char movimento = _getch();

                int linhaVazia, colunaVazia;
                for (int i = 0; i < linhas; i++)
                {
                    for (int j = 0; j < colunas; j++)
                    {
                        if (tabuleiro[i][j] == 0)
                        {
                            linhaVazia = i;
                            colunaVazia = j;
                        }
                    }
                }

                switch (movimento)
                {
                case 'w':
                    linha = linhaVazia - 1;
                    coluna = colunaVazia;
                    break;
                case 's':
                    linha = linhaVazia + 1;
                    coluna = colunaVazia;
                    break;
                case 'a':
                    linha = linhaVazia;
                    coluna = colunaVazia - 1;
                    break;
                case 'd':
                    linha = linhaVazia;
                    coluna = colunaVazia + 1;
                    break;
                default:
                    printf("Movimento inválido!\n");
                    Sleep(2000);
                    continue;
                }

                if (movimentacaoValida( linha, coluna))
                {
                    movimentacao(linha, coluna);
                }
                else
                {
                    printf("Movimentação inválida!\n");
                    Sleep(2000);
                }
            }
            break;

        case 'b':
        case 'B':
            exibirRegras();
            system("cls");
            break;

        case 'c':
        case 'C':
            printf("\nObrigado por Jogar!");
            break;

        default:
            printf("\nopcao invalida");
            break;
        }
    } while (opcao != 'c' && opcao != 'C');
}
