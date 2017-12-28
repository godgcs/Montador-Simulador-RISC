#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define TamMemoria 65536
#define TamRegistradores 32
#define TamInstrucoes 100

typedef struct tipo1{
	unsigned int rc:8;
	unsigned int rb:8;
	unsigned int ra:8;
	unsigned int opcode:8;
}*Tipo1;

typedef struct tipo2{
	unsigned int rc:8;
	unsigned int end:16;
	unsigned int opcode:8;
}*Tipo2;

typedef struct tipo3{
	unsigned int end:24;
	unsigned int opcode:8;
}*Tipo3;

/*Cabeçalho de Funções privadas*/
void VerificaNeg(int rc);
void VerificaZero(int rc);
void VerificaCarry(int ra, int rb, int rc);
/*Funções de ciclo do processador*/
int  InstructionFetch(void);
void InstructionDecode(long int Instrucao);

/*Variaveis Globais do simulador*/
int Memoria[TamMemoria];
int Registradores[TamRegistradores];
int IR, neg, zero, carry, overflow, ADDRESS = 0;
int ContMemoria=0, PC=0;

/*Implementaões de Funções*/
void LeInstrucoesArquivo(char ArquivoEntrada[]){
  char Instrucao[TamInstrucoes], ADDRE[32];
	FILE *Arq;

	Arq = fopen(ArquivoEntrada, "r");
	/*Carrega arquivo para memoria*/
  while(fgets(Instrucao, sizeof(Instrucao), Arq)){
		if(Instrucao[0] == 'A'){
			sscanf(Instrucao,"ADDRESS %s", ADDRE);
			ADDRESS = strtol(ADDRE, NULL, 2);
			ContMemoria = ADDRESS;
			PC = ADDRESS;
			printf("ADDRESS %d\n", ADDRESS);
		}else{
	    Memoria[ContMemoria] = strtol(Instrucao, NULL, 2);
	    ContMemoria++;
		}
  }
}

void VerificaNeg(int rc){
	if(Registradores[rc] < 0){
		neg = 1;
	}else{
		neg = 0;
	}
}

void VerificaZero(int rc){
	if(Registradores[rc] == 0){
		zero = 1;
	}else{
		zero = 0;
	}
}

void VerificaCarry(int ra, int rb, int rc){
	if((Registradores[ra] >= 0 && Registradores[rb] < 0 && Registradores[rc] >= 0) ||
		 (Registradores[ra] < 0 && Registradores[rb] >= 0 && Registradores[rc] >= 0) ||
	 	 (Registradores[ra] < 0 && Registradores[rb] < 0)){
		carry = 1;
	}else{
		carry = 0;
	}
}

int InstructionFetch(void){
	int OldPC = PC;

	PC++;
	return(Memoria[OldPC]);
}

void InstructionDecode(long int Instrucao){
	int opcode, rc, rb, ra, constan, Result;
	Tipo1 InstrucaoTipo1 = (Tipo1) &Instrucao;
	Tipo2 InstrucaoTipo2 = (Tipo2) &Instrucao;
	Tipo3 InstrucaoTipo3 = (Tipo3) &Instrucao;

	opcode = InstrucaoTipo1->opcode;
	switch(opcode){
		case 0:	 //NOP
				/*Operação*/

				/*Flags*/

				printf("NOP\n");
			break;
		// ALU
		case 1:	 //ADD
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] + Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if((Registradores[ra] >= 0 && Registradores[rb] < 0 && Registradores[rc] >= 0) ||
					 (Registradores[ra] < 0 && Registradores[rb] >= 0 && Registradores[rc] >= 0) ||
				 	 (Registradores[ra] < 0 && Registradores[rb] < 0)){
					carry = 1;
				}else{
					carry = 0;
				}

				if((Registradores[ra] > 0 && Registradores[rb] > 0 && Registradores[rc] < 0) ||
					 (Registradores[ra] < 0 && Registradores[rb] < 0 && Registradores[rc] > 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("add r%d = %d+%d que resulta em %d\n", rc, Registradores[ra], Registradores[rb], Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 2:	 //SUB
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] - Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if((Registradores[ra] >= 0 && Registradores[rb] < 0 && Registradores[rc] >= 0) ||
					 (Registradores[ra] < 0 && Registradores[rb] >= 0 && Registradores[rc] >= 0) ||
				 	 (Registradores[ra] < 0 && Registradores[rb] < 0)){
					carry = 1;
				}else{
					carry = 0;
				}

				if((Registradores[ra] > 0 && Registradores[rb] < 0 && Registradores[rc] < 0) ||
					 (Registradores[ra] < 0 && Registradores[rb] > 0 && Registradores[rc] > 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("sub r%d = %d-%d que resulta em %d\n", rc, Registradores[ra], Registradores[rb], Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 3:	 //ZERO
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = 0;
				/*Flags*/
				neg = 0;
				zero = 1;
				carry = 0;
				overflow = 0;

				printf("zeros r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 4:	 //XOR
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] ^ Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("xor r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 5:	 //OR
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] | Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("or r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 6:	 //NOT
				ra = InstrucaoTipo1->ra;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = ~Registradores[ra];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("not r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 7:	 //AND
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] & Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("add r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 8:	 //SHIFT ARI ESQ
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] << Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("asl r%d = %d", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 9:	 //SHIFT ARI DIR
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] >> Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("asr r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 10: //SHIFT LOG ESQ
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] << Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("lsl r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 11: //SHIFT LOG DIR
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] >> Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("lsr r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 12: //COPIA
				ra = InstrucaoTipo1->ra;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				overflow = 0;

				printf("passa r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		// CONSTANTES E ACESSO A MEMORIA
		case 14: //LOAD MAIS SIG
				rc 			= InstrucaoTipo2->rc;
				constan = InstrucaoTipo2->end;

				/*Operação*/
				Registradores[rc] = (constan << 16) | (rc & 0x0000ffff);
				/*Flags*/

				printf("lch r%d = %d\n", rc, Registradores[rc]);
			break;
		case 15: //LOAD MENOS SIG
				rc 			= InstrucaoTipo2->rc;
				constan	= InstrucaoTipo2->end;

				/*Operação*/
				Registradores[rc] = constan | (rc & 0xffff0000);
				/*Flags*/

				printf("lcl r%d = %d\n", rc, Registradores[rc]);
			break;
		case 16: //LOAD
				ra = InstrucaoTipo1->ra;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Memoria[Registradores[ra]];
				/*Flags*/

				printf("load r%d = M[%d] que tem %d\n", rc, Registradores[ra], Memoria[Registradores[ra]]);
			break;
		case 17: //STORE
				ra = InstrucaoTipo1->ra;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Memoria[Registradores[rc]] = Registradores[ra];
				/*Flags*/

				printf("store M[%d] = r%d que tem %d\n", Registradores[rc], ra, Registradores[ra]);
			break;
		// MEUS OPERANDOS
		case 18: //MULT
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] * Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if((Registradores[ra] > 0 && Registradores[rb] > 0 && Registradores[rc] < 0) ||
					 (Registradores[ra] > 0 && Registradores[rb] < 0 && Registradores[rc] > 0) ||
				   (Registradores[ra] < 0 && Registradores[rb] > 0 && Registradores[rc] > 0) ||
				   (Registradores[ra] < 0 && Registradores[rb] < 0 && Registradores[rc] < 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				carry = 0;

				printf("mult r%d = %d*%d que resulta em %d\n", rc, Registradores[ra], Registradores[rb], Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 19: //DIV
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = Registradores[ra] / Registradores[rb];
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;

				if((Registradores[ra] > 0 && Registradores[rb] > 0 && Registradores[rc] < 0) ||
					 (Registradores[ra] > 0 && Registradores[rb] < 0 && Registradores[rc] > 0) ||
				   (Registradores[ra] < 0 && Registradores[rb] > 0 && Registradores[rc] > 0) ||
				   (Registradores[ra] < 0 && Registradores[rb] < 0 && Registradores[rc] < 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("div r%d =  %d/%d que resulta em %d\n", rc, Registradores[ra], Registradores[rb], Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 20: //LOAD IMEDIATO
				rc 			= InstrucaoTipo2->rc;
				constan	= InstrucaoTipo2->end;

				/*Operação*/
				Registradores[rc] = constan;
				/*Flags*/

				printf("loadi r%d = %d\n", rc, constan);
 		case 21: //STORE IMEDIATO
				rc 			= InstrucaoTipo2->rc;
				constan	= InstrucaoTipo2->end;

				/*Operação*/
				Memoria[constan] = Registradores[rc];
				/*Flags*/

				printf("storei M[%d] = r%d que tem %d\n", constan, rc, Registradores[rc]);
			break;
		case 22: //DEC
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Result = Registradores[rc];
				Registradores[rc] = Result-1;
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if(Registradores[rc] < 0 && Result > 0){
					overflow = 1;
				}else{
					overflow = 0;
				}
				carry = 0;
				printf("dec r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 23: //INC
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Result = Registradores[rc];
				Registradores[rc] = Result+1;
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if(Registradores[rc] >= 0 && Result < 0){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("inc r%d = %d\n", rc, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 24: //SQRT
				rc = InstrucaoTipo1->rc;
				ra = InstrucaoTipo1->ra;

				/*Operação*/
				float Result2 = sqrt(ra);
				Registradores[rc] = Result2;
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				if(Registradores[rc] != Result2){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("sqrt r%d = sqrt(%d) que resulta em %d\n", rc, ra, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 25: //POW
				rc = InstrucaoTipo1->rc;
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;

				/*Operação*/
				Registradores[rc] =pow(ra,rb);
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;
				if((ra >= 0 && Registradores[rc] < 0) ||
					 (ra < 0 && rb < 0 && Registradores[rc] > 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("pow r%d = %d^%d que resulta em %d\n", rc, ra, rb, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 26: //ADD IMEDIATO
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = ra + rb;
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				if((ra >= 0 && rb < 0 && Registradores[rc] >= 0) ||
					 (ra < 0 && rb >= 0 && Registradores[rc] >= 0) ||
				 	 (ra < 0 && rb < 0)){
					carry = 1;
				}else{
					carry = 0;
				}

				if((ra > 0 && rb > 0 && Registradores[rc] < 0) ||
					 (ra < 0 && rb < 0 && Registradores[rc] > 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}
				printf("addi r%d = %d+%d que resulta em %d\n", rc, ra, rb, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		case 27: //SUB IMEDIATO
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				Registradores[rc] = ra - rb;
				/*Flags*/
				VerificaNeg(rc);
				VerificaZero(rc);
				carry = 0;

				if((ra > 0 && rb < 0 && Registradores[rc] < 0) ||
					 (ra < 0 && rb > 0 && Registradores[rc] > 0)){
					overflow = 1;
				}else{
					overflow = 0;
				}

				printf("subi r%d = %d-%d que resulta em %d\n", rc, ra, rb, Registradores[rc]);
				printf("\t**Flags: neg=%d | zero=%d | carry=%d | overflow=%d\n",neg,zero,carry,overflow);
			break;
		// TRANSFERENCIA DE CONTROLE
		case 32: //JUMP AND LINK
				constan = InstrucaoTipo3->end;

				/*Operação*/
				Registradores[31] = PC;
				PC = constan + ADDRESS;
				/*Flags*/

				printf("jal r31 = %d & PC = %d\n", Registradores[31], PC);
			break;
		case 33: //JUMP REGISTER
				rc = InstrucaoTipo1->rc;

				/*Operação*/
				PC = Registradores[31] + ADDRESS;
				/*Flags*/

				printf("jr PC = r31 que tem %d\n", PC);
			break;
		case 34: //JUMP SE IGUAL
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				constan = InstrucaoTipo1->rc;

				printf("beq if(%d = %d)", Registradores[ra], Registradores[rb]);
				/*Operação*/
				if(Registradores[ra] == Registradores[rb]){
					PC = constan + ADDRESS;
					printf("{PC = %d}\n", PC);
				}else{
					printf("{Não acontece nada}\n");
				}
				/*Flags*/
			break;
		case 35: //JUMP SE DIFERENTE
				ra = InstrucaoTipo1->ra;
				rb = InstrucaoTipo1->rb;
				constan = InstrucaoTipo1->rc;

				printf("bne if(%d != %d)", Registradores[ra], Registradores[rb]);
				/*Operação*/
				if(Registradores[ra] != Registradores[rb]){
					PC = constan + ADDRESS;
					printf("{PC = %d}\n", PC);
				}else{
					printf("{Não acontece nada}\n");
				}
				/*Flags*/
			break;
		case 36: //JUMP INCONDICIONAL
				constan = InstrucaoTipo3->end;

				/*Operação*/
				PC = constan + ADDRESS;
				/*Flags*/

				printf("j PC = %d\n", PC);
			break;
		case 255://HALT
				/*Operação*/

				/*Flags*/

				printf("halt\n");
			break;
	}
}

void ExecutaInstrucoes(void){
	int zeroo = 0;
	Tipo1 InstrucaoTipo1 = (Tipo1) &zeroo;

	while(InstrucaoTipo1->opcode != 255){
		IR = InstructionFetch();
		InstrucaoTipo1 = (Tipo1) &IR;
		InstructionDecode(IR);
	}
}

void Simulador(char ArquivoEntrada[]){
	LeInstrucoesArquivo(ArquivoEntrada);
	ExecutaInstrucoes();
}
