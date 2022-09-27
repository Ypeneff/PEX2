/** pex2.c
 * =============================================================
 * Name: Yury Peneff
 * Section:  T6-7
 * Project:  PEX 2 
 * Purpose:  Make a sick Blackjack game
 * Documentation Statement:  The only help that I got on this project
 * from other cadets was a conversation that I overheard in my ECE 210
 * class.  Someone was talking about using time as a random seed, and
 * I made sure to implement that by digging around some old Zybooks readings.
 * I don't remember who these people were, but shoutout to them regardless.
 * I used stack overflow to debug some of the errors I got while making all
 * this work.  In particular, the pointers were giving me hell at times.
 * I never copied code, this was mainly used to understand what the compiler
 * was trying to tell me.  I'd say the biggest use of stack overflow is the
 * powerCardHandler function where I edit arrays using pointers.  I found
 * that method as a result of me trying to see if I could even accomplish
 * what I was trying to do within the C language.  I consider thjs justified
 * since I now know how to accomplish such a task in the future, as evidenced
 * by the same technique's use in the removePowerCard function.  I didn't
 * have to Google that one!
 * 
 * Final Note:  I really hope you like this, sir.  I put a little extra effort
 * in since I'm a fan of video games, and making my own was pretty fun.  I do
 * have to warn you though, PEX 3 is getting the absolute bare minimum of effort
 * for full points, since I've only got the brainpower left for one impressive
 * project, and I'm saving that for the Final Project.  Enjoy!
 * ============================================================= */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pex2functs.h"
#include "pex2test.h"

#ifndef MAX_CHAR_LENGTH
#define MAX_CHAR_LENGTH 100
#endif

int main() {
    //Uncomment to run all tests
    //runAllTests();
    
    //Global variables that need to be carried across games

    int gameCorruptionLevel = 0;
    int player1Blood = 100;
    int player2Blood = 100;
    int player1PowerCards[50];
    int numPowerCardsPlayer1 = 0;
    int player1PowerCardIndex = 0;
    int player2PowerCards[50];
    int numPowerCardsPlayer2 = 0;
    int player2PowerCardIndex = 0;
    int bloodSpilt = 0;
    int powerCard;

    //Scores stored at round end
    int player1Score = 0;
    int player2Score = 0;
    initiateRandomSeed();
    char player1Name[MAX_CHAR_LENGTH];
    char player2Name[MAX_CHAR_LENGTH];
    char decision;
    printf("Welcome to the Ace High Casino & Resort!\n");
    printf("My name is Charlie, I'll be your dealer for today.\nWhat's your name? ");
    scanf("%s", player1Name);
    printf("I see you've got a friend with you, what does he go by? ");
    scanf("%s", player2Name);
    printf("%s and %s then.  Well, welcome to the table folks!\n", player1Name, player2Name);
    printf("Here at the Ace High we play a very \033[33mSPECIAL\033[0m kind of Blackjack, \ndo you need a rundown of the rules?\n(y)es\t(n)o\n");
    decision = getCharSafe();
    if(decision == 'y'){ printTutorial();}
    else if(decision == 'n') { printf("Very well then, let's get started!\n"); }
    else {
        printf("I'll take that as a yes.\n");
        printTutorial();
    }
    printf("\n\nOK, let's play! \n");
    
    //Continues to play rounds until one player loses all their blood
    while (player1Blood > 0 && player2Blood > 0) {
        
        player1Score = playBlackjack(player1Name, gameCorruptionLevel, player1Blood, &numPowerCardsPlayer1, player1PowerCards, &player1PowerCardIndex);
        
        //Gives power cards if needed
        powerCard = givePowerCard(player1Blood, gameCorruptionLevel);
        if (powerCard != 0) {
            player1PowerCards[player1PowerCardIndex] = powerCard;
            player1PowerCardIndex++;
            numPowerCardsPlayer1++;
        }
        
        printf("End of %s's turn, enter any key to continue...\n", player1Name);
        getCharSafe();

        player2Score = playBlackjack(player2Name, gameCorruptionLevel, player2Blood, &numPowerCardsPlayer2, player2PowerCards, &player2PowerCardIndex);
        //Gives power cards if needed
        powerCard = givePowerCard(player2Blood, gameCorruptionLevel);
        if (powerCard != 0) {
            player2PowerCards[player2PowerCardIndex] = powerCard;
            player2PowerCardIndex++;
            numPowerCardsPlayer2++;
        }

        //Determines who won and subtracts life from loser
        int whoWon = getWinningPlayer(player1Score, player2Score);
        printf("\n\n\n");
        printf("---ROUND END---\n");
        if (whoWon != 3) {
            printf("\033[31mBLOOD FOR THE BLOOD GODS\n\033[0m");
            if (whoWon == 1) {
                printf("%s won.\n", player1Name);
                printf("For now, we \033[35mfeast\033[0m on \033[36m%s\033[0m\n", player2Name);
                player2Blood -= player1Score;
                bloodSpilt += player1Score;
            }
            else if (whoWon == 2) {
                printf("%s won.\n", player2Name);
                printf("For now, we \033[35mfeast\033[0m on \033[36m%s\033[0m\n", player1Name);
                player1Blood -= player2Score;
                bloodSpilt += player2Score;
            }
            else {
                printf("Both \033[31mvicti\033[0m - ahem - players lost.  \033[35mMORE BLOOD FOR US!\033[0m\n");
                player1Blood -= 21;
                player2Blood -= 21;
                bloodSpilt += 42;
            }
        }
        else {
            printf("A tie... \033[33mno blood for now.\033[0m\n");
        }
        //Can't use else if here since we want to check for each value
        if (bloodSpilt > 50) {
            gameCorruptionLevel = 1;
        }
        if (bloodSpilt > 100) {
            gameCorruptionLevel = 2;
        }
        if (bloodSpilt > 150) {
            gameCorruptionLevel = 3;
        }
        printf("\n");
        //Adds a little atmosphere
        flavorText(gameCorruptionLevel);
        printf("\nEnter any key to continue...\n");
        getCharSafe();
    }

    //Determines name of losing player
    char losingPlayer[MAX_CHAR_LENGTH];
    if (player1Blood <= 0) {
        strncpy(losingPlayer, player1Name, MAX_CHAR_LENGTH);
    }
    else {
        strncpy(losingPlayer, player2Name, MAX_CHAR_LENGTH);
    }

    //Leaves the player feeling satisfied with their victory
    printf("\033[35mSHLUUUUUUURP\033[0m\n");
    printf("My my, %s was mighty tasty.\n", losingPlayer);
    printf("Please come again some time, and bring some more friends with you!\n");
    printf("Ta ta!\n");
    return 0;

}
