#include "Solver.hpp"

namespace slvr
{
    namespace nogroup
    {
        bool dfs(Cube& cube, regi depth, regi maxDepth)
        {
            if (cube.isSolved()) {return true;}
            if (depth == maxDepth) {return false;}

            for (byte i = 0; i < 18; ++i)
            {
                Move move = static_cast<Move>(i);

                Face cubeFace = cube.lastFace();
                Face moveFace = toFace(move);

                if (cubeFace == moveFace) {continue;}

                if (moveFace == opposite(cubeFace) && moveFace == cube.secondLastFace()) {continue;}

                cube += move;

                if (dfs(cube, depth + 1, maxDepth)) {return true;}

                cube.undo();
            }

            return false;
        }
    }

    namespace thistlethwaite
    {
        bool inG1(const Cube& cube) noexcept
        {
            if (cube.edgeOrientations() != edge_arr{0}) {return false;}

            return true;
        }

        bool inG2(const Cube& cube) noexcept
        {
            if (!inG1(cube)) {return false;}

            if (cube.cornerOrientations() != corner_arr{0}) {return false;}

            for (byte i = 4; i < 8; ++i)
            {
                if (cube.edgePositions()[i] < 4 || cube.edgePositions()[i] > 7) {return false;}
            }

            return true;
        }

        bool inG3(const Cube& cube) noexcept
        {
            if (!inG2(cube)) {return false;}

            corner_arr corners(cube.cornerPositions());
            edge_arr edges(cube.edgePositions());

            for (byte i = 0; i < 8; ++i)
            {
                byte corner = corners[i];

                if (i == 0 || i == 2 || i == 5 || i == 7)
                {
                    if (corner != 0 && corner != 2 && corner != 5 && corner != 7) {return false;}
                }
            }

            for (byte i = 0; i < 12; ++i)
            {
                byte edge = edges[i];

                if (i == 0 || i == 2 || i == 8 || i == 10)
                {
                    if (edge != 0 && edge != 2 && edge != 8 && edge != 10) {return false;}
                }

                if (i == 1 || i == 3 || i == 9 || i == 11)
                {
                    if (edge != 1 && edge != 3 && edge != 9 && edge != 11) {return false;}
                }
            }

            return true;
        }

        State state(const Cube& cube) noexcept
        {
            if (!inG1(cube)) {return State::G0;}
            if (!inG2(cube)) {return State::G1;}
            if (cube.isSolved()) {return State::G4;}
            if (!inG3(cube)) {return State::G2;}
            return State::G3;
        }
    }

    namespace kociemba
    {

    }
}