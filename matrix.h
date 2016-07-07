#pragma once

#include <vector>

template <class T>
class Matrix {
    T *data;
    int h, w;
    int id;
    static int newId;

public:
    Matrix();
    Matrix(int h, int w);

    Matrix(const Matrix &m);
    Matrix(Matrix &&m);

    Matrix(const std::vector<std::vector<T>> &v);

    ~Matrix();

    Matrix &operator=(const Matrix &m);
    Matrix &operator=(Matrix &&m);

    int height() const;
    int width() const;

    T at(int i, int j) const;

    T *operator[](int i);

    std::vector<T> multiply(const std::vector<T> &v) const;
    std::vector<T> multiplyTransposed(const std::vector<T> &v) const;
    Matrix transposed() const;
};

template <class T>
int Matrix<T>::newId = 0;

#include <iostream>

template <class T>
Matrix<T>::Matrix()
    : data(0), h(0), w(0), id(newId++) {
    std::cout << "id: " << id << "Matrix()\n" << std::flush;
}

template <class T>
Matrix<T>::Matrix(int h, int w)
    : h(h), w(w), id(newId++) {
    std::cout << "id: " << id << " Matrix(int h, int w)\n" << std::flush;
    data = new T[h * w];
}

template <class T>
Matrix<T>::Matrix(const Matrix &m)
    : data(0), h(0), w(0), id(newId++) {
    std::cout << "id: " << id << " Matrix(const Matrix &m)\n" << std::flush;
    *this = m;
}

template <class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &v)
    : data(new T[v.size() * v[0].size()]), h(v.size()), w(v[0].size()), id(newId++) {
    std::cout << "id: " << id << " Matrix(const std::vector<std::vector<T>> &v)\n" << std::flush;
    for (int i = 0; i < h; i++)
        std::copy(v[i].begin(), v[i].end(), (*this)[i]);
}

template <class T>
Matrix<T>::Matrix(Matrix<T> &&m)
    : data(0), h(0), w(0), id(newId++) {
    std::cout << "id: " << id << " Matrix(Matrix<T> &&m)\n" << std::flush;
    *this = std::move(m);
}

template <class T>
Matrix<T>::~Matrix() {
    std::cout << "id: " << id << " ~Matrix()\n" << std::flush;
    std::cout << (int)data << "\n" << std::flush;
    delete[] data;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &m) {
    std::cout << "id: " << id << " operator=(const Matrix<T> &m)\n" << std::flush;
    delete[] data;

    h = m.h;
    w = m.w;

    data = new T[h * w];

    std::copy(m.data, m.data + h * w, data);

    return *this;
}

template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix<T> &&m) {
    std::cout << "id: " << id << " operator=(Matrix<T> &&m)\n" << std::flush;
    delete[] data;

    data = m.data;
    h = m.h;
    w = m.w;

    m.data = 0;
    m.h = 0;
    m.w = 0;

    return *this;
}

template <class T>
int Matrix<T>::height() const {
    return h;
}

template <class T>
int Matrix<T>::width() const {
    return w;
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
