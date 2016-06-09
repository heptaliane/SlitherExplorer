#ifndef IMAGEWRITER_H_
#define IMAGEWRITER_H_

#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include "../util/utilarray.h"

using namespace std;

class ImageWriter {
public:
    ImageWriter(const ArrayD<short> &cell);
    ImageWriter();
    ~ImageWriter();
    void exportImg(const string &url);
    void exportImg(const string &url,
                   const ArrayD<short> &row_grid,
                   const ArrayD<short> &col_grid);
    void exportImg(const string &url,
                   const ArrayD<short> &row_grid,
                   const ArrayD<short> &col_grid,
                   const ArrayD<short> &vertex);
    void writeCell(int row, int col, short state);
    void writeActive(int row, int col, bool isActivate = true);
private:
    const string kRowDataUrl = "../lib/row/0.dat";
    const string kColDataUrl = "../lib/col/0.dat";
    const string kVertexDataUrl= "../lib/vertex/0.dat";
    const string kCellDataUrl= "../lib/cell/0.dat";
    const string kActiveDataUrl= "../lib/cell/active.dat";
    const int kRowDirIndex = sizeof("../lib/row/");
    const int kColDirIndex = sizeof("../lib/col/");
    const int kVertexDirIndex = sizeof("../lib/vertex/");
    const int kCellDirIndex = sizeof("../lib/cell/");
    const int kRowImageNum = 3;
    const int kColImageNum = 3;
    const int kVertexImageNum = 7;
    const int kCellImageNum = 5;
    const int kImgWidthLong = 40;
    const int kImgWidthShort = 20;
    const int kImgHeightLong = 40;
    const int kImgHeightShort = 20;
    const int kImgType = 16;
    const cv::Vec3b kForeColor = cv::Vec3b(0x00, 0x00, 0x00);
    const cv::Vec3b kBackColor = cv::Vec3b(0xff, 0xff, 0xff);
    const cv::Vec3b kActiveColor = cv::Vec3b(0x00, 0x00, 0xff);

    void init();
    void loadPixelData(const string &url, ArrayD<char> *pixel);
    void setImagePixel(int beginRows, int beginCols, const ArrayD<char> &pixData);
    void writeRow(int row, int col, short state);
    void writeCol(int row, int col, short state);
    void writeVertex(int row, int col, short state);

    //loaded image data first parameter = height, second parameter = width
    ArrayD<char> row_pix[3];
    ArrayD<char> col_pix[3];
    ArrayD<char> vertex_pix[7];
    ArrayD<char> cell_pix[5];
    ArrayD<char> active_pix;
    cv::Mat *image = NULL;
};

#endif /* IMAGEWRITER_H_ */
