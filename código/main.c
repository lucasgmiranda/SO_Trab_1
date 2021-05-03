#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_size 15

// Listas que recebem os dados de cada processo
int priority[max_size];             // Guarda as prioridades
int time_left[max_size];            // Guarda o tempo restante 
int total_time[max_size];           // Guarda o tempo total 
int input_time[max_size];           // Guarda o tempo de read_input_file 
int disk_instant_time[max_size];    // Guarda o instante de uma I/O, disk
int tape_instant_time[max_size];    // Guarda o instante de uma I/O, tape
int printer_instant_time[max_size]; // Guarda o instante de uma I/O, printer

// Filas de execução
int high_priority_queue[max_size];     // fila de alta prioridade
int low_priority_queue[max_size];      // fila de baixa prioridade
int disk_IO_queue[max_size];           // fila de I/O disco
int tape_IO_queue[max_size];           // fila de I/O tape
int printer_IO_queue[max_size];        // fila de I/O printer

// guarda a quantidade de processos
int size = 0;   

// fatia de tempo do processo
int time_slice = 5;

// I/0 time duration
int disk_duration = 4;
int tape_duration = 7;
int printer_duration = 6;

//  Lê e trata o arquivo de entrada
void read_input_file() {

   int j;
   for (j = 0; j < max_size; j++){
      priority[j] = 0;
      time_left[j] = 0;
      total_time[j] = 0;
      input_time[j] = 0;
      disk_instant_time[j] = 0;
      tape_instant_time[j] = 0;
      printer_instant_time[j] = 0;
      high_priority_queue[j]= 0;
      low_priority_queue[j]= 0;
      disk_IO_queue[j]= 0;
      tape_IO_queue[j]= 0;
      printer_IO_queue[j]= 0;
   }

   FILE *fPointer;
   fPointer = fopen("test.txt", "r");

   char singleLine[150];
   char *sub;
   char *sub2;
   int count;
   int pc = 0;

   while(!feof(fPointer)){
      size +=1;
      fgets(singleLine, 150, fPointer);
      //  puts(singleLine);
      sub = strtok(singleLine,";");
      count = 0;

      while(sub != NULL){
         count = count + 1;

         //  printf ("SUB: %s\n", sub);
        
         switch (count){
            case 1:
               break;

            case 2:
               priority[pc] = atoi(sub);
               break;

            case 3:
               time_left[pc] = atoi(sub);
               total_time[pc] = atoi(sub);
               break;

            case 4:
               input_time[pc] = atoi(sub);
               break;

            case 5:
               disk_instant_time[pc] = atoi(sub);
               break;

            case 6:
               tape_instant_time[pc] = atoi(sub);
               break;

            case 7:
               printer_instant_time[pc] = atoi(sub);
               break; 
         }

         sub = strtok(NULL,";");
      }
      // printf ("count: %d\n",count);
      pc+=1;
   }
   
}
   
//  Desloca todos os elementos do array para a esquerda
void shiftl(int a[]){
   int i;
   for(i=0;i<size-1;i++){
      a[i]=a[i+1];
   }
   a[size-1] = 0;
}

//  Coloca o valor na primeira posição = 0 do vetor
void append(int a[], int elem){
   int i = 0;
   while(a[i] != 0){
      i++;
   }
   a[i] = elem;
}

//  Verifica quanto tempo de processo ainda resta ao todo para ser executado
int total_process_time_left(){
   int sum = 0;
   int i;
   for(i=0;i<size;i++){
      sum += time_left[i];
   }
   return sum;
}

//  Pega um novo processo das filas e coloca em execução
int change_running_process(int pid){
   pid = high_priority_queue[0];
   shiftl(high_priority_queue);
   if(pid==0){
      pid = low_priority_queue[0];
      shiftl(low_priority_queue);
   }
   return pid - 1;
}


int main(){

   read_input_file();

   FILE *fp;
   fp = fopen("output", "w");
   
   // Guarda o tempo total de processo restante
   int total_time_left = 0;

   // Guarda o index do processo em execução
   int pid = 0;

   // Guarda a unidade de tempo em que se encontra a execução
   int running_current_time = 0;
   
   // Contadores do tempo de ciclo
   int process_cicle_timer = 1;  // de execução de processo
   int disk_cicle_timer = 0;     // de execução de uma I/O, disk 
   int tape_cicle_timer = 0;     // de execução de uma I/O, tape 
   int printer_cicle_timer = 0;  // de execução de uma I/O, printer 

   // Imprime o header do output
   fprintf(fp,"|     ");
   int i;
   for(i=1;i<=size;i++)
      fprintf(fp,"P%d  ",i);
   fprintf(fp,"|");
   fprintf(fp," Disk Tape Printer |\n");

   do{
      
      // Incrementa a unidade de tempo em que se encontra o processamento
      running_current_time ++;

      // Imprime cada linha do output
      fprintf(fp,"|%3.0d: ",running_current_time);
      int k;
      for(k=0;k<size;k++){
         if(k != pid){
            fprintf(fp,"    ");
         }else{
            fprintf(fp,"II  ");
         }
      }
      fprintf(fp,"|  ");

      // Contabiliza a execução de uma unidade de tempo do processo
      if(pid != -1){
         time_left[pid]--;
      }

      // Checa se houve a entrada de um processo novo
      int i;
      for(i = 0;i<size;i++){
         if(input_time[i] == running_current_time){
            append(high_priority_queue, i+1);
         }
      }

      // Checa se algum processo finalizou uma operação disco
      if(disk_IO_queue[0]!=0){
         disk_cicle_timer+=1;
         fprintf(fp,"%d     ",disk_IO_queue[0]);
         if(disk_cicle_timer==disk_duration){
            disk_cicle_timer=0;
            append(low_priority_queue, disk_IO_queue[0]);
            shiftl(disk_IO_queue);
         }
      }else{
         fprintf(fp,"      ");
      }

      // Checa se algum processo finalizou uma operação de fita magnética
      if(tape_IO_queue[0]!=0){
         tape_cicle_timer+=1;
         fprintf(fp,"%d    ",tape_IO_queue[0]);
         if(tape_cicle_timer==tape_duration){
            tape_cicle_timer=0;
            append(high_priority_queue, tape_IO_queue[0]);
            shiftl(tape_IO_queue);
         }
      }else{
         fprintf(fp,"     ");
      }

      // Checa se algum processo finalizou uma operação de impressora
      if(printer_IO_queue[0]!=0){
         printer_cicle_timer+=1;
         fprintf(fp,"%d     |\n",printer_IO_queue[0]);
         if(printer_cicle_timer==printer_duration){
            printer_cicle_timer=0;
            append(high_priority_queue, printer_IO_queue[0]);
            shiftl(printer_IO_queue);
         }
      }else{
         fprintf(fp,"      |\n");
      }

      // Checa se existe algum processo ainda a ser terminado
      total_time_left = total_process_time_left();
      if(total_time_left == 0){
         break;
      }

      // Caso não tenha processo em execução ele verifica se tem algum na fila
      if(pid == -1){
         pid = change_running_process(pid);
         continue;
      }

      // Verifica se o processo do ciclo atual ja foi finalizado
      if(time_left[pid]==0){
         process_cicle_timer = 0;
         pid = change_running_process(pid);
      }

      // Checa se o processo entrou em uma operação de I/O de disco
      if((total_time[pid] - time_left[pid] == disk_instant_time[pid]) && ((total_time[pid] - time_left[pid]) >0)){
         process_cicle_timer =0;
         disk_instant_time[pid] = 0;
         append(disk_IO_queue, pid+1);
         pid = change_running_process(pid);
      }

      // Checa se o processo entrou em uma operação de I/O de fita magnética
      if((total_time[pid] - time_left[pid] == tape_instant_time[pid]) && (total_time[pid] - time_left[pid]) >0){
         process_cicle_timer =0;
         tape_instant_time[pid] = 0;
         append(tape_IO_queue, pid+1);
         pid = change_running_process(pid);
      }

      // Checa se o processo entrou em uma operação de I/O de impressora
      if((total_time[pid] - time_left[pid] == printer_instant_time[pid]) && (total_time[pid] - time_left[pid]) >0){
         process_cicle_timer =0;
         printer_instant_time[pid] = 0;
         append(printer_IO_queue, pid+1);
         pid = change_running_process(pid);
      }

      // Verifica se o ciclo atual ja foi concluido
      if(process_cicle_timer == time_slice){
         process_cicle_timer = 0;
         append(low_priority_queue, pid+1);
         pid = change_running_process(pid);
      }
      
      // Incrementa o contador do ciclo atual
      process_cicle_timer+=1;

   }while(1);

   fclose(fp);
   
   return 0;
}