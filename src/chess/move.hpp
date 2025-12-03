#ifndef MOVE_HPP
#define MOVE_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include "location.hpp"

namespace chschr
{

class Move {
public:
	Move(Location f, Location t);
	Move(const char *note);

	Location from, to;

	friend std::ostream& operator<<(std::ostream &out, const Move &move);
	friend bool operator==(const Move &m1, const Move &m2);
};

}

#endif
