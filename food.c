#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "gameStructs.h"
#include "food.h"
#include "snake.h"

int checkFoodEaten(game* currentGame) 
{
	if ((*currentGame)->snake->head->posX == (*currentGame)->food->foodX && (*currentGame)->snake->head->posY == (*currentGame)->food->foodY) //Se il serpente va sul cibo
	{
		(*currentGame)->score += (*currentGame)->food->value; //Aggiorno il punteggio
		stretchSnake(currentGame); //Allungo il serpente
		spawnFood(currentGame); //Spawno il cibo
		return 0;
	}
	return 1;
}

void spawnFood(game* currentGame) 
{
		int x,y;
		getmaxyx(stdscr, y, x); //Misuro la grandezza dello schermo
		if((*currentGame)->food!=NULL)
		{
			free((*currentGame)->food); //Libero la memoria del precedente pezzo di cibo
		}
		struct food* newFood = (struct food*) malloc(sizeof(struct food)); //Alloco il nuovo pezzo di cibo
		if(newFood == NULL) {erase(); printw("Errore durante lo spawn del cibo. Memoria insufficiente."); endwin();}
		while(1)
		{
			newFood->foodX = (rand() % (x-3)) +1; //Spawno entro i margini dello schermo
			newFood->foodY = (rand() % (y-3)) +2;
			if(checkPos(*currentGame, newFood)){break;} //Controllo se il cibo viene posizionato in una posizione valida
		}
		if ((rand() % 10) > 7) //Cibo speciale dorato
		{
			newFood->value = 200;
		}
		else //Cibo non speciale
		{
			newFood->value = 100;
		}
		newFood->food = '@';
		
		if (newFood->value == 100) 
		{
			//init_pair(3, COLOR_RED, COLOR_BLACK); //Imposto le proprietà del cibo normale
			attron(COLOR_PAIR(3));
			mvaddch(newFood->foodY, newFood->foodX, newFood->food);
			attroff(COLOR_PAIR(3));
		}
		else 
		{
			//init_pair(4, COLOR_YELLOW, COLOR_BLACK); //Oppure imposto le proprietà di quello dorato
			attron(COLOR_PAIR(4));
			mvaddch(newFood->foodY, newFood->foodX, newFood->food);
			attroff(COLOR_PAIR(4));
		}
		(*currentGame)->food = newFood;
}

int checkPos(game currentGame, struct food* food) //Controllo la posizione del cibo 
{
	int valid=1;
	struct body*p;
	p=currentGame->snake->head;
	while(p!=NULL)
	{
		if((p->posX==food->foodX) && (p->posY==food->foodY)) //Se il cibo viene posizionato in un posto occupato dal serpente, il piazzamento non è valido
		{
			valid=0;
		}
		p=p->next;
	}
	return valid;
}
