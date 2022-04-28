#include <stdio.h>
#include <stdlib.h>


void makeSnake(game* currentGame); //Restituisce un puntatore a struct snake

void printSnake(game* currentGame); //Stampa il serpente a schermo

int moveSnake(game* currentGame); //Muove il serpente in base alla direzione inserita dall'utente.

void stretchSnake(game* currentGame); //Allunga il serpente di 1

int moveTrue(struct body* body, char input); //Controlla se la mossa è valida.

int checkDeath(game currentGame); //Controlla se il serpente si morde la coda.


