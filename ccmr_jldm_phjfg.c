//  GRUPO:
//  Caio Cordeiro
//  Jean Mendonça
//  Pedro Henrique

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 10
#define NUM 5
#define DIAMETRO 2

int k=0;
float arr[NUM*NUM_THREADS][2];    
void addCord()
{
    srand(time(NULL));
    float limiter = (float)RAND_MAX/(float)DIAMETRO;
    // printf("Limiter => %f",limiter);
    for (int i=0; i < NUM; i++)
    {
        float x = (float)rand() / limiter;
        float y = (float)rand() / limiter;
        arr[k][0] = x;
        arr[k][1] = y;
        // printf("%d\n",k);
        printf("%f \t/ %f", arr[k][0], arr[k][1]);
        k+=1;
        // printf("%f/%f", x, y);
    }


}
void getPi()
{
    float res;
    float inCirc=0;
    for (int i=0; i<k;i++){
        float a=arr[i][0]*arr[i][0];
        float b = arr[i][1]*arr[i][1];
        // printf("%f/%f\n",a,b);
        if( ( a+ b) < 1 ){
            // printf("ENTROU\n");
            inCirc+=1;
        }
    }
    // printf("inCirc-> %f K-> %d\n",inCirc,k);
    res =4*(inCirc/(NUM*NUM_THREADS));
    printf("%f",res);
}
/* Cada threads agora controla sua função */
void *runner(void *param)
{
    addCord();

    pthread_exit(0);
}

int main()
{
    int scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t atributos;
    /* Pegando os aributos default*/
    pthread_attr_init(&atributos);

    if (pthread_attr_getscope(&atributos, &scope) != 0)
    {
        fprintf(stderr, "DEU ERRO");
    }
    else
    {
        /*Caso não deu erro , dizer qual o scope*/
        /*Tava dando erro e fizemos esses else/if pra debugar*/
        if (scope == PTHREAD_SCOPE_PROCESS)
        {
            printf("Processo\n");
        }
        else if (scope == PTHREAD_SCOPE_SYSTEM)
        {
            printf("System\n");
        }
        else
        {
            fprintf(stderr, "ERRO\n");
        }
        /* Setando o escopo */
        pthread_attr_setscope(&atributos, PTHREAD_SCOPE_SYSTEM);
        /* criar threads */
        for (int i = 0; i < NUM_THREADS; i++)
        {
            pthread_create(&tid[i], &atributos, runner, NULL);
        }

        /* Dar join em todas as threads */
        for (int i = 0; i < NUM_THREADS; i++)
        {
            printf("=");
            pthread_join(tid[i], NULL);
        }
    }
    getPi();
    return 0;
}