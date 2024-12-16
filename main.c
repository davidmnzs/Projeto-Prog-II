#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 4

int tabuleiro[TAM][TAM];
int vazioX, vazioY;

void exibirTabuleiro(){
    printf("\nTabuleiro:\n\n");
    for(int i=0; i<TAM; i++){
        printf("+----+----+----+----+\n");
        for(int j=0; j<TAM; j++){
            if(tabuleiro[i][j] == 0){
                printf("|  ");
            }
            else{
                printf("| %2d ", tabuleiro[i][j]);
            }
        }
        printf("|\n");
    }
    printf("+----+----+----+----+\n");
}





void exibirRegras(){

    printf("\nRegras do Jogo dos 15:\n");
    printf("1. O tabuleiro e composto por numeros de 1 a 15 e um espaco vazio.\n");
    printf("2. O objetivo e organizar os numeros em ordem crescente, deixando o espaco vazio no final.\n");
    printf("3. Voce pode mover pecas para o espaco vazio adjacente.\n");
    printf("\nPressione ENTER para voltar ao menu...");
    getchar();
    getchar();

}

void menu(){
    char opcao;

    do{
        
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
            while (1){
                system("cls");
                exibirTabuleiro();
                char movimento;
                printf("\nUse 'w' (cima), 's' (baixo), 'a' (esquerda), 'd' (direita) para fazer os movimentos: ");
                scanf(" %c", &movimento);
                moverEspaco(movimento);
            }
            break;
    
        case 'b':
        case 'B':
            exibirRegras();
            break;
    
        case 'c':
        case 'C':
            printf("\nObrigado por Jogar!");
            break;
    
        default:
            printf("\nopcao invalida");
            break;
        }
    }while(opcao != 'c' && opcao != 'C');

}

int main(){

    srand(time(NULL));

   // embaralharTabuleiro();

    menu();

    return 0;
}