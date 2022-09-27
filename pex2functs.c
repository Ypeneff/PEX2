/** pex2functs.c
 * =============================================================
 * Name: Yury Peneff
 * Section:  T6-7
 * Project:  PEX-2
 * Purpose:  Establish functions for use in pex2.c
 * Documentation: getCharSafe function was modeled after the 
 * safe input library in Lab 9.  See Documentation statement in main
 * for everything else.
 * ============================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifndef MAX_STRING_SIZE
#define MAX_STRING_SIZE 100
#endif // !MAX_STRING_SIZE



//Initiates random libary using time as a seed for an actual random number
void initiateRandomSeed() {
    //Got this from the zybooks reading with random, creates better randomness by seeding with time
    srand(time(NULL));
}

/**
 * @brief Ensures that a user input is a valid character
 * @return returns the inputted character if valid, otherwise exits the program.
 * @pre none
 * @post none
 */
char getCharSafe(){
   char character;
   int scanfReturn = 0;
   // attempt to read an integer value
   scanfReturn = scanf(" %c", &character);
   fflush(stdin); // eliminate any excess input
   // quit if unsuccessful
   if(scanfReturn != 1){
      fprintf(stderr, "Bad character input - terminating\n");
      exit(1);
   }
   return character;

}
/**
 * @brief Gets a random number corresponding to a card in the deck
 * @param card is a pointer to represent the value of the card, A-K
 * @param suit is a point to represent the suit of the card, H,C,S,D
 * @return returns a number corresponding to a card
 */
int getNextCard(int* card, char* suit) {
    //13 possible values with 4 different suits
    *card = (rand() % 13)+1; //Generates random number between 1 and 13
    int suitValue = (rand() % 4) + 1; //Generates random number between 1 and 4
    switch (suitValue) {
        case 1:
            *suit = 'C';
            break;
        case 2:
            *suit = 'S';
            break;
        case 3:
            *suit = 'H';
            break;
        case 4:
            *suit = 'D';
            break;
    }
    return 0;
}

/**
* @brief gets the probabilty that the player will draw a card equal to or lower than the target value
* @param desired Value the value that the player needs to get to keep playing
* @return A double representing the percent chance that the desired value or lower is achieved.
*/

double getProbabilityOfValueOrLower(int desiredValue) {
    /*
    * For my game an ace can be a one or an eleven.  We'll just treat them as ones here since
    * that's the lowest value that they can possibly be.  This also works in the case where the
    * desiredValue is 11, since 1s are factored in when they wouldn't be normally and we don't
    * add 11s to the deck.  The probability would still be accurate since the number of cards below
    * or at the desired value remains the same, which is kinda neat if you ask me.
    */
    int cardValues[52];

    //Lazy man's method of getting an array filled with all the possible card values
    int valueToPush = 1;
    int toggle = 0;
    for (int i = 0; i < 52; i++) {
        cardValues[i] = valueToPush;
        if (valueToPush != 10) {
            toggle++;
        }
        if (toggle == 4) {
            toggle = 0;
            valueToPush++;
        }
    }
    //Iterates through every element of the array and determines how many are below desiredValue
    int numCardsBelowDesiredValue = 0;
    for (int i = 0; i < 52; i++) {
        if (cardValues[i] <= desiredValue) {
            numCardsBelowDesiredValue++;
        }
    }
    //Quick maths
    double probability =  (double)numCardsBelowDesiredValue / 52 * 100;
    //Money
    return probability;
    /*
    * Note: If you've got a table for various desiredValue values and my probabilities don't match up,
    * remember that you can get a 1 in my game which throws things off a little.
    */

}

/**
 * @brief Determines keywords the host uses depending on the progress of the game, cueing player to
 * level of demonic corruption.  Really just sets the atmosphere.
 * @param playerName The player's name.
 * @param playerDescription The string that's going to be used in displayGameState.
 * @param corruptionLevel Lets the function know just how demented the game has gotten.
 * @return nothing
*/
void hostCorruption(char* playerName, char* playerDescription, int corruptionLevel) {
    switch (corruptionLevel) {
    case 0:
        strncpy(playerDescription, playerName, MAX_STRING_SIZE);
        break;
    case 1:
        strncpy(playerDescription, "\033[33mBlood Vessel\033[0m", MAX_STRING_SIZE);
        break;
    case 2:
        strncpy(playerDescription, "\033[35mFUEL FOR THE BLOOD GODS\033[0m", MAX_STRING_SIZE);
        break;
    case 3:
        strncpy(playerDescription, "\033[31mMEEEAAAAT\033[0m", MAX_STRING_SIZE);
        break;
    }
}

/**
* @brief Lets the player know important information about his hand. Had to convert this so that it displays
* the HUD too, because that just looks better.  I'm absolutely positive that there's a better way of doing this
* because this takes a stupid amount of parameters, but frankly, I'm past the point of caring.  It looks cool
* and that's all that matters.
* @param playerName The player's name
* @param corruptionLevel The level of game corruption, fed into hostCorruption();
* @return Nothing
*/
void displayGameState(char* playerName, int corruptionLevel, int cardIndex, int* cardValues, char* cards, char* cardSuits, int blood, int powerCards, int aces) {
    char playerDescription[MAX_STRING_SIZE];
    int playerScore = 0;
    hostCorruption(playerName, playerDescription, corruptionLevel);

    printf("\033[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\033[0m\n");
    printf("PLAYER: %s\n", playerName);
    printf("\033[31mBLOOD: %d   \033[33mPOWER CARDS: %d\033[0m   ACES: %d\n", blood, powerCards, aces);
    printf("\033[36m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\033[0m");

    printf("Your hand is:\n");
    for (int i = 0; i < cardIndex; i++) {
        if (cards[i] == 'A') {
            printf("A(%d)", cardValues[i]);
        }
        else {
            printf("%c", cards[i]);
        }
        printf("%c   ", cardSuits[i]);
        //Sums the players score since we're already looping through card values
        playerScore += cardValues[i];
    }
    printf("\n\n");
    printf("%s, you're currently at %d.\n", playerDescription, playerScore);
    printf("You need a %d or lower to not bust.\n", 21 - playerScore);
    printf("The odds of this happening are %0.1lf%%\n", getProbabilityOfValueOrLower(21 - playerScore));
}
/**
* @brief Determines which player won or if there's a tie.
* @param player1Score Player 1's score
* @param player2Score Player 2's score
* @return 1 for player 1 win, 2 for player 2 win, 3 for tie, 4 if both players bust
*/
int getWinningPlayer(int player1Score, int player2Score) {
    int player1CorrectedScore = 21 - player1Score;
    int player2CorrectedScore = 21 - player2Score;
    //Both players busted
    if (player1CorrectedScore < 0 && player2CorrectedScore < 0) {
        return 4;
    }
    //Player 1 busted
    else if (player1CorrectedScore < 0) {
        return 2;
    }
    //Player 2 busted
    else if (player2CorrectedScore < 0) {
        return 1;
    }
    //Player 1 wins
    else if (player1CorrectedScore < player2CorrectedScore) {
        return 1;
    }
    //Player 2 wins
    else if (player2CorrectedScore < player1CorrectedScore) {
        return 2;
    }
    //Tie
    else {
        return 3;
    }
}
/**
* @brief Handling J's, Q's, K's and aces became a little harder than I thought
* so to cut down on bloat inside of playBlackjack I made this nifty little function to handle
* the array assignments for me.
* @param cardList the array which has the information of the cards for user reading
* @param suitList the array which has the suits of the cards
* @param valueList the array which has the actual values of the cards
* @param aces the number of aces the player has, changes depending on if they draw an ace or not
* @param cardDrawn the value returned from getNextCard, needs to be parsed
* @param suitDrawn the suit returned from getNextCard
* @param index the index of the arrays
*/
void arrayHandler(char* cardList, char* suitList, int* valueList, int* aces, int cardDrawn, char suitDrawn, int index) {
    suitList[index] = suitDrawn;
    if (cardDrawn == 1) {
        cardList[index] = 'A';
        valueList[index] = 1;
        *aces = *aces + 1;
    }
    else if (cardDrawn == 10) {
        cardList[index] = 'T';
        valueList[index] = 10;
    }
    else if (cardDrawn == 11) {
        cardList[index] = 'J';
        valueList[index] = 10;
    }
    else if (cardDrawn == 12) {
        cardList[index] = 'Q';
        valueList[index] = 10;
    }
    else if (cardDrawn == 13) {
        cardList[index] = 'K';
        valueList[index] = 10;
    }
    else{
        cardList[index] = cardDrawn + '0'; //Converts the int to a char
        valueList[index] = cardDrawn;
    }
}

/**
* @brief Handles power cards and their effects.  Used in playBlacjack to both print a list of the player's
* power cards and to actually implement them.
* @param powerCardIndex the index of the power card the players wants to use (where its located in the array)
* @param powerCardName name of the power card (edited in the function to show to the player)
* @param powerCardEffect printed effect of the power card, used similarly to powerCardName
* @param runEffect whether or not the effect should run (1 for yes, 0 for no)
* @param cardList pointer to array with card types, edited by certain power card functions
* @param suitList pointer to array with card suits, edited by certain power card functions
* @param valueList pointer to array with card values, edited by certain power card functions
* @param aces number of aces the player has in playBlacjack function, edited by certain power card functions
* @param cardIndex the cardIndex used by the playBlackjack function, edited by certain power card functions
*/
void powerCardHandler(int powerCardIndex, char* powerCardName, char* powerCardEffect, int runEffect, char* cardList, char* suitList, int* valueList, int* aces, int* cardIndex) {
    /*
    * Index and associate action:
    * 1) Add ace to hand
    * 2) Subtract 5 from hand
    * 3) Add 5 to hand
    * 4) Remove last drawn card
    * 5) Duplicate last drawn card
    */
    switch (powerCardIndex) {
    case 1:
        strncpy(powerCardName, "Ace High", MAX_STRING_SIZE);
        strncpy(powerCardEffect, "Add an ace to your hand", MAX_STRING_SIZE);
        if (runEffect == 1) {
            printf("\n\n\033[33mUsing power card Ace High\033[0m\n");
            /**
            * Okay, I know for a fact you're looking at this and thinking "I definitely didn't teach them that.
            * And you are correct, we didn't learn this in class.  I was stumped for a good minute trying to
            * index these arrays with a pointer and not understanding why it wasn't working until I realized
            * (thanks to a little online research) that the arrays are stored in memory, and indexing them 
            * is as simple as adding an ending to that memory.  Voila.
            */
            
            *(cardList + *cardIndex) = 'A';
            *(suitList + *cardIndex) = 'S';
            *(valueList + *cardIndex) = 1;

            *aces = *aces + 1;
            *cardIndex = *cardIndex + 1;
        }
        break;
    case 2:
        strncpy(powerCardName, "High 5", MAX_STRING_SIZE);
        strncpy(powerCardEffect, "Add a 5 to your hand", MAX_STRING_SIZE);
        if (runEffect == 1) {
            printf("\n\n\033[33mUsing power card High 5\033[0m\n");

            *(cardList + *cardIndex) = '5';
            *(suitList + *cardIndex) = 'S';
            *(valueList + *cardIndex) = 5;

            *cardIndex = *cardIndex + 1;
        }
        break;
    case 3:
        strncpy(powerCardName, "Low 5", MAX_STRING_SIZE);
        strncpy(powerCardEffect, "Subtract 5 from your hand", MAX_STRING_SIZE);
        //Kinda cheeky, but we're adding a -5 as a card type basically.
        if (runEffect == 1) {
            printf("\n\n\033[33mUsing power card Low 5\033[0m\n");

            *(cardList + *cardIndex) = '-';
            *(suitList + *cardIndex) = '5';
            *(valueList + *cardIndex) = -5;

            *cardIndex = *cardIndex + 1;
        }
        break;
    case 4:
        strncpy(powerCardName, "Second Chance", MAX_STRING_SIZE);
        strncpy(powerCardEffect, "Remove last drawn card from hand", MAX_STRING_SIZE);
        if (runEffect == 1) {
            printf("\n\n\033[33mUsing power card Second Chance\033[0m\n");

            *(valueList + *cardIndex) = 0;

            *cardIndex = *cardIndex - 1;
        }
        break;
    case 5:
        strncpy(powerCardName, "Make It a Double", MAX_STRING_SIZE);
        strncpy(powerCardEffect, "Duplicate last drawn card", MAX_STRING_SIZE);
        if (runEffect == 1) {
            printf("\n\n\033[33mUsing power card Make It a Double\033[0m\n");

            *(cardList + *cardIndex) = *(cardList + *cardIndex - 1);
            *(suitList + *cardIndex) = *(suitList + *cardIndex - 1);
            *(valueList + *cardIndex) = *(valueList + (*cardIndex - 1));

            *cardIndex = *cardIndex + 1;
        }
        break;
    default:
        printf("Not a valid power card\n");
        break;
    }

}

/**
* @brief I have flown to close to the sun, and now I must burn.  I realize now the foolishness of my ambition,
* and also why you had us use auto-refilling decks.  Removing information from an array is not as easy in C
* as it is in Python.  But I'm too far down the rabbit hole.  I WILL make a function that removes a power card
* that's already been used from the player's hand, or I'm going to die trying.
* 
* @param powerCardList Pointer to the list of power cards this is gonna iterate through.  Has to be a pointer
* so it can actually edit the contents.
* @param index The index of the thing we're tryna remove.
* @param size The size of the array, which will reduce by 1 once this is over.
* @param the powerCardIndex used in the main function.  Needs to be decremented to maintain array flow
*/
void removePowerCard(int* powerCardList, int index, int* size, int* powerCardIndex) {
    for (int i = 0; i < *size - index; i++) {
        int increment = index + i;
        *(powerCardList + increment) = *(powerCardList + increment + 1);
    }
    *size = *size - 1;
    *powerCardIndex = *powerCardIndex - 1;
    /*Okay that actually wasn't that bad.  I pretty much just did the same thing as the previous function.
    Unfortunately this is gonan require another pointer to the number of power cards the player has, so
    I'll have to rewrite some stuff.*/
}


/**
* @brief The big boy.  Where the game is actually player.
* @param playerName Name of the player
* @param gameCorruptionLevel How far along the game has gone, literally only used for flavor text.
* @param blood The player's blood level, used for the HUD.
* @param numPowerCards Used for the HUD, whenever player wants to use a power card, and passed to
* removePowerCard after-the-fact.
* @param powerCardList List of the player's power cards. Integers correspond to type of power card.
* @param powerCardIndex The powerCardIndex used in main().  Must be passed to removePowerCard.
* @return Returns the player's score.
*/
int playBlackjack(char* playerName, int gameCorruptionLevel, int blood, int* numPowerCards, int* powerCardList, int* powerCardIndex) {
    int aces = 0;
    int playerScore = 0;

    char powerCardName[MAX_STRING_SIZE] = "initialized now lmao";
    char powerCardEffect[MAX_STRING_SIZE] = "initialized now lmao";

    //Since we use a renewing deck, the max possible cards you could get w/o busting is 21
    //Could've done that with a multidimensional array but my brain doesn't wanna work hard rn
    char cards[22];
    int cardValues[22];
    char cardSuits[22];
    //So we know where to stick the cards if you get a new one.
    int cardIndex = 0;
    
    //Draw two starting cards
    int drawnCard;
    char drawnCardSuit;
    getNextCard(&drawnCard, &drawnCardSuit);
    arrayHandler(cards, cardSuits, cardValues, &aces, drawnCard, drawnCardSuit, cardIndex);
    cardIndex++;
    getNextCard(&drawnCard, &drawnCardSuit);
    arrayHandler(cards, cardSuits, cardValues, &aces, drawnCard, drawnCardSuit, cardIndex);
    cardIndex++;

    displayGameState(playerName, gameCorruptionLevel, cardIndex, cardValues, cards, cardSuits, blood, *numPowerCards, aces);
    printf("\n\n");
    //Gets player selection: hit, stand, powerCard, convert ace
    printf("\n");
    char playerSelection = 'Y'; //Y for Yury
    //Run until the player stands or they bust/get a blackjack
    while (playerSelection != 's') {
        //clears and then updates player score
        playerScore = 0;
        for (int i = 0; i < cardIndex; i++) {
            playerScore += cardValues[i];
        }
        printf("AVAILABLE ACTIONS:\n");
        printf("\033[32m(h)it, \033[36m(s)tand, \033[33m(p)ower card, \033[0m(c)onvert ace\n");
        playerSelection = getCharSafe();
        //Gets a new card if the player wants to hit
        if (playerSelection == 'h') {
            printf("Confirm, you'd like to hit for another card? (y)es, (n)o\n");
            playerSelection = getCharSafe();
            if (playerSelection == 'y') {
                printf("\n");
                getNextCard(&drawnCard, &drawnCardSuit);
                arrayHandler(cards, cardSuits, cardValues, &aces, drawnCard, drawnCardSuit, cardIndex);
                printf("You drew a %c%c\n", cards[cardIndex], cardSuits[cardIndex]);
                playerScore += cardValues[cardIndex];
                cardIndex++;
                displayGameState(playerName, gameCorruptionLevel, cardIndex, cardValues, cards, cardSuits, blood, *numPowerCards, aces);
            }
            //Otherwise loop back around
            else{}
        }
        //Lists player's power cards, their affects, confirms usage
        else if (playerSelection == 'p') {
            if (*numPowerCards == 0) {
                printf("You have no power cards at this time\n");
            }
            else{
                printf("You have the following power cards:\n");
                for (int i = 0; i < *numPowerCards; i++) {
                    //Lists power cards
                    printf("%d)", i+1);
                    
                    //Gets info to display to player
                    powerCardHandler(powerCardList[i], powerCardName, powerCardEffect, 0, cards, cardSuits, cardValues, &aces, &cardIndex);

                    printf("%s: ", powerCardName);
                    printf("%s\n", powerCardEffect);
                }
                printf("(n)evermind\n");
                printf("Enter the number of the power card you wish to play, or (n)evermind...\n");
                playerSelection = getCharSafe();
                if (playerSelection != 'n') {
                    powerCardHandler(powerCardList[(int)playerSelection - 49], powerCardName, powerCardEffect, 1, cards, cardSuits, cardValues, & aces, & cardIndex);
                    
                    //Interesting, we don't use the & sign here since we actually want it to use the same pointer.
                    removePowerCard(powerCardList, (int)playerSelection - 49, numPowerCards, powerCardIndex);
                    printf("\n");
                    displayGameState(playerName, gameCorruptionLevel, cardIndex, cardValues, cards, cardSuits, blood, *numPowerCards, aces);
                }
            }
        }
        //Handles converting aces from 1 to 11 and vice versa
        else if (playerSelection == 'c') {
            if (aces == 0) {
                printf("You have no aces to convert\n");
            }
            else {
                printf("You have %d aces to convert\n", aces);
                for (int i = 0; i < cardIndex; i++) {
                    if (cardValues[i] == 1 || cardValues[i] == 11) {
                        printf("You have an ace valued at %d, would you like to convert it?", cardValues[i]);
                        printf("(y)es, (n)o\n");
                        playerSelection = getCharSafe();
                        if (playerSelection == 'y') {
                            //Checks to make sure the player won't bust if they convert the ace
                            if (cardValues[i] == 1) {
                                if (playerScore + 10 <= 21) {
                                    cardValues[i] = 11;
                                    playerScore += 10;
                                }
                                else {
                                    printf("If you convert this ace, you'll bust, we'll cancel this one on the house.\n");
                                }
                            }
                            else {
                                cardValues[i] = 1;
                                playerScore -= 10;
                            }
                        }
                    }
                }
                displayGameState(playerName, gameCorruptionLevel, cardIndex, cardValues, cards, cardSuits, blood, *numPowerCards, aces);
            }
        }
        //Handles standing
        else if (playerSelection == 's') {
            printf("Confirm, you'd like to stand on %d? (y)es, (n)o\n", playerScore);
            playerSelection = getCharSafe();
            if (playerSelection == 'y') {
                playerSelection = 's';
                printf("You end the round with a %d\n", playerScore);
            }
        }

        //Checks to see if there's a blackjack or bust
        if (playerScore > 21) {
            printf("You bust!\n");
            playerSelection = 's'; //force a stand, essentially
        }
        else if (playerScore == 21) {
            printf("Blackjack!\n");
            playerSelection = 's';
        }
    }
    return playerScore;
}

/**
 * @brief Prints out the tutorial
 * @return nothing
 */
void printTutorial(){
    char dummyVariable;
    printf("Don't worry, it's a pretty simple game. All you have to do is count to 21!\n");
    printf("Each card has a value: number cards use their numbers; K's, Q's, and J's are worth 10.\n");
    printf("Aces are worth 1 or 11, that's why we call it the Ace High Casino!\n");
    printf("The object of the game is to get as close to 21 as possible without going over (which is called busting).\n");
    printf("Unlike other casinos, you won't be playing against me, you'll be playing against each other!\n");
    printf("You start off with two cards. Add them together to get you're value.\n");
    printf("We'll do the math for you, don't worry.\n");
    printf("Enter any key to continue...\n");
    scanf(" %c", &dummyVariable);
    printf("At this point, you have several options.  If you'd like another card, say \033[32m\"(h)it me!\"\n\033[0m");
    printf("If you're happy with your hand, then say \033[36m\"(s)tand\".\n\033[0m");
    printf("If you're luck enough to have one, you can use a \033[33m(p)ower card\033[0m.\n");
    printf("If you have an ace and want to convert it from 1 to 11 (or vice versa), just (c)onvert it.\n");
    printf("Once you're finished, your opponent will do the same thing.\n");
    printf("Once they're done, you'll compare hands to see who the winner is!\n");
    printf("Enter any key to continue...\n");
    scanf(" %c", &dummyVariable);
    printf("Remember, here at the Ace High we don't bet with money, that would be irresponsible!\n");
    printf("Instead, we play with \033[31mBLOOD\033[0m.\n");
    printf("Whoever has the hand closest to 21 wins!  The loser forfeits their delicious life essence.\n");
    printf("Y'ai'ng'ngah Yog-Sothoth. Now, let's get to the game!\n");
};

/**
 * @brief Determines type of power card and whether the player gets one or not.
 * @param Player's life.  The lower it is, the more likely you get a power card.
 * @param How far along the game has gone.  The more blood is spilled, the more likely power cards are.
 * @return nothing
*/
int givePowerCard(int life, int gameCorruption) {
    double powerCardChance = (rand() % 100) + 1;
    double powerCardEligibility = (100.0 - ((double)life/2.0)) * (((double)gameCorruption + 1) / 2.0);
    if (powerCardEligibility > powerCardChance) {
        printf("You got a \033[33mpower card!\033[0m\n");
        int powerCardIndex = (rand() % 5) + 1;
        return powerCardIndex;
    }
    else {
        return 0;
    }
}

/**
* @brief Sets the mood based on how the game is going.
* @param bloodSpilt How much blood has been let.
* @return Nothing
*/
void flavorText(int corruptionLevel) {
    //Randomly selects flavor text
    int flavorIndex = rand() % 5;

    //Flavor text gets crazier the higher corruptionLevel
    if (corruptionLevel == 0) {
        switch (flavorIndex) {
        case 0:
            printf("I must say, you both have a wonderfully rosy complexion.\n");
            break;
        case 1:
            printf("The boss lives downstairs.  I'm sure he'd love to meet you someday.\n");
            break;
        case 2:
            printf("You should tell your friends about us when you leave today.  Well... one of you at least. \n");
            break;
        case 3:
            printf("Have you watched Sinister?  My cousin was in that movie!\n");
            break;
        case 4:
            printf("Consider hitting the spa after this, the leeches will have you feeling tip-top!\n");
            break;
        }
    }
    else if (corruptionLevel == 1) {
        printf("\033[33m");
        switch (flavorIndex) {
        case 0:
            printf("Mmmm, tasty. I mean - are you feeling alright, friend?\n");
            break;
        case 1:
            printf("You're both doing great, keep bleedi-playing\n");
            break;
        case 2:
            printf("I fell from the heavens and ended up in... Vegas?  City of second chances I guess.\n");
            break;
        case 3:
            printf("If you'd like more \033[31mBLOOD\033[33m, you can sell your soul at the booth over there.\n");
            break;
        case 4:
            printf("\033[36mughhn\033[0m");
            break;
        }
        printf("\033[0m");
    }
    else if (corruptionLevel == 2) {
        printf("\033[35m");
        switch (flavorIndex) {
        case 0:
            printf("ABBADON LOOKS ON WITH INTEREST\n");
            break;
        case 1:
            printf("THEY LOCKED US OUT OF THE GARDEN, BURNED OUR SOULS WITH POKERS...\n");
            break;
        case 2:
            printf("O friend and companion of the night, thou who rejoicest in the baying of dogs and spilt blood,\n");
            printf("thousand-faced moon, look favourably on our sacrifices!\n");
            break;
        case 3:
            printf("SATE THE UNSATIABLE HUNGER, FEED TEMPTATION WITH VICE UNTIL IT IS DONE\n");
            break;
        case 4:
            printf("UNDER THE WHIP OF ABRAXAS WE TOIL THE FIELDS OF FIRE, BLACKENED FEET SUPPORT THE WEIGHT OF SIN\n");
            break;
        }
        printf("\033[0m");
    }
    else if (corruptionLevel == 3) {
        printf("\033[31m");
        switch (flavorIndex) {
        case 0:
            printf("Ph'nglui mglw'nafh Cthulhu R'lyeh wgah'nagl fhtagn");
            break;
        case 1:
            printf("Itz rachu mantantu vespacha kaltamu\n");
            break;
        case 2:
            printf("I’chalaz Itz’rechel Ahn’tal Ah’tan’tel Regrezitahl\n");
            break;
        case 3:
            printf("Etsel Mala’kel Test’zel Sam’tan’el Itz Hel Asta’Rel Me Vaskalla\n");
            break;
        case 4:
            printf("Y'AI 'NG'NGAH YOG-SOTHOTH H'EE--L'GEB F'AI THRODOG UAAAH\n");
            break;
        }
        printf("\033[0m");
    }
}