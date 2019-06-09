#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SLEEP 10  // numero maximo de ciclos dormindo
#define MAX_RELEASE 2 // tempo máximo que o cliente vai levar para resolver o recurso

#define NO_DEBUG 0
#define LOW 1
#define MEDIUM 2
#define HIGH 3
#define DEBUG_LEVEL HIGH

#define DEADLOCK -1
#define NO_RESOURCES 0
#define RESOURCE_ALLOCATED 1

typedef struct
{
    int num_ciclos_req; // quantidade de tempo para resolver um recurso
    int *valor_max;     // array de valores máximos de cada cliente
    int id;             // valor no array de clientes
    int *allocated;     // valores já alocados para o cliente
    int *needs;         // valores aleatórios para se retirar do bunker
} Cliente;

struct thread_args
{
    Cliente *clientList;
    int clientThread;
};

typedef struct
{
    int *max_values;  //valor maximos dos valores do banco
    int *live_values; // valores atuais do banco
    int num_threads;  // número de threads
    int NUMBER_OF_RESOURCES;
} Banker;

// int request_resources(int customer_num, int request[]);

// void release_resources(int customer_num, int release[]);

void debugHigh(const char *format, ...);
void debugMedium(const char *format, ...);
void debugLow(const char *format, ...);
int askForResource(int clientId);
void *runner(void *vargp);
int bankerAlgorithm();
Cliente *client_list;
Banker banker;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
    srand(time(NULL));
    //START DEBUG LEVEL
    debugHigh("\nDEBUG HIGH => ATIVO\n");
    debugMedium("\nDEBUG MEDIO => ATIVO\n");
    debugLow("\nDEBUG LOW => ATIVO\n");
    //END DEBUG LEVEL

    //START DECLARATIONS
    const int TIME_OF_EXECUTION = atoi(argv[argc - 1]);
    banker.NUMBER_OF_RESOURCES = (argc - 2);
    //END DECLARATIONS

    //START INITIATION OF BANKER STRUCTURE
    banker.max_values = malloc(sizeof(int) * banker.NUMBER_OF_RESOURCES);
    banker.live_values = malloc(sizeof(int) * banker.NUMBER_OF_RESOURCES);
    debugMedium("\nTIME_OF_EXECUTION => %d\n", TIME_OF_EXECUTION);

    for (int i = 1; i < argc - 1; i++)
    {
        banker.max_values[i - 1] = atoi(argv[i]);
        banker.live_values[i - 1] = atoi(argv[i]);
        debugHigh("\nBanker array position => %d->%d \n", banker.max_values[i - 1], i);
    }
    //END INITIATION OF BANKER STRUCTURE
    //START READ FILE

    scanf("%d", &banker.num_threads); //pegar numeros de threads
    debugMedium("num_threads => %d\n", banker.num_threads);
    client_list = malloc(sizeof(Cliente) * banker.num_threads);

    for (int thread = 0; thread < banker.num_threads; thread++)
    {
        client_list[thread].valor_max = malloc(sizeof(int) * banker.NUMBER_OF_RESOURCES);
        client_list[thread].allocated = malloc(sizeof(int) * banker.NUMBER_OF_RESOURCES);
        client_list[thread].needs = malloc(sizeof(int) * banker.NUMBER_OF_RESOURCES);
        client_list[thread].num_ciclos_req = rand() % MAX_SLEEP + 1;
        client_list[thread].id = thread;
        debugMedium("\n**** Thread Number %d****\n", thread);
        debugMedium("num_ciclos_req => %d \n", client_list[thread].num_ciclos_req);

        for (int res = 0; res < banker.NUMBER_OF_RESOURCES; res++)
        {
            int value;
            value = scanf("%d", &value);
            client_list[thread].valor_max[res] = value;
            client_list[thread].allocated[res] = 0;
            client_list[thread].needs[res] = rand() % value + 1;
            debugHigh("Value valor_max[%d] => %d | needs[%d] => %d \n", res, client_list[thread].valor_max[res], res, client_list[thread].needs[res]);
        }
        debugHigh("\nEnd intialization of Thread %d\n", thread);
    }

    // Starting thread
    pthread_t tid[banker.num_threads];

    for (int thread = 0; thread < banker.num_threads; thread++)
    {
        pthread_create(&tid[thread], NULL, runner, (void *)thread);
    }

    for (int thread = 0; thread < banker.num_threads; thread++)
    {
        pthread_join(tid[thread], NULL);
    }

    return 0;
}

int askForResource(int clientId)
{
    // DONE(OUTSIDE FUNCTION): Add mutex handler
    int status = bankerAlgorithm(); // TODO: Get this status from the banker function
    return status;
}

void *runner(void *vargp)
{
    int arg = (int)vargp;
    Cliente selfClient = client_list[arg];
    debugHigh("thread id: %d - client-id: %d\n", selfClient.id, arg);
    // Dome por um periodo de tempo
    sleep(selfClient.num_ciclos_req);
    debugHigh("Thread %d woke up\n", selfClient.id);
    //Essa área do código acessa uma váriavel global que outras thrads também vão acessar, tornando-a uma seção critica
    //Para isso é aplicado um multex aqui, para evitar condição de corrida

    //=============//START SEÇÃO CRITICA//==============
    pthread_mutex_lock(&mutex); // Thread da lock no mutex para alterar as variaveis
    // Ask for resource
    debugHigh("thread %d Asked resource\n", selfClient.id);
    int status = askForResource(selfClient.id);
    debugHigh("thread %d banker status %d\n", selfClient.id, status);
    pthread_mutex_unlock(&mutex); // Thread libera o mutex para a proxima thread
                                  //=============//END SEÇÃO CRITICA//==============

    switch (status)
    {
    case DEADLOCK:
        debugMedium("Deadlock at thread %d\n", selfClient.id);
        break;
    case NO_RESOURCES:
        debugMedium("No resources available at thread %d\n", selfClient.id);
        break;
    case RESOURCE_ALLOCATED:
        debugMedium("Resource alocated with sucess at thread %d\n", selfClient.id);
        break;
    }

    pthread_exit(NULL);
}

int bankerAlgorithm()
{
    bool *finish;

    finish = malloc(sizeof(bool) * banker.num_threads);

    debugHigh("Called banker with num threads %d\n", banker.num_threads);
    for (int thread = 0; thread < banker.num_threads; thread++)
    {
        finish[thread] = true;
    }

    // Verificação se o estado do sistema está safe ou não
    for (int thread = 0; thread < banker.num_threads; thread++)
    {
        for (int res = 0; res < banker.NUMBER_OF_RESOURCES; res++)
        {
            if (client_list[thread].allocated[res] <= client_list[thread].needs[res])
            {
                if (client_list[thread].allocated[res] <= banker.live_values)
                {
                    banker.live_values -= client_list[thread].allocated[res];
                    client_list[thread].allocated[res] += client_list[thread].needs[res];
                    client_list[thread].needs[res] -= client_list[thread].allocated[res];
                }
                else
                {
                    finish[thread] = false;
                }
            }
            else
            {
                printf("Error: Request exceeded maximum claim.");
                return NO_RESOURCES;
            }
        }

        if (finish[thread] == false)
        {
            printf("The system is in an unsafe state.");
            return DEADLOCK;
        }
        else
        {
            printf("The system is in a safe state.");
            return RESOURCE_ALLOCATED;
        }
    }
    free(finish);
}

void debugHigh(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    if (DEBUG_LEVEL >= HIGH)
        vfprintf(stdout, format, arg);
    va_end(arg);
}

void debugMedium(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    if (DEBUG_LEVEL >= MEDIUM)
        vfprintf(stdout, format, arg);
    va_end(arg);
}

void debugLow(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    if (DEBUG_LEVEL >= LOW)
        vfprintf(stdout, format, arg);
    va_end(arg);
}
