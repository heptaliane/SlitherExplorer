#include "localFiles.h"

const std::string LocalFileDealer::identifier = "slitherlink";

const int LocalFileDealer::headerSize = 13;

void LocalFileDealer::popMatrix(Matrix *mat) {

    if (length == 0) {
        mat->resize(0, 0);
        return;
    }

    mat->resize(width, height);

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
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

    char c;
    for (int i = 0; i < headerSize; i++) {
        fin.read(&c, sizeof(c));
        header.push_back(c);
    }
    while (!fin.eof()) {
        fin.read(&c, sizeof(c));
        data.push_back(c);
    }

    fin.close();

    readHeader();
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

    length = data.back();
    data.pop_back();
    height = data.back();
    data.pop_back();
    width = data.back();
    data.pop_back();
}

