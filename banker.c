#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define MAX_SLEEP 10 // numero maximo de ciclos dormindo
#define MAX_RELEASE 2 // tempo máximo que o cliente vai levar para resolver o recurso

#define NO_DEBUG 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3
#define DEBUG_LEVEL HIGH


typedef struct{
    int num_ciclos_req; // quantidade de tempo para resolver um recurso
    int *valor_max; // array de valores máximos de cada cliente
    int id; // valor no array de clientes
    int *allocated; // valores já alocados para o cliente
    int *needs; // valores aleatórios para se retirar do bunker 
}Cliente;

typedef struct{
    int *max_values;//valor maximos dos valores do banco
    int *live_values;// valores atuais do banco
}Banker;


// int request_resources(int customer_num, int request[]);

// void release_resources(int customer_num, int release[]);

void debugHigh(const char *format, ...);
void debugMedium(const char *format, ...);
void debugLow(const char *format, ...);


int main(int argc, char *argv[]){
    srand(time(NULL));
    //START DEBUG LEVEL
    debugHigh("\nDEBUG HIGH => ATIVO\n");
    debugMedium("\nDEBUG MEDIO => ATIVO\n");
    debugLow("\nDEBUG LOW => ATIVO\n");
    //END DEBUG LEVEL
    
    //START DECLARATIONS
    const int TIME_OF_EXECUTION = atoi(argv[argc-1]);
    const int NUMBER_OF_RESOURCES =(argc-2);
    Banker banker;
    int num_threads;

    //END DECLARATIONS

    //START INITIATION OF BANKER STRUCTURE
    banker.max_values = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    banker.live_values = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    debugMedium("\nTIME_OF_EXECUTION => %d\n", TIME_OF_EXECUTION);
    
    for(int i=1;i<argc-1;i++){
        banker.max_values[i-1]= atoi(argv[i]);
        banker.live_values[i-1]= atoi(argv[i]);
        debugHigh("\nBanker array position => %d->%d \n",banker.max_values[i-1],i);
    }
    //END INITIATION OF BANKER STRUCTURE
    //START READ FILE
    
    scanf("%d",&num_threads);//pegar numeros de threads
    debugMedium("num_threads => %d\n",num_threads );    
    Cliente *client_list= malloc(sizeof(Cliente)*num_threads);
    
    for(int thread = 0; thread <= num_threads; thread++){
        client_list[thread].valor_max = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
        client_list[thread].allocated = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
        client_list[thread].needs = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
        client_list[thread].num_ciclos_req = rand() % MAX_SLEEP+1;
        debugMedium("\n**** Thread Number %d****\n", thread);
        debugMedium("num_ciclos_req => %d \n", client_list[thread].num_ciclos_req);
        
        for(int res=0; res<NUMBER_OF_RESOURCES; res++){
            int value;
            value = scanf("%d",&value);
            client_list[thread].valor_max[res]=value;
            client_list[thread].allocated[res] =0;
            client_list[thread].needs[res] = rand() % value+1;
            debugHigh("Value valor_max[%d] => %d | needs[%d] => %d \n", res, client_list[thread].valor_max[res], res,client_list[thread].needs[res]);
        }
        debugHigh("\nEnd Thread %d\n", thread);
    }
    return 0;
}

void debugHigh(const char *format, ...){
    va_list arg;
    va_start (arg, format);
    if(DEBUG_LEVEL>=HIGH)
        vfprintf(stdout, format, arg);
    va_end (arg);
}

void debugMedium(const char *format, ...){
    va_list arg;
    va_start (arg, format);
    if(DEBUG_LEVEL>=MEDIUM)
        vfprintf(stdout, format, arg);
    va_end (arg);
}

void debugLow(const char *format, ...){
    va_list arg;
    va_start (arg, format);
    if(DEBUG_LEVEL>=LOW)
        vfprintf(stdout, format, arg);
    va_end (arg);
}
