#ifndef DEFINITE_H_
#define DEFINITE_H_

#include "slarray.h"

bool searchCell(const Matrix &cell, char num, Coord* coord);

bool checkCell(const Matrix &cell, const Matrix &rgrid, const Matrix &cgrid);

bool setGridWithVertex(Matrix *rgrid, Matrix *cgrid);

#endif /* DEFINITE_H_ */
