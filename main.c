#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
// iNCLUINDO AS BIBLIOTECAS DA INTERFACE
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#define TAM 4
// DEFININDO OS ESPACOS NA TELA
#define TAMANHO_CELULA 150
#define ESPACO_ENTRE_CELULA 7
#define LARGURA_JANELA (TAM * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA))
#define ALTURA_JANELA (TAM * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA))

int tabuleiro[TAM][TAM];
int linhas = TAM, colunas = TAM;
int linhaVazia, colunaVazia; // Para rastrear a posição do espaço vazio

void menu();
void JogadorVenceu(SDL_Window *window, SDL_Renderer *renderer);
void vitoria();

// Inicializar o tabuleiro com números de 1 a 15 e espaço vazio
void inicializarTabuleiro() {
  int valor = 1;
  for (int i = 0; i < TAM; i++) {
    for (int j = 0; j < TAM; j++) {
      if (valor < TAM * TAM) {
        tabuleiro[i][j] = valor++;
      } else {
        tabuleiro[i][j] = 0; // Espaço vazio
        linhaVazia = i;      // Salvar posição inicial do espaço vazio
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

int verificarVitoria(int tabuleiro[linhas][colunas]) {
  int count = 1, verif = 0;
  for (int i = 0; i < linhas; i++) {
    for (int j = 0; j < colunas; j++) {
      if (tabuleiro[i][j] == count) {
        verif = verif + 1;
      }
      count = count + 1;
    }
  }
  int veri = 0;
  if (verif == 15) {
      if(veri == 0){
          vitoria();
          veri = 1;
      }

    menu();
  }
  return 0;
}

// Embaralhar o tabuleiro
void embaralharPeca() {
  // Define o número de movimentos para embaralhar
  int movimentos = 5; // Altere este número para mais ou menos dificuldade

  srand(time(NULL));

  // Faz um número limitado de movimentos válidos
  for (int i = 0; i < movimentos; i++) {
    int novaLinha = linhaVazia;
    int novaColuna = colunaVazia;

    // Escolhe uma direção aleatória válida para mover a peça
    int direcao = rand() % 4;
    switch (direcao) {
    case 0: // Cima
      novaLinha = linhaVazia - 1;
      break;
    case 1: // Baixo
      novaLinha = linhaVazia + 1;
      break;
    case 2: // Esquerda
      novaColuna = colunaVazia - 1;
      break;
    case 3: // Direita
      novaColuna = colunaVazia + 1;
      break;
    }

    // Verifica se o movimento é válido
    if (novaLinha >= 0 && novaLinha < TAM && novaColuna >= 0 &&
        novaColuna < TAM) {
      // Troca a peça com o espaço vazio
      tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
      tabuleiro[novaLinha][novaColuna] = 0;

      // Atualiza a posição do espaço vazio
      linhaVazia = novaLinha;
      colunaVazia = novaColuna;
    }
  }
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
  if (novaLinha >= 0 && novaLinha < TAM && novaColuna >= 0 &&
      novaColuna < TAM) {
    tabuleiro[linhaVazia][colunaVazia] = tabuleiro[novaLinha][novaColuna];
    tabuleiro[novaLinha][novaColuna] = 0;
    linhaVazia = novaLinha;
    colunaVazia = novaColuna;

    // aqui
    verificarVitoria(tabuleiro);
  } else {
    printf("Movimento inválido!\n");
  }
}

void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fonte, const char *texto,
                     SDL_Color cor, int x, int y) {
  SDL_Surface *surface = TTF_RenderText_Solid(fonte, texto, cor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect destino = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &destino);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void desenharTabuleiro(SDL_Renderer *renderer, TTF_Font *fonte) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Fundo branco
  SDL_RenderClear(renderer);

  SDL_Color corTexto = {255, 255, 255, 255}; // Cor do texto: branco

  for (int i = 0; i < TAM; i++) {
    for (int j = 0; j < TAM; j++) {
      SDL_Rect rect = {j * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA),
                       i * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA),
                       TAMANHO_CELULA, TAMANHO_CELULA};

      if (tabuleiro[i][j] != 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Azul
        SDL_RenderFillRect(renderer, &rect);

        // Renderizar número no centro da célula
        char numero[3];
        sprintf(numero, "%d", tabuleiro[i][j]);

        int textoX =
            rect.x + (TAMANHO_CELULA / 2) - 10; // Centraliza aproximadamente
        int textoY = rect.y + (TAMANHO_CELULA / 2) - 10;

        renderizarTexto(renderer, fonte, numero, corTexto, textoX, textoY);
      } else {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200,
                               255); // Cinza para o espaço vazio
        SDL_RenderFillRect(renderer, &rect);
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto para bordas
      SDL_RenderDrawRect(renderer, &rect);
    }
  }

  SDL_RenderPresent(renderer);
}
// Exibir regras do jogo
void exibirRegras(SDL_Window *window, SDL_Renderer *renderer) {
  TTF_Font *fonte = TTF_OpenFont("arial.ttf", 16);

  SDL_Color corTexto = {255, 255, 255, 255};
  bool exibindo = true;
  SDL_Event evento;

  while (exibindo) {
    // Limpar a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto
    SDL_RenderClear(renderer);

    // Renderizar texto
    renderizarTexto(renderer, fonte, "Regras do Jogo:", corTexto, 80, 30);
    renderizarTexto(
        renderer, fonte,
        "1. O tabuleiro e composto por números de 1 a 15 e um espaço vazio.",
        corTexto, 20, 80);
    renderizarTexto(renderer, fonte,
                    "2. O objetivo e organizar os números em ordem crescente,",
                    corTexto, 20, 120);
    renderizarTexto(renderer, fonte, " deixando o espaço vazio no final.",
                    corTexto, 20, 160);
    renderizarTexto(
        renderer, fonte,
        "3. Voce pode mover peças para o espaco vazio adjacente usando:",
        corTexto, 20, 200);
    renderizarTexto(renderer, fonte,
                    " W cima, A esquerda, S baixo e D direita.", corTexto, 20,
                    240);
    renderizarTexto(renderer, fonte, "Pressione ESC para voltar ao menu.",
                    corTexto, 80, 300);

    // Apresentar na tela
    SDL_RenderPresent(renderer);

    // Ciclo de eventos para sair
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        exibindo = false;
        break;
      } else if (evento.type == SDL_KEYDOWN &&
                 evento.key.keysym.sym == SDLK_ESCAPE) {
        exibindo = false;
        break;
      }
    }
  }

  // Liberar recursos
  TTF_CloseFont(fonte);
}

void vitoria() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  // para fechar janela
  int run = 1;
  SDL_Event event;
    if (event.type == SDL_QUIT) {
      run = 0; // Sai do loop ao clicar no "X"
    }
    SDL_Window *janela = SDL_CreateWindow(
        "O jogo dos 15", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGURA_JANELA, ALTURA_JANELA, SDL_WINDOW_SHOWN);
    if (!janela) {
      printf("Erro ao criar janela: %s\n", SDL_GetError());
      TTF_Quit();
      SDL_Quit();
      // return;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
      printf("Erro ao criar renderizador: %s\n", SDL_GetError());
      SDL_DestroyWindow(janela);
      TTF_Quit();
      SDL_Quit();
      // return;
    }

    TTF_Font *fonte = TTF_OpenFont("arial.ttf", 30);
    if (!fonte) {
      printf("Erro ao carregar fonte: %s\n", TTF_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(janela);
      TTF_Quit();
      SDL_Quit();
      // return;
    }
    // Exibir a tela de vitória
    JogadorVenceu(janela, renderer);

    // Esperar 7 segundos antes de fechar a janela de vitória
    SDL_Delay(3000);

    // Liberar recursos e fechar janela
    TTF_CloseFont(fonte);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);
    TTF_Quit();
    SDL_Quit();


}
// tela exibida quando o jogador vence
void JogadorVenceu(SDL_Window *window, SDL_Renderer *renderer) {

  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
    return;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
    return;
  }
  Mix_Music *musica = Mix_LoadMUS("musicaVVitoria_convertido.mp3");
  if (musica == NULL) {
    printf("Erro ao carregar música: %s\n", Mix_GetError());
    return;
  }
  Mix_PlayMusic(musica, 1);

  TTF_Font *fonte = TTF_OpenFont("arial.ttf", 16);

  SDL_Color corTexto = {255, 255, 255, 255};
  bool exibindo = true;
  SDL_Event evento;
  Uint32 tempoInicio = SDL_GetTicks();
  while (exibindo) {
    // Limpar a tela
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto
    SDL_RenderClear(renderer);

    // Renderizar texto
    renderizarTexto(renderer, fonte, "Parabens voce Venceu", corTexto, 80, 30);

    // Apresentar na tela
    SDL_RenderPresent(renderer);

    Uint32 tempoAtual = SDL_GetTicks();
    if (tempoAtual - tempoInicio >= 7000) { // 7000 ms = 7 segundos
      exibindo = false;                     // Fechar a janela após 7 segundos
    }

    // Ciclo de eventos para sair
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        exibindo = false;
        break;
      }
    }
  }

  // Liberar recursos
  TTF_CloseFont(fonte);
  Mix_FreeMusic(musica);

}

// Jogar o jogo sao passados como parametro a janela menu
void jogar(SDL_Window *window, SDL_Renderer *renderer) {
  TTF_Font *fonte = TTF_OpenFont("arial.ttf", 24);
  if (!fonte) {
    printf("Erro ao carregar fonte: %s\n", TTF_GetError());
    return;
  }

  inicializarTabuleiro();
  embaralharPeca();

  bool rodando = true;
  SDL_Event evento;

  while (rodando) {
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        rodando = false;
      } else if (evento.type == SDL_KEYDOWN) {
        char direcao = '\0';

        switch (evento.key.keysym.sym) {
        case SDLK_w:
          direcao = 'w';
          break;
        case SDLK_a:
          direcao = 'a';
          break;
        case SDLK_s:
          direcao = 's';
          break;
        case SDLK_d:
          direcao = 'd';
          break;
        }

        if (direcao) {
          movimentarEspaco(direcao);
          verificarVitoria(tabuleiro);
        }
      }
    }

    desenharTabuleiro(renderer, fonte);
  }

  TTF_CloseFont(fonte);
}

// Exclusao do antigo menu e inclusao do novo com interface

void menu() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  // para fechar janela
  int run = 1;
  SDL_Event event;
  while (run) {
    // Processa eventos
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        run = 0; // Sai do loop ao clicar no "X"
      }
    }

    SDL_Window *janela = SDL_CreateWindow(
        "O jogo dos 15", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGURA_JANELA, ALTURA_JANELA, SDL_WINDOW_SHOWN);
    if (!janela) {
      printf("Erro ao criar janela: %s\n", SDL_GetError());
      TTF_Quit();
      SDL_Quit();
      return;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
      printf("Erro ao criar renderizador: %s\n", SDL_GetError());
      SDL_DestroyWindow(janela);
      TTF_Quit();
      SDL_Quit();
      return;
    }

    TTF_Font *fonte = TTF_OpenFont("arial.ttf", 30);
    if (!fonte) {
      printf("Erro ao carregar fonte: %s\n", TTF_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(janela);
      TTF_Quit();
      SDL_Quit();
      return;
    }

    bool rodando = true;
    SDL_Event evento;

    while (rodando) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);

      SDL_Color corTexto = {0, 0, 0, 255};
      renderizarTexto(renderer, fonte, "Bem-vindo ao Jogo dos 15!", corTexto,
                      80, 30);
      renderizarTexto(renderer, fonte, "A. Jogar", corTexto, 160, 100);
      renderizarTexto(renderer, fonte, "B. Regras do jogo", corTexto, 160, 140);
      renderizarTexto(renderer, fonte, "C. Sair", corTexto, 160, 180);

      SDL_RenderPresent(renderer);

      while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
          rodando = false;
        } else if (evento.type == SDL_KEYDOWN) {
          switch (evento.key.keysym.sym) {
          case SDLK_a:
            jogar(janela, renderer);
            break;
          case SDLK_b:
            exibirRegras(janela, renderer);
            break;
          case SDLK_c:
            rodando = false;
            break;
          }
        }
      }
    }
    TTF_CloseFont(fonte);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(janela);
    TTF_Quit();
    SDL_Quit();
  }
}

int main() {
  menu();
  return 0;
}
