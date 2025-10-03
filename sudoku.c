#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    if(!n) return 0; 

    for(int k=0; k<9;k++){
      int vistos[10] = {0}; 
      for(int i=0; i<9;i++){
        int v = n->sudo[k][i];
        if(v == 0) continue; // si está vacio continua. 
        if(v < 0 || v > 9) return 0; // fuera del rango.
        if(vistos[v]) return 0;  // repetido en la fila. 
        vistos[v] = 1; 
      }
    }

    for(int i = 0; i<9; i++){
      int vistos[10] = {0};
      for(int k=0; k<9; k++){
        int v = n->sudo[k][i];
        if(v == 0) continue; 
        if(v < 0 || v > 9) return 0; 
        if(vistos[v]) return 0; // repetido en la columna. 
        vistos[v] = 1; 
      }
    }

    for(int k = 0; k<9; k++){
      int vistos[10] = {0}; 
      for(int p = 0; p<9; p++){
        int i = 3*(k/3) + (p/3);
        int j = 3*(k%3) + (p%3);

        int v = n->sudo[i][j]; 

        if(v == 0) continue;
        if(v<0 || v>9) return 0; 
        if(vistos[v]) return 0; 
        
        vistos[v] = 1; 
      }
    }
    return 1;
}


List* get_adj_nodes(Node* n){
    List* list=createList();
    if(!n) return list; 

    int fil = -1, col = -1; 
    
    // Encontrar casilla vacía. 
    for(int k=0; k<9 && fil == -1; k++){
      for(int i=0; i<9; i++){
        if(n->sudo[k][i] == 0){
          fil = k; 
          col = i; 
          break; 
        }
      }
    }
    if(fil == -1) return list; 

    for(int valor = 1; valor <= 9; valor++){
      Node* hijo = copy(n); 
      hijo->sudo[fil][col] = valor; 
      if(is_valid(hijo)){
        pushBack(list, hijo); 
      }
      else{
        free(hijo);
      }
    }
    return list;
}


int is_final(Node* n){
    if(!n) return 0; 

    for(int k=0; k<9; k++){
      for(int i=0; i<9; i++){
        if(n->sudo[k][i]==0) return 0; 
      }
    }
    return 1;
}

Node* DFS(Node* initial, int* cont){
  if(!initial) return NULL; 
  if(cont) *cont =0; 

  Stack* S = createStack();
  push(S, initial); 

  while(1){
    Node* actual = (Node*)top(S); 
    if(!actual) break; 
    pop(S);

    if(cont) (*cont)++;

    if(is_final(actual)){
      return actual; 
    }

    List* hijos = get_adj_nodes(actual); 

    for(Node* h = first(hijos); h != NULL; h = next(hijos)){
      if(is_valid(h)){
        push(S, h);
      }
      else{
        free(h);
      }
    }

    free(actual); 
  }

  return NULL;
}



/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/