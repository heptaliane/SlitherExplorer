#include "fileuser.h"

Fileuser::Fileuser(const string &url) {
    cout << "#=== File Loading Process ===#" << endl;

    cout << "loading file name..";
    filename = url;
    cout << "Done" << endl;
}

Fileuser::Fileuser(const Fileuser &obj) {

}

Fileuser::~Fileuser() {
    delete fout;
    fout = NULL;
}

void Fileuser::getArray(ArrayD<short> *array) {
    // if file is not loaded
    if (data.size() == 0) {
        cout << "loading " << filename << "..";
        if (importData()) cout << "Done" << endl;
        else cout << "Failed" << endl;
    }
    //input data to array
    int row_size, col_size;
    if (!readHeader(&row_size, &col_size)) {
        return;
    }

    readData(row_size, col_size, array);
}
void Fileuser::saveArray(const ArrayD<short> &array, bool isClose) {
    if (fout == NULL) {
        fout = new ofstream(filename, ios::binary);
    }
    writeHeader(array.getlen1(), array.getlen2(), fout);
    writeData(array, fout);

    if (isClose) fout->close();
}

bool Fileuser::readHeader(int *row_size, int *col_size) {
    cout << "loading header from " << filename << "..";
    for (unsigned int i = 0; i < kHeader.size(); i++, index++) {
        if (data[index + i] != kHeader[i]) {
            cout << "Failed" << endl;
            return false;
        }
    }

    *row_size = static_cast<int>(data[index]);
    index++;
    *col_size = static_cast<int>(data[index]);
    index++;

    cout << "Done" << endl;
    return true;
}
void Fileuser::writeHeader(int row_size, int col_size, ofstream *out) const {
    char buf;
    cout << "writing header to " << filename << "..";
    out->write(kHeader.c_str(), kHeader.size());

    buf = static_cast<char>(row_size);
    out->write(&buf, sizeof(buf));
    buf = static_cast<char>(col_size);
    out->write(&buf, sizeof(buf));
    cout << "Done" << endl;
}
void Fileuser::readData(int row_size, int col_size, ArrayD<short> *array) {
    ArrayD<short> temp(row_size, col_size);
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < col_size; j++, index++) {
            temp.setArray(i, j, static_cast<short>(data[index]));
        }
    }

    array->clone(temp);
}
void Fileuser::writeData(const ArrayD<short> &array, ofstream *out) const {
    char buf;
    for (int i = 0; i < array.getlen1(); i++) {
        for (int j = 0; j < array.getlen2(); j++) {
            buf = static_cast<char>(array.getArray(i, j));
            out->write(&buf, sizeof(buf));
        }
    }
}

bool Fileuser::importData() {
    ifstream fin(filename, ios::binary);
    // if file not exist
    if (!fin) return false;

    char c;
    while (!fin.eof()) {
        fin.read(&c, sizeof(c));
        data.push_back(c);
    }
    fin.close();
    return true;
}
