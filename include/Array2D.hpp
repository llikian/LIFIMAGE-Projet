/***************************************************************************************************
 * @file  Array2D.hpp
 * @brief Declaration of the Array2D struct
 **************************************************************************************************/

#pragma once

template <typename Type>
struct Array2D {
    Array2D(unsigned int rows, unsigned int columns);
    ~Array2D();

    Array2D(const Array2D& array);
    Array2D& operator=(const Array2D& array);

    Type& operator ()(int x, int y);
    const Type& operator ()(int x, int y) const;

    unsigned int rows;
    unsigned int columns;

    Type** data;
};

template <typename Type>
Array2D<Type>::Array2D(unsigned int rows, unsigned int columns)
    : rows(rows), columns(columns), data(new Type*[rows]) {
    for(unsigned int i = 0 ; i < rows ; ++i) {
        data[i] = new Type[columns];

        for(unsigned int j = 0 ; j < columns ; ++j) {
            data[i][j] = Type();
        }
    }
}

template <typename Type>
Array2D<Type>::~Array2D() {
    for(unsigned int i = 0 ; i < columns ; ++i) {
        delete[] data[i];
    }

    delete data;
}

template <typename Type>
Array2D<Type>::Array2D(const Array2D& array)
    : rows(array.rows), columns(array.columns), data(new Type*[rows]) {
    for(unsigned int i = 0 ; i < rows ; ++i) {
        data[i] = new Type[columns];

        for(unsigned int j = 0 ; j < columns ; ++j) {
            data[i][j] = array(i, j);
        }
    }
}

template <typename Type>
Array2D<Type>& Array2D<Type>::operator=(const Array2D& array) {
    if(rows == array.rows && columns == array.columns && data != nullptr) {
        for(unsigned int i = 0 ; i < rows ; ++i) {
            for(unsigned int j = 0 ; j < columns ; ++j) {
                data[i][j] = array(i, j);
            }
        }
    } else {
        if(data != nullptr) {
            for(unsigned int i = 0 ; i < columns ; ++i) {
                delete[] data[i];
            }
            delete data;
        }

        rows = array.rows;
        columns = array.columns;
        data = new Type*[rows];

        for(unsigned int i = 0 ; i < rows ; ++i) {
            data[i] = new Type[columns];

            for(unsigned int j = 0 ; j < columns ; ++j) {
                data[i][j] = array(i, j);
            }
        }
    }

    return *this;
}

template <typename Type>
Type& Array2D<Type>::operator()(int x, int y) {
    return data[x][y];
}

template <typename Type>
const Type& Array2D<Type>::operator()(int x, int y) const {
    return data[x][y];
}
