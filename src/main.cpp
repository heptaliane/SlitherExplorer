#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include "slarray.h"
#include "definite.h"
#include "localFiles.h"
#include "search.h"

std::vector<char> splitCSVLine(const std::string str) {
    std::string buf;
    std::vector<char> line;

    for (auto itr = str.begin(); itr != str.end(); ++itr) {
        if (isdigit(*itr)) {
            buf += *itr;
        } else if (*itr == ',' && buf.size() == 0) {
            line.push_back(static_cast<char>(4));
        } else if (*itr == ',') {
            line.push_back(static_cast<char>(std::atoi(buf.c_str())));
            buf.clear();
        }
    }

    if (buf.size() != 0) {
        line.push_back(static_cast<char>(std::atoi(buf.c_str())));
    } else {
        line.push_back(static_cast<char>(4));
    }

    return line;
}

void readCSV(const std::string filename, Matrix* container) {

    std::ifstream fin(filename);
    std::string str;
    std::vector<std::vector<char> > data;
    int length = 0;

    if (!fin) {
        std::cout << "CSV file not found" << std::endl;
        return;
    }

    while (getline(fin, str)) {
        data.push_back(splitCSVLine(str));
        if (length < static_cast<int>(data.back().size())) {
            length = static_cast<int>(data.back().size());
        }
    }

    container->resize(data.size(), length);
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < length; ++j) {
            if (data[i].size() > j) {
                container->set(i, j, data[i][j]);
            } else {
                container->set(i, j, static_cast<char>(4));
            }
        }
    }
}

int main(int argc, char** argv) {

    Matrix cell;
    readCSV(std::string(argv[1]), &cell);
    Matrix rgrid(cell.rows() + 1, cell.cols(), 2);
    Matrix cgrid(cell.rows(), cell.cols() + 1, 2);
    Matrix vertex(cell.rows() + 1, cell.cols() + 1);

    if (!applyDefinite(cell, &rgrid, &cgrid)) {
        std::cout << "Invalid data" << std::endl;
        return -1;
    }

    ImageBuilder img("graphics.dat");
    executeSearch(cell, &rgrid, &cgrid);
    reloadVertex(rgrid, cgrid, &vertex);
    img.write(argv[2], cell, rgrid, cgrid, vertex);
}
