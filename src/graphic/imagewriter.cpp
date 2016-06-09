#include "imagewriter.h"

// --- constructor ---
ImageWriter::ImageWriter(const ArrayD<short> &cell) {
    cout << endl << "#=== LOADING IMAGE DATA PROCESS ===#" << endl;
    init();

    int width = cell.getlen1() * (kImgWidthLong + kImgWidthShort) + kImgWidthShort;
    int height = cell.getlen2() * (kImgHeightLong + kImgHeightShort) + kImgHeightShort;
    image = new cv::Mat(cv::Size(width, height), kImgType);
    image->setTo(cv::Scalar(kBackColor[0], kBackColor[1], kBackColor[2]));

    cout << "#=== ABOUT GENERATED IMAGE ===#" << endl;
    cout << " * WIDTH = " << width << endl;
    cout << " * HEIGHT = " << height << endl;

    cout << endl << "Image preparation.. ";
    for (int i = 0; i < cell.getlen1(); i++) {
        for (int j = 0; j < cell.getlen2(); j++) {
            writeCell(i, j, cell.getArray(i, j));
        }
    }
}
ImageWriter::ImageWriter() {
    cout << endl << "#=== LOADING IMAGE DATA PROCESS ===#" << endl;
    init();
}
ImageWriter::~ImageWriter() {
    delete image;
    image = NULL;
}

// --- public ---
void ImageWriter::exportImg(const string &url) {
    cv::imwrite(url, *image);
}
void ImageWriter::exportImg(const string &url,
               const ArrayD<short> &row_grid,
               const ArrayD<short> &col_grid) {
    ArrayD<short> vertex(row_grid.getlen1(), col_grid.getlen2(), static_cast<short>(0));
    exportImg(url, row_grid, col_grid, vertex);
}
void ImageWriter::exportImg(const string &url,
               const ArrayD<short> &row_grid,
               const ArrayD<short> &col_grid,
               const ArrayD<short> &vertex) {
    for (int i = 0; i < row_grid.getlen1(); i++) {
        for (int j = 0; j < row_grid.getlen2(); j++) {
            writeRow(i, j, row_grid.getArray(i, j));
        }
    }
    for (int i = 0; i < col_grid.getlen1(); i++) {
        for (int j = 0; j < col_grid.getlen2(); j++) {
            writeCol(i, j, col_grid.getArray(i, j));
        }
    }
    for (int i = 0; i < vertex.getlen1(); i++) {
        for (int j = 0; j < vertex.getlen2(); j++) {
            writeVertex(i, j, vertex.getArray(i, j));
        }
    }

    imwrite(url, *image);
}
void ImageWriter::writeCell(int row, int col, short state) {
    if (state < 0 || state >= kCellImageNum) state = 4;

    int beginRows = (kImgHeightLong + kImgHeightShort) * row + kImgHeightShort;
    int beginCols = (kImgWidthLong + kImgWidthShort) * col + kImgWidthShort;

    setImagePixel(beginRows, beginCols, cell_pix[state]);
}
void ImageWriter::writeActive(int row, int col, bool isActivate) {
    int beginRows = (kImgHeightLong + kImgHeightShort) * row + kImgHeightShort;
    int beginCols = (kImgWidthLong + kImgWidthShort) * col + kImgWidthShort;

    int pixRows = 0, pixCols = 0;
    for (int y = 0; y < active_pix.getlen1(); y++) {
        cv::Vec3b *ptr = image->ptr<cv::Vec3b>(y);
        for (int x = 0; x < active_pix.getlen2(); x++) {
            pixRows = beginRows + x;
            pixCols = beginCols + y;

            if (pixRows > image->rows || pixCols > image->cols) {
                cout << "invalid index is called" << endl;
                cout << "image size : " << image->rows << 'x' << image->cols << endl;
                cout << "accessed pixel : (" << pixRows << ',' << pixCols << ')' << endl;
                return;
            }

            if (active_pix.getArray(y, x) == 1) {
                if (isActivate)
                    ptr[x] = kActiveColor;
                else
                    ptr[x] = kBackColor;
            }
        }
    }
}

// --- private ---
void ImageWriter::init() {
    cout << "initalizing ImageWriter..";

    string url = kRowDataUrl;
    for (int i = 0; i < kRowImageNum; i++) {
        loadPixelData(url, &row_pix[i]);
        url[kRowDirIndex]++;
    }

    url = kColDataUrl;
    for (int j = 0; j < kColImageNum; j++) {
        loadPixelData(url, &col_pix[j]);
        url[kColDirIndex]++;
    }

    url = kVertexDataUrl;
    for (int k = 0; k < kVertexImageNum; k++) {
        loadPixelData(url, &vertex_pix[k]);
        url[kVertexDirIndex]++;
    }

    url = kCellDataUrl;
    for (int l = 0; l < kCellImageNum; l++) {
        loadPixelData(url, &cell_pix[l]);
        url[kCellDirIndex]++;
    }

    loadPixelData(kActiveDataUrl, &active_pix);

    cout << "Done" << endl;
}

void ImageWriter::loadPixelData(const string &url, ArrayD<char> *pixel) {
    cout << "loading " << url << ".. ";

    ifstream fin(url, ios::binary);
    if (!fin) {
        cout << "Failed" << endl;
        cout << " * cannot open " << url << endl;
        return;
    }

    int row = 0, col = 0, type = 0;
    fin.read(reinterpret_cast<char *>(&row), sizeof(row));
    fin.read(reinterpret_cast<char *>(&col), sizeof(col));
    fin.read(reinterpret_cast<char *>(&type), sizeof(type));

    ArrayD<char> temp(row, col);
    char c = 0;
    for (int i = 0; i < pixel->getlen1(); i++) {
        for (int j = 0; j < pixel->getlen2(); j++) {
            fin.read(&c, sizeof(c));
            temp.setArray(i, j, c);
        }
    }
    fin.close();
    pixel->clone(temp);

    cout << "Done" << endl;
    cout << "size " << row << 'x' << col << endl;
}
void ImageWriter::setImagePixel(int beginRows,
                                int beginCols,
                                const ArrayD<char> &pixData) {
    int pixRows = 0, pixCols = 0;
    for (int y = 0; y < pixData.getlen1(); y++) {
        cv::Vec3b *ptr = image->ptr<cv::Vec3b>(y);
        for (int x = 0; x < pixData.getlen2(); x++) {
            pixRows = beginRows + x;
            pixCols = beginCols + y;

            if (pixRows > image->rows || pixCols > image->cols) {
                cout << "invalid index is called" << endl;
                cout << "image size : " << image->rows << 'x' << image->cols << endl;
                cout << "accessed pixel : (" << pixRows << ',' << pixCols << ')' << endl;
                return;
            }

            if (pixData.getArray(y, x) == 1) {
                ptr[x] = kForeColor;
            } else {
                ptr[x] = kBackColor;
            }
        }
    }
}
void ImageWriter::writeRow(int row, int col, short state) {
    if (state < 0 || state >= kRowImageNum) state = 2;

    int beginRows = (kImgHeightLong + kImgHeightShort) * row;
    int beginCols = (kImgWidthLong + kImgWidthShort) * col + kImgWidthShort;

    setImagePixel(beginRows, beginCols, row_pix[state]);
}
void ImageWriter::writeCol(int row, int col, short state) {
    if (state < 0 || state >= kColImageNum) state = 2;

    int beginRows = (kImgHeightLong + kImgHeightShort) * row + kImgHeightShort;
    int beginCols = (kImgWidthLong + kImgWidthShort) * col;

    setImagePixel(beginRows, beginCols, col_pix[state]);
}
void ImageWriter::writeVertex(int row, int col, short state) {
    if (state < 0 || state > kVertexImageNum) state = 0;

    int beginRows = (kImgHeightLong + kImgHeightShort) * row;
    int beginCols = (kImgWidthLong + kImgWidthShort) * col;

    setImagePixel(beginRows, beginCols, vertex_pix[state]);
}

