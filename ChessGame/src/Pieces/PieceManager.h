#include "PieceFactory.h"
#include "interfaces/IPiece.h"
#include "interfaces/IPieceManager.h"
#include <vector>

class PieceManager : public IPieceManager {
public:
  PieceManager(PieceFactory &piece_factory);

private:
  enum class PieceColor { WHITE, BLACK };
  std::vector<std::unique_ptr<Pawn>> m_pawns;
};
