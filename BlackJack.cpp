#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <array>

using namespace std;

int standCounter = 0;
int bet = 0;
int pot = 0;

vector<array<string, 2>> playerHand;
vector<array<string, 2>> dealerHand;

int determineHandValue(vector<array<string, 2>> hand) {

    int handValue = 0;
    int aces = 0;

    for (int i = 0; i < hand.size(); i++) {

        if (hand[i][0] == "Ace") handValue += 11;
        if (hand[i][0] == "Queen" || hand[i][0] == "Jack" || hand[i][0] == "King") handValue += 10;
        else handValue += stoi(hand[i][0]);
    }

    if (handValue > 21) {

        handValue -= aces;
    }   
    
    return handValue;
}

string gameResults(vector<array<string, 2>> player, vector<array<string, 2>> dealer) {
    
    int playerHandValue = determineHandValue(player);
    int dealerHandValue = determineHandValue(dealer);
    string result = " ";

    if (playerHandValue == 21 || dealerHandValue > 21 || (playerHandValue < 21 && playerHandValue > dealerHandValue)) {
       return "Congratulations, you win $" + to_string(pot);
    } else if (playerHandValue == dealerHandValue) {
        return "Game is a draw. Pot is returned to players: $" + to_string(pot / 2);
    }
    return "House wins $" + to_string(pot);
}

char houseRules(int handValue) {

    return (handValue >= 17) ? 's' : 'h';
}

int choice(char decision) {

    int numOfCards = 0;

    switch (toupper(decision)) {

        case 'H':
        case 'D': numOfCards += 1; break;
        case 'S': 0; break;
    }

    return numOfCards;
}

int doubleDown(char decision) {

    if (toupper(decision) == 'D') pot *= 2;

    return pot;
}

string showHand(vector<array<string, 2>> hand) {

    string humanReadableCards = " ";

    for (int i = 0; i < hand.size(); i++) {

        humanReadableCards += hand[i][0] + " of " + hand[i][1];

        if (i < hand.size() - 1) {

            humanReadableCards += ", ";
        }
    }

    return humanReadableCards;
}

void showGameState() {

    cout << "\x1b[32m" << "Pot: $" << pot << ". " << "\x1b[0m" << endl
         << "The Dealer has " << showHand(dealerHand) << "." << endl
         << "Value: " << determineHandValue(dealerHand) << "." << endl
         << "Your hand is: " << showHand(playerHand) << "." << endl
         << "Value: " << determineHandValue(playerHand) << endl;
}

bool isDuplicate(array<string, 2> card) {

    for (int i = 0; i < playerHand.size(); i++) {

       if (playerHand[i][0] == card[0] && playerHand[i][1] == card[1]) {
            return true;
       }
    }
    
    for (int i = 0; i < dealerHand.size(); i++){

        if (dealerHand[i][0] == card[0] && dealerHand[i][1] == card[1]) {
            return true;
        }
    }

    return false;
}

void distributeCards(int numOfCards, string person) {

    string numbers[13] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    string suits[4] = {"Clubs", "Diamonds", "Hearts", "Spades"};

    for (int i = 0; i < numOfCards; i++) {
        
        array<string, 2> card = {numbers[rand() % 12], suits[rand() % 3]};

        while (isDuplicate(card)) {

            card[0] = numbers[rand() % 12];
            card[1] = suits[rand() % 3];
        }

        if (person == "player") {
            playerHand.push_back(card);
        } else {
            dealerHand.push_back(card);
        }
    }
}

bool isGameOver(vector<array<string, 2>> player, vector<array<string, 2>> dealer) {

    
    return standCounter == 2 || determineHandValue(player) >= 21 || determineHandValue(dealer) >= 21;
}

int main() {

    srand((uint)time(0));

    cout << "Welcome to the table. We'll be playing Blackjack. Throughout the game, you will have the opportunity to hit, stand, or double down.\n"
         << "For this table, we do not allow splitting. To make a move, type the first letter of your decision and hit enter.\n"
         << "First, how much would you like to bet?" << endl;
    cin >> bet;

    pot = bet * 2;

    cout << "The pot is $" << pot << "." << endl;
    distributeCards(2, "dealer");
    distributeCards(2, "player");
    showGameState();

    while (!isGameOver(playerHand, dealerHand)) {

        char decision = 'x';
        cout << "What is your move?" << endl;
        cin >> decision;
        distributeCards(choice(decision), "player");
        distributeCards(choice(houseRules(determineHandValue(dealerHand))), "dealer");
        doubleDown(decision);
        showGameState();

        if (decision == 'd') {

            while (houseRules(determineHandValue(dealerHand)) != 's') {
                
                distributeCards(choice(houseRules(determineHandValue(dealerHand))), "dealer");
            }
            break;
        } else if (decision == 's') {

            standCounter++;
        }
    }

    cout << gameResults(playerHand, dealerHand) << endl;
}

