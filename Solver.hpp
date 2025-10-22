#pragma once

#include "Cube.hpp"

namespace slvr
{
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
        
        bool inG1(const Cube& cube) noexcept;
        bool inG2(const Cube& cube) noexcept;
        bool inG3(const Cube& cube) noexcept;

        State state(const Cube& cube) noexcept;
    }

    namespace kociemba
    {

    }
}