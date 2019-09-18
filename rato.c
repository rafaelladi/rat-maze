#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct r {
  int x, y;
} ra;

typedef struct p {
  int x, y;
  struct p *prox;
} nodo;

nodo *pilha;
int saidax, saiday;
ra rato;

int lab[30][30];

#define LIVRE 1
#define PAREDE 0
#define VISITADA 2
#define BECO 3

void clrscr() { //LIMPA A TELA
  printf("\33[H\33[2J");
}

void ini_pilha() { //INICIALIZA A PILHA
  pilha = NULL;
}

nodo * cria_nodo(int x, int y) { //CRIA NOVO NODO DA PILHA
  nodo *novo;
  novo = (nodo *) malloc(sizeof(nodo));
  novo->x = x;
  novo->y = y;
  novo->prox = NULL;
}

void wait(float a) { //"TIMER"
  float b=0;
  while(b <= a) {
    b+=0.0001;
  }
}

void push(int x, int y) { //ADICIONA UMA NOVA POSICAO(X, Y) NA PILHA
  nodo *aux = cria_nodo(x, y);
  lab[x][y] = VISITADA;
  if(pilha == NULL) { //PILHA VAZIA
    aux->prox = NULL;
    pilha = aux;
  } else {
    aux->prox = pilha;
    pilha = aux;
  }
}

int pop() { //TIRA A ULTIMA POSICAO DA PILHA E VOLTA UMA POSICAO COM O RATO
  nodo *aux;
  if(pilha == NULL) { //PILHA VAZIA
    return 0;
  } else {
    aux = pilha;
    if(aux->prox == NULL) { //PILHA COM 1 ELEMENTO
      return 0;
    } else {
      pilha = aux->prox;
      rato.x = pilha->x;
      rato.y = pilha->y;
      free(aux);
      return 1;
    }
  }
}

void abre_saida() { //ESCOLHE UMA POSICAO DO LABIRINTO PARA SAIDA
  int x, y;
  x = rand() % 30;
  if(x == 0 || x == 29) {
    y = rand() % 30;
  } else {
    y = rand() % 2;
    if(y > 0)
      y = 29;
  }
  saidax = x;
  saiday = y;
  lab[x][y] = LIVRE;
}

int cria_lab() { //CRIA LABIRINTO ALEATORIAMENTE
  int a, b, cx, cy;
  srand(time(NULL));
  for(a=0;a<30;a++) { //CRIA AS PAREDES EM VOLTA DO LABIRINTO
    for(b=0;b<30;b++) {
      if(a == 0 || b == 0 || b == 29 || a == 29)
        lab[a][b] = PAREDE;
      else
        lab[a][b] = LIVRE;
    }
  }
  for(a=1;a<29;a++) { //CRIA A PARTE DE DENTRO DO LABIRINTO
    for(b=1;b<29;b++) {
      lab[a][b] = rand() % 4;
      if(lab[a][b] > 1)
        lab[a][b] = LIVRE;
    }
  }
  abre_saida(); //ABRE A SAIDA
}

void imprime_lab() { //IMPRIME O LABIRINTO DE ACORDO COM O VALOR DO ELEMENTO DA MATRIZ
  int a, b;
  for(a=0;a<30;a++) {
    for(b=0;b<30;b++) {
      if(lab[a][b] == PAREDE)
        printf("█");
      else if(a == rato.x && b == rato.y)
        printf("☻");//☻
      else if(lab[a][b] == VISITADA)
        printf(".");
      else if(lab[a][b] == BECO)
        printf("X");
      else if(lab[a][b] == LIVRE)
        printf(" ");
    }
    printf("\n");
  }
}

int ver_beco() { //VERIFICA SE RATO ESTA EM UM BECO
  int np=0;
  if(lab[rato.x-1][rato.y] == PAREDE)
    np++;
  if(lab[rato.x][rato.y+1] == PAREDE)
    np++;
  if(lab[rato.x][rato.y-1] == PAREDE)
    np++;
  if(lab[rato.x+1][rato.y] == PAREDE)
    np++;
  if(np > 2) {
    return 1;
  } else {
    return 0;
  }
}

void ini_rato() { //ESCOLHE UMA POSICAO ALEATORIA PARA O RATO
  int x, y, a=1;
  while(a) {
    x = rand() % 30;
    y = rand() % 30;
    if(lab[x][y] == LIVRE) {
      rato.x=x;
      rato.y=y;
      lab[rato.x][rato.y] = VISITADA;
      a=0;
    }
  }
  push(rato.x, rato.y);
}

int cbde() { //TENTA ANDAR PARA CIMA, DIREITA, ESQUERDA OU BAIXO COM O RATO (RETORNA 1 CASO CONSIGA)
  if(lab[rato.x-1][rato.y] == LIVRE) {
    printf("CIMA\n");
    rato.x--;
    push(rato.x, rato.y);
    return 1;
  } else if(lab[rato.x][rato.y+1] == LIVRE) {
    printf("DIREITA\n");
    rato.y++;
    push(rato.x, rato.y);
    return 1;
  } else if(lab[rato.x][rato.y-1] == LIVRE) {
    printf("ESQUERDA\n");
    rato.y--;
    push(rato.x, rato.y);
    return 1;
  }  else if(lab[rato.x+1][rato.y] == LIVRE) {
    printf("BAIXO\n");
    rato.x++;
    push(rato.x, rato.y);
    return 1;
  } else {
    return 0;
  }
}

int fechado() {
  if(lab[rato.x+1][rato.y] == PAREDE && lab[rato.x-1][rato.y] == PAREDE && lab[rato.x][rato.y+1] == PAREDE && lab[rato.x][rato.y-1] == PAREDE)
    return 1;
  else
    return 0;
}

int andar() { //ANDA COM O RATO
  if(rato.x == saidax && rato.y == saiday) { //VERIFICA SAIDA DO RATO
    return 0;
  } else if(ver_beco()) { //VERIFICA BECO
    printf("BECO\n");
    lab[rato.x][rato.y] = BECO;
    if(!pop()) { //TENTA VOLTAR UMA POSICAO COM O RATO, CASO NAO CONSIGA(COMO NO CASO DE COMECAR PERTO DE UM BECO)
      if(!cbde()) { //TENTA CIMA, BAIXO, ESQUERDA, DIREITA
        return -1;
      }
    }
    return 1;
  } else if(cbde()) { //TENTA CIMA, BAIXO, ESQUERDA, DIREITA
    return 1;
  } else { //SE NAO CONSEGUIR NENHUMA DAS ANTERIORES VOLTA UMA POSICAO
    printf("VOLTAR");
    if(!pop()) { //SE NAO CONSEGUIR VOLTAR MAIS (SEM SOLUCAO)
      return -1;
    } else if(fechado()){
      return -1;
    } else {
        return 1;
    }
  }
}

void main() {
  int a, op=1;

  clrscr();
  while(op) {
    a=op=1;
    ini_pilha();
    cria_lab();
    ini_rato();
    clrscr();
    imprime_lab();
    system("read -p \"Pressione enter para continuar\" saindo");
    while(a==1) {
      a = andar();
      clrscr();
      imprime_lab();
      wait(360.0);
    }
    if(a == 0) {
      printf("Rato fugiu\n");
    } else if(a == -1) {
      printf("Sem solucao\n");
    }
    printf("Deseja jogar novamente? 1 - Sim 0 - Nao\n");
    scanf("%d", &op);
    while(op<0 || op>1) {
      printf("Opcao invalida, tente novamente\n");
      scanf("%d", &op);
    }
  }
}
