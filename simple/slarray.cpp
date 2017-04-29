#include "slarray.h"


const char Matrix::exception = 0xff;

Matrix::Matrix(int row, int col, char value) :
    width(col), height(row), array(new char[col * row]) {

    init(value);
}

Matrix::Matrix(const Matrix &obj) :
    width(obj.cols()), height(obj.rows()),
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

void Matrix::copy(const Matrix &obj) {

    resize(obj.rows(), obj.cols());

    const char* pointer = obj.getPointer();

    for (int i = 0; i < height * width; i++) {
        array[i] = *pointer;
        pointer++;
    }
}


