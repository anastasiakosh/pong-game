#include "GameLogic.hpp"

GameLogic::GameLogic() : player1Score(0), player2Score(0) {}

void GameLogic::checkScore(int ballPositionX, int windowWidth) {
    if (ballPositionX < 0) {
        player2Score++;
        resetGame();
    } else if (ballPositionX > windowWidth) {
        player1Score++;
        resetGame();
    }
}

void GameLogic::resetGame() {
    // Reset logic here
}

int GameLogic::getPlayer1Score() const {
    return player1Score;
}

int GameLogic::getPlayer2Score() const {
    return player2Score;
}