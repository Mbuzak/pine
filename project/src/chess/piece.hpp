#ifndef PIECE_HPP
#define PIECE_HPP

#include <fstream>


namespace chschr
{

enum class Piece {k, q, r, b, n, p, x, K, Q, R, B, N, P};

std::ostream& operator<<( std::ostream &out, const Piece &piece );

}

#endif
