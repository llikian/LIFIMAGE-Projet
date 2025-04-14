#include "analyse/Hull.hpp"
#include <analyse/uvec2.hpp>

void Hull::bool_array_to_uvec2_vector(const Array2D<bool>& outline) {
    for (unsigned int x = 0; x < outline.rows; ++x) {
        for (unsigned int y = 0; y < outline.columns; ++y) {
            if (outline(x, y)) {
                outline_vector.emplace_back(x, y);
            }
        }
    }
}

Array2D<bool> Hull::hull_to_bool_array(unsigned int rows, unsigned int columns) {
    Array2D<bool> h(rows, columns, false);
    for (uvec2 &p : hull) {
        h(p.x, p.y) = true;
    }
    return h;
}

void Hull::quickhull(const Array2D<bool>& outline) {
    bool_array_to_uvec2_vector(outline);

    int leftmost = 0, rightmost = 0;
    for (int i = 0; i < outline_vector.size(); ++i) {
        if (outline_vector[i].x < outline_vector[leftmost].x) {
            leftmost = i;
        }
        if (outline_vector[i].x > outline_vector[rightmost].x) {
            rightmost = i;
        }
    }
    hull.push_back(outline_vector[leftmost]);
    hull.push_back(outline_vector[rightmost]);

    std::vector<uvec2> top = set_on_right_of_line(outline_vector, outline_vector[leftmost], outline_vector[rightmost]);
    std::vector<uvec2> bottom = set_on_right_of_line(outline_vector, outline_vector[rightmost], outline_vector[leftmost]);

    find_hull(top, outline_vector[leftmost], outline_vector[rightmost]);
    find_hull(bottom, outline_vector[rightmost], outline_vector[leftmost]);
}

bool is_on_right(const uvec2& a, const uvec2& b, const uvec2& p) {
    int cross = (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x);
    return cross <= 0;
}

std::vector<uvec2> Hull::set_on_right_of_line(const std::vector<uvec2>& set, uvec2 a, uvec2 b) {
    std::vector<uvec2> subset;
    for (const uvec2 &p : set) {
        if (is_on_right(a, b, p)
            && p != a && p != b) {
            subset.push_back(p);
        }
    }
    return subset;
}

void Hull::find_hull(const std::vector<uvec2> &subset, const uvec2& a, const uvec2& b) {
    if (subset.empty()) {
        hull.push_back(a);
        hull.push_back(b);
        return;
    }

    unsigned int farthest = 0;
    float max_area = 0.0f;
    for (int i=0; i < subset.size(); ++i) {
        float area = triangle_area(a, subset[i], b);
        if ( area >= max_area) {
            max_area = area;
            farthest = i;
        }
    }

    if (max_area <= 0.0f) {
        for (const uvec2 &p : subset) {
            hull.push_back(p);
        }
        return;
    }

    uvec2 p = subset[farthest];
    std::vector<uvec2> outside_triangle;
    for (const uvec2 &point : subset) {
        if (!is_in_triangle(point, a, b, p)) {
            outside_triangle.push_back(point);
        }
    }

    hull.push_back(p);
    find_hull(set_on_right_of_line(outside_triangle, a, p), a, p);
    find_hull(set_on_right_of_line(outside_triangle, p, b), p, b);
}

Array2D<bool> Hull::do_hull(const Array2D<bool>& outline) {
    outline_vector.clear();
    hull.clear();
    quickhull(outline);
    Array2D<bool> h = hull_to_bool_array(outline.rows, outline.columns);
    return h;
}
