#include <stdio.h>
#include "gameStructs.h"
#include "highscores.h"
#include <time.h>
#include <string.h>
#include "curses.h"

void saveScore(game* currentGame) 
{
	struct scores* saveScore = (struct scores*) malloc(sizeof(struct scores)); //Alloco la struct che salva il punteggio
	if (saveScore == NULL) {erase(); printw("Errore durante il salvataggio del punteggio."); int err=getch(); endwin();} //Caso in cui la memoria non viene allocata
	saveScore->score = (*currentGame)->score;
	strcpy(saveScore->name,(*currentGame)->logged->nickname);
	time_t t = time(NULL); //Salvo data del punteggio
	struct tm tm = *localtime(&t);
	saveScore->year = tm.tm_year+1900;
	saveScore->month = tm.tm_mon +1;
	saveScore->day = tm.tm_mday;
	if((*currentGame)->logged->userScores == NULL) //Se la lista degli score è vuota, inserisco e faccio puntare il prossimo elemento a NULL
	{
		(*currentGame)->logged->userScores = saveScore;
		saveScore->prev = NULL;
	}
	else 
	{
		saveScore->prev = (*currentGame)->logged->userScores; //Se la lista non è vuota, inserisco in testa e faccio puntare al precedente primo elemento della lista (l'ultimo inserito)
		(*currentGame)->logged->userScores = saveScore;
	}
	(*currentGame)->score = 0; //Resetto lo score della struct
}

void displayHighScores(game currentGame) //Stampa a schermo i 3 punteggi più alti e il punteggio dell'utente
{
	int i=0; //Contatore per il while loop
	int users = countUsers(currentGame); //Conta quanti utenti hanno effettuato almeno una partita
	utente user=currentGame->listHead; //Contatore per scorrere la lista di utenti
	struct scores* punteggi[users]; //Creo un'array di puntatori da ordinare per i punteggi degli utenti, grande quanto il numero di utenti che hanno giocato almeno una partita
	while(i<users && (user!=NULL)) //Scorro per il numero di utenti
	{
		if(user->userScores != NULL) 
		{
			punteggi[i] = findHighscore(user); //Riempio l'array di puntatori ai punteggi, utente per utente
			i++;
		}
		user=user->nextUser; //Passo al prossimo utente
	}
	scoreSort(punteggi, 0, (users-1)); //Metto in ordine gli score partendo dall'ultimo
	erase();
	int y,x,j, k=0;
	getmaxyx(stdscr, y, x);
	for(j=users-1; j>(users-3) && j>=0 ; j--) //Stampo i 3 punteggi più alti cioè i primi 3 punteggi dell'array
	{
		wmove(stdscr, (y/5)+k, (x/5));
		printw("%d: %s....................%d %d/%d/%d", k+1, (punteggi[j])->name, (punteggi[j])->score, (punteggi[j])->day, (punteggi[j])->month, (punteggi[j])->year); 
		k++;
	}
	wmove(stdscr,(y/5)+k+1, (x/5));
	int m=0;
	if(currentGame->logged->userScores != NULL) //Se l'utente ha effettuato almeno una partita, lo informo sulla posizione in classifica del suo punteggio migliore
	{
		while(strcmp(currentGame->logged->nickname, (punteggi[m])->name) != 0 && m<=users)
		{
			m++;
		}
		printw("Il tuo miglior punteggio: %d      Posizione:%d", (punteggi[m])->score, (users)-m);
	}
	printw("\n\t premi un tasto qualunque per tornare al menù...");
	int ch= getch();
	erase();
}

struct scores* findHighscore(utente currentUser) //Trova il punteggio più alto per ogni utente
{
	int highscore=0;
	struct scores* findScore = currentUser->userScores; //Puntatore che scorre la lista dei punteggi
	struct scores* scoreFound = currentUser->userScores; //Puntatore che punta al punteggio più alto trovato
	while(findScore != NULL) //Scorro tutti gli score dell'utente 
	{
		if(findScore->score > highscore) //Se lo score trovato è più alto di quello precedente
		{
			highscore = findScore->score; //Imposto il nuovo score come highscore
			scoreFound = findScore;
		}
		findScore=findScore->prev;//Passo al prossimo punteggio
	}
	return scoreFound; //Ritorno il puntatore alla struct col punteggio più alto
}

int countUsers(game currentGame) //Conta il numero di utenti
{
	int count=0; //Variabile da ritornare
	utente counter=currentGame->listHead; //Inizio dalla testa della lista
	while(counter != NULL) //Scorro fino alla fine
	{
		if(counter->userScores != NULL)
			count++; //Incremento il conteggio solo se l'utente ha completato almeno una partita
		counter = counter->nextUser; //Passo al prossimo utente
	}
	return count; //Ritorno il conteggio di utenti
}

void scoreSort(struct scores* punteggi[],int min, int max) //Fa un quicksort dei puntatori, in modo tale che il puntatore all'indice 0 dell'array punti allo score più alto in assoluto
							     //Il puntatore [1] punti al secondo punteggio e così via in ordine decrescente.
{
	if(min<max) 
	{
		int pi = scorePartition(punteggi, min, max);
		scoreSort(punteggi, min, pi-1);
		scoreSort(punteggi, pi+1, max);
	}
	
}

int scorePartition(struct scores** punteggi, int min, int max) //Partizioni per il quicksort.
{
	int i,j,pivot; //Questo quicksort non fa scorrere i e j in direzioni diverse come visto a lezione ma li fa scorrere nella stessa direzione,
			// eliminando così qualche controllo che era necessario su j. Il funzionamento è pressochè analogo
	struct scores* change;
	i=min-1;
	pivot=(punteggi[max])->score;
	for(j=min; j<max; j++) 
	{
		if((punteggi[j]->score)<pivot) 
		{
			i++;
			change=punteggi[j];
			punteggi[j]=punteggi[i];
			punteggi[i]=change;
		}
	}
	change=punteggi[i+1];
	punteggi[i+1] = punteggi[max];
	punteggi[max]=change;
	return (i+1);
}
