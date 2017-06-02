#include <iostream>
#include "slarray.h"
#include "definite.h"
#include "localFiles.h"
#include "search.h"


int main() {
    Matrix cell(10, 10, 4);
    Matrix rgrid(11, 10, 2);
    Matrix cgrid(10, 11, 2);
    Matrix vertex(11, 11);

    // cell.set(0, 1, 2);
    // cell.set(0, 3, 2);
    // cell.set(0, 5, 1);
    // cell.set(0, 7, 2);
    // cell.set(0, 8, 2);
    // cell.set(0, 9, 1);
    // cell.set(1, 3, 2);
    // cell.set(1, 4, 2);
    // cell.set(1, 5, 3);
    // cell.set(1, 6, 3);
    // cell.set(1, 7, 1);
    // cell.set(1, 8, 1);
    // cell.set(2, 0, 1);
    // cell.set(2, 1, 2);
    // cell.set(2, 2, 2);
    // cell.set(2, 4, 2);
    // cell.set(2, 7, 2);
    // cell.set(2, 8, 1);
    // cell.set(3, 1, 3);
    // cell.set(3, 8, 2);
    // cell.set(3, 9, 3);
    // cell.set(4, 1, 2);
    // cell.set(4, 4, 1);
    // cell.set(4, 5, 1);
    // cell.set(4, 6, 2);
    // cell.set(4, 8, 2);
    // cell.set(4, 9, 2);
    // cell.set(5, 0, 3);
    // cell.set(5, 1, 1);
    // cell.set(5, 3, 2);
    // cell.set(5, 4, 1);
    // cell.set(5, 5, 1);
    // cell.set(5, 8, 2);
    // cell.set(6, 0, 2);
    // cell.set(6, 1, 2);
    // cell.set(6, 8, 2);
    // cell.set(7, 1, 3);
    // cell.set(7, 2, 1);
    // cell.set(7, 5, 1);
    // cell.set(7, 7, 2);
    // cell.set(7, 8, 2);
    // cell.set(7, 9, 1);
    // cell.set(8, 1, 2);
    // cell.set(8, 2, 3);
    // cell.set(8, 3, 2);
    // cell.set(8, 4, 2);
    // cell.set(8, 5, 3);
    // cell.set(8, 6, 3);
    // cell.set(9, 0, 2);
    // cell.set(9, 1, 1);
    // cell.set(9, 2, 2);
    // cell.set(9, 4, 1);
    // cell.set(9, 6, 0);
    // cell.set(9, 8, 2);

    LocalFileDealer lfd;
    // lfd.read("sample/01.dat");
    lfd.read("sample/02.dat");
    lfd.popMatrix(&cell);

    bool flag = applyDefinite(cell, &rgrid, &cgrid);
    if (!flag) {
        std::cout << "invalid" << std::endl;
    }

    // LocalFileDealer lfd;
    // lfd.pushMatrix(cell);
    // lfd.write("sample/03.dat");

    ImageBuilder imgBuild("graphics.dat");
    imgBuild.write("test.png", cell, rgrid, cgrid, vertex);
    executeSearch(cell, &rgrid, &cgrid);
    imgBuild.write("result.png", cell, rgrid, cgrid, vertex);
};
