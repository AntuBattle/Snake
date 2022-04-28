struct food 
{
	int foodX; //Posizione x del cibo
	int foodY; //Posizione Y del cibo
	char food; //Carattere del cibo
	int value; //Valore del cibo
};

struct body
{
	int posX; //Posizione del serpente
	int posY;
	char bodypart; //Carattere per stampare il serpente
	char direction; //Direzione in cui sta andando il cubo del serpente
	struct body* next; //Puntatore al prossimo pezzo di corpo del serpente
};

struct snake
{
	struct body* head; //Puntatore alla testa
	struct body* tail; //Puntatore alla coda
};

typedef struct snake* serpente;

struct scores 
{
	char name[17];
	int score;
	int day;
	int month;
	int year;
	struct scores* prev; //Puntatore allo score precedente
};

struct running //Struct delle partite in corso
{
	int currentScore; 
	int currentDifficulty;
	struct body* currentSnake; //Struct del serpente nel momento della pausa
	struct food currentFood;
	int day;
	int month;
	int year;
	int hour;
	int minutes;
	struct running* prev;
	struct running* next;
};

struct utente 
{
	char nickname[17];
	char password[17];
	struct utente* nextUser; //Prossimo utente
	struct scores* userScores; //Puntatore a tutti i punteggi dell'utente
	struct running* running; //Puntatore alle partite in sospeso dell'utente
};

typedef struct utente* utente;

struct game //Struct principale della partita
{
	struct utente* logged; //Utente loggato
	struct utente* listHead; //Testa alla lista di utenti
	struct snake* snake; //Puntatore al serpente
	struct food* food; //Puntatore al cibo
	int score;
	int difficulty;
};

typedef struct game* game;
