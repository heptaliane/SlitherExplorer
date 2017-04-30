#ifndef DEFINITE_H_
#define DEFINITE_H_

#include "slarray.h"

bool searchCell(const Matrix &cell, char num, Coord* coord);

bool setGridWithCell(const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool setGridWithVertex(Matrix* rgrid, Matrix* cgrid);

bool applyDiagonalTheoryWith0(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

#endif /* DEFINITE_H_ */
