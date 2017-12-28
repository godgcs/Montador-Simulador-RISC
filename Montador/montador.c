#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ADDRESS 170
#define TamMemoria 65536
#define TamInstrucoes 100

typedef struct labels{
  char Nome[TamInstrucoes];
  int posiMemoria;
}Labels;

/*Cabeçalho de Funções privadas*/
/*Funções de codificação de binarios*/
void IndenficaOpcode(char Instrucao[], char opcode[], int* FimInstrucao);
void ConverteInstrucao(char Instrucao[], char InstrucaoBinario[]);
void itob(int Num, int Bits, char Binario[]);
int  ConverteOpcodeInteiro(char opcode[]);
/*Funções de labels*/
int  VerificaLabel(char Instrucao[]);
int  LabelBuscaEndereco(char Label[]);
void LabelInsere(char Label[]);

/*Variaveis Globais do montador*/
char Memoria[TamMemoria][TamInstrucoes];
Labels VetLabels[TamMemoria];
int ContMemoria=0, ContLabel=0;

/*Implementaões de Funções*/
void itob(int Num, int Bits, char Binario[]){
  int i, Deslocamento;
  /*O algorimo foi encontrado no seguinte endereço:
    http://www.programasprontos.com/algoritmos-conversores/conversao-decimal-para-binario-c/
    e foi adaptado para ser utilizado genericamente para quantos bits forem passados por parametro
  */
  Bits--;
  /*Inicia String com tamanho 0*/
  Binario[0]='\0';
  /*Utiliza um número de 8 bits como base para a conversão.*/
  for(i=Bits; i >= 0; i--){
    /*Executa a operação shift right até a
      última posição da direita para cada bit.
    */
    Deslocamento = Num >> i;
    /*Por meio do "e" lógico ele compara se o valor
      na posição mais à direita é 1 ou 0
      e imprime na tela até reproduzir o número binário.
    */
    if(Deslocamento & 1){
      strcat(Binario,"1");
    }else{
      strcat(Binario,"0");
    }
  }
  /*Finaliza a String*/
  Binario[i]='\0';
}

void IndenficaOpcode(char Instrucao[], char opcode[], int* FimInstrucao){
  int i;
  /*Copio todos os caracteres do opcode até que ele termine*/
  for(i=0; isalpha(Instrucao[i]); i++){
    opcode[i]=Instrucao[i];
  }
  /*Finaliza a copia do opcode na ultima posição*/
  opcode[i]='\0';
  /*Armazena onde termina o opcode em uma variavel*/
  *FimInstrucao = i+1;
}

int ConverteOpcodeInteiro(char opcode[]){
  int NumOpCode;
  /* Verifica qual o opcode lido e retorna um numero referente a ele*/
  if(!strcmp(opcode, "nop")){
    // printf("NOP 0\n");
    NumOpCode = 0;
  }
  if(!strcmp(opcode, "add")){
    // printf("add 1\n");
    NumOpCode = 1;
  }
  if(!strcmp(opcode, "sub")){
    // printf("sub 2\n");
    NumOpCode = 2;
  }
  if(!strcmp(opcode, "zeros")){
    // printf("zeros 3\n");
    NumOpCode = 3;
  }
  if(!strcmp(opcode, "xor")){
    // printf("xor 4\n");
    NumOpCode = 4;
  }
  if(!strcmp(opcode, "or")){
    // printf("or 5\n");
    NumOpCode = 5;
  }
  if(!strcmp(opcode, "not")){
    // printf("sub 6\n");
    NumOpCode = 6;
  }
  if(!strcmp(opcode, "and")){
    // printf("and 7\n");
    NumOpCode = 7;
  }
  if(!strcmp(opcode, "asl")){
    // printf("asl 8\n");
    NumOpCode = 8;
  }
  if(!strcmp(opcode, "asr")){
    // printf("sub 9\n");
    NumOpCode = 9;
  }
  if(!strcmp(opcode, "lsl")){
    // printf("lsl 10\n");
    NumOpCode = 10;
  }
  if(!strcmp(opcode, "lsr")){
    // printf("lsr 11\n");
    NumOpCode = 11;
  }
  if(!strcmp(opcode, "passa")){
    // printf("passa 12\n");
    NumOpCode = 12;
  }
  if(!strcmp(opcode, "lch")){
    // printf("lch 14\n");
    NumOpCode = 14;
  }
  if(!strcmp(opcode, "lcl")){
    // printf("lcl 15\n");
    NumOpCode = 15;
  }
  if(!strcmp(opcode, "load")){
    // printf("load 16\n");
    NumOpCode = 16;
  }
  if(!strcmp(opcode, "store")){
    // printf("store 17\n");
    NumOpCode = 17;
  }
  if(!strcmp(opcode, "mult")){
    // printf("mult 18\n");
    NumOpCode = 18;
  }
  if(!strcmp(opcode, "div")){
    // printf("div 19\n");
    NumOpCode = 19;
  }
  if(!strcmp(opcode, "loadi")){
    // printf("loadm 20\n");
    NumOpCode = 20;
  }
  if(!strcmp(opcode, "storei")){
    // printf("storem 21\n");
    NumOpCode = 21;
  }
  if(!strcmp(opcode, "dec")){
    // printf("dec 22\n");
    NumOpCode = 22;
  }
  if(!strcmp(opcode, "inc")){
    // printf("inc 23\n");
    NumOpCode = 23;
  }
  if(!strcmp(opcode, "sqrt")){
    // printf("sqrt 24\n");
    NumOpCode = 24;
  }
  if(!strcmp(opcode, "pow")){
    // printf("pow 25\n");
    NumOpCode = 25;
  }
  if(!strcmp(opcode, "addi")){
    // printf("addi 26\n");
    NumOpCode = 26;
  }
  if(!strcmp(opcode, "subi")){
    // printf("subi 27\n");
    NumOpCode = 27;
  }
  if(!strcmp(opcode, "jal")){
    // printf("jal 32\n");
    NumOpCode = 32;
  }
  if(!strcmp(opcode, "jr")){
    // printf("jr 33\n");
    NumOpCode = 33;
  }
  if(!strcmp(opcode, "beq")){
    // printf("beq 34\n");
    NumOpCode = 34;
  }
  if(!strcmp(opcode, "bne")){
    // printf("bne 35\n");
    NumOpCode = 35;
  }
  if(!strcmp(opcode, "j")){
    // printf("j 36\n");
    NumOpCode = 36;
  }
  if(!strcmp(opcode, "halt")){
    // printf("halt 255\n");
    NumOpCode = 255;
  }

  return(NumOpCode);
}

void ConverteInstrucao(char Instrucao[], char InstrucaoBinario[]){
  int NumOpCode, FimInstrucao, rc, rb, ra, constan;
  char opcode[10], BinTemp[40], Label[10];

  /*Pego a string opcode na variavel opcode*/
  IndenficaOpcode(Instrucao, opcode, &FimInstrucao);
  /*Converto a string para um numero referente ao opcode*/
  NumOpCode = ConverteOpcodeInteiro(opcode);
  /*Converto o numero para um binario de 8 bits*/
  itob(NumOpCode, 8, InstrucaoBinario);
  /*Gero a o binario de acordo com os valores seguidos do opcode*/
  switch(NumOpCode){
    case 0:	 //NOP
        itob(NumOpCode, 24, BinTemp);
        strcat(InstrucaoBinario,BinTemp);
      break;
    case 1:	 //ADD
        sscanf(Instrucao,"add r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 2:	 //SUB
        sscanf(Instrucao,"sub r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 3:	 //ZERO
        sscanf(Instrucao,"zeros r%d", &rc);
        itob(rc, 24, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 4:	 //XOR
        sscanf(Instrucao,"xor r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 5:	 //OR
        sscanf(Instrucao,"or r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 6:	 //NOT
        sscanf(Instrucao,"not r%d,r%d", &rc, &ra);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 7:	 //AND
        sscanf(Instrucao,"and r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 8:	 //SHIFT ARI ESQ
        sscanf(Instrucao,"asl r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 9:	 //SHIFT ARI DIR
        sscanf(Instrucao,"asr r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 10: //SHIFT LOG ESQ
        sscanf(Instrucao,"lsl r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 11: //SHIFT LOG DIR
        sscanf(Instrucao,"lsr r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 12: //COPIA
        sscanf(Instrucao,"passa r%d,r%d", &rc, &ra);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 14: //LOAD MAIS SIG
        sscanf(Instrucao,"lch r%d,%d", &rc, &constan);
        itob(constan, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 15: //LOAD MENOS SIG
        sscanf(Instrucao,"lcl r%d,%d", &rc, &constan);
        itob(constan, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 16: //LOAD
        sscanf(Instrucao,"load r%d,r%d", &rc, &ra);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 17: //STORE
        sscanf(Instrucao,"store r%d,r%d", &rc, &ra);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 18: //MULT
        sscanf(Instrucao,"mult r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 19: //DIV
        sscanf(Instrucao,"div r%d,r%d,r%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 20: //LOAD IMEDIATO
        sscanf(Instrucao,"loadi r%d,%d", &rc, &constan);
        itob(constan, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 21: //STORE IMEDIATO
        sscanf(Instrucao,"storei r%d,%d", &rc, &constan);
        itob(constan, 16, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 22: //DECREMENTA
        sscanf(Instrucao,"dec r%d", &rc);
        itob(rc, 24, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 23: //INCREMENTA
        sscanf(Instrucao,"inc r%d", &rc);
        itob(rc, 24, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 24: //SQRT
        sscanf(Instrucao,"sqrt r%d,%d", &rc,&ra);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(0, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 25: //POW
        sscanf(Instrucao,"pow r%d,%d,%d", &rc,&ra,&rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 26: //ADD IMEDIATO
        sscanf(Instrucao,"addi r%d,%d,%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 27: //SUB IMEDIATO
        sscanf(Instrucao,"subi r%d,%d,%d", &rc, &ra, &rb);
        itob(ra, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rb, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        itob(rc, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 32: //JUMP AND LINK
        sscanf(Instrucao,"jal %s", Label);
        constan = LabelBuscaEndereco(Label);
        if(constan!=-1){
          itob(constan, 24, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
        }else{
          printf("Label %s não foi encontrado", Label);
          exit(-1);
        }
      break;
    case 33: //JUMP REGISTER
        sscanf(Instrucao,"jr r%d", &rc);
        itob(rc, 24, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
    case 34: //JUMP SE IGUAL
        sscanf(Instrucao,"beq r%d,r%d,%s", &ra, &rb, Label);
        constan = LabelBuscaEndereco(Label);
        if(constan!=-1){
          itob(ra, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
          itob(rb, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
          itob(constan, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
        }else{
          printf("Label %s não foi encontrado", Label);
          exit(-1);
        }
      break;
    case 35: //JUMP SE DIFERENTE
        sscanf(Instrucao,"bne r%d,r%d,%s", &ra, &rb, Label);
        constan = LabelBuscaEndereco(Label);
        if(constan!=-1){
          itob(ra, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
          itob(rb, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
          itob(constan, 8, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
        }else{
          printf("Label %s não foi encontrado", Label);
          exit(-1);
        }
      break;
    case 36: //JUMP INCONDICIONAL
        sscanf(Instrucao,"j %s", Label);
        constan = LabelBuscaEndereco(Label);
        if(constan!=-1){
          itob(constan, 24, BinTemp);
          strcat(InstrucaoBinario, BinTemp);
        }else{
          printf("Label %s não foi encontrado", Label);
          exit(-1);
        }
      break;
    case 255://HALT
        itob(NumOpCode, 8, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
        strcat(InstrucaoBinario, BinTemp);
      break;
  }
}

int VerificaLabel(char Instrucao[]){
  /*Verifica se uma instruçao lida se trata de um label*/
  if(Instrucao[strlen(Instrucao)-1] == ':'){
    return(1);
  }
  return(0);
}

void LabelInsere(char Label[]){
  /*Coloco o Label na proxima posição vazia*/
  strcat(VetLabels[ContLabel].Nome,Label);
  /*O endereço onde irá começar este label é a proxima posição de memoria vazia*/
  VetLabels[ContLabel].posiMemoria = ContMemoria;
  ContLabel++;
}

int LabelBuscaEndereco(char Label[]){
  int i;
  /*Retorna o entedeço de memoria onde começa um determinado label*/
  for(i=0; i < ContLabel; i++){
    if(!strcmp(Label, VetLabels[i].Nome)){
      return(VetLabels[i].posiMemoria);
    }
  }
  return(-1);
}

void LeInstrucoesArquivo(char ArquivoEntrada[]){
  char InstrucaoLida[100];
	FILE *ArqEnt;

	ArqEnt = fopen(ArquivoEntrada, "r");
  while(fgets(InstrucaoLida, sizeof(InstrucaoLida), ArqEnt)){
    InstrucaoLida[strlen(InstrucaoLida)-1] = '\0';    /*Tira o enter da instrução*/
    /*Caso a instrução seja um label ele não é contado no contador de memoria,
      e é armazenado em um vetor contendo os labels para que quando for codificar os binarios
      ele possa ser consultado
    */
    if(VerificaLabel(InstrucaoLida)){
      InstrucaoLida[strlen(InstrucaoLida)-1] = '\0';  /*Tira o ":" do label*/
      LabelInsere(InstrucaoLida);
    }else{
      strcat(Memoria[ContMemoria], InstrucaoLida);
      ContMemoria++;
    }
  }
  fclose(ArqEnt);
}

int TamBitsNumero(int Num){
  int Result = 1, cont = 0;
  /*Apartir de um numero passado por parametro é verificado quandos bits são necessarios para representa-lo em binario*/
  while(Result <= Num){
    Result = Result*2;
    cont++;
  }
  if(cont==0){
    cont++;
  }
  return(cont);
}

void CodificaInstrucoes(char ArquivoSaida[]){
  char InstrucaoBinario[35], BinTemp[35];
  FILE *ArqSai;
  int i;

  ArqSai = fopen(ArquivoSaida, "wt");
  InstrucaoBinario[0] = '\0';
  strcat(InstrucaoBinario, "ADDRESS ");
  itob(ADDRESS, TamBitsNumero(ADDRESS), BinTemp);
  strcat(InstrucaoBinario, BinTemp);

  fprintf(ArqSai, "%s\n", InstrucaoBinario);          /*Gera o ADDRESS do codigo em binario*/
  for(i=0; i < ContMemoria; i++){
    InstrucaoBinario[0] = '\0';
    ConverteInstrucao(Memoria[i], InstrucaoBinario);  /*Converte a instrução para binario*/
    fprintf(ArqSai, "%s\n", InstrucaoBinario);        /*Imprime a instrução no arquivo de saida*/
  }
  fclose(ArqSai);
}

void Montador(char ArquivoEntrada[], char ArquivoSaida[]){
  LeInstrucoesArquivo(ArquivoEntrada);
  CodificaInstrucoes(ArquivoSaida);
}
