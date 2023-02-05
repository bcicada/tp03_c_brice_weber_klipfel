/**
 * \file tabidium.c
 * \brief Processus & threads.
 * \author Brice W. K. <b.cicada@protonmail.com>
 * \version 0.1
 * \date 05 fev. 2023
 *
 * Processus & threads in c file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE (int)1e8
#define NUM_THREADS 4 
// En augmentant la taille de threads, on peut observer une réduction du temps de traitement. 
// Néanmoins, ici, passé à 8 threads semble être contre-productif, les temps sont rallongés.
// Par empirisme, le nombre de threads optimale semble être 4.

int tab[SIZE];
int min_global, max_global;
pthread_mutex_t lock;

struct ThreadData {
    int start;
    int end;
};

void *findMinMax(void *threadarg) {
    struct ThreadData *my_data = (struct ThreadData *)threadarg;
    int start = my_data->start;
    int end = my_data->end;

    int min = tab[start];
    int max = tab[start];

    for (int i = start + 1; i < end; i++) {
        if (tab[i] < min) {
        min = tab[i];
        }
        
        if (tab[i] > max) {
        max = tab[i];
        }
    }

    pthread_mutex_lock(&lock);
    if (min < min_global) {
        min_global = min;
    }
    
    if (max > max_global) {
        max_global = max;
    }

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main() {
    // Initialiser le tableau avec des valeurs aléatoires
    srand(time(NULL));
    
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand();
    }

    // Initialiser le verrou
    pthread_mutex_init(&lock, NULL);

    // Calculer le temps de recherche du min et du max
    struct timeval start, end;
    gettimeofday(&start, NULL);

    min_global = tab[0];
    max_global = tab[0];
    pthread_t threads[NUM_THREADS];
    struct ThreadData threadData[NUM_THREADS];
    int chunk_size = SIZE / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].start = i * chunk_size;
        threadData[i].end = (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, findMinMax, (void *)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    // Afficher le résultat
    printf("Minimum : %d\n", min_global); 
    printf("Maximum : %d\n", max_global);
    printf("Time taken : %f sec\n", time_taken);
    
    pthread_mutex_destroy(&lock);
    
    return 0;
}