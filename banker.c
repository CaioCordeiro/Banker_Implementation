#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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


int main(int argc, char *argv[]){
    //START DEBUG LEVEL
    if(DEBUG_LEVEL>=HIGH)
        printf("\nDEBUG HIGH => ATIVO\n");
    if(DEBUG_LEVEL>=MEDIUM)
        printf("\nDEBUG MEDIO => ATIVO\n");
    if(DEBUG_LEVEL>=LOW)
        printf("\nDEBUG LOW => ATIVO\n");
    //END DEBUG LEVEL
    
    //START DECLARATIONS
    const int TIME_OF_EXECUTION = atoi(argv[argc-1]);
    const int NUMBER_OF_RESOURCES =(argc-2);
    Banker banker;
    int lenght;

    //END DECLARATIONS

    //START INITIATION OF BANKER STRUCTURE
    banker.max_values = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    banker.live_values = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    if(DEBUG_LEVEL>=HIGH)
        printf("\nTIME_OF_EXECUTION => %d\n",TIME_OF_EXECUTION);
    for(int i=1;i<argc-1;i++){
        banker.max_values[i]= atoi(argv[i]);
        banker.live_values[i]= atoi(argv[i]);
        if(DEBUG_LEVEL>=HIGH)
            printf("\nBanker array position => %d->%d \n",banker.max_values[i],i);
    }
    //END INITIATION OF BANKER STRUCTURE
    //START READ FILE
    
    // scanf("%d",&lenght);//pegar numeros de threads
    if(DEBUG_LEVEL>=HIGH)
        printf("num_threads => %d\n",lenght );    
    // Cliente *client_list= malloc(sizeof(Cliente)*num_threads);
    // for(int thread = 0;thread <= num_threads;thread++){
    //     client_list[thread].valor_max = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    //     client_list[thread].allocated = malloc(sizeof(int)*NUMBER_OF_RESOURCES);
    //     client_list[thread].needs = malloc(sizeof(int)*NUMBER_OF_RESOURCES);

         
    // }'
    
    return 0;
}

