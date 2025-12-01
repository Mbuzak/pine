#ifndef CHESS_HPP
#define CHESS_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include "piece.hpp"
#include "move.hpp"
#include "direction.hpp"


namespace chschr
{

class Chess
{
public:
    Chess();

    Piece getPiece( int rank, int file );
    void setPiece( const Move &move );

    std::vector<int> LegalMoves(int field_id);

    bool isWhite( int rank, int file );
    bool isBlack( int rank, int file );

    bool perform( const Move &move );
    void undomove( Piece original, const Move &move );
    std::vector<Move> generateMoves();
    bool check() const;

    bool isInScope( int rank, int file );
    bool isValidLocation( int srcRank, int srcFile, int dstRank, int dstFile );

    friend std::ostream& operator<<( std::ostream &out, const Chess &chess );

//private:
    std::array<Piece, 64> mBoard;

    const std::unordered_map<Piece, const char*> pieceName  = {
        {Piece::k, "king"}, {Piece::K, "king"},
        {Piece::q, "queen"}, {Piece::Q, "queen"},
        {Piece::r, "rook"}, {Piece::R, "rook"},
        {Piece::b, "bishop"}, {Piece::B, "bishop"},
        {Piece::n, "knight"}, {Piece::N, "knight"},
        {Piece::p, "pawn"}, {Piece::P, "pawn"},
        {Piece::x, "x"}
    };
};

}

#endif
