#include "localFiles.h"


const std::string LocalFileDealer::identifier = "slitherlink";

const int LocalFileDealer::headerSize = 14;

void LocalFileDealer::popMatrix(Matrix *mat) {

    if (length == 0) {
        mat->resize(0, 0);
        return;
    }

    mat->resize(width, height);

    int i, j;
    for (i = height - 1; i >= 0; --i) {
        for (j = width - 1; j >= 0; --j) {
            mat->set(i, j, data.back());
            data.pop_back();
        }
    }

    length--;
}

void LocalFileDealer::pushMatrix(const Matrix &mat) {

    if (length == 0) {
        width = mat.cols();
        height = mat.rows();
        data.clear();
    }

    const char* ptr = mat.getPointer();
    for (int i = 0; i < width * height; i++) {
        data.push_back(*ptr);
        ptr++;
    }

    length++;
}

void LocalFileDealer::read(const std::string &path) {

    std::ifstream fin(path, std::ios::binary);
    clear();

    char c;
    for (int i = 0; i < headerSize; ++i) {
        fin.read(&c, sizeof(c));
        header.push_back(c);
    }
    readHeader();

    for (int i = 0; i < width * height * length; ++i) {
        fin.read(&c, sizeof(c));
        data.push_back(c);
    }

    fin.close();
}

void LocalFileDealer::write(const std::string &path) {

    std::ofstream fout(path, std::ios::binary);

    writeHeader();
    for (auto itr = header.begin(); itr != header.end(); ++itr) {
        fout.write(&(*itr), sizeof(*itr));
    }

    for (auto itr = data.begin(); itr != data.end(); ++itr) {
        fout.write(&(*itr), sizeof(*itr));
    }
    fout.close();
}

void LocalFileDealer::clear() {

    header.clear();
    data.clear();
    length = 0;
    width = 0;
    height = 0;
}

void LocalFileDealer::writeHeader() {

    for (auto itr = identifier.begin(); itr != identifier.end(); ++itr) {
        header.push_back(*itr);
    }

    header.push_back(static_cast<char>(width));
    header.push_back(static_cast<char>(height));
    header.push_back(static_cast<char>(length));
}

void LocalFileDealer::readHeader() {

    length = header.back();
    header.pop_back();
    height = header.back();
    header.pop_back();
    width = header.back();
    header.pop_back();
}


const cv::Vec3b ImageBuilder::foreColor = cv::Vec3b(0x00, 0x00, 0x00);

const cv::Vec3b ImageBuilder::backColor = cv::Vec3b(0xff, 0xff, 0xff);

ImageBuilder::ImageBuilder(const std::string &path) {

    char c, width, height;
    int i, j, k;
    std::ifstream fin(path, std::ios::binary);

    if (!fin) {
        std::cout << "data set not found" << std::endl;
    }

    for (k = 0; k < 18; ++k) {
        fin.read(&height, sizeof(height));
        fin.read(&width, sizeof(width));
        data[k].resize(height, width);

        for (i = 0; i < height; ++i) {
            for (j = 0; j < width; ++j) {
                fin.read(&c, sizeof(c));
                data[k].set(i, j, c);
            }
        }
    }

    fin.close();
}

void ImageBuilder::write(
        const std::string &path, const Matrix &cell, const Matrix &rgrid,
        const Matrix &cgrid, const Matrix &vertex) const {

    int width = data[0].cols() * cell.cols() + data[11].cols() * vertex.cols();
    int height = data[0].rows() * cell.rows()
        + data[11].rows() * vertex.rows();
    cv::Mat image(cv::Size(width, height), 16);

    int i, j;
    for (i = 0; i < cell.rows(); ++i) {
        for (j = 0; j < cell.cols(); ++j) {
            drawCell(i, j, cell.get(i, j), &image);
        }
    }
    for (i = 0; i < rgrid.rows(); ++i) {
        for (j = 0; j < rgrid.cols(); ++j) {
            drawRow(i, j, rgrid.get(i, j), &image);
        }
    }
    for (i = 0; i < cgrid.rows(); ++i) {
        for (j = 0; j < cgrid.cols(); ++j) {
            drawCol(i, j, cgrid.get(i, j), &image);
        }
    }
    for (i = 0; i < vertex.rows(); ++i) {
        for (j = 0; j < vertex.cols(); ++j) {
            drawVertex(i, j, vertex.get(i, j), &image);
        }
    }

    cv::imwrite(path, image);
}

void ImageBuilder::drawCell(
        int row, int col, int status, cv::Mat* image) const {

    if (status < 0 || status > 4) {
        status = 4;
    }

    int rbegin = data[0].rows() * row + data[5].rows() * (row + 1);
    int cbegin = data[0].cols() * col + data[8].cols() * (col + 1);

    setImagePixel(rbegin, cbegin, data[status], image);
}

void ImageBuilder::drawRow(
        int row, int col, int status, cv::Mat* image) const {

    if (status < 0 || status > 2) {
        status = 2;
    }

    int rbegin = data[0].rows() * row + data[5].rows() * row;
    int cbegin = data[5].cols() * col + data[11].cols() * (col + 1);

    setImagePixel(rbegin, cbegin, data[status + 5], image);
}

void ImageBuilder::drawCol(
        int row, int col, int status, cv::Mat* image) const {

    if (status < 0 || status > 2) {
        status = 2;
    }

    int rbegin = data[8].rows() * row + data[11].rows() * (row + 1);
    int cbegin = data[0].cols() * col + data[8].cols() * col;

    setImagePixel(rbegin, cbegin, data[status + 8], image);
}

void ImageBuilder::drawVertex(
        int row, int col, int status, cv::Mat* image) const {

    if (status < 0 || status > 7) {
        status = 0;
    }

    int rbegin = data[11].rows() * row + data[8].rows() * row;
    int cbegin = data[11].cols() * col + data[5].cols() * col;

    setImagePixel(rbegin, cbegin, data[status + 11], image);
}

void ImageBuilder::setImagePixel(
        int rbegin, int cbegin, const Matrix &mat, cv::Mat* image) const {

    int i, j, row = 0, col = 0;
    cv::Vec3b* ptr;

    for (i = 0; i < mat.rows(); ++i) {
        row = rbegin + i;
        ptr = image->ptr<cv::Vec3b>(row);

        for (j = 0; j < mat.cols(); ++j) {
            col = cbegin + j;

            if (mat.get(i, j) == 1) {
                ptr[col] = foreColor;
            } else {
                ptr[col] = backColor;
            }
        }
    }
}
