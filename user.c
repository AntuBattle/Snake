#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include "gameStructs.h"
#include "user.h"
#define KEY "k7"

void printAuthScreen()//Stampa la schermata di autenticazione
{
	int y,x;
	getmaxyx(stdscr,y,x);
	wmove(stdscr, y/4, x/3);
	printw("1. Registrati");
	wmove(stdscr, (y/4)+2, x/3);
	printw("2. Accedi");
}

int checkDoubles(game currentGame, utente user) 
{
	utente p = currentGame->listHead; //Puntatore per scorrere la lista di utenti
	while(p != NULL) 
	{
		if(strcmp(p->nickname, user->nickname)==0)
		{
			return 1;
		}
		p = p->nextUser;
	}
	return 0;
}

void auth(game* currentGame) //Questa funzione quando viene chiamata permette all'utente di registrarsi o loggarsi
{
	int y,x;
	getmaxyx(stdscr,y,x); //Misuro la grandezza dello schermo
	cbreak(); //Questa funzione fa sì che quando premo un carattere (per una getch()) il carattere venga preso immediatamente senza aspettare l'invio
	noecho(); //Questa funzione fa sì che la funzione getch() (che è praticamente la getchar() di ncurses), non stampi a schermo il carattere appena premuto
	int input;//Questa variabile conterrà la scelta dell'utente. (1 Registrati 2 Accedi)
	while (2) //Questo ciclo racchiude tutto il sistema di registrazione e login. Serve a fare si che qualunque input diverso da 1 o 2 venga completamente ignorato dal calcolatore. Invece di dover creare un else che implica un messaggio di errore, in questo modo l'input viene semplicemente ignorato.
	{	printAuthScreen(); //Stampa la schermata di login
		cbreak();
		noecho(); 
		input = getch(); //Prende l'input del giocatore
		erase(); //Cancella la schermata
		nocbreak(); //Reabilito il buffer
		if (input == 49) //Se l'utente sceglie "registrati"
		{
			utente newUser = (utente) malloc(sizeof(struct utente)); //Alloco un nuovo utente
			if (newUser == NULL) //Se non basta la memoria
			{
				erase(); printw("Errore. Impossibile registrare l'utente con successo. Si prega di riprovare."); endwin();
			}
			newUser->nextUser=NULL; //L'utente appena creato è l'ultimo utente della lista e quindi non punta a nulla
			newUser->running = NULL; //L'utente appena creato non ha partite pregresse in sospeso e quindi il puntatore a struct running sarà NULL
			newUser->userScores = NULL;	
			//----------- Metto il cursore circa al centro dello schermo 
			//---------
			int invalidName;
			do 
			{
				echo(); //Da adesso i caratteri inseriti dall'utente verranno stampati a schermo, così che vedrà l'username mentre lo digiterà in fase di registrazione o login
				wmove(stdscr, y/4, x/3); 
				printw("Inserisci un username, fino a 15 caratteri: ");
				getstr(newUser->nickname); //Metto il nome dell'utente nella variabile nickname dell'utente che ho creato poco fa (newUser)
				if(checkDoubles(*currentGame, newUser)==1)
				{
					erase();
					printw("Errore. Username gia' esistente. Si prega di inserire un altro username.");
					int existing=getch();
					cbreak(); noecho(); invalidName = 1; erase();
				}
				else invalidName = 0;
			}
			while(invalidName == 1);
			erase(); //Cancello le cose a schermo
			wmove(stdscr, y/4, x/3); //Sposto il cursore
			printw("Inserisci una password, fino a 15 caratteri: ");
			int passw = 0; //contatore per la password
			int c; // carattere inserito dall'utente
			cbreak(); //disabilito il buffering
			noecho(); //non stampo il carattere premuto sullo schermo
			while (passw<16) //Tutta questa cosa della password, sempre per pulizia del codice, potrei racchiuderla dentro un'altra funzione e chiamarla magari getpassword(). Alla fine è così lunga solo perchè invece di prendere la password inserita, ogni volta che l'utente preme un carattere, se il carattere è valido viene inserito un '*'. Cosa puramente estetica. Il mio dubbio grosso però è la portabilità: controllando da terminale, il mio 'backspace' è 127, ma non per tutti i calcolatori è così. Lo stesso vale per gli altri tasti. Temo che questo possa inficiare sulla portabilità del programma.  
			{
				c = getch(); //L'utente preme un tasto
				if (c == 10) //Se viene inserito enter come input
				{
					newUser->password[passw] = '\0';
					break;
				}
				else if ((c > 96 && c<123) || (c >64 && c<91) || (c<58 && c>47)) //Se è stata inserita una lettera o un numero
				{
					printw("*");
					newUser->password[passw] = c;
					passw++;
				}
				else if (c == 127) //Se viene inserito il carattere di backspace
				{	if(passw != 0) 
					{
						getyx(stdscr, y, x);
						mvaddch(y, x-1,' '); //Muove il cursore di uno indietro e cancella il carattere se presente
						wmove(stdscr, y, x-1);
						passw--;
					}
				}
			}
			strcpy(newUser->password, crypt(newUser->password, KEY)); //Codifico la password
		
			erase(); //Cancello le cose a schermo
			wmove(stdscr, y/4, x/3); //Sposto il cursore
			printw("Per favore, ripeti la password: ");
			char verify[17];
			int k, passv=0; //k carattere di input e passv contatore della verifica della password
			while (passv<16) //Esattamente lo stesso algoritmo di prima per l'inserimento della password. Solo con altre variabili
			{
				k = getch();
				if (k == 10) //Se viene inserito enter come input
				{
					verify[passv] = '\0';
					break;
				}
				else if ((k > 96 && k<123) || (k >64 && k<91) || (k<58 && k>47)) //Se è stata inserita una lettera o un numero
				{
					printw("*");
					verify[passv] = k;
					passv++;
				}
				else if (k == 127) //Se viene inserito il carattere di backspace
				{	
					if(passv != 0) 
					{
						getyx(stdscr, y, x);
						mvaddch(y, x-1,' ');
						wmove(stdscr, y, x-1);
						passv--;
					}
				}
			}
			erase(); 
			wmove(stdscr, y/4, x/3);
			if (strcmp(newUser->password, crypt(verify, KEY)) == 0) //Se la password e la verifica corrispondono
			{
				printw("Registrazione completata correttamente.");
				 if ((*currentGame)->listHead == NULL) //Se la lista degli utenti è vuota
				{
					(*currentGame)->listHead = newUser; //Il primo utente diventa questo
				}
				else 
				{
					utente p; //puntatore per trovare l'ultimo utente nella lista degli utenti;
					p = (*currentGame)->listHead;
					while (p->nextUser != NULL) //Scorre fino all'ultimo utente 
					{
						p = p->nextUser;
					}
					p->nextUser = newUser; //Fa puntare l'ultimo utente all'utente appena creato
				}
				(*currentGame)->logged = newUser; // L'utente loggato diventa questo appena creato
				 erase(); break; //Aspetto l'input ed esco dal ciclo
			}
			else {printw("Errore, le password non corrispondono. Si prega di ritentare la registrazione."); cbreak(); free(newUser); erase();} //Se c'è un errore nel login, torno all'inizio. E libero la memoria allocata.	 
		}



		else if (input == 50) //Se l'utente sceglie login, cioè preme '2' 
		{
			char inputUsername[17]; //Username inserito dall'utente
			char inputPassword[17]; //Password inserita dall'utente
			utente userList; //Puntatore che scorre la lista di utenti e vede se l'username esiste
		
			while(1) //Questo ciclo serve a far tornare l'utente ad inserire l'username nel caso sbagli la password. Evita di dover rieseguire il programma se si sbaglia a digitare qualcosa
			{
				nocbreak(); //Aspetta l'invio
				echo(); //Fa eco del carattere sullo schermo
				erase();
				wmove(stdscr, y/4, x/3); //Sposto il cursore
				userList = (*currentGame)->listHead; //Faccio cominciare il puntatore dalla testa della lista di utenti
				printw("Per favore, inserisci l'username del tuo account: ");
				getstr(inputUsername); //Prende l'username dell'utente e lo mette in inputUsername
				while(userList != NULL) //Scorre fino alla fine della lista
				{
					if (strcmp(userList->nickname, inputUsername) == 0) //Se l'username esiste e corrisponde
					{	
						break; //esco dal loop
					}
					userList = userList->nextUser; //Altrimenti continuo a scorrere fino alla fine
				}
				if (userList == NULL) //Se uscito dal loop sono arrivato alla fine
				{
					cbreak();
					noecho();
					erase();
					wmove(stdscr, y/4, x/3);
					printw("Il nome utente non corrisponde a nessun utente registrato. Si prega di riprovare.");
					char ch=getch(); //Aspetto e ricomincio da capo
				}
				else if (strcmp(userList->nickname, inputUsername) == 0) //Se l'username esiste e corrisponde
				{	
					break; //esco dal loop
				}
			}
			while (1) //Ciclo che serve a farmi inserire la password carattere per carattere. Ad ogni carattere il ciclo viene ripetuto
			{
				erase();
				wmove(stdscr, y/4, x/3);
				printw("Utente trovato con successo. Per favore, inserisci la password: ");
				cbreak();
				noecho();
				int g, lpassword=0; //g carattere di input e password contatore della verifica della password
				while (lpassword<16) //Esattamente lo stesso algoritmo per la registrazione, solo con altre variabili.
				{
					g = getch();
					if (g == 10) //Se viene inserito enter come input
					{
						inputPassword[lpassword] = '\0';
						break; //Esco dal ciclo che mi fa impostare la password
					}
					else if ((g > 96 && g<123) || (g >64 && g<91) || (g<58 && g>47)) //Se è stata inserita una lettera o un numero
					{
						printw("*");
						inputPassword[lpassword] = g;
						lpassword++;
					}
					else if (g == 127) //Se viene inserito il carattere di backspace
					{	if(lpassword != 0) 
						{
							getyx(stdscr, y, x);
							mvaddch(y, x-1,' ');
							wmove(stdscr, y, x-1);
							lpassword--;
						}
					}
				}
				if (strcmp(crypt(inputPassword, KEY), userList->password) == 0) //Se la password inserita è corretta
				{
					break; //Esco dal ciclo while(1)
				}
				erase();
				wmove(stdscr, y/4, x/3);
				printw("Errore. Username o password errati. Si prega di riprovare o di rieffettuare il login");
				int lgerr=getch();
				erase();
			}
			erase();
			wmove(stdscr, y/4, x/3);
			printw("Login avvenuto con successo");
			noecho();
			cbreak();
			int ls=getch();
			(*currentGame)->logged = userList; //L'utente si logga con successo
			break; //Esco dal ciclo while(2) che racchiude tutto il login
		}
	}
}

void saveUsers(game currentGame) //Funzione che salva la lista di utenti nel file
{
	FILE *fp; //Puntatore a file
	fp = fopen("savedata/user_list.txt", "w"); //Apro file in scrittura
	utente p = currentGame->listHead; //Puntatore che inizia dalla testa della lista
	while(p != NULL) 
	{
		fprintf(fp, "%s %s", p->nickname, p->password); //Scrivo riga per riga le informazioni sugli utenti
		p=p->nextUser; //Passo al prossimo utente
		if(p!=NULL){fprintf(fp, "\n");}
	}
	fclose(fp); //Chiudo il file una volta finiti gli utenti
}

void saveRunning(utente user) //Salva in questo modo: currentScore currentDifficulty currentFood{int int char int} day month year hour minutes *tutte le struct snake* \n 
{
	FILE *fp; //puntatore a file
	char open_file[70] = "savedata/"; //Stringa che compone il nome del file da aprire
	strcat(open_file, user->nickname);
	strcat(open_file,"_running.txt");
	fp = fopen(open_file, "w"); //Apro il file corrispondente alla stringa che ho creato
	struct running* p = user->running; //Puntatore per scorrere le partite dell'utente
	while(p != NULL) //Scorro fino alla fine delle struct running
	{
		fprintf(fp, "%d %d %d %d %c %d %d %d %d %d %d ", p->currentScore, p->currentDifficulty, (p->currentFood).foodX, (p->currentFood).foodY, (p->currentFood).food, (p->currentFood).value, p->day, p->month, p->year, p->hour, p->minutes); //Scrivo nel file tutte le informazioni sulla partita in corso tranne la lista do body del serpente
		while(p->currentSnake!=NULL) //Ora scrivo tutta la lista di body del serpente, body per body
		{
			fprintf(fp, "%d %d %c %c ", p->currentSnake->posX, p->currentSnake->posY, p->currentSnake->bodypart, p->currentSnake->direction);
			p->currentSnake = p->currentSnake->next;
		}
		fprintf(fp, "-1");//Carattere per identificare la fine della linea 
		if(p->next!=NULL){fprintf(fp, "\n");} //Newline per passare alla prossima partita da salvare
		p=p->next; //Scorro in avanti la lista
	}
	fclose(fp);	
}

void saveScores(utente user)
{
	FILE *fp; //puntatore a file
	char open_file[70] = "savedata/"; //Stringa che compone il nome del file da aprire
	strcat(open_file, user->nickname);
	strcat(open_file,"_scores.txt");
	fp = fopen(open_file, "w"); //Apro il file corrispondente alla stringa che ho creato
	struct scores* p; //Puntatore per scorrere la lista dei punteggi
	p = user->userScores; //Inizio dalla testa della lista
	while(p!=NULL)
	{
		fprintf(fp, "%s %d %d %d %d", p->name, p->score, p->day, p->month, p->year); //Salvo nel file nome, score, giorno mese e anno del punteggio.
		p=p->prev;
		if(p!=NULL){fprintf(fp, "\n");}
	}
	fclose(fp);
}

void loadUsers(game* currentGame) 
{
	FILE *fp;
	fp = fopen("savedata/user_list.txt", "r"); //Apro file in lettura
	if(fp!=NULL)//Se il file esiste
	{
		utente u=NULL; //Puntatore per creare la lista degli utenti
		utente p=NULL; //Puntatore che tiene traccia dell'elemento precedente di u
		u =(utente) malloc(sizeof(struct utente)); //Creo il primo nodo della lista
		u->running=NULL;
		u->userScores=NULL;
		u->nextUser=NULL;
		if(u==NULL) endwin(); //Se ci sono errori nell'allocazione della memoria termino il programma
		(*currentGame)->listHead=u;//Aggancio la lista di utenti alla struct della partita
		while(feof(fp)==0) //Ciclo fino a quando il puntatore non arriva a EOF
		{
			fscanf(fp, "%s", u->nickname); //Prendo il nickname
			fscanf(fp, "%s", u->password); //Prendo la password
			u->nextUser=(utente)malloc(sizeof(struct utente)); //Alloco il prossimo utente
			p=u;//P tiene traccia di u prima che u passi avanti, così poi da impostare a NULL l'ultimo elemento della lista
			u=u->nextUser; //Passo al prossimo utente
			u->running=NULL;
			u->userScores=NULL;
			u->nextUser=NULL;
			if(u==NULL) endwin(); //Se ci sono errori nell'allocazione della memoria termino il programma
		}
		free(u);//In questo tipo di implementazione, il ciclo esce e rimane allocata una struct che va deallocata
		p->nextUser=NULL;//Adesso l'ultimo elemento della lista punta a NULL
		fclose(fp);
	}
}

void loadRunning(utente user) //Carica le partite in corso
{
	FILE *fp;
	char open_file[70] = "savedata/"; //Stringa che compone il nome del file da aprire
	strcat(open_file, user->nickname);
	strcat(open_file,"_running.txt");
	fp = fopen(open_file, "r"); //Apro il file in lettura
	if(fp!=NULL)//Se il file esiste
	{
    		fseek (fp, 0, SEEK_END);
    		int size = ftell(fp);
    		if (size!=0) //Se il file non è vuoto
    		{
    			fseek(fp, 0, SEEK_SET); //Torno all'inizio del file
			int check=0; ///Variabile per verificare la fine della riga
			struct running* p = NULL, *pp = NULL; //Puntatore ai vari struct running che verranno creati
			p = (struct running*) malloc(sizeof(struct running));
			if(p==NULL) endwin();
			user->running=p;
			struct body* s = NULL, *u; //Puntatore per caricare le informazioni del serpente
			while(feof(fp)==0)
			{
				u=NULL;
				s=(struct body*) malloc(sizeof(struct body));
				p->currentSnake=s;
				if(s==NULL) endwin();
				fscanf(fp, "%d %d %d %d %c %d %d %d %d %d %d", &(p->currentScore), &(p->currentDifficulty), &((p->currentFood).foodX), &((p->currentFood).foodY), &((p->currentFood).food), &((p->currentFood).value), &(p->day), &(p->month), &(p->year), &(p->hour), &(p->minutes)); //Inserisco tutti i valori nella struct running tranne quelli del serpente
				if(p->currentScore==EOF || p->currentDifficulty == EOF){break;}
				while(1)
				{
					fscanf(fp, "%d", &check); //Controllo se sono arrivato alla fine dei valori da copiare
					if(check==-1 || feof(fp)!=0) //Se ci sono arrivato, esco
					{
						break;
					}
					(s->posX)=check; //Visto che check non è -1, allora assegno il valore letto alla variabile corretta
					fscanf(fp, "%d %c %c ", &(s->posY), &(s->bodypart), &(s->direction)); //Assegno il resto dei valori
					s->next=(struct body*) malloc(sizeof(struct body)); //Alloco il prossimo nodo
					u=s; //U adesso punta a S
					s=s->next; //S può procedere
				}
				u->next = NULL; //Setto a Null il nodo puntato dall'ultimo utilizzato
				free(s); //Libero l'ultimo nodo non utilizzato per via dell'implementazione
				p->next = (struct running*) malloc(sizeof(struct running)); //alloco il prossimo nodo
				p->prev=pp;
				pp=p; //Punto il nodo precedente nel caso sia l'ultimo
				p=p->next; //Passo al prossimo nodo
			}
			free(p);
			pp->next = NULL;
			fclose(fp);
		}
	}
}

void loadScores(utente user) //Carica la lista di score dell'utente
{
	FILE* fp;
	utente p;//Puntatore alla struct utente, per scorrere la lista di utenti e attaccare la lista di punteggi una volta caricata
	struct scores* s, *u; //Puntatore alla lista di punteggi
	p=user;
	while(p!=NULL)
	{
		s=NULL;
		u=NULL;
		char open_file[70] = "savedata/"; //Stringa che compone il nome del file da aprire
		strcat(open_file, p->nickname);
		strcat(open_file,"_scores.txt");
		fp = fopen(open_file, "r"); //Apro il file in lettura
		if(fp==NULL) break; //Se il file non esiste esco
		fseek (fp, 0, SEEK_END);
    		int size = ftell(fp);
    		fseek(fp, 0, SEEK_SET); //Torno all'inizio del file
		if(size==0) break;//Se il file è vuoto esco
		s=(struct scores*) malloc(sizeof(struct scores));//Alloco la memoria per il nodo del punteggio
		if(s==NULL)endwin();
		p->userScores=s;
		while(feof(fp)==0) //Fino alla fine del file
		{
			fscanf(fp, "%s %d %d %d %d", s->name, &(s->score), &(s->day), &(s->month), &(s->year)); //Inserisco nel nodo le informazioni sul punteggio
			s->prev=(struct scores*) malloc(sizeof(struct scores)); //Alloco la memoria del prossimo nodo
			u=s; //Punto al nodo attuale
			s=s->prev; //Punto al prossimo nodo
		}
		u->prev=NULL;//Faccio puntare il penultimo nodo a null
		free(s);//Libero l'ultimo nodo in più che è stato allocato
		p=p->nextUser;//Mi sposto al prossimo utente
		fclose(fp);//Chiudo il file e ricomincio
	}
}
