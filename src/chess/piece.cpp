#include "piece.hpp"

namespace chschr
{

std::ostream& operator<<(std::ostream &out, const Piece &piece) {
	switch (piece) {
		case Piece::k:  out << "k";  break;
		case Piece::q:  out << "q";  break;
		case Piece::r:  out << "r";  break;
		case Piece::b:  out << "b";  break;
		case Piece::n:  out << "n";  break;
		case Piece::p:  out << "p";  break;
		case Piece::x:  out << "x";  break;
		case Piece::K:  out << "K";  break;
		case Piece::Q:  out << "Q";  break;
		case Piece::R:  out << "R";  break;
		case Piece::B:  out << "B";  break;
		case Piece::N:  out << "N";  break;
		case Piece::P:  out << "P";  break;
	
		default: out << "-";  break;
	}

	return out;
}

}
