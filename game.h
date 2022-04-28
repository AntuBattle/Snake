void startScreen(); //Stampa l'immaginetta del serpente

void printBorders(); //Stampa i bordi di gioco

game startGame(); //Inizializza la partita. (La struct)

void beginGame(game* currentGame); //Fa iniziare effettivamente la partita

int pauseGame(game* currentGame); //Mette in pausa la partita

void updateScore(game currentGame); //Aggiorna il punteggio a schermo

int resume(game* currentGame); //Carica la partita precedente 

void save(game* currentGame); //Nel caso in cui si esca dal gioco durante la pausa, questa funzione salva in un file lo stato del serpente, la sua posizione e la posizione del cibo.

void exit(); //esce dal programma.

void freeSnake(game* currentGame); //Libera tutte le body del serpente quando la partita finisce

void gameOverScreen(); //Stampa la schermata di game over

int playSnake(); //Funzione dentro la quale è racchiuso tutto il gioco, da chiamare nel main.
