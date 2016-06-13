#ifndef UTILARRAY_H_
#define UTILARRAY_H_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

//---ArrayD---
template<class T>
class ArrayD {
public:
    ArrayD() {
        len1 = 0;
        len2 = 0;
    }
    ArrayD(int xlen, int ylen) {
        init(xlen, ylen);
    }
    ArrayD(int xlen, int ylen, const T &value) {
        init(xlen, ylen);
        resetValue(value);
    }
    explicit ArrayD(const ArrayD<T> &obj) {
        len1 = obj.getlen1();
        len2 = obj.getlen2();
        array = new T[len1 * len2];
    }
    ~ArrayD() {
        delete array;
        array = NULL;
    }

    T getArray(int index1, int index2) const {
        //ensure the safety of array
        if (index1 >= len1 || index2 >= len2 || index1 < 0 || index2 < 0) {
            arrayConsoleLogger(index1, index2);
            return -1;
        }
        //if there is reference to array, this returns value of relevant index
        return array[index1 * len2 + index2];
    }
    void setArray(int index1, int index2, const T &obj) {
        //ensure the safety of array
        if (index1 >= len1 || index2 >= len2 || index1 < 0 || index2 < 0) {
            arrayConsoleLogger(index1, index2);
        }
        //if there is no reference to out of array, this returns value of relevant index
        array[index1 * len2 + index2] = obj;
    }
    //this sets all components of array the value of argument.
    void resetValue(const T &value) {
        for (int i = 0; i < len1; i++) {
            for (int j = 0; j < len2; j++) {
                setArray(i, j, value);
            }
        }
    }
    //clone component of array
    void clone(const ArrayD<T> &obj) {
        if (obj.getlen1() != len1 || obj.getlen2() != len2) {
            init(obj.getlen1(), obj.getlen2());
        }
        for (int i = 0; i < len1; i++) {
            for (int j = 0; j < len2; j++) {
                setArray(i, j, obj.getArray(i, j));
            }
        }
    }
    //get array length
    inline int getlen1() const { return len1; }
    inline int getlen2() const { return len2; }
private:
    void init(int xlen, int ylen) {
        delete array;
        array = NULL;
        len1 = xlen;
        len2 = ylen;
        array = new T[len1 * len2];
    }
    //print error log
    inline void arrayConsoleLogger(int x, int y) const {
        cout << "ArrayIndexOutOfBoundsException" << endl;
        cout << "array size : " << len1 << " x " << len2 << endl;
        cout << "index : [" << x << "][" << y << "]" << endl;
    }

    T *array = NULL;
    int len1, len2;
    T buffer;
};

//---Utilarray--
class Utilarray {
public:
    Utilarray(const ArrayD<short> &obj);
    Utilarray(int row_size, int col_size);
    Utilarray(int row_size, int col_size, int value);
    Utilarray(int row_size, int col_size, int value, int reserve_size);
    explicit Utilarray(const Utilarray &obj);
    ~Utilarray();

    //get array value
    //if there is no reference, this returns exceptional value
    short get(int row, int col, int exception) const;
    //store the value
    void set(int row, int col, int value);
    void undo();
    void clone(const Utilarray &obj);
    //delete stored array and its log
    void clear(int value = 0);
    bool equals(const Utilarray &obj) const;
    //get array length
    inline int getlen1() const { return array->getlen1(); }
    inline int getlen2() const { return array->getlen2(); }
    //return vector "log" is reserved or not
    inline bool getLogReservedProperty() const { return is_log_reserved; }
    inline int getLogReservedSize() const { return log_reserved_size; }
    void exportLog(vector<pair<pair<int, int>, short> > *vec) const;
    void exportArray(ArrayD<short> *obj) const;
private:
    void init(int row_size, int col_size);
    //write to array_log
    void setLog(int row, int col);
    //delete the oldest log
    void killLogStack();

    bool is_log_reserved = false;
    //reserved memory size of log
    unsigned int log_reserved_size = 0;
    ArrayD<short> *array;
    //store changed coodinate and its value
    vector<pair<pair<int, int>, short> > log;
};

#endif /* UTILARRAY_H_ */
