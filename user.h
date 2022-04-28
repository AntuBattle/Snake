void printAuthScreen(); //Schermata di autenticazione

void auth(game* currentGame); //Funzione che permette all'utente di loggarsi o di registrarsi

int checkDoubles(game currentGame, utente user); //Previene che ci siano utenti con lo stesso nome

void loadUsers(game* currentGame); //Funzione che carica gli utenti dal file nel gioco

void loadRunning(utente user); //Carica le partite in corso

void loadScores(utente user); //Carica la lista di score dell'utente

void saveUsers(game currentGame); //Funzione che salva gli utenti in un file.

void saveRunning(utente user); //Salva le partite in corso da terminare

void saveScores(utente user); //Salva la lista di score dell'utente
