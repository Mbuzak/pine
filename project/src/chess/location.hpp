#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <fstream>


namespace chschr
{

class Location
{
public:
    Location() = default;
    Location( int rank, int file );
    Location( char file, char rank);

    int getRank() const;
    int getFile() const;

    friend std::ostream& operator<<( std::ostream &out, const Location &location );

    friend bool operator==( const Location &l1, const Location &l2 );

private:
    int mRank, mFile;
};

}

#endif
