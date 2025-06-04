#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

class GameLogic {
public:
    GameLogic();
    void checkScore(int ballPositionX, int windowWidth);
    void resetGame();
    int getPlayer1Score() const;
    int getPlayer2Score() const;

private:
    int player1Score;
    int player2Score;
};

#endif // GAMELOGIC_HPP