#ifndef DEFINITE_H_
#define DEFINITE_H_

#include "slarray.h"

bool searchCell(const Matrix &cell, char num, Coord* coord);

bool searchEdge(const Matrix &rgrid, const Matrix &cgrid, Coord* coord);

bool connectEdgePair(
        const Coord &coord, const Matrix &rgrid,
        const Matrix &cgrid, Coord* edge);

bool setGridWithCell(const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool setGridWithVertex(Matrix* rgrid, Matrix* cgrid);

bool setGridWithEdgePair(const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool isSatisfiedAboutCell(
        const Matrix &cell, const Matrix &rgrid, const Matrix &cgrid);

bool isSatisfiedAboutEdge(const Matrix &rgrid, const Matrix &cgrid);

bool isSatisfiedAboutCircle(const Matrix &rgrid, const Matrix &cgrid);

bool applyDiagonalTheoryWith0(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyDiagonalTheoryWith1(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyDiagonalTheoryWith2(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyDiagonalTheoryWith3(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyCrossTheoryWith1(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyCrossTheoryWith3(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

bool applyDefinite(const Matrix &cell, Matrix* rgrid, Matrix* cgrid);

void reloadVertex(const Matrix &rgrid, const Matrix &cgrid, Matrix* vertex);

#endif /* DEFINITE_H_ */
