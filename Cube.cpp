#include "Cube.hpp"

#define ARR_LEN "Improper array lengths"
#define CORNER_POS "Improper corner position values"
#define EDGE_POS "Improper edge position values"
#define CORNER_OR "Improper corner orientation values"
#define EDGE_OR "Improper edge orientation values"

namespace slvr
{
	void Cube::cycle4(small arr[], small idx1, small idx2, small idx3, small idx4) noexcept
	{
		small temp = arr[idx4];
		arr[idx4] = arr[idx3];
		arr[idx3] = arr[idx2];
		arr[idx2] = arr[idx1];
		arr[idx1] = temp;
	}

	void Cube::cycle2(small arr[], small idx1, small idx2, small idx3, small idx4) noexcept
	{
		small temp = arr[idx1];
		arr[idx1] = arr[idx2];
		arr[idx2] = temp;

		temp = arr[idx3];
		arr[idx3] = arr[idx4];
		arr[idx4] = temp;
	}

	void Cube::move(small corner1, small corner2, small corner3, small corner4,
		            small edge1,   small edge2,   small edge3,   small edge4  ) noexcept
	{
		cycle4(cornerP_, corner1, corner2, corner3, corner4);
		cycle4(cornerO_, corner1, corner2, corner3, corner4);
		cycle4(edgeP_,   edge1,   edge2,   edge3,   edge4  );
		cycle4(edgeO_,   edge1,   edge2,   edge3,   edge4  );
	}

	void Cube::swap(small corner1, small corner2,
		            small corner3, small corner4,
				    small edge1,   small edge2,
				    small edge3,   small edge4) noexcept
	{
		cycle2(cornerP_, corner1, corner2, corner3, corner4);
		cycle2(cornerO_, corner1, corner2, corner3, corner4);
		cycle2(edgeP_,   edge1,   edge2,   edge3,   edge4  );
		cycle2(edgeO_,   edge1,   edge2,   edge3,   edge4  );
	}

	void Cube::flipEdge(small pos) noexcept {edgeO_[pos] ^= 1;}

	void Cube::twistCorner(small pos, small val) noexcept
	{
		cornerO_[pos] = (cornerO_[pos] + val) % 3;
	}

	void Cube::checkLengths(small cornerP[], small cornerO[], small edgeP[], small edgeO[])
	{
		if (sizeof(cornerP) !=  8 || sizeof(cornerO) !=  8
	     || sizeof(edgeP)   != 12 || sizeof(edgeO)   != 12)
		{
			throw std::invalid_argument(ARR_LEN);
		}
	}

	void Cube::checkPositions(small corners[], small edges[])
	{
		bool tempc[ 8] = {0};
		bool tempe[12] = {0};

		for (small i = 0; i < 8; ++i)
		{
			if (tempc[corners[i]] || corners[i] >= 8 || corners[i] < 0) {throw std::invalid_argument(CORNER_POS);}

			tempc[corners[i]] = -1;
		}

		for (small i = 0; i < 12; ++i)
		{
			if (tempe[edges[i]] || edges[i] >= 12 || edges[i] < 0) {throw std::invalid_argument(EDGE_POS);}

			tempe[edges[i]] = -1;
		}
	}

	void Cube::checkOrientations(small corners[], small edges[])
	{
		for (small i = 0; i < 8; ++i)
		{
			if (corners[i] < 0 || corners[i] >= 3) {throw std::invalid_argument(CORNER_OR);}
		}

		for (small i = 0; i < 12; ++i)
		{
			if (edges[i] != 0 && edges[i] != 1) {throw std::invalid_argument(EDGE_OR);}
		}
	}

	Cube::Cube() noexcept
	{
		for (small i = 0; i < 12;)
		{
			edgeP_[i] = i;
			edgeO_[i++] = 0;
		}
		for (small i = 0; i < 8;)
		{
			cornerP_[i] = i;
			cornerO_[i++] = 0;
		}
	}

	Cube::Cube(const std::string &moves)
	{
		*this = Cube();
		(*this).applyMoves(moves);
	}

	Cube::Cube(const Cube &other) noexcept
	{
		memcpy(cornerO_, other.cornerO_, sizeof(cornerO_));
		memcpy(cornerP_, other.cornerP_, sizeof(cornerP_));
		memcpy(edgeO_, other.edgeO_, sizeof(edgeO_));
		memcpy(edgeP_, other.edgeP_, sizeof(edgeP_));
		solution_ = other.solution_;
	}

	Cube &Cube::operator=(const Cube &other) noexcept
	{
		if (this != &other)
		{
			memcpy(cornerO_, other.cornerO_, sizeof(cornerO_));
			memcpy(cornerP_, other.cornerP_, sizeof(cornerP_));
			memcpy(edgeO_, other.edgeO_, sizeof(edgeO_));
			memcpy(edgeP_, other.edgeP_, sizeof(edgeP_));
			solution_ = other.solution_;
		}
		return *this;
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
		}
	}

	void Cube::addMove(Move move)
	{
		applyMove(move);
		solution_.push_back(move);
	}

	void Cube::applyMoves(const std::string &moves)
	{
		std::size_t n = moves.size();

		for (std::size_t i = 0; i < n; ++i)
		{
			small moveSmall;

			if (!(moves[i] == 'r' || moves[i] == 'R'
			   || moves[i] == 'l' || moves[i] == 'L'
			   || moves[i] == 'u' || moves[i] == 'U'
			   || moves[i] == 'd' || moves[i] == 'D'
			   || moves[i] == 'f' || moves[i] == 'F'
			   || moves[i] == 'b' || moves[i] == 'B')) {continue;}
			
			switch (moves[i])
			{
				case 'r':
				case 'R': moveSmall = 0;  break;

				case 'l':
				case 'L': moveSmall = 3;  break;

				case 'u':
				case 'U': moveSmall = 6;  break;

				case 'd':
				case 'D': moveSmall = 9;  break;

				case 'f':
				case 'F': moveSmall = 12; break;

				case 'b':
				case 'B': moveSmall = 15; break;

				default: throw std::invalid_argument("Illegal move\n");
			}
			
			std::size_t j = i + 1;

			if (j == n) {applyMove(static_cast<Move>(moveSmall)); return;}

			switch (moves[j])
			{
				case '\'': applyMove(static_cast<Move>(moveSmall + 1)); break;

				case '2': applyMove(static_cast<Move>(moveSmall + 2)); break;

				default: applyMove(static_cast<Move>(moveSmall)); break;
			}
		}
	}

	void Cube::addMoves(const std::string &moves)
	{
		std::size_t n = moves.size();

		for (std::size_t i = 0; i < n; ++i)
		{
			small moveSmall;

			if (!(moves[i] == 'r' || moves[i] == 'R'
			   || moves[i] == 'l' || moves[i] == 'L'
			   || moves[i] == 'u' || moves[i] == 'U'
			   || moves[i] == 'd' || moves[i] == 'D'
			   || moves[i] == 'f' || moves[i] == 'F'
			   || moves[i] == 'b' || moves[i] == 'B')) {continue;}

			switch (moves[i])
			{
				case 'r':
				case 'R': moveSmall = 0;  break;

				case 'l':
				case 'L': moveSmall = 3;  break;

				case 'u':
				case 'U': moveSmall = 6;  break;

				case 'd':
				case 'D': moveSmall = 9;  break;

				case 'f':
				case 'F': moveSmall = 12; break;

				case 'b':
				case 'B': moveSmall = 15; break;

				default: throw std::invalid_argument("Illegal move\n");
			}
			
			std::size_t j = i + 1;

			if (j == n) {addMove(static_cast<Move>(moveSmall)); return;}

			switch (moves[j])
			{
				case '\'': addMove(static_cast<Move>(moveSmall + 1)); break;

				case '2': addMove(static_cast<Move>(moveSmall + 2)); break;

				default: addMove(static_cast<Move>(moveSmall)); break;
			}
		}
	}

	std::ostream &operator<<(std::ostream &os, const Cube &other)
	{
		small i = 0;

		os << "Corner Positions:";
		for (; i < 8;)
		{
			os << ' '
			   << static_cast<int>(other.cornerP_[i++]);
		}

		os << "\nCorner Orientations:";
		for (i = 0; i < 8;)
		{
			os << ' '
			   << static_cast<int>(other.cornerO_[i++]);
		}

		os << "\nEdge Positions:";
		for (i = 0; i < 12;)
		{
			os << ' '
			   << static_cast<int>(other.edgeP_[i++]);
		}

		os << "\nEdge Orientations:";
		for (i = 0; i < 12;)
		{
			os << ' '
			   << static_cast<int>(other.edgeO_[i++]);
		}

		return os;
	}
}