#ifndef SEARCH_H_
#define SEARCH_H_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include "slarray.h"
#include "definite.h"
#include "localFiles.h"

struct GridSearchDataStore {
public:

    GridSearchDataStore () :
        direction(-1) {};

    GridSearchDataStore (
            const Matrix &nrgrid, const Matrix &ncgrid,
            const Coord &ncoord, int ndirection) : direction(ndirection) {
        rgrid.copy(nrgrid);
        cgrid.copy(ncgrid);
        coord = ncoord;
    }

    GridSearchDataStore (const GridSearchDataStore &obj) :
        direction(obj.direction) {
        rgrid.copy(obj.rgrid);
        cgrid.copy(obj.cgrid);
        coord = obj.coord;
    }

    Matrix rgrid;
    Matrix cgrid;
    Coord coord;
    int direction;
};

void executeSearch (const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

void executeSearchWithLog (
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid,
        const std::string logdirpath);

#endif /* SEARCH_H_ */
