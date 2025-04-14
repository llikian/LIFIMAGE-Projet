#pragma once

#include "Array2D.hpp"
#include "uvec2.hpp"
#include "utility.hpp"
#include <vector>

struct Hull {
    std::vector<uvec2> outline_vector;
    std::vector<uvec2> hull;

    Hull()=default;
    explicit Hull(const std::vector<uvec2>& hull) : hull(hull) {}

    void bool_array_to_uvec2_vector(const Array2D<bool>& outline);
    Array2D<bool> hull_to_bool_array(unsigned int rows, unsigned int columns);

    void quickhull(const Array2D<bool>& outline);
    void find_hull(const std::vector<uvec2> &subset, const uvec2& a, const uvec2& b);
    std::vector<uvec2> set_on_right_of_line(const std::vector<uvec2>& set, uvec2 a, uvec2 b);

    Array2D<bool> do_hull(const Array2D<bool>& outline);
};