#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <array>

using byte = unsigned char;
using regi = std::size_t;

using corner_arr = std::array<byte,  8>;
using edge_arr   = std::array<byte, 12>;

namespace slvr
{
	enum class Move : byte
	{
		R, R_PRIME, R2,
		L, L_PRIME, L2,
		U, U_PRIME, U2,
		D, D_PRIME, D2,
		F, F_PRIME, F2,
		B, B_PRIME, B2,

		NULL_MOVE = UCHAR_MAX
	};

	std::ostream& operator<<(std::ostream& os, Move move);

	std::ostream& operator<<(std::ostream& os, const std::vector<Move>& moves);

	enum class Face : byte
	{
		R, L, U, D, F, B,

		NULL_FACE = UCHAR_MAX
	};

	[[nodiscard]] Face toFace(Move move) noexcept;
	[[nodiscard]] Face opposite(Face face) noexcept;

	class Cube
	{
	private:
		corner_arr cornerP_;
		corner_arr cornerO_;
		edge_arr edgeP_;
		edge_arr edgeO_;
		std::vector<Move> solution_;

		template <regi N>
		static void cycle4(std::array<byte, N>& arr, byte idx1, byte idx2, byte idx3, byte idx4) noexcept
		{
			byte temp = arr[idx4];
			arr[idx4] = arr[idx3];
			arr[idx3] = arr[idx2];
			arr[idx2] = arr[idx1];
			arr[idx1] = temp;
		}

		template <regi N>
		static void cycle2(std::array<byte, N>& arr, byte idx1, byte idx2, byte idx3, byte idx4) noexcept
		{
			byte temp = arr[idx1];
			arr[idx1] = arr[idx2];
			arr[idx2] = temp;

			temp = arr[idx3];
			arr[idx3] = arr[idx4];
			arr[idx4] = temp;
		}

		template <regi N>
		static bool checkParity(const std::array<byte, N>& arr) noexcept
		{
			bool parity = false;

			for (regi i = 0; i < N - 1; ++i)
			{
				for (regi j = i + 1; j < N; ++j)
				{
					if (arr[i] > arr[j]) {parity = !parity;}
				}
			}

			return parity;
		}
			
		void move(byte corner1, byte corner2, byte corner3, byte corner4,
				  byte edge1,   byte edge2,   byte edge3,   byte edge4  ) noexcept;

		void swap(byte corner1, byte corner2,
				  byte corner3, byte corner4,
				  byte edge1,   byte edge2,
				  byte edge3,   byte edge4) noexcept;

		void flipEdge(byte pos) noexcept;
		void twistCorner(byte pos, byte val) noexcept;

		static void checkPositions(const corner_arr& corners, const edge_arr& edges);
		static void checkOrientations(const corner_arr& corners, const edge_arr& edges);
		static void checkOrientationSum(const corner_arr& cornerO, const edge_arr& edgeO);
		static void checkPermutationParity(const corner_arr& corners, const edge_arr& edges);
		static void checkCube(const corner_arr& cornerP, const corner_arr& cornerO, const edge_arr& edgeP, const edge_arr& edgeO);

	public:
		Cube() noexcept;
		Cube(const std::string& moves);
		Cube(const corner_arr& cornerP, const corner_arr& cornerO, const edge_arr& edgeP, const edge_arr& edgeO);
		Cube(const Cube& other) noexcept;
		Cube& operator=(const Cube& other) noexcept;
		Cube(Cube&& other) noexcept;
		Cube& operator=(Cube&& other) noexcept;
		~Cube() = default;

		[[nodiscard]] const corner_arr&        cornerPositions()    const noexcept;
		[[nodiscard]] const corner_arr&        cornerOrientations() const noexcept;
		[[nodiscard]] const edge_arr&          edgePositions()      const noexcept;
		[[nodiscard]] const edge_arr&          edgeOrientations()   const noexcept;
		[[nodiscard]] const std::vector<Move>& solution()           const noexcept;
		[[nodiscard]]       Face               lastFace()           const noexcept;
		[[nodiscard]]  	  	Face               secondLastFace()	  	const noexcept;

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
		Cube& operator+=(Move move);
		Cube operator+(Move move) const;

		void applyMoves(const std::string& moves);
		void addMoves(const std::string& moves);
		Cube& operator+=(const std::string& moves);
		Cube operator+(const std::string& moves) const;

		void undo() noexcept;
		void operator--() noexcept;

		[[nodiscard]] bool operator==(const Cube& other) const noexcept;
		[[nodiscard]] bool equals(const Cube& other) const noexcept;
		[[nodiscard]] bool operator!=(const Cube& other) const noexcept;

		[[nodiscard]] bool isSolved() const noexcept;

		[[nodiscard]] bool pruneMove(Move move) const noexcept;

		friend std::ostream& operator<<(std::ostream& os, const Cube& other);
	};
}