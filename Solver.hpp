#pragma once

#include "Cube.hpp"
#include <thread>
#include <mutex>
#include <atomic>

namespace slvr
{
    namespace nogroup
    {
        bool dfs(Cube& cube, regi depth, regi maxDepth);
    }

    namespace thistlethwaite
    {
        enum class State : byte
        {
            G0, // < U,  D,  R,  L,  F,  B>
            G1, // < U,  D,  R,  L, F2, B2>
            G2, // < U,  D, R2, L2, F2, B2>
            G3, // <U2, D2, R2, L2, F2, B2>
            G4  // <>
        };

        constexpr const std::array<Move, 18> g0Moves {Move::R, Move::R_PRIME, Move::R2,
		                                              Move::L, Move::L_PRIME, Move::L2,
                                                      Move::U, Move::U_PRIME, Move::U2,
                                                      Move::D, Move::D_PRIME, Move::D2,
                                                      Move::F, Move::F_PRIME, Move::F2,
                                                      Move::B, Move::B_PRIME, Move::B2};
        
        constexpr const std::array<Move, 14> g1Moves {Move::R, Move::R_PRIME, Move::R2,
		                                              Move::L, Move::L_PRIME, Move::L2,
                                                      Move::U, Move::U_PRIME, Move::U2,
                                                      Move::D, Move::D_PRIME, Move::D2,
                                                      Move::F2,
                                                      Move::B2};

        constexpr const std::array<Move, 10> g2Moves {Move::R2,
		                                              Move::L2,
                                                      Move::U, Move::U_PRIME, Move::U2,
                                                      Move::D, Move::D_PRIME, Move::D2,
                                                      Move::F2,
                                                      Move::B2};

        constexpr const std::array<Move,  6> g3Moves {Move::R2,
		                                              Move::L2,
                                                      Move::U2,
                                                      Move::D2,
                                                      Move::F2,
                                                      Move::B2};

        template <State G>
        constexpr const auto& validMoves() noexcept;

        template<> constexpr const auto& validMoves<State::G0>() noexcept {return g0Moves;}
        template<> constexpr const auto& validMoves<State::G1>() noexcept {return g1Moves;}
        template<> constexpr const auto& validMoves<State::G2>() noexcept {return g2Moves;}
        template<> constexpr const auto& validMoves<State::G3>() noexcept {return g3Moves;}

        constexpr const byte numG0(18),
                             numG1(14),
                             numG2(10),
                             numG3( 6);

        template <State G>
        constexpr const byte numMoves() noexcept;

        template<> constexpr const byte numMoves<State::G0>() noexcept {return numG0;}
        template<> constexpr const byte numMoves<State::G1>() noexcept {return numG1;}
        template<> constexpr const byte numMoves<State::G2>() noexcept {return numG2;}
        template<> constexpr const byte numMoves<State::G3>() noexcept {return numG3;}
        
        [[nodiscard]] bool inG1(const Cube& cube) noexcept;
        [[nodiscard]] bool inG2(const Cube& cube) noexcept;
        [[nodiscard]] bool inG3(const Cube& cube) noexcept;

        [[nodiscard]] State state(const Cube& cube) noexcept;

        template <State G>
        bool dfsNextGroup(Cube& cube, regi depth, regi maxDepth, std::atomic<bool>& solutionFound)
        {
            if (static_cast<byte>(G) < static_cast<byte>(state(cube))) {return true;}
            if (depth == maxDepth) {return false;}
            if (solutionFound.load(std::memory_order_relaxed)) {return false;}

            auto moves = validMoves<G>();

            for (Move move : moves)
            {
                if (cube.pruneMove(move)) {continue;}

                cube += move;

                if (solutionFound.load(std::memory_order_relaxed)) {cube.undo(); return false;}

                if (dfsNextGroup<G>(cube, depth + 1, maxDepth, solutionFound)) {return true;}

                cube.undo();
            }

            return false;
        }
        
        template<> inline bool dfsNextGroup<State::G4>(Cube& cube, regi depth, regi maxDepth, std::atomic<bool>& solutionFound) 
        {return false;}

        template <State G>
        Cube dfsNextGroupThread(Cube& cube, regi depth, regi maxDepth)
        {
            if (static_cast<byte>(G) < static_cast<byte>(state(cube))) {return cube;}
            if (depth == maxDepth) {return Cube();}

            std::atomic<bool> solutionFound(false);
            std::mutex mtx;
            Cube result;

            auto moves = validMoves<G>();
            const byte n = numMoves<G>();

            std::array<std::thread, n> threads;

            for (byte i = 0; i < n; ++i)
            {
                Cube next = cube + moves[i];

                threads[i] = std::thread([&, next = std::move(next)] mutable 
                {
                    if (solutionFound.load(std::memory_order_relaxed)) {return;}

                    if (dfsNextGroup<G>(next, depth + 1, maxDepth, solutionFound))
                    {
                        if (!solutionFound.exchange(true, std::memory_order_relaxed)) 
                        {
                            std::lock_guard<std::mutex> lock(mtx);

                            result = std::move(next);
                        }
                    }
                });

                if (solutionFound.load()) {break;}
            }

            for (auto& t : threads)
            {
                if (t.joinable()) {t.join();}
            }

            return result;
        }

        template<> inline Cube dfsNextGroupThread<State::G4>(Cube& cube, regi depth, regi maxDepth) {return Cube();}
    }

    namespace kociemba
    {

    }
}