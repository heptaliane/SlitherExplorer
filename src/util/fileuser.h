#ifndef FILEUSER_H_
#define FILEUSER_H_

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "utilarray.h"

using namespace std;

class Fileuser {
public:
    Fileuser(const string &url);
    explicit Fileuser(const Fileuser &obj);
    ~Fileuser();
    void getArray(ArrayD<short> *array);
    void saveArray(const ArrayD<short> &array, bool isClose = false);
private:
    bool readHeader(int *row_size, int *col_size);
    void writeHeader(int row_size, int col_size, ofstream *out) const;
    void readData(int row_size, int col_size, ArrayD<short> *array);
    void writeData(const ArrayD<short> &array, ofstream *out) const;
    bool importData();

    const string kHeader = "slt";
    unsigned int index = 0;
    string filename;
    vector<char> data;
    ofstream *fout = NULL;
};

#endif /* FILEUSER_H_ */
