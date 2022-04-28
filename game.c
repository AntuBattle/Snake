#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gameStructs.h"
#include "snake.h"
#include "game.h"
#include "food.h"
#include "highscores.h"
#include "user.h"


void startScreen() //Stampa il disegnino di un serpente all'avvio del gioco
{
	//init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	int y,x;
	getmaxyx(stdscr,y,x);
	wmove(stdscr, (y/4), (x/3));
	printw("    _________          _________ 			");
	wmove(stdscr, (y/4)+1, (x/3));
	printw("   /         \\        /         \\ 			");
	wmove(stdscr, (y/4)+2, (x/3));
	printw("  /  /~~~~~\\  \\      /  /~~~~~\\  \\ 			");
	wmove(stdscr, (y/4)+3, (x/3));
	printw("  |  |     |  |     |  |       |  | 			");
	wmove(stdscr, (y/4)+4, (x/3));
	printw("  |  |     |  |     |  |       |  | 			");
	wmove(stdscr, (y/4)+5, (x/3));
	printw("  |  |     |  |     |  |       |  |         /	");
	wmove(stdscr, (y/4)+6, (x/3));
	printw("  |  |     |  |     |  |       |  |       //	");
	wmove(stdscr, (y/4)+7, (x/3));
	printw(" (o  o)    \\  \\_____/  /       \\  \\_____/ / 	");
	wmove(stdscr, (y/4)+8, (x/3));
	printw("  \\__/      \\         /         \\        / 	");
	wmove(stdscr, (y/4)+9, (x/3));
	printw("    |        ~~~~~~~~~            ~~~~~~~~ 		");
	wmove(stdscr, (y/4)+10, (x/3));
	printw("    ^											");
	wmove(stdscr, (y/4)+11, (x/3));
	printw("		Benvenuto su Snake!			");
	wmove(stdscr, (y/4)+12, (x/3));
	printw("			    Premi un tasto qualsiasi per continuare...	");
	
	char ch= getch();
	attroff(COLOR_PAIR(1));
	erase();
}

void printBorders() 
{
	//init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	int x, y;
	getmaxyx(stdscr, y, x);
	int i,j;
	int borderY = '|';
	int borderX = '_';
	for (i=1; i<y; i++) 
	{
		if (i==1) 
		{
			wmove(stdscr, i, 0);
			for (j=0; j<x; j++) 
			{
				printw("%c", borderX);
			}
		}
		else if (i == y-1) 
		{
			wmove(stdscr, i, 0);
			for (j=0; j<x; j++) 
			{
				printw("^");
			}
		}
		else 
		{
			wmove(stdscr, i, 0);
			printw("%c", borderY);
			wmove(stdscr, i, x-1);
			printw("%c", borderY);
		}
	}
	attroff(COLOR_PAIR(1));
}

game startGame() //Crea la struct del gioco
{
	game newGame;
	newGame = (game) malloc(sizeof(struct game)); //Alloco nuova struttura della partita
	if (newGame == NULL) {erase(); printw("Errore. Impossibile inizializzare la partita."); int err = getch(); endwin();} //Se vi è un errore, si chiude il gioco
	newGame->logged = NULL; //Nessuno è loggato
	newGame->listHead = NULL; //Non ci sono altri utenti appena si avvia il gioco perchè deve essere caricato un file
	newGame->snake = NULL;
	newGame->food = NULL;
	newGame->score=0;
	newGame->difficulty=0;
	return newGame;
}

void beginGame(game* currentGame) //All'interno di questa funzione si svolgerà tutto il gioco vero e proprio
{
	erase();
	cbreak();
	noecho();
	int x,y, gameChoice, inpdifficulty; //x e y per la grandezza dello schermo, gameChoice per la scelta che l'utente fa (nuova partita, riprendi, highscores, esci), inpdifficulty per la difficoltà.
	getmaxyx(stdscr, y, x);
	while (1) 
	{
		//init_pair(1, COLOR_WHITE, COLOR_BLACK);
		attron(COLOR_PAIR(1));	
		wmove(stdscr, y/4, x/3);
		printw("Bentornato %s, per favore scegli una delle opzioni premendo il tasto corrispondente", (*currentGame)->logged->nickname);
		wmove(stdscr, (y/4)+2, x/3);
		printw("1. Nuova Partita");
		wmove(stdscr, (y/4)+4, x/3);
		printw("2. Riprendi partita");
		wmove(stdscr, (y/4)+6, x/3);
		printw("3. Visuallizza highscores");
		wmove(stdscr, (y/4)+8, x/3);
		printw("4. Esci");
		gameChoice = getch();
		erase();
		refresh();
		if (gameChoice == '1')//Se l'utente sceglie nuova partita 
		{
			wmove(stdscr, y/4, x/5);
			printw("Per favore, inserisci la difficoltà a cui desideri giocare (1-5)");
			while (2) //Anche questo ciclo serve a ignorare qualunque input che non sia 1, 2, 3, 4 o 5.
			{
				inpdifficulty = getch();
				switch (inpdifficulty)//La difficoltà saranno i decimi di secondo che ci saranno tra un refresh e l'altro dello schermo 
				{
					case '1': (*currentGame)->difficulty = 400; break;
					case '2': (*currentGame)->difficulty = 300; break;
					case '3': (*currentGame)->difficulty = 200; break;
					case '4': (*currentGame)->difficulty = 125; break;
					case '5': (*currentGame)->difficulty = 75; break;
					default: break;
				}
				if (inpdifficulty == '1' || inpdifficulty == '2' || inpdifficulty == '3' || inpdifficulty == '4' || inpdifficulty == '5') {erase(); break;}
			}
			//Fine selezione della difficoltà
			printBorders();
			makeSnake(currentGame);
			printSnake(currentGame);
			spawnFood(currentGame);
			while (moveSnake(currentGame) != 2) //Il serpente si muove fino a quando non si esce dal gioco. Da inserire le condizioni di controllo di morte del serpente.
			{
				checkFoodEaten(currentGame);
				updateScore(*currentGame);
				if(checkDeath(*currentGame) == 1) 
				{
					break;
				}
			}
			erase();
		}
		if (gameChoice == '2') //Se l'utente sceglie riprendi partita 
		{
			if(resume(currentGame) == 1) //Se ci sono partite da riprendere 
			{	
				erase();
				printBorders();
				printSnake(currentGame);
				if ((*currentGame)->food->value == 100) 
				{
					//init_pair(3, COLOR_RED, COLOR_BLACK);
					attron(COLOR_PAIR(3));
					mvaddch((*currentGame)->food->foodY, (*currentGame)->food->foodX, (*currentGame)->food->food);
					attroff(COLOR_PAIR(3));
				}
				else 
				{
					//init_pair(4, COLOR_YELLOW, COLOR_BLACK);
					attron(COLOR_PAIR(4));
					mvaddch((*currentGame)->food->foodY, (*currentGame)->food->foodX, (*currentGame)->food->food);
					attroff(COLOR_PAIR(4));
				}
				while (moveSnake(currentGame) != 2) //Il serpente si muove fino a quando non si esce dal gioco. Da inserire le condizioni di controllo di morte del serpente.
				{
					checkFoodEaten(currentGame);
					updateScore(*currentGame);
					if(checkDeath(*currentGame) == 1) 
					{
						break;
					}
				}
				erase();
			} 
		}
		if (gameChoice == '3') 
		{
			displayHighScores(*currentGame);
		}
		if (gameChoice == '4') //Se l'utente sceglie esci
			{
				//Va deallocata la memoria di tutte le struct una volta implementati i file, tranne la struct della partita che contiene la lista degli utenti.
				break;
			}
	
	}
}

void updateScore(game currentGame) //Aggiorna costantemente lo score a schermo
{
	wmove(stdscr, 0, 0);
	printw("Score: %d", currentGame->score);
}

int pauseGame(game* currentGame) //Mette in pausa il gioco
{
	erase();
	nodelay(stdscr, FALSE);
	cbreak(); //Queste sono tutte funzioni di curses che modificano il comportamento di getch() in modo che attenda o no il tasto invio per ricevere il carattere inserito
	noecho(); //Questa funzione di curses modifica il comportamento della getch() in modo che non stampi a schermo il carattere inserito
	int x,y; //Variabili per prendere la grandezza della finestra di gioco
	getmaxyx(stdscr, y, x); //Funzione di curses che misura la finestra
	wmove(stdscr, y/3, x/6); //Funzione di curses che muove il cursore
	attron(COLOR_PAIR(1));
	printw("Gioco in pausa. Premere un tasto qualunque per riprendere il gioco..."); //Funzione di curses che stampa testo nella finestra di gioco
	wmove(stdscr, (y/3)+3, x/3);
	printw("Premere E per uscire dal gioco");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(2));
	char choice = getch(); //Input dell'utente
	if(choice == 'e') //Se l'utente decide di uscire dal gioco
	{
		save(currentGame);
		return 2;
	}
	else //Altrimenti fa riprendere a giocare
	{
		erase();
		printBorders();
		printSnake(currentGame);
		if ((*currentGame)->food->value == 100) //Stampa il cibo
		{
			//init_pair(3, COLOR_RED, COLOR_BLACK); //Questi sono i colori del cibo se il cibo vale 100 punti, sotto l'else il colore è giallo se il cibo vale 200 punti.
			attron(COLOR_PAIR(3));
			mvaddch((*currentGame)->food->foodY, (*currentGame)->food->foodX, (*currentGame)->food->food);
			attroff(COLOR_PAIR(3));
		}
		else 
		{
			//init_pair(4, COLOR_YELLOW, COLOR_BLACK);
			attron(COLOR_PAIR(4));
			mvaddch((*currentGame)->food->foodY, (*currentGame)->food->foodX, (*currentGame)->food->food);
			attroff(COLOR_PAIR(4));
		}
		return 0;
	}
}

void save(game* currentGame) //Attualmente salva copiando le struct del gioco in struct running dell'utente. Da implementare con i file.
{
	struct running* p = (struct running*) malloc(sizeof(struct running)); //Alloco la memoria della struct che mi permette di salvare la partita in corso
	if(p==NULL) {erase(); printw("Spazio insufficiente per salvare la partita"); int saveErr = getch(); endwin();} //Verifico che l'allocazione sia andata a buon fine
	p->currentScore = (*currentGame)->score; //copio lo score attuale
	p->currentDifficulty = (*currentGame)->difficulty; //copio la difficoltà
	p->currentSnake = (*currentGame)->snake->head; //Faccio puntare alla testa della lista del serpente attuale così da non perderlo una volta sovrascritto il puntatore currentGame
	p->currentFood.foodX = (*currentGame)->food->foodX; //Salvo la posizione e il valore del cibo
	p->currentFood.foodY = (*currentGame)->food->foodY;
	p->currentFood.food = (*currentGame)->food->food;
	p->currentFood.value = (*currentGame)->food->value;
	time_t t = time(NULL); //Salvo data e ora della partita
	struct tm tm = *localtime(&t);
	p->year = tm.tm_year+1900;
	p->month = tm.tm_mon +1;
	p->day = tm.tm_mday;
	p->hour = tm.tm_hour;
	p->minutes = tm.tm_min;
	struct running* i = (*currentGame)->logged->running; //Puntatore per scorrere fino alla fine della lista delle partite salvate
	if(i == NULL) //Se la lista è vuota
	{
		(*currentGame)->logged->running = p; //La partita da salvare diventa la prima della lista
		p->prev = NULL;
		p->next = NULL;
	}
	else 
	{
		while(i->next != NULL) //Scorre fino alla fine della lista 
		{
			i = i->next;
		}
		p->prev = i; //L'ultima punta alla penultima
		i->next = p; //La penultima punta all'ultima
		p->next = NULL; //L'ultima non punta a niente
	}
	(*currentGame)->score = 0; //Resetto lo score per le prossime partite
	(*currentGame)->snake->head = NULL; //La testa del serpente ora può puntare a una nuova lista una volta iniziata una nuova partita
}

int resume(game* currentGame) 
{
	erase();
	int x, y, maxx, maxy, i=1;
	getmaxyx(stdscr, maxy, maxx);
	if((*currentGame)->logged->running == NULL) //Se la lista delle partite in corso è vuota, torno indietro
	{
		erase();
		wmove(stdscr, maxy/3, maxx/6);
		printw("Nessuna partita in corso da riprendere.");
		int ch=getch();
		erase();
		return 0;
	}
	wmove(stdscr, (maxy/5)-1, maxx/6);
	printw("Per favore, seleziona una partita da riprendere premendo E"); //Altrimenti scorro e stampo tutte le partite da riprendere
	struct running* p = (*currentGame)->logged->running; //Parto dalla prima
	while(p != NULL) 
	{
		wmove(stdscr, (maxy/5)+i, maxx/6);
		printw("  %d/%d/%d. %d:%d. Punteggio: %d", p->day, p->month, p->year, p->hour, p->minutes, p->currentScore); //Stampo la partita
		i++; //Indice che serve per muovere il cursore ogni volt più in basso
		p = p->next; //Vado alla prossima partita
	}
	p=(*currentGame)->logged->running; //---- Questa parte della funzione serve a stampare il menù delle partite e far scegliere all'utente la partita da riprendere
	int choice, count=1; //Prendo l'input dell'utente
	char selection = '>'; //Indicatore che a schermo fa vedere l'opzione selezionata
	mvaddch((maxy/5)+1, maxx/6, selection); //Metto a posto il cursore
	while(choice = getch()) 
	{
		if(choice == 'e') //Se l'utente seleziona
		{
			break;
		}
		if(choice == 'w') //Se l'utente sale la lista
		{
			if(count>1) 
			{
				getsyx(y,x); //Prende la posizione attuale del cursore
				move(y, x-1); //Lo sposta indietro di uno
				printw(" ");
				mvaddch((y)-1, (maxx/6)-1, selection);
				p=p->prev;
				count--;
			}
		}
		if(choice == 's') //Se l'utente scende la lista
		{
			if(p->next != NULL)
			{
				getsyx(y,x);
				move(y, x-1);
				printw(" ");
				mvaddch((y)+1, (maxx/6)-1, selection);
				p=p->next;
				count++;
			}
		}
	}
	if((*currentGame)->snake==NULL)
	{
		(*currentGame)->snake=(serpente)malloc(sizeof(struct snake));
		(*currentGame)->snake->tail=NULL;
		(*currentGame)->snake->head=NULL;
	}
	(*currentGame)->snake->head = p->currentSnake; //Faccio puntare il puntatore del serpente della partita generale (struct game) a quello della partita da riprendere (struct running)
	if((*currentGame)->food!=NULL) {free((*currentGame)->food); (*currentGame)->food=NULL;}
	(*currentGame)->food = (struct food*) malloc(sizeof(struct food)); //Alloco e ristabilisco la posizione del cibo
	(*currentGame)->food->foodX = p->currentFood.foodX;
	(*currentGame)->food->foodY = p->currentFood.foodY;
	(*currentGame)->food->food = p->currentFood.food;
	(*currentGame)->food->value = p->currentFood.value;
	(*currentGame)->score = p->currentScore;//Imposto lo score
	(*currentGame)->difficulty = p->currentDifficulty;//Imposto la difficoltà
	if(p->prev != NULL) //Se il struct running precedente esiste
	{
		(p->prev)->next = p->next; //Il running next del running precedente nella lista sarà il running next (Da A->B->C diventa A->C senza B)
		if(p->next != NULL) //Se il body next esiste
		{
			(p->next)->prev = p->prev; //Il precedente del running next sarà il running precedente (Da A<-B<-C diventa A<-C)
		}
	}
	else if(p->prev == NULL) 
	{
		(*currentGame)->logged->running = p->next; //Se viene scelta la prima della lista, faccio puntare alla seconda e libero la prima.
	}
	free(p); //Libero la struct running
	return 1;
}

int playSnake()
{
	initscr(); //Inizializza la finestra del gioco
	curs_set(0); //Rende il cursore invisibile
	if (has_colors() == TRUE) //Se il terminale supporta i colori
	{
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_GREEN, COLOR_BLACK);
		init_pair(3, COLOR_RED, COLOR_BLACK);
		init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	}
	struct stat st = {0}; //Struct di stat.h per controllare lo stato delle cartelle e dei file

	if (stat("savedata", &st) == -1) //Se la cartella non esiste
	{
    		mkdir("savedata", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //Creo la cartella che conterrà i salvataggi degli utenti.
	}
	startScreen(); //Stampa l'immaginetta del serpente a schermo che da il benvenuto
	game currentGame = startGame(); //Crea la struct principale
	loadUsers(&currentGame); //Carico la lista di utenti
	auth(&currentGame); //Permette all'utente di registrarsi o loggarsi
	loadRunning(currentGame->logged); //Carico le partite in corso
	loadScores(currentGame->listHead); //Carico i punteggi
	beginGame(&currentGame); //Fa iniziare il gioco
	saveUsers(currentGame); //Salvo gli utenti
	saveRunning(currentGame->logged); //Salvo le partite in corso
	saveScores(currentGame->logged); //Salvo i punteggi
	endwin(); //Termina la finestra entro la quale si svolge il gioco
	return 0;
}
