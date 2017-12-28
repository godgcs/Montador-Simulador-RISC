#include <stdlib.h>
#include <stdio.h>
#include "simulador.h"

int main(int argc, char** argv){
  if(argc == 2){
    Simulador(argv[1]);
    return(0);
  }else{
    printf("./exe <ArquivoEntrada>\n");
  }
  return(-1);
}
