#include <stdio.h>
#include <stdlib.h>

void saveScore(game* currentGame); //Salva il punteggio

void displayHighScores(game currentGame); //Stampa a schermo i 3 punteggi più alti complessivi e il miglior punteggio dell'utente.

struct scores* findHighscore(utente currentUser); //Trova lo score più alto dell'utente.

int countUsers(game currentGame); //Ritorna il numero degli utenti registrati oer la creazione dell'array degli score

void scoreSort(struct scores* punteggi[],int min, int max); //Un quicksort delle struct dei punteggi

int scorePartition(struct scores* punteggi[], int min, int max); //Partizione per il quicksort
