#include "move.hpp"

namespace chschr
{

Move::Move(Location f, Location t):
from(f), to(t) {}

Move::Move(const char *note) {
	from = Location(note[0], note[1]);
	to = Location(note[2], note[3]);
}

std::ostream& operator<<(std::ostream &out, const Move &move) {
	return out << "(" << move.from << " - " << move.to << ")";
}

bool operator==(const Move &m1, const Move &m2) {
	return m1.from == m2.from && m1.to == m2.to;
}

}
