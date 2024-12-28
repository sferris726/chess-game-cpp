class IPiece {
public:
  virtual bool isMoveValid(int start_x, int start_y, int end_x,
                           int end_y /*Board& board*/) = 0;
  virtual char getSymbol() const = 0;
  virtual ~IPiece(){};
};