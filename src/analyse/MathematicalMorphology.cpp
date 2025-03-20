/***************************************************************************************************
 * @file  MathematicalMorphology.cpp
 * @brief Implementation of the MathematicalMorphology namespace
 **************************************************************************************************/

#include "analyse/MathematicalMorphology.hpp"

#include <utility>

int MathematicalMorphology::applyStructuringElement(const Array2D<bool>& data, int x, int y,
                                                    StructuringElement structuringElement) {
    static constexpr std::pair<int, int> offsets[9]{
        /* Offsets Present for Both the Square and the Cross */
        { -1, 0 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 1, 0 },
        /* Offsets Only Present for the Square */
        { -1, -1 }, { -1, 1 }, { 1, -1 }, { 1, 1 }
    };

    unsigned int n = 0;

    for(int i = 0 ; i < neighborsAmount(structuringElement) ; ++i) {
        unsigned int offsetX = x + offsets[i].first;
        unsigned int offsetY = y + offsets[i].second;

        if(offsetX >= data.rows || offsetY >= data.columns) { continue; }

        if(data(offsetX, offsetY) == 0) { ++n; }
    }

    return n;
}

Array2D<bool> MathematicalMorphology::dilate(const Array2D<bool>& data,
                                             StructuringElement structuringElement) {
    Array2D<bool> result(data.rows, data.columns);

    for(unsigned int i = 0 ; i < data.rows ; ++i) {
        for(unsigned int j = 0 ; j < data.columns ; ++j) {
            result(i, j) = applyStructuringElement(data, i, j, structuringElement) == 0;
        }
    }

    return result;
}

Array2D<bool> MathematicalMorphology::erode(const Array2D<bool>& data,
                                            StructuringElement structuringElement) {
    Array2D<bool> result(data.rows, data.columns);

    int neighbors = neighborsAmount(structuringElement);

    for(unsigned int i = 0 ; i < data.rows ; ++i) {
        for(unsigned int j = 0 ; j < data.columns ; ++j) {
            result(i, j) = applyStructuringElement(data, i, j, structuringElement) < neighbors;
        }
    }

    return result;
}
