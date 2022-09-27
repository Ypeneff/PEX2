/** pex2functs.h
 * =============================================================
 * Name: Yury Peneff
 * Section:  T6-7
 * Project:  PEX-2
 * Purpose:  Define functions for pex2.c
 * ============================================================= */

void initiateRandomSeed();

char getCharSafe();

int getNextCard(int* card, char* suit);

double getProbabilityOfValueOrLower(int desiredValue);

void displayGameState(char* playerName, int corruptionLevel, int cardIndex, int* cardValues, char* cards, char* cardSuits, int blood, int powerCards, int aces);

int getWinningPlayer(int player1Score, int player2Score);

void arrayHandler(char* cardList, char* suitList, int* valueList, int* aces, int cardDrawn, char suitDrawn, int index);

void powerCardHandler(int powerCardIndex, char* powerCardName, char* powerCardEffect, int runEffect, char* cardList, char* suitList, int* valueList, int* aces, int cardIndex);

void removePowerCard(int* powerCardList, int index, int* size, int* powerCardIndex);

int playBlackjack(char* playerName, int gameCorruptionLevel, int blood, int* numPowerCards, int* powerCardList, int* powerCardIndex);

void printTutorial();

void hostCorruption(char* playerName, char* playerDescription, int corruptionLevel);

int givePowerCard(int life, int gameCorruption);

void flavorText(int corruptionLevel);