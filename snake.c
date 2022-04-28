#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include "gameStructs.h"
#include "snake.h"
#include "game.h"
#include "highscores.h"

void makeSnake (game* currentGame) //Crea un serpente di 3 unità e lo collega alla struct game
{
	int y, x;
	getmaxyx(stdscr, y, x);
	serpente newSnake =(serpente) malloc(sizeof(struct snake)); //Alloco la struct serpente
	if (newSnake == NULL) {erase(); printw("Errore. Memoria insufficiente per poter inizializzare il serpente."); cbreak(); int snakeErr = getch(); endwin();}
	newSnake->head = (struct body*) malloc(sizeof(struct body)); //Alloco la testa del serpente
	if (newSnake->head == NULL) {erase(); printw("Errore. Memoria insufficiente per poter inizializzare il serpente."); cbreak(); int snakeErr2 = getch(); endwin();}
	newSnake->head->bodypart = '#'; //Carattere della testa
	newSnake->head->posX = 5; //Posizione X di spawn della testa
	newSnake->head->posY = y/2; //Posizione Y di spawn della testa
	newSnake->head->direction = 'd'; //Direzione di default della testa
	if((*currentGame)->snake!=NULL)
	{
		free((*currentGame)->snake);
		(*currentGame)->snake=NULL;
	}
	(*currentGame)->snake = newSnake; //Lego il serpente creato alla struct del gioco
	struct body* p = (*currentGame)->snake->head; //Variabile ausiliaria per scorrere
	for (int i=0; i<3; i++) //Creo tutto il corpo del serpente
	{
		p->next = (struct body*) malloc(sizeof(struct body)); //Creo il prossimo pezzo di corpo del serpente
		if ((*currentGame)->snake->head->next == NULL) {erase(); printw("Errore. Memoria insufficiente per poter inizializzare il serpente."); cbreak(); int snakeErr3 = getch(); endwin();}
		p->next->bodypart = '#';
		p->next->posX = (p->posX) - 1 ;
		p->next->posY = y/2;
		p->next->direction = 'd';
		p = p->next;
		if(i==2)p->next=NULL;//Se sono arrivato alla fine del ciclo imposto il puntatore al prossimo nodo a NULL
	}
	
}

void printSnake(game* currentGame) //Stampo il serpente
{
	attron(COLOR_PAIR(2));
	struct body* p=(*currentGame)->snake->head; //Puntatore ausiliario
	while (p != NULL) //Scorro fino alla fine della lista
	{
		wmove(stdscr, p->posY, p->posX); //Muovo il puntatore
		printw("%c", p->bodypart); //Stampo il corpo del serpente
		p = p->next; //Passo al pezzo di corpo successivo
	}
	attroff(COLOR_PAIR(2));
}

int moveSnake(game* currentGame) //Muove il serpente
{
	cbreak();
	noecho();
	//init_pair(2, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(2));
	timeout((*currentGame)->difficulty); //Attendo in base alla difficoltà
	char input = getch();
	char oldInput;
	struct body* p = (*currentGame)->snake->head; //Puntatore ausiliario
	if (moveTrue(p, input)==0) {input = p->direction;} //Se il tasto non viene premuto
	if(moveTrue(p, input)==2) //Se viene premuto il tasto pausa
	{
		if (pauseGame(currentGame) == 2) {return 2;}; //Se si sceglie di uscire dal gioco, la funzione esce
		input = p->direction;	
	}
	while (p != NULL) 
	{ 	
			oldInput  = p->direction; //Immagazino la vecchia direzione del corpo
			p->direction = input; //Muovo lungo la nuova direzione
			mvaddch(p->posY, p->posX, ' '); //Cancello la parte di corpo per spostarla
			switch(input) 
			{
				case 'a': p->posX -= 1;
					break;
				case 'd': p->posX +=1;
					break;
				case 's': p->posY +=1;
					break;
				case 'w': p->posY -=1;
					break;
			}
			wmove(stdscr, p->posY, p->posX); //Muovo il puntatore
			printw("%c", p->bodypart); //Stampo il corpo del serpente
			p = p->next;
			input = oldInput;
		
	}
	attroff(COLOR_PAIR(2));
	return 0;
	
}

int moveTrue(struct body* body, char input) 
{
	switch(input) 
	{
		case 'd': if (body->direction == 'a') return 0;
				else return 1;
		case 'a': if(body->direction == 'd') return 0; 
				else return 1;
		case 's':  if(body->direction == 'w') return 0; 
				else return 1;
		case 'w': if(body->direction == 's') return 0; 
				else return 1;
		case 'p': return 2;
		
		default: return 0;
	}
}

void stretchSnake(game* currentGame) 
{
	struct body* newBody = (struct body*) malloc(sizeof(struct body));
	if (newBody == NULL) {erase(); printw("Memoria insufficiente per continuare il gioco."); endwin();}
	newBody->bodypart = '#';
	struct body* p =(*currentGame)->snake->head;
	while(p->next != NULL)
	{
		p = p->next;
	}
	newBody->direction = p->direction;
	p->next = newBody;
	newBody->next = NULL;
	switch(p->direction) 
		{
			case 'a': newBody->posX = (p->posX)+1;
				newBody->posY = (p->posY);
				break;
			case 'd': newBody->posX =(p->posX)-1;
				newBody->posY = (p->posY);
				break;
			case 's': newBody->posY = (p->posY)-1;
				newBody->posX = (p->posX);
				break;
			case 'w': newBody->posY =(p->posY)+1;
				newBody->posX = (p->posX);
				break;
		}
	
}

int checkDeath(game currentGame) 
{
	int x, y, deathTrue = 0;
	getmaxyx(stdscr, y, x);
	struct body* h = currentGame->snake->head;
	struct body* p = h->next; //Parto dal pezzo di corpo dopo la testa;
	while (p != NULL) 
	{
		if(p->posX == h->posX && p->posY == h->posY) //Controllo prima se il serpente si morde la coda 
		{
			deathTrue = 1;
			break;
		}
		p = p->next; //Controllo per ogni pezzo della coda
	}
	
	if(h->posY == 1 || h->posY == (y-1) || h->posX == 0 || h->posX == (x-1))
	{
		deathTrue = 1;
	}
	if(deathTrue == 1) //Se il serpente si è morso la coda
	{
		erase();
		saveScore(&currentGame);
		freeSnake(&currentGame);
		gameOverScreen(); //Stampo la schermata di game over
		return 1;
	}
	return 0;
}
void freeSnake(game* currentGame)
{
	struct body* freeS = (*currentGame)->snake->head; //Puntatore che scorre tutte le body del serpente
	struct body* p = freeS->next; //Puntatore all'elemento successivo
	while(p != NULL)
	{
		free(freeS);
		freeS = p;
		p = p->next;
	}
	free(freeS);
	(*currentGame)->snake->head=NULL;
}
void gameOverScreen()
{
	nodelay(stdscr, FALSE);
	cbreak();
	noecho();
	int y, x;
	getmaxyx(stdscr, y,x);
	y=y/20;
	x=x/10;
	move(y++, x);
	printw(":'######::::::'###::::'##::::'##:'########:\n");
	move(y++,x);
	printw("'##... ##::::'## ##::: ###::'###: ##.....::\n");
	move(y++,x);
	printw(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
	move(y++,x);
	printw(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
	move(y++,x);
	printw(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
	move(y++,x);
	printw(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
	move(y++,x);
	printw(". ######::: ##:::: ##: ##:::: ##: ########:\n");
	move(y++,x);
	printw(":......::::..:::::..::..:::::..::........::\n");
	move(y++,x);
	printw(":'#######::'##::::'##:'########:'########::'####:\n");
	move(y++,x);
	printw("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
	move(y++,x);
	printw(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
	move(y++,x);
	printw(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
	move(y++,x);
	printw(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
	move(y++,x);
	printw(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
	move(y++,x);
	printw(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
	move(y++,x);
	printw(":.......::::::...:::::........::..:::::..::....::\n");
	move(y+3,x);
	printw("Premi un tasto per tornare al menu'...");
	int ch = getch();
}
