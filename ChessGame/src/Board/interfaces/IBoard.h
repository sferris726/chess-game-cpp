class IBoard {
    public:
    virtual void displayBoard() = 0;
    virtual bool movePiece(int start_x, int start_y, int end_x, int end_y) = 0;
};