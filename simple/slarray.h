#ifndef SLITHER_ARRAY_H_
#define SLITHER_ARRAY_H_

#include <cstddef>

struct Coord {
public:

    Coord() :
        x(0), y(0) {};

    Coord(int xvalue, int yvalue) :
        x(xvalue), y(yvalue) {};

    Coord(const Coord &obj) :
        x(obj.x), y(obj.y) {};

    int x;
    int y;
};


class Matrix {
public:

    Matrix() :
        width(0), height(0), array(NULL) {};

    Matrix(int row, int col, char value = 0);

    Matrix(const Matrix &obj);

    ~Matrix();

    // get matrix value
    const char &get(int row, int col) const;

    // set matrix value
    void set(int row, int col, char value);

    // set all
    void init(char value);

    void resize(int row, int col);

    // copy matrix
    void copy(const Matrix &obj);

    // get first pointer
    const char* getPointer() const {
        return array;
    }

    // get width
    const int &cols() const { return width; }

    // get height
    const int &rows() const { return height; }

private:

    bool isIndexSafety(int row, int col) const {
        return (row < height) && (col < width) && (row >= 0) && (col >= 0);
    }

    static const char exception;
    int width;
    int height;
    char* array;

};

#endif /* SLITHER_ARRAY_H_ */
