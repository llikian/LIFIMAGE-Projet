/***************************************************************************************************
 * @file  MathematicalMorphology.hpp
 * @brief Declaration of the MathematicalMorphology namespace
 **************************************************************************************************/

#pragma once

#include "Array2D.hpp"

namespace MathematicalMorphology {
    enum StructuringElement : bool {
        Square = true,
        Cross  = false
    };

    constexpr int neighborsAmount(StructuringElement structuringElement) { return structuringElement ? 9 : 5; }

    int applyStructuringElement(const Array2D<bool>& data, int x, int y, StructuringElement structuringElement);

    Array2D<bool> dilate(const Array2D<bool>& data, StructuringElement structuringElement);
    Array2D<bool> erode(const Array2D<bool>& data, StructuringElement structuringElement);
}