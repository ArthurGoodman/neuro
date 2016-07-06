#pragma once

#include <vector>

template <class T>
class Matrix {
    T *data;
    int w, h;

public:
    Matrix();
    Matrix(int w, int h);

    Matrix(const Matrix &m);
    Matrix(Matrix &&m);

    Matrix(const std::vector<std::vector<T>> &v);

    ~Matrix();

    Matrix &operator=(const Matrix &m);
    Matrix &operator=(Matrix &&m);

    int width() const;
    int height() const;

    T at(int i, int j) const;

    T *operator[](int i);

    std::vector<T> multiply(const std::vector<T> &v) const;
    std::vector<T> multiplyTransposed(const std::vector<T> &v) const;
    Matrix transposed() const;
};

template <class T>
Matrix<T>::Matrix()
    : data(0) {
}

template <class T>
Matrix<T>::Matrix(int w, int h)
    : w(w), h(h) {
    data = new T[w * h];
}

template <class T>
Matrix<T>::Matrix(const Matrix &m)
    : w(m.w), h(m.h) {
    data = new T[w * h];

    for (int i = 0; i < w * h; i++)
        data[i] = m.data[i];
}

template <class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &v) {
    w = v[0].size();
    h = v.size();

    data = new T[w * h];

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            (*this)[i][j] = v[i][j];
}

template <class T>
Matrix<T>::Matrix(Matrix<T> &&m)
    : data(m.data), w(m.w), h(m.h) {
    m.data = 0;
    m.w = 0;
    m.h = 0;
}

template <class T>
Matrix<T>::~Matrix() {
    delete[] data;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m) {
    w = m.w;
    h = m.h;

    delete[] data;

    data = new T[w * h];

    for (int i = 0; i < w * h; i++)
        data[i] = m.data[i];

    return *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&m) {
    w = m.w;
    h = m.h;

    delete[] data;

    data = m.data;

    m.data = 0;
    m.w = 0;
    m.h = 0;

    return *this;
}

template <class T>
int Matrix<T>::width() const {
    return w;
}

template <class T>
int Matrix<T>::height() const {
    return h;
}

template <class T>
T Matrix<T>::at(int i, int j) const {
    return data[i * w + j];
}

template <class T>
T *Matrix<T>::operator[](int i) {
    return data + i * w;
}

template <class T>
std::vector<T> Matrix<T>::multiply(const std::vector<T> &v) const {
    std::vector<T> result(w);
    std::fill(result.begin(), result.end(), (T)0);

    for (int j = 0; j < w; j++)
        for (int i = 0; i < h; i++)
            result[j] += at(i, j) * v[i];

    return result;
}

template <class T>
std::vector<T> Matrix<T>::multiplyTransposed(const std::vector<T> &v) const {
    std::vector<T> result(h);
    std::fill(result.begin(), result.end(), (T)0);

    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++)
            result[j] += at(j, i) * v[i];

    return result;
}

template <class T>
Matrix<T> Matrix<T>::transposed() const {
    Matrix t(h, w);

    for (int j = 0; j < w; j++)
        for (int i = 0; i < h; i++)
            t[j][i] = at(i, j);

    return t;
}
