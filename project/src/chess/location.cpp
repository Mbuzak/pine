#include "location.hpp"


namespace chschr
{

Location::Location(int rank, int file):
mRank(rank), mFile(file) {
	if(rank < 0 || rank > 7 || file < 0 || file > 7)
		throw std::invalid_argument("rank and file must be in range [0, 7]");
}

Location::Location(char file, char rank) {
	mRank = 8 - (rank - '0');
	mFile = (int)file - 97;
}

int Location::getRank() const {
	return mRank;
}

int Location::getFile() const {
	return mFile;
}

std::ostream& operator<<(std::ostream &out, const Location &location) {
	return out << char('a' + location.mFile) << char('8' - location.mRank);
}

bool operator==(const Location &l1, const Location &l2) {
	return l1.mRank == l2.mRank && l1.mFile == l2.mFile;
}

}
