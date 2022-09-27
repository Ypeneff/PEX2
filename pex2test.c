/** pex2test.h
 * =============================================================
 * Name: Yury Peneff
 * Section:  T6-7
 * Project:  PEX-2
 * Purpose:  Make test functions for pex2.c
 * ============================================================= */

#include <assert.h>
#include "pex2functs.h"
#include <stdio.h>
#include <stdlib.h>
//Use structure assert(HrMinToMin(0, 0) == 0);


/*
* As I'm sure you're aware sir, the rubric only says I need to test getNextCard,
* getProbabilityOfCardOrLower, and getWinningPlayer.  Are there more functions in my code?
* Yes indeed.  Am I so incredibly sick of this project by now that I no longer care? Bingo.
*/

/*
* int getNextCard(int* card, char* suit);

double getProbabilityOfValueOrLower(int desiredValue);
int getWinningPlayer(int player1Score, int player2Score);
*/
void runAllTests() {
	//Tests for getNextCard
	int cardValue;
	char cardSuit;
	/*getNextCard actually returns 0 every time.You can see how this actually works in the
	function itself, but in order to have actual face cards (for realism of course) I couldn't just
	return 1-10.  Well, I guess I could've, and honestly that might've been a smarter option, but it's
	too late now to be frank.
	*/
	assert(getNextCard(&cardValue, &cardSuit) == 0);
	if (cardValue <= 13 && cardValue > 0) {
		//This is good
	}
	else {
		printf("getNextCard set cardValue equal to %d", cardValue);
		exit(1);
	}
	if (cardSuit == 'C' || cardSuit == 'S' || cardSuit == 'H' || cardSuit == 'D') {
		//This is good too
	}
	else {
		printf("getNextCard set cardSuit equal to %c, which isn't a correct suit", cardSuit);
		exit(1);
	}

	//Tests for getProbabilityOfCardOrLower
	/*
	* Please note, these probabilities are factoring in the fact that aces can be both a one or an 11.
	* This means that the probability of getting a card worth 10 or lower is 100%, since the game deals all
	* aces as ones initially.  Unless my math is wrong, all these probabilities should be correct.
	*/
	//Should be 100%
	assert(getProbabilityOfValueOrLower(10) == 100.0);
	//Should be 4/52, or 7.69%
	assert(getProbabilityOfValueOrLower(1) - 7.69 < 0.01);
	//Should be 4/52 * 4, or 30.76%
	assert(getProbabilityOfValueOrLower(4) - 30.76 < 0.01);
	//Should be 4/52 * 9, or 69.23%
	assert(getProbabilityOfValueOrLower(9) - 69.23 < 0.01);

	//Tests for getWinningPlayer
	//Should return 1 for player 1 win
	assert(getWinningPlayer(21, 1) == 1);
	//Should return 2 for player 2 win
	assert(getWinningPlayer(1, 21) == 2);
	//Should return 3 for tie
	assert(getWinningPlayer(21, 21) == 3);
	//Should also return 3
	assert(getWinningPlayer(1, 1) == 3);
	//Specific to my program, I have a special scenario for two busts, so this is necessary
	assert(getWinningPlayer(25, 25) == 4); //Psycho lmao
}

//All done :)