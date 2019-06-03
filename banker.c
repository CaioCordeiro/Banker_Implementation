#include <pthread.h>
#include <stdio.h>

#define MAX_SLEEP 10 // numero maximo de ciclos dormindo
#define MAX_RELEASE 2 // tempo máximo que o cliente vai levar para resolver o recurso
struct Cliente{
    int num_ciclos_req; // quantidade de tempo para resolver um recurso
    int *valor_max; // array de valores máximos de cada cliente
    int id; // valor no array de clientes
    int *allocated; // valores já alocados para o cliente
    int *needs; // valores aleatórios para se retirar do bunker 

};


int request_resources(int customer_num, int request[]);

void release_resources(int customer_num, int release[]);


int main(){
const int NUMBER_OF_CUSTOMERS = 5;
const int NUMBER_OF_RESOURCES = 4;









// struct Cliente clientes[NUMBER_OF_CUSTOMERS];

// /* the available amount of each resource */
// int available[NUMBER_OF_RESOURCES];
// /*the maximum demand of each customer */
// struct Cliente maximum[NUMBER_OF_CUSTOMERS];
// /* the amount currently allocated to each customer */
// int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
// /* the remaining need of each customer */
// int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

