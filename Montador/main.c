#include <stdlib.h>
#include <stdio.h>
#include "montador.h"

int main(int argc, char** argv){
  if(argc == 3){
    Montador(argv[1], argv[2]);
    return(0);
  }
  printf("./exe <ArquivoEntrada> <ArquivoSaida>\n");
  return(-1);
}
