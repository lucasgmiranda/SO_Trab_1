#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Mtam 15


//lista que recebem os dados de cada processo
int PRI[Mtam];    //Prioridade
int TP[Mtam];     //Guarda o tempo restante 
int TP2[Mtam];    //Guarda o tempo total 
int TC[Mtam];     //Guarda o tempo de entrada 
int TEIOA[Mtam];  //Guarda o instante de uma I/O, A
int TEIOB[Mtam];  //Guarda o instante de uma I/O, B
int TEIOC[Mtam];  //Guarda o instante de uma I/O, C

//filas de execução
int AP[Mtam];     //fila de alta prioridade
int BP[Mtam];     //fila de baixa prioridade
int AIO[Mtam];    //fila da I/O A
int BIO[Mtam];    //fila da I/O B
int CIO[Mtam];    //fila da I/O C

//guarda a quantidade de processos
int tam = 0;   

//cicle time duration
int cicle_duration = 5;

//I/0 time duration
int A_duration = 4;
int B_duration = 7;
int C_duration = 6;

//trata os dados de I/0
void entrada() {

   int j;
   for (j = 0; j < Mtam; j++){
      PRI[j] = 0;
      TP[j] = 0;
      TP2[j] = 0;
      TC[j] = 0;
      TEIOA[j] = 0;
      TEIOB[j] = 0;
      TEIOC[j] = 0;
      AP[j]= 0;
      BP[j]= 0;
      AIO[j]= 0;
      BIO[j]= 0;
      CIO[j]= 0;
   }

   FILE *fPointer;
   fPointer = fopen("test.txt", "r");

   char singleLine[150];
   char *sub;
   char *sub2;
   int count;
   int pc = 0;

   while(!feof(fPointer)){
      tam +=1;
      fgets(singleLine, 150, fPointer);
      // puts(singleLine);
      sub = strtok(singleLine,";");
      count = 0;

      while(sub != NULL){
         count = count + 1;

         // printf ("SUB: %s\n", sub);
        
         switch (count){
            case 1:
               break;

            case 2:
               PRI[pc] = atoi(sub);
               break;

            case 3:
               TP[pc] = atoi(sub);
               TP2[pc] = atoi(sub);
               break;

            case 4:
               TC[pc] = atoi(sub);
               break;

            case 5:
               TEIOA[pc] = atoi(sub);
               break;

            case 6:
               TEIOB[pc] = atoi(sub);
               break;

            case 7:
               TEIOC[pc] = atoi(sub);
               break; 
         }

         sub = strtok(NULL,";");
      }
      //printf ("count: %d\n",count);
      pc+=1;
   }
   
}
   
//desloca todos os elementos do array para a esquerda
void shiftl(int A[]){
   int i;
   for(i=0;i<tam-1;i++){
      A[i]=A[i+1];
   }
   A[tam-1] = 0;
}

//coloca o valor na primeira posição = 0 do vetor
void append(int A[], int elem){
   int i = 0;
   while(A[i] != 0){
      i++;
   }
   A[i] = elem;
}

//verifica quanto tempo de processo ainda resta ao todo para ser executado
int pttotal(){
   int soma = 0;
   int i;
   for(i=0;i<tam;i++){
      soma += TP[i];
   }

   return soma;
}

//pega um novo processo das filas e coloca em execução
int changeP(int ie){
   ie = AP[0];
   shiftl(AP);
   if(ie==0){
      ie = BP[0];
      shiftl(BP);
   }
   return ie - 1;
}


int main(){

   entrada();

   FILE *fp;
   fp = fopen("output", "w");
   
   //Guarda o tempo total de processo restante
   int pt = 0;

   //Guarda o index do processo em execução
   int ie = 0;

   //Guarda a unidade de tempo em que se encontra a execução
   int ut = 0;
   
   //Contadores do tempo de ciclo
   int cicle_timer = 1;     //de execução de processo
   int A_timer = 0;     //de execução de uma I/O, A 
   int B_timer = 0;     //de execução de uma I/O, B 
   int C_timer = 0;     //de execução de uma I/O, C 

   //printa o header do output
   fprintf(fp,"|     ");
   int i;
   for(i=1;i<=tam;i++)
      fprintf(fp,"P%d  ",i);
   fprintf(fp,"|");
   fprintf(fp,"  A  B  C  |\n");

   do{
      
      //incrementa a unidade de tempo em que se encontra o processamento
      ut ++;

      //printa cada linha do output
      fprintf(fp,"|%3.0d: ",ut);
      int k;
      for(k=0;k<tam;k++){
         if(k != ie){
            fprintf(fp,"    ");
         }else{
            fprintf(fp,"II  ");
         }
      }
      fprintf(fp,"|  ");

      //contabiliza a execução de uma ut do processo
      if(ie != -1){
         TP[ie]--;
      }

      //checa se houve entrada de um processo novo
      int i;
      for(i = 0;i<tam;i++){
         if(TC[i] == ut){
            append(AP, i+1);
         }
      }

      //checa se algum processo finalizou uma operação A de I/O
      if(AIO[0]!=0){
         A_timer+=1;
         fprintf(fp,"%d  ",AIO[0]);
         if(A_timer==A_duration){
            A_timer=0;
            append(AP, AIO[0]);
            shiftl(AIO);
         }
      }else{
         fprintf(fp,"   ");
      }

      //checa se algum processo finalizou uma operação B de I/O
      if(BIO[0]!=0){
         B_timer+=1;
         fprintf(fp,"%d  ",BIO[0]);
         if(B_timer==B_duration){
            B_timer=0;
            append(AP, BIO[0]);
            shiftl(BIO);
         }
      }else{
         fprintf(fp,"   ");
      }

      //checa se algum processo finalizou uma operação C de I/O
      if(CIO[0]!=0){
         C_timer+=1;
         fprintf(fp,"%d  |\n",CIO[0]);
         if(C_timer==C_duration){
            C_timer=0;
            append(AP, CIO[0]);
            shiftl(CIO);
         }
      }else{
         fprintf(fp,"   |\n");
      }

      //checa se existe algum processo ainda a ser terminado
      pt = pttotal();
      if(pt == 0){
         break;
      }

      //caso não tenha processo em execução ele verifica se tem algum na fila
      if(ie == -1){
         ie = changeP(ie);
         continue;
      }

      //verifica se o processo do ciclo atual ja foi finalizado
      if(TP[ie]==0){
         cicle_timer = 0;
         ie = changeP(ie);
      }

      //checa se o processo entrou em uma operação de I/O A
      if((TP2[ie] - TP[ie] == TEIOA[ie]) && ((TP2[ie] - TP[ie]) >0)){
         cicle_timer =0;
         TEIOA[ie] = 0;
         append(AIO, ie+1);
         ie = changeP(ie);
      }

      //checa se o processo entrou em uma operação de I/O B
      if((TP2[ie] - TP[ie] == TEIOB[ie]) && (TP2[ie] - TP[ie]) >0){
         cicle_timer =0;
         TEIOB[ie] = 0;
         append(BIO, ie+1);
         ie = changeP(ie);
      }

      //checa se o processo entrou em uma operação de I/O C
      if((TP2[ie] - TP[ie] == TEIOC[ie]) && (TP2[ie] - TP[ie]) >0){
         cicle_timer =0;
         TEIOC[ie] = 0;
         append(CIO, ie+1);
         ie = changeP(ie);
      }

      //verifica se o ciclo atual ja foi concluido
      if(cicle_timer == cicle_duration){
         cicle_timer = 0;
         append(BP, ie+1);
         ie = changeP(ie);
      }
      
      //incrementa o contador do ciclo atual
      cicle_timer+=1;

   }while(1);

   fclose(fp);
   
   return 0;
}