#include "PieceUtilities.h"

namespace PieceUtilities {
std::string convertPieceColorToStr(IPiece::PieceColor color_type) {
  if (color_type == IPiece::PieceColor::WHITE) {
    return "w";
  } else {
    return "b";
  }
}
} // namespace PieceUtilities
