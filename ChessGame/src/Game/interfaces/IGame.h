class IGame {
    public:
    virtual void start() = 0;
    virtual void playMove(int start_x, int start_y, int end_x, int end_y) = 0;
    virtual bool isGameOver() = 0;
};