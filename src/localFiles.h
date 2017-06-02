#ifndef LOCAL_FILES_H_
#define LOCAL_FILES_H_

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
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

class ImageBuilder {
public:

    explicit ImageBuilder(const std::string &path);

    void write(
            const std::string &path, const Matrix &cell,
            const Matrix &rgrid, const Matrix &cgrid,
            const Matrix &vertex) const;
private:

    void drawCell(int row, int col, int status, cv::Mat* image) const;

    void drawRow(int row, int col, int status, cv::Mat* image) const;

    void drawCol(int row, int col, int status, cv::Mat* image) const;

    void drawVertex(int row, int col, int status, cv::Mat* image) const;

    void setImagePixel(
            int rbegin, int cbegin, const Matrix &mat, cv::Mat* Image) const;

    static const cv::Vec3b foreColor;
    static const cv::Vec3b backColor;

    // 0 - 4 : cell
    // 5 - 7 : row
    // 8 - 10 : col
    // 11 - 17 : vertex
    Matrix data[18];
};

#endif
