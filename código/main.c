#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Mtam 15
int PRI[Mtam];
int TP[Mtam];
int TP2[Mtam];
int TC[Mtam];
int TEIOA[Mtam];
int TEIOB[Mtam];
int TEIOC[Mtam];
int AP[Mtam];
int BP[Mtam];
int AIO[Mtam];
int BIO[Mtam];
int CIO[Mtam];
int tam = 0;

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
   for(int i=0;i<tam-1;i++){
      A[i]=A[i+1];
   }
   A[tam-1] = 0;
}

//coloca o valor na primeira posição = 0
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
   for(int i=0;i<tam;i++){
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
   
   int pt = 0;
   int ie = 0;
   int ct = 1;
   int ut = 0;
   int ac = 0;
   int bc = 0;
   int cc = 0;

   fprintf(fp,"|     ");
   for(int i=1;i<=tam;i++)
      fprintf(fp,"P%d  ",i);
   fprintf(fp,"|\n");

   do{
      
      ut ++;

      fprintf(fp,"|%3.0d: ",ut);
      for(int k=0;k<tam;k++){
         if(k != ie){
            fprintf(fp,"    ");
         }else{
            fprintf(fp,"II  ");
         }
      }
      fprintf(fp,"|\n");

      //contabiliza a execução de uma ut do processo
      TP[ie]--;

      //checa se existe algum processo ainda a ser terminado
      pt = pttotal();
      if(pt == 0){
         break;
      }

      //checa se houve entrada de um processo novo
      for(int i = 0;i<tam;i++){
         if(TC[i] == ut){
            append(AP, i+1);
         }
      }

      //checa se algum processo finalizou sua I/O
      if(AIO[0]!=0){
         ac+=1;
         if(ac==4){
            ac=0;
            append(AP, AIO[0]);
            shiftl(AIO);
         }
      }

      if(BIO[0]!=0){
         bc+=1;
         if(bc==7){
            bc=0;
            append(AP, BIO[0]);
            shiftl(BIO);
         }
      }

      if(CIO[0]!=0){
         cc+=1;
         if(cc==6){
            cc=0;
            append(AP, CIO[0]);
            shiftl(CIO);
         }
      }

      //verifica se o processo do ciclo atual ja foi finalizado
      if(TP[ie]==0){
         ct = 0;
         ie = changeP(ie);
      }

      //checa se o processo entrou em uma operação de I/O
      if((TP2[ie] - TP[ie] == TEIOA[ie]) && ((TP2[ie] - TP[ie]) >0)){
         ct =0;
         TEIOA[ie] = 0;
         append(AIO, ie+1);
         ie = changeP(ie);
      }
      if((TP2[ie] - TP[ie] == TEIOB[ie]) && (TP2[ie] - TP[ie]) >0){
         ct =0;
         TEIOB[ie] = 0;
         append(BIO, ie+1);
         ie = changeP(ie);
      }
      if((TP2[ie] - TP[ie] == TEIOC[ie]) && (TP2[ie] - TP[ie]) >0){
         ct =0;
         TEIOC[ie] = 0;
         append(CIO, ie+1);
         ie = changeP(ie);
      }

      //verifica se o ciclo atual ja foi concluido
      if(ct == 5){
         ct = 0;
         append(BP, ie+1);
         ie = changeP(ie);
      }
      ct+=1;

   }while(1);

   fclose(fp);
   return 0;
}