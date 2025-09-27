#ifndef CUBE_HPP
#define CUBE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using small = unsigned char;

namespace slvr
{
	enum class Move : small
	{
		R, R_PRIME, R2,
		L, L_PRIME, L2,
		U, U_PRIME, U2,
		D, D_PRIME, D2,
		F, F_PRIME, F2,
		B, B_PRIME, B2
	};

	class Cube
	{
	private:
		small cornerP_[8];
		small cornerO_[8];
		small edgeP_[12];
		small edgeO_[12];
		std::vector<Move> solution_;

		static void cycle4(small arr[], small idx1, small idx2, small idx3, small idx4) noexcept;
		static void cycle2(small arr[], small idx1, small idx2, small idx3, small idx4) noexcept;
			
		void move(small corner1, small corner2, small corner3, small corner4,
				  small edge1,   small edge2,   small edge3,   small edge4  ) noexcept;

		void swap(small corner1, small corner2,
				  small corner3, small corner4,
				  small edge1,   small edge2,
				  small edge3,   small edge4) noexcept;

		void flipEdge(small pos) noexcept;
		void twistCorner(small pos, small val) noexcept;

		static void checkLengths(small cornerP[], small cornerO[], small edgeP[], small edgeO[]);
		static void checkPositions(small corners[], small edges[]);
		static void checkOrientations(small corners[], small edges[]);

	public:
		Cube() noexcept;
		Cube(const std::string &moves);
		Cube(const Cube &other) noexcept;
		Cube &operator=(const Cube &other) noexcept;
		~Cube() = default;

		void R()      noexcept;
		void RPrime() noexcept;
		void R2()     noexcept;

		void L()      noexcept;
		void LPrime() noexcept;
		void L2()     noexcept;

		void U()      noexcept;
		void UPrime() noexcept;
		void U2()     noexcept;

		void D()      noexcept;
		void DPrime() noexcept;
		void D2()     noexcept;

		void F()      noexcept;
		void FPrime() noexcept;
		void F2()     noexcept;

		void B()      noexcept;
		void BPrime() noexcept;
		void B2()     noexcept;

		void applyMove(Move move) noexcept;
		void addMove(Move move);

		void applyMoves(const std::string &moves);
		void addMoves(const std::string &moves);

		friend std::ostream &operator<<(std::ostream &os, const Cube &other);
	};
}

#endif