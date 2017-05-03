#include "slarray.h"


Matrix::Matrix(int row, int col, char value) :
    exception(0xff), width(col), height(row), array(new char[col * row]) {

    init(value);
}

Matrix::Matrix(const Matrix &obj) :
    exception(obj.get(-1, 0)), width(obj.cols()), height(obj.rows()),
    array(new char[obj.cols() * obj.rows()]) {

    copy(obj);
}

Matrix::~Matrix() {
    delete array;
}

const char &Matrix::get(int row, int col) const {

    if (isIndexSafety(row, col)) {
        return array[row * width + col];
    }

    return exception;
}

void Matrix::set(int row, int col, char value) {

    if (isIndexSafety(row, col)) {
        array[row * width + col] = value;
    }
}

void Matrix::init(char value) {

    for (int i = 0; i < height * width; i++) {
        array[i] = value;
    }
}

void Matrix::resize(int row, int col) {

    delete array;
    array = new char[row * col];
    height = row;
    width = col;

    init(0);
}

void Matrix::setExceptValue(char value) {
    exception = value;
}

void Matrix::copy(const Matrix &obj) {

    resize(obj.rows(), obj.cols());

    const char* ptr = obj.getPointer();

    for (int i = 0; i < height * width; ++i) {
        array[i] = *ptr;
        ++ptr;
    }
}

bool Matrix::equals(const Matrix &obj) const {

    if (obj.rows() != height || obj.cols() != width) {
        return false;
    }

    const char* ptr = obj.getPointer();
    for (int i = 0; i < height * width; ++i) {
        if (*ptr != array[i]) {
            return false;
        }
        ++ptr;
    }

    return true;
}

