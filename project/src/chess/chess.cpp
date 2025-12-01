#include "chess.hpp"

namespace chschr
{

Chess::Chess()
{
    mBoard = {
        Piece::r, Piece::n, Piece::b, Piece::q, Piece::k, Piece::b, Piece::n, Piece::r,
        Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p, Piece::p,
        Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
        Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
        Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
        Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x, Piece::x,
        Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P, Piece::P,
        Piece::R, Piece::N, Piece::B, Piece::Q, Piece::K, Piece::B, Piece::N, Piece::R,
    };
}

std::vector<int> Chess::LegalMoves(int field_id) {
  std::vector<Move> moves = generateMoves();

  std::vector<int> piece_destination_ids;

  for (Move &move: generateMoves()) {
    int src_id = 8 * move.from.getRank() + move.from.getFile();

    if (field_id == src_id) {
      int dst_id = 8 * move.to.getRank() + move.to.getFile();

      piece_destination_ids.push_back(dst_id);
    }
  }

  return piece_destination_ids;
}

Piece Chess::getPiece( int rank, int file )
{
    int position = rank * 8 + file;
    return mBoard[position];
}

void Chess::setPiece( const Move &move )
{
    int srcPr = move.from.getRank();
    int srcPf = move.from.getFile();

    int dstPr = move.to.getRank();
    int dstPf = move.to.getFile();

    int srcPos = srcPr * 8 + srcPf;
    int dstPos = dstPr * 8 + dstPf;

    mBoard[dstPos] = getPiece( srcPr, srcPf );
    mBoard[srcPos] = Piece::x;
}

bool Chess::isWhite( int rank, int file )
{
    Piece p = getPiece( rank, file );

    return p == Piece::K || p == Piece::Q || p == Piece::R || p == Piece::B || p == Piece::N || p == Piece::P;
}

bool Chess::isBlack( int rank, int file )
{
    Piece p = getPiece( rank, file );

    return p == Piece::k || p == Piece::q || p == Piece::r || p == Piece::b || p == Piece::n || p == Piece::p;
}

bool Chess::perform( const Move &move )
{
    std::vector<Move> pseudomoves = generateMoves();

    for( const Move &pseudomove : pseudomoves )
    {
        if( pseudomove == move )
        {
            std::cout << "Move found!\n";

            setPiece( move );

            return true;
        }
    }

    return false;
}

void Chess::undomove( Piece original, const Move &move )
{}

std::vector<Move> Chess::generateMoves()
{
    std::vector<Move> pseudomoves;

    for( int rank = 0; rank < 8; rank++ )
    {
        for( int file = 0; file < 8; file++ )
        {
            Location src{rank, file};

            if( getPiece( rank, file ) == Piece::n || getPiece( rank, file ) == Piece::N )
            {
                std::vector<Direction> directions = {
                    {2, 1}, {2, -1}, {1, 2}, {1, -2}, 
                    {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}
                };

                for( const Direction &direction: directions )
                {
                    int dstRank = rank + direction.y;
                    int dstFile = file + direction.x;

                    if( isValidLocation( rank, file, dstRank, dstFile ) )
                    {
                        Location dst{rank + direction.y, file + direction.x};
                        pseudomoves.push_back(Move{src, dst});
                    }
                }
            }

            if( getPiece( rank, file ) == Piece::P )
            {
                if( rank == 6 && getPiece( rank - 1, file ) == Piece::x
                    && getPiece( rank - 2, file ) == Piece::x )
                    pseudomoves.push_back(Move{src, {rank - 2, file}});
                if( getPiece( rank - 1, file ) == Piece::x )
                    pseudomoves.push_back(Move{src, {rank - 1, file}});
                if( file != 0 && isBlack( rank - 1, file - 1 ) )
                    pseudomoves.push_back(Move{src, {rank - 1, file - 1}});
                if( file != 7 && isBlack( rank - 1, file + 1 ) )
                    pseudomoves.push_back(Move{src, {rank - 1, file + 1}});
            }

            if( getPiece( rank, file ) == Piece::p )
            {
                if( rank == 1 && getPiece( rank + 1, file ) == Piece::x
                    && getPiece( rank + 2, file ) == Piece::x )
                    pseudomoves.push_back(Move{src, {rank + 2, file}});
                if( getPiece( rank + 1, file ) == Piece::x )
                    pseudomoves.push_back(Move{src, {rank + 1, file}});
                if( file != 0 && isWhite( rank + 1, file - 1 ) )
                    pseudomoves.push_back(Move{src, {rank + 1, file - 1}});
                if( file != 7 && isWhite( rank + 1, file + 1 ) )
                    pseudomoves.push_back(Move{src, {rank + 1, file + 1}});
            }

            if( getPiece( rank, file ) == Piece::b || getPiece( rank, file ) == Piece::B )
            {
                std::vector<Direction> directions = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

                for( const Direction &direction: directions )
                {
                    for( int k = 1; k <= 7; k++ )
                    {
                        int dstRank = rank + k * direction.y;
                        int dstFile = file + k * direction.x;

                        if( isValidLocation( rank, file, dstRank, dstFile ) )
                        {
                            Location dst{rank + k * direction.y, file + k * direction.x};
                            pseudomoves.push_back(Move{src, dst});

                            bool opposite_1 = isWhite( rank, file ) && isBlack( dstRank, dstFile );
                            bool opposite_2 = isBlack( rank, file ) && isWhite( dstRank, dstFile );

                            if( opposite_1 || opposite_2 ) break;
                        }
                        else break;
                    }
                }
            }

            if( getPiece( rank, file ) == Piece::r || getPiece( rank, file ) == Piece::R )
            {
                std::vector<Direction> directions = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};

                for( const Direction &direction: directions )
                {
                    for( int k = 1; k <= 7; k++ )
                    {
                        int dstRank = rank + k * direction.y;
                        int dstFile = file + k * direction.x;

                        if( isValidLocation( rank, file, dstRank, dstFile ) )
                        {
                            Location dst{rank + k * direction.y, file + k * direction.x};
                            pseudomoves.push_back(Move{src, dst});

                            bool opposite_1 = isWhite( rank, file ) && isBlack( dstRank, dstFile );
                            bool opposite_2 = isBlack( rank, file ) && isWhite( dstRank, dstFile );

                            if( opposite_1 || opposite_2 ) break;
                        }
                        else break;
                    }
                }
            }

            if( getPiece( rank, file ) == Piece::q || getPiece( rank, file ) == Piece::Q )
            {
                std::vector<Direction> directions = {
                    {1, 1}, {1, 0}, {1, -1},
                    {0, 1}, {0, -1},
                    {-1, 1}, {-1, 0}, {-1, -1}
                };

                for( const Direction &direction: directions )
                {
                    for( int k = 1; k <= 7; k++ )
                    {
                        int dstRank = rank + k * direction.y;
                        int dstFile = file + k * direction.x;

                        if( isValidLocation( rank, file, dstRank, dstFile ) )
                        {
                            Location dst{rank + k * direction.y, file + k * direction.x};
                            pseudomoves.push_back(Move{src, dst});

                            bool opposite_1 = isWhite( rank, file ) && isBlack( dstRank, dstFile );
                            bool opposite_2 = isBlack( rank, file ) && isWhite( dstRank, dstFile );

                            if( opposite_1 || opposite_2 ) break;
                        }
                        else break;
                    }
                }
            }

            if( getPiece( rank, file ) == Piece::k || getPiece( rank, file ) == Piece::K )
            {
                std::vector<Direction> directions = {
                    {1, 1}, {1, 0}, {1, -1},
                    {0, 1}, {0, -1},
                    {-1, 1}, {-1, 0}, {-1, -1}
                };

                for( const Direction &direction: directions )
                {
                    int dstRank = rank + direction.y;
                    int dstFile = file + direction.x;

                    if( isValidLocation( rank, file, dstRank, dstFile ) )
                    {
                        Location dst{rank + direction.y, file + direction.x};
                        pseudomoves.push_back(Move{src, dst});
                    }
                }
            }
       }
    }

    return pseudomoves;
}

bool Chess::isInScope( int rank, int file )
{
    return rank >= 0 && file >= 0 && rank <= 7 && file <= 7;
}

bool Chess::isValidLocation( int srcRank, int srcFile, int dstRank, int dstFile )
{
    if ( !isInScope( dstRank, dstFile ) )
        return false;

    bool whites = isWhite( srcRank, srcFile ) && isWhite( dstRank, dstFile );
    bool blacks = isBlack( srcRank, srcFile ) && isBlack( dstRank, dstFile );

    if( whites || blacks )
        return false;

    return true;
}

std::ostream& operator<<( std::ostream &out, const Chess &chess )
{
    for( int i = 0; i < chess.mBoard.size(); i++ )
    {
        out << chess.mBoard[i];

        if( (i + 1) % 8 == 0 ) out << "\n";
        else out << " ";
    }

    return out;
}

}
