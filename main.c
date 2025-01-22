//importacoes
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
//definicoes
#define TAM 4
#define TAMANHO_CELULA 150
#define ESPACO_ENTRE_CELULA 7
#define LARGURA_JANELA (TAM * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA))
#define ALTURA_JANELA (TAM * (TAMANHO_CELULA + ESPACO_ENTRE_CELULA))
//variaveis
int tabuleiro[TAM][TAM];
int linhas = TAM, colunas = TAM;
int linhaVazia, colunaVazia;
int dificuldade = 1000;
int veri = 0;
bool exibevitoria = true;
bool rodando = true;
Mix_Chunk *somMovimento = NULL;
//declaracao de funcao
void inicializarTabuleiro(std::vector<std::vector<int>>& tabuleiro);
void embaralharPeca(std::vector<std::vector<int>>& tabuleiro);
void exibirTabuleiro(const std::vector<std::vector<int>>& tabuleiro);
void desenharTabuleiro(SDL_Renderer *renderer, TTF_Font *fonte);
void movimentarEspaco(char direcao, std::vector<std::vector<int>>& tabuleiro);
int verificarVitoria(const std::vector<std::vector<int>>& tabuleiro);
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fonte, const char *texto,
                     SDL_Color cor, int x, int y);
void JogadorVenceu(SDL_Window *window, SDL_Renderer *renderer);
void vitoria();
void exibirRegras(SDL_Window *window, SDL_Renderer *renderer);
void Escolha_dificuldade(SDL_Window *window, SDL_Renderer *renderer);
void jogar(SDL_Window *window, SDL_Renderer *renderer);
void menu();



int main(int argc, char *argv[]) {
  menu();
  return 0;
}

// Inicializa o tabuleiro com números de 1 a 15 e um espaço vazio
// Inicializar o tabuleiro com números de 1 a 15 e espaço vazio
void inicializarTabuleiro(int (&tabuleiro)[4][4]) {
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
// Embaralha as peças do tabuleiro
void embaralharPeca(int (&tabuleiro)[4][4]) {
  srand(time(NULL));
  // Variável para contar o número de inversões
  int inversoes;
  // Embaralhamento até que o número de inversões seja par
  do {
    inversoes = 0;
    // Realiza o embaralhamento de peças
    for (int i = 0; i < dificuldade; i++) {
      // Escolhe duas posições aleatórias
      int x1 = rand() % TAM;
      int y1 = rand() % TAM;
      int x2 = rand() % TAM;
      int y2 = rand() % TAM;
      // Troca os valores das duas posições
      int temp = tabuleiro[x1][y1];
      tabuleiro[x1][y1] = tabuleiro[x2][y2];
      tabuleiro[x2][y2] = temp;
      // Se alguma das posições trocadas for a posição vazia (0), atualiza a
      // posição da linha e coluna vazia
      if (tabuleiro[x1][y1] == 0) {
        linhaVazia = x1;
        colunaVazia = y1;
      } else if (tabuleiro[x2][y2] == 0) {
        linhaVazia = x2;
        colunaVazia = y2;
      }
    }
    // Conta o número de inversões no tabuleiro
    int arr[TAM * TAM]; // Cria um array para armazenar as peças do tabuleiro
    int idx = 0;
    // Transforma o tabuleiro em um array unidimensional
    for (int i = 0; i < TAM; i++) {
      for (int j = 0; j < TAM; j++) {
        arr[idx] = tabuleiro[i][j];
        idx++;
      }
    }
    // Conta o número de inversões no array
    for (int i = 0; i < TAM * TAM - 1; i++) {
      for (int j = i + 1; j < TAM * TAM; j++) {
        // Se arr[i] e arr[j] estão fora de ordem e não são o número 0, conta
        // como uma inversão
        if (arr[i] != 0 && arr[j] != 0 && arr[i] > arr[j]) {
          inversoes++;
        }
      }
    }
    // Continua embaralhando até o número de inversões ser par
  } while (inversoes % 2 != 0);
}
// Exibe o tabuleiro no console
void exibirTabuleiro(const std::vector<std::vector<int>>& tabuleiro) {
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
// Renderiza o tabuleiro na interface gráfica
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
        //DAVID MENEZES 11/01/2025 13:17
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
// Movimenta o espaço vazio
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
    // reproduz som ao movimentar
    Mix_PlayChannel(-1, somMovimento, 0);
    // aqui
    int verificarVitoria(int tabuleiro[4][4]);
  } else {
    printf("Movimento inválido!\n");
  }
}

// Verifica se o jogador venceu
int verificarVitoria(int tabuleiro[TAM][TAM]) {
  int count = 1, verif = 0;
  for (int i = 0; i < linhas; i++) {
    for (int j = 0; j < colunas; j++) {
      if (tabuleiro[i][j] == count) {
        verif = verif + 1;
      }
      count = count + 1;
    }
  }
if (verif == 15) {
  while(veri == 0){
    
    vitoria();
    exibevitoria = false;// chama a tela de vitoria
    veri = 1;
  } 
    //rodando = false;
    menu(); // chama o menu somente se veri já foi alterado
    return 0;
}
return 0;

}
// Renderiza texto na tela
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fonte, const char *texto,
                     SDL_Color cor, int x, int y) {
  SDL_Surface *surface = TTF_RenderText_Solid(fonte, texto, cor);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect destino = {x, y, surface->w, surface->h};
  SDL_RenderCopy(renderer, texture, NULL, &destino);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}
// Exibe a mensagem de vitória
// tela exibida quando o jogador vence
void JogadorVenceu(SDL_Window *window, SDL_Renderer *renderer) {

  Mix_Music *musica = Mix_LoadMUS("arquivos/musicaVVitoria_convertido.mp3");
  Mix_PlayMusic(musica, -1);

  TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 40);

  SDL_Color corTexto = {1, 1, 1, 255};
  SDL_Event evento;
  Uint32 tempoInicio = SDL_GetTicks();
  while (exibevitoria) {
    // Limpar a tela
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    SDL_RenderClear(renderer);
    // Renderizar texto
    renderizarTexto(renderer, fonte, "Parabens voce Venceu", corTexto, 135, 250);
    // Apresentar na tela
    SDL_RenderPresent(renderer);
    Uint32 tempoAtual = SDL_GetTicks();
    if (tempoAtual - tempoInicio >= 7000) {
      //7000 ms = 7 segundos
      exibevitoria = false;
      veri = 0;
      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);
      break;
      
  // Fechar a janela após 7 segundos
    }
    // Ciclo de eventos para sair
    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        Mix_FreeMusic(musica);
        exibevitoria = false;
        break;

        
      }
    }

  }
  // Liberar recursos
  TTF_CloseFont(fonte);
  Mix_FreeMusic(musica);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
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
  SDL_Window *janela = SDL_CreateWindow("O jogo dos 15", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, LARGURA_JANELA,
                                        ALTURA_JANELA, SDL_WINDOW_SHOWN);
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
  TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 30);
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
  veri =  0;

  SDL_DestroyWindow(janela);
  SDL_DestroyRenderer(renderer);
  TTF_CloseFont(fonte);
  TTF_Quit();
  SDL_Quit();
}

// Exibir regras do jogo
void exibirRegras(SDL_Window *window, SDL_Renderer *renderer) {
  TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 16);

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
        "1. O tabuleiro e composto por numeros de 1 a 15 e um espaco vazio.",
        corTexto, 20, 80);
    renderizarTexto(renderer, fonte,
                    "2. O objetivo e organizar os numeros em ordem crescente,",
                    corTexto, 20, 120);
    renderizarTexto(renderer, fonte, " deixando o espaco vazio no final.",
                    corTexto, 20, 160);
    renderizarTexto(
        renderer, fonte,
        "3. Voce pode mover pecas para o espaco vazio adjacente usando:",
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

void Escolha_dificuldade(SDL_Window *window, SDL_Renderer *renderer) {
  TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 24);
  if (!fonte) {
    printf("Erro ao carregar fonte: %s\n", TTF_GetError());
    return;
  }
  bool escolhendo = true;
  SDL_Event evento;

  while (escolhendo) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Color corTexto = {0, 0, 0, 255};
    renderizarTexto(renderer, fonte, "Escolha a dificuldade:", corTexto, 80,
                    30);
    renderizarTexto(renderer, fonte, "1. Facil", corTexto, 160, 100);
    renderizarTexto(renderer, fonte, "2. Medio", corTexto, 160, 140);
    renderizarTexto(renderer, fonte, "3. Dificil", corTexto, 160, 180);
    renderizarTexto(renderer, fonte, "Pressione ESC para voltar", corTexto, 80,
                    240);

    SDL_RenderPresent(renderer);

    while (SDL_PollEvent(&evento)) {
      if (evento.type == SDL_QUIT) {
        escolhendo = false;
      } else if (evento.type == SDL_KEYDOWN) {
        switch (evento.key.keysym.sym) {
        case SDLK_1:
          dificuldade = 0; // Fácil
          jogar(window, renderer);
          escolhendo = false;
          break;
        case SDLK_2:
          dificuldade = 1000; // Médio
          jogar(window, renderer);
          escolhendo = false;
          break;
        case SDLK_3:
          dificuldade = 2000; // Difícil
          jogar(window, renderer);
          escolhendo = false;
          break;
        case SDLK_ESCAPE:
          escolhendo = false;

          break;
        }
      }
    }
  }

  TTF_CloseFont(fonte);
}
// Jogar o jogo sao passados como parametro a janela menu
void jogar(SDL_Window *window, SDL_Renderer *renderer) {
  TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 24);
  if (!fonte) {
    printf("Erro ao carregar fonte: %s\n", TTF_GetError());
    return;
  }

  inicializarTabuleiro(tabuleiro);
  embaralharPeca(tabuleiro);

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
  SDL_Event event;
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
    TTF_Font *fonte = TTF_OpenFont("arquivos/arial.ttf", 30);
    if (!fonte) {
      printf("Erro ao carregar fonte: %s\n", TTF_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(janela);
      TTF_Quit();
      SDL_Quit();
      return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erro ao inicializar SDL_mixer: %s\n", Mix_GetError());
        // return;
      }
      somMovimento = Mix_LoadWAV("arquivos/cliick.mp3");
      if (somMovimento == NULL) {
        printf("Erro ao carregar som: %s\n", Mix_GetError());
        // return;
      }
    SDL_Event evento;
    while (rodando) {
      exibevitoria = true;
      veri = 0;
        //inicia audio
      

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
            Escolha_dificuldade(janela, renderer);
            break;
          case SDLK_b:
            exibirRegras(janela, renderer);
            break;
          case SDLK_c:
           // return;
           exibevitoria = false;
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
