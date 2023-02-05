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
#include <sys/time.h>

#define SIZE (int)1e8
int tab[SIZE];

int main() {
    // Initialiser le tableau avec des valeurs aléatoires
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++) {
        tab[i] = rand();
    }

    // Calculer le temps de recherche du min et du max
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int min = tab[0];
    int max = tab[0];
    for (int i = 1; i < SIZE; i++) {
        if (tab[i] < min) {
        min = tab[i];
        }
        if (tab[i] > max) {
        max = tab[i];
        }
    }

    gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6;

    // Afficher le résultat
    printf("Minimum : %d\n", min);
    printf("Maximum : %d\n", max);
    printf("Temps de recherche : %f sec\n", time_taken);
    
    return 0;
}