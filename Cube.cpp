#include "Cube.hpp"

#define ARR_LEN "Improper array lengths"
#define CORNER_POS "Improper corner position values"
#define EDGE_POS "Improper edge position values"
#define CORNER_OR "Improper corner orientation values"
#define EDGE_OR "Improper edge orientation values"
#define CORNER_SUM "Improper corner orientation sum value"
#define EDGE_SUM "Improper edge orientation sum value"
#define PERMUTATION "Improper position permutations"

namespace slvr
{
	std::ostream& operator<<(std::ostream& os, Move move)
	{
		if (move != Move::NULL_MOVE)
		{
			byte num = static_cast<byte>(move);

			switch (num / 3)
			{
				case 0: os << 'R'; break;
				case 1: os << 'L'; break;
				case 2: os << 'U'; break;
				case 3: os << 'D'; break;
				case 4: os << 'F'; break;
				case 5: os << 'B'; break;
			}

			switch (num % 3)
			{
				case 0: return os;           break;
				case 1: return (os << '\''); break;
				case 2: return (os << '2' ); break;
			}
		}

		return os;
	}

	std::ostream& operator<<(std::ostream& os, const std::vector<Move>& moves)
	{
		regi n = moves.size();

		os << '[';

		for (regi i = 0; i < n; ++i)
		{
			os << moves[i];

			if (i + 1 != n) {os << ", ";}
		}

		return (os << ']');
	}

	Face toFace(Move move) noexcept
	{
		if (move == Move::NULL_MOVE) {return Face::NULL_FACE;}

		byte num = static_cast<byte>(move);
		num /= 3;
		Face face = static_cast<Face>(num);

		return face;
	}

	Face opposite(Face face) noexcept
	{
		if (face == Face::NULL_FACE) {return face;}

		byte num = static_cast<byte>(face);
		
		if (num % 2) {return static_cast<Face>(--num);}

		return static_cast<Face>(++num);
	}

	void Cube::move(byte corner1, byte corner2, byte corner3, byte corner4,
		            byte edge1,   byte edge2,   byte edge3,   byte edge4  ) noexcept
	{
		cycle4(cornerP_, corner1, corner2, corner3, corner4);
		cycle4(cornerO_, corner1, corner2, corner3, corner4);
		cycle4(edgeP_,   edge1,   edge2,   edge3,   edge4  );
		cycle4(edgeO_,   edge1,   edge2,   edge3,   edge4  );
	}

	void Cube::swap(byte corner1, byte corner2,
		            byte corner3, byte corner4,
				    byte edge1,   byte edge2,
				    byte edge3,   byte edge4) noexcept
	{
		cycle2(cornerP_, corner1, corner2, corner3, corner4);
		cycle2(cornerO_, corner1, corner2, corner3, corner4);
		cycle2(edgeP_,   edge1,   edge2,   edge3,   edge4  );
		cycle2(edgeO_,   edge1,   edge2,   edge3,   edge4  );
	}

	void Cube::flipEdge(byte pos) noexcept {edgeO_[pos] ^= 1;}

	void Cube::twistCorner(byte pos, byte val) noexcept
	{
		cornerO_[pos] = (cornerO_[pos] + val) % 3;
	}

	void Cube::checkPositions(const corner_arr& corners, const edge_arr& edges)
	{
		bool tempc[ 8] = {0};
		bool tempe[12] = {0};

		for (byte i : corners)
		{
			if (tempc[i] || i >= 8) {throw std::invalid_argument(CORNER_POS);}

			tempc[i] = true;
		}

		for (byte i : edges)
		{
			if (tempe[i] || i >= 12) {throw std::invalid_argument(EDGE_POS);}

			tempe[i] = true;
		}
	}

	void Cube::checkOrientations(const corner_arr& corners, const edge_arr& edges)
	{
		for (byte i : corners)
		{
			if (i > 2) {throw std::invalid_argument(CORNER_OR);}
		}

		for (byte i : edges)
		{
			if (i > 1) {throw std::invalid_argument(EDGE_OR);}
		}
	}

	void Cube::checkOrientationSum(const corner_arr& cornerO, const edge_arr& edgeO)
	{
		regi cSum = 0, eSum = 0;

		for (byte i : cornerO) {cSum += i;}
		for (byte i : edgeO)   {eSum += i;}

		if (cSum % 3) {throw std::invalid_argument(CORNER_SUM);}
		if (eSum & 1) {throw std::invalid_argument(EDGE_SUM);}
	}

	void Cube::checkPermutationParity(const corner_arr& corners, const edge_arr& edges)
	{
		if (!(checkParity(corners) ^ checkParity(edges))) {throw std::invalid_argument(PERMUTATION);}
	}

	void Cube::checkCube(const corner_arr& cornerP, const corner_arr& cornerO, const edge_arr& edgeP, const edge_arr& edgeO)
	{
		checkPositions(cornerP, edgeP);
		checkOrientations(cornerO, edgeO);
		checkOrientationSum(cornerO, edgeO);
		checkPermutationParity(cornerP, edgeP);
	}

	Cube::Cube() noexcept :
		cornerP_{0,1,2,3,4,5,6,7},
		cornerO_{},
		edgeP_{0,1,2,3,4,5,6,7,8,9,10,11},
		edgeO_{}
	{}
	

	Cube::Cube(const std::string& moves) : Cube()
	{
		(*this).applyMoves(moves);
	}

	Cube::Cube(const Cube& other) noexcept :
		cornerP_(other.cornerP_),
		cornerO_(other.cornerO_),
		edgeP_(other.edgeP_),
		edgeO_(other.edgeO_),
		solution_(other.solution_)
	{}

	Cube::Cube(const corner_arr& cornerP, const corner_arr& cornerO, const edge_arr& edgeP, const edge_arr& edgeO)
	{
		checkCube(cornerP, cornerO, edgeP, edgeO);
		cornerP_ = cornerP;
		cornerO_ = cornerO;
		edgeP_ = edgeP;
		edgeO_ = edgeO;
	}

	Cube& Cube::operator=(const Cube& other) noexcept
	{
		if (this != &other)
		{
			cornerP_ = other.cornerP_;
			cornerO_ = other.cornerO_;
			edgeP_ = other.edgeP_;
			edgeO_ = other.edgeO_;
			solution_ = other.solution_;
		}
		return *this;
	}

	Cube::Cube(Cube&& other) noexcept :
		cornerP_(std::move(other.cornerP_)),
		cornerO_(std::move(other.cornerO_)),
		edgeP_(std::move(other.edgeP_)),
		edgeO_(std::move(other.edgeO_)),
		solution_(std::move(other.solution_))
	{}

	Cube& Cube::operator=(Cube&& other) noexcept
	{
		if (this != &other)
		{
			cornerP_ = std::move(other.cornerP_);
			cornerO_ = std::move(other.cornerO_);
			edgeP_ = std::move(other.edgeP_);
			edgeO_ = std::move(other.edgeO_);
			solution_ = std::move(other.solution_);
		}

		return *this;
	}

	const corner_arr&        Cube::cornerPositions()    const noexcept {return cornerP_; }
	const corner_arr&        Cube::cornerOrientations() const noexcept {return cornerO_; }
	const edge_arr&          Cube::edgePositions()      const noexcept {return edgeP_;   }
	const edge_arr&          Cube::edgeOrientations()   const noexcept {return edgeO_;   }
	const std::vector<Move>& Cube::solution()           const noexcept {return solution_;}
	
	Face Cube::lastFace() const noexcept 
	{
		if (solution_.empty()) {return Face::NULL_FACE;}

		return toFace(solution_.back());
	}

	Face Cube::secondLastFace() const noexcept
	{
		if (solution_.size() <= 1) {return Face::NULL_FACE;}

		using RevIterator = std::reverse_iterator<std::vector<slvr::Move>::const_iterator>;

		RevIterator iterator = ++solution_.rbegin();

		return toFace(*iterator);
	}

	void Cube::R() noexcept
	{
		move(0, 4, 5, 1, 
             1, 4, 9, 5);
        twistCorner(1, 1);
        twistCorner(0, 2);
        twistCorner(4, 1);
        twistCorner(5, 2);
	}

	void Cube::RPrime() noexcept
	{
		move(0, 1, 5, 4,
             1, 5, 9, 4);
        twistCorner(0, 2);
        twistCorner(1, 1);
        twistCorner(4, 1);
        twistCorner(5, 2);
	}

	void Cube::R2() noexcept
	{
		swap(0, 5,
             1, 4,
             1, 9,
             4, 5);
	}

	void Cube::L() noexcept
	{
		move(2, 6,  7, 3,
             3, 6, 11, 7);
        twistCorner(2, 2);
        twistCorner(3, 1);
        twistCorner(6, 1);
        twistCorner(7, 2);
	}

	void Cube::LPrime() noexcept
	{
		move(2, 3,  7, 6,
             3, 7, 11, 6);
        twistCorner(2, 2);
        twistCorner(3, 1);
        twistCorner(6, 1);
        twistCorner(7, 2);
	}

	void Cube::L2() noexcept
	{
		swap(2,  7,
             3,  6,
             3, 11,
             6,  7);
	}

	void Cube::U() noexcept
	{
		move(0, 1, 2, 3,
             0, 1, 2, 3);
	}

	void Cube::UPrime() noexcept
	{
		move(0, 3, 2, 1,
             0, 3, 2, 1);
	}

	void Cube::U2() noexcept
	{
		swap(0, 2,
             1, 3,
             0, 2,
             1, 3);
	}

	void Cube::D() noexcept
	{
		move(4,  7,  6, 5,
             8, 11, 10, 9);
	}

	void Cube::DPrime() noexcept
	{
		move(4, 5,  6,  7,
             8, 9, 10, 11);
	}

	void Cube::D2() noexcept
	{
		swap(4,  6,
             5,  7,
             8, 10,
             9, 11);
	}

	void Cube::F() noexcept
	{
		move(1, 5,  6, 2,
             2, 5, 10, 6);
        flipEdge( 5);
        flipEdge( 6);
        flipEdge( 2);
        flipEdge(10);
        twistCorner(1, 2);
        twistCorner(2, 1);
        twistCorner(5, 1);
        twistCorner(6, 2);
	}

	void Cube::FPrime() noexcept
	{
		move(1, 2,  6, 5,
             2, 6, 10, 5);
        flipEdge( 5);
        flipEdge( 6);
        flipEdge( 2);
        flipEdge(10);
        twistCorner(1, 2);
        twistCorner(2, 1);
        twistCorner(5, 1);
        twistCorner(6, 2);
	}

	void Cube::F2() noexcept
	{
		swap(1,  6,
             2,  5,
             2, 10,
             5,  6);
	}

	void Cube::B() noexcept
	{
		move(0, 3, 7, 4,
             0, 7, 8, 4);
        flipEdge(4);
        flipEdge(7);
        flipEdge(0);
        flipEdge(8);
        twistCorner(0, 1);
        twistCorner(3, 2);
        twistCorner(4, 2);
        twistCorner(7, 1);
	}

	void Cube::BPrime() noexcept
	{
		move(0, 4, 7, 3,
             0, 4, 8, 7);
        flipEdge(4);
        flipEdge(7);
        flipEdge(0);
        flipEdge(8);
        twistCorner(0, 1);
        twistCorner(3, 2);
        twistCorner(4, 2);
        twistCorner(7, 1);
	}

	void Cube::B2() noexcept
	{
		swap(0, 7,
             3, 4,
             0, 8,
             4, 7);
	}

	void Cube::applyMove(Move move) noexcept
	{
		switch (move)
		{
			case Move::R:       R();      break;
			case Move::R_PRIME: RPrime(); break;
			case Move::R2:      R2();     break;

			case Move::L:       L();      break;
			case Move::L_PRIME: LPrime(); break;
			case Move::L2:      L2();     break;

			case Move::U:       U();      break;
			case Move::U_PRIME: UPrime(); break;
			case Move::U2:      U2();     break;

			case Move::D:       D();      break;
			case Move::D_PRIME: DPrime(); break;
			case Move::D2:      D2();     break;

			case Move::F:       F();      break;
			case Move::F_PRIME: FPrime(); break;
			case Move::F2:      F2();     break;

			case Move::B:       B();      break;
			case Move::B_PRIME: BPrime(); break;
			case Move::B2:      B2();     break;

			default: break;
		}
	}

	void Cube::addMove(Move move)
	{
		if (move != Move::NULL_MOVE)
		{
			applyMove(move);

			solution_.push_back(move);
		}
	}

	Cube& Cube::operator+=(Move move)
	{
		addMove(move);

		return *this;
	}

	Cube Cube::operator+(Move move) const
	{
		Cube c = *this;

		c.addMove(move);

		return c;
	}

	void Cube::applyMoves(const std::string& moves)
	{
		regi n = moves.size();

		for (regi i = 0; i < n; ++i)
		{
			byte moveByte;

			if (!(moves[i] == 'r' || moves[i] == 'R'
			   || moves[i] == 'l' || moves[i] == 'L'
			   || moves[i] == 'u' || moves[i] == 'U'
			   || moves[i] == 'd' || moves[i] == 'D'
			   || moves[i] == 'f' || moves[i] == 'F'
			   || moves[i] == 'b' || moves[i] == 'B')) {continue;}
			
			switch (moves[i])
			{
				case 'r':
				case 'R': moveByte = 0;  break;

				case 'l':
				case 'L': moveByte = 3;  break;

				case 'u':
				case 'U': moveByte = 6;  break;

				case 'd':
				case 'D': moveByte = 9;  break;

				case 'f':
				case 'F': moveByte = 12; break;

				case 'b':
				case 'B': moveByte = 15; break;

				default: throw std::invalid_argument("Illegal move\n");
			}
			
			regi j = i + 1;

			if (j == n) {applyMove(static_cast<Move>(moveByte)); return;}

			switch (moves[j])
			{
				case '\'': applyMove(static_cast<Move>(moveByte + 1)); break;

				case '2': applyMove(static_cast<Move>(moveByte + 2)); break;

				default: applyMove(static_cast<Move>(moveByte)); break;
			}
		}
	}

	void Cube::addMoves(const std::string& moves)
	{
		regi n = moves.size();

		for (regi i = 0; i < n; ++i)
		{
			byte moveByte;

			if (!(moves[i] == 'r' || moves[i] == 'R'
			   || moves[i] == 'l' || moves[i] == 'L'
			   || moves[i] == 'u' || moves[i] == 'U'
			   || moves[i] == 'd' || moves[i] == 'D'
			   || moves[i] == 'f' || moves[i] == 'F'
			   || moves[i] == 'b' || moves[i] == 'B')) {continue;}

			switch (moves[i])
			{
				case 'r':
				case 'R': moveByte = 0;  break;

				case 'l':
				case 'L': moveByte = 3;  break;

				case 'u':
				case 'U': moveByte = 6;  break;

				case 'd':
				case 'D': moveByte = 9;  break;

				case 'f':
				case 'F': moveByte = 12; break;

				case 'b':
				case 'B': moveByte = 15; break;

				default: throw std::invalid_argument("Illegal move\n");
			}
			
			regi j = i + 1;

			if (j == n) {addMove(static_cast<Move>(moveByte)); return;}

			switch (moves[j])
			{
				case '\'': addMove(static_cast<Move>(moveByte + 1)); break;

				case '2': addMove(static_cast<Move>(moveByte + 2)); break;

				default: addMove(static_cast<Move>(moveByte)); break;
			}
		}
	}

	Cube& Cube::operator+=(const std::string& moves)
	{
		addMoves(moves);

		return *this;
	}

	Cube Cube::operator+(const std::string& moves) const
	{
		Cube c = *this;

		c.addMoves(moves);

		return c;
	}

	void Cube::undo() noexcept
	{
		if (!solution_.empty())
		{
			byte move = static_cast<byte>(solution_.back());

			switch(move % 3)
			{
				case 0: ++move; break;
				case 1: --move; break;
				case 2:         break;
			}

			applyMove(static_cast<Move>(move));

			solution_.pop_back();
		}
	}

	void Cube::operator--() noexcept {undo();}

	bool Cube::operator==(const Cube& other) const noexcept
	{
		return (cornerP_ == other.cornerP_
			 && cornerO_ == other.cornerO_
			 && edgeP_   == other.edgeP_
			 && edgeO_   == other.edgeO_);
	}

	bool Cube::equals(const Cube& other) const noexcept
	{
		return (*this == other);
	}

	bool Cube::operator!=(const Cube& other) const noexcept
	{
		return !(*this == other);
	}

	bool Cube::isSolved() const noexcept
	{
		return (cornerP_ == corner_arr{0,1,2,3,4,5,6,7}
			 && cornerO_ == corner_arr{}
			 && edgeP_ == edge_arr{0,1,2,3,4,5,6,7,8,9,10,11}
			 && edgeO_ == edge_arr{});
	}

	bool Cube::pruneMove(Move move) const noexcept
	{
		Face cubeFace = (*this).lastFace();
        Face moveFace = toFace(move);

        if (cubeFace == moveFace) {return true;}

        if (moveFace == opposite(cubeFace) && moveFace == (*this).secondLastFace()) {return true;}

		return false;
	}

	std::ostream& operator<<(std::ostream& os, const Cube& other)
	{
		os << "Corner Positions:";
		for (byte i : other.cornerP_)
		{
			os << ' '
			   << static_cast<int>(i);
		}

		os << "\nCorner Orientations:";
		for (byte i : other.cornerO_)
		{
			os << ' '
			   << static_cast<int>(i);
		}

		os << "\nEdge Positions:";
		for (byte i : other.edgeP_)
		{
			os << ' '
			   << static_cast<int>(i);
		}

		os << "\nEdge Orientations:";
		for (byte i : other.edgeO_)
		{
			os << ' '
			   << static_cast<int>(i);
		}

		return os;
	}
}