#ifndef LOCAL_FILES_H_
#define LOCAL_FILES_H_

#include <fstream>
#include <vector>
#include <string>
#include "slarray.h"

class LocalFileDealer {
public:

    LocalFileDealer() :
        length(0), width(0), height(0) {};

    void popMatrix(Matrix *mat);

    void pushMatrix(const Matrix &mat);

    void clear();

    void read(const std::string &path);

    void write(const std::string &path);

private:

    static const std::string identifier;
    static const int headerSize;

    LocalFileDealer(const LocalFileDealer &obj);

    void writeHeader();

    void readHeader();

    void writeData(const Matrix &mat);

    void readData();

    int length;
    int width;
    int height;
    std::vector<char> header;
    std::vector<char> data;
};

#endif
