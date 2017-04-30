#include "definite.h"

namespace {

void getVertexGrid(
        const Coord &coord,
        const Matrix &rgrid, const Matrix &cgrid, char* grid) {

    grid[0] = rgrid.get(coord.y, coord.x - 1);
    grid[1] = cgrid.get(coord.y - 1, coord.x);
    grid[2] = rgrid.get(coord.y, coord.x);
    grid[3] = cgrid.get(coord.y, coord.x);
}

void applyVertexGrid(
        const Coord &coord,
        Matrix* rgrid, Matrix* cgrid, char* grid) {

    rgrid->set(coord.y, coord.x - 1, grid[0]);
    cgrid->set(coord.y - 1, coord.x, grid[1]);
    rgrid->set(coord.y, coord.x, grid[2]);
    cgrid->set(coord.y, coord.x, grid[3]);
}

void getCellGrid(
        const Coord &coord, const Matrix &rgrid,
        const Matrix &cgrid, char* grid) {

    grid[0] = rgrid.get(coord.y, coord.x);
    grid[1] = cgrid.get(coord.y, coord.x);
    grid[2] = rgrid.get(coord.y + 1, coord.x);
    grid[3] = cgrid.get(coord.y, coord.x + 1);
}

void applyCellGrid(
        const Coord &coord, Matrix* rgrid, Matrix* cgrid, char* grid) {

    rgrid->set(coord.y, coord.x, grid[0]);
    cgrid->set(coord.y, coord.x, grid[1]);
    rgrid->set(coord.y + 1, coord.x, grid[2]);
    cgrid->set(coord.y, coord.x + 1, grid[3]);
}

void getDiagonalGrid(
        const Coord &coord, const Matrix &cell,
        const Matrix &rgrid, const Matrix &cgrid, char* grid) {

    grid[0] = rgrid.get(coord.y, coord.x - 1);
    grid[1] = cgrid.get(coord.y - 1, coord.x);
    grid[2] = rgrid.get(coord.y, coord.x + 1);
    grid[3] = cgrid.get(coord.y - 1, coord.x + 1);
    grid[4] = rgrid.get(coord.y + 1, coord.x - 1);
    grid[5] = cgrid.get(coord.y + 1, coord.x);
    grid[6] = rgrid.get(coord.y + 1, coord.x + 1);
    grid[7] = cgrid.get(coord.y + 1, coord.x + 1);
}

void applyDiagonalGrid(
        const Coord &coord, Matrix* rgrid, Matrix* cgrid, char* grid) {

    rgrid->set(coord.y, coord.x - 1, grid[0]);
    cgrid->set(coord.y - 1, coord.x, grid[1]);
    rgrid->set(coord.y, coord.x + 1, grid[2]);
    cgrid->set(coord.y - 1, coord.x + 1, grid[3]);
    rgrid->set(coord.y + 1, coord.x - 1, grid[4]);
    cgrid->set(coord.y + 1, coord.x, grid[5]);
    rgrid->set(coord.y + 1, coord.x + 1, grid[6]);
    cgrid->set(coord.y + 1, coord.x + 1, grid[7]);
}

void getDiagonalCell(
        const Coord coord, const Matrix &cell, char* dcell) {

    dcell[0] = cell.get(coord.y - 1, coord.x - 1);
    dcell[1] = cell.get(coord.y - 1, coord.x + 1);
    dcell[2] = cell.get(coord.y + 1, coord.x - 1);
    dcell[3] = cell.get(coord.y + 1, coord.x + 1);
}

void getCrossCell(
        const Coord coord, const Matrix &cell, char* ccell) {

    ccell[0] = cell.get(coord.y - 1, coord.x);
    ccell[1] = cell.get(coord.y, coord.x - 1);
    ccell[2] = cell.get(coord.y + 1, coord.x);
    ccell[3] = cell.get(coord.y, coord.x + 1);
}

void checkGridStatus(char* grid, int* count0, int* count1) {

    int cnt0 = 0, cnt1 = 0;

    for (int i = 0; i < 4; i++) {
        if (grid[i] == 1) {
            cnt1++;

        } else if (grid[i] != 2) {
            cnt0++;
        }
    }

    *count0 = cnt0;
    *count1 = cnt1;
}

void setGridValue(char num, char* grid) {

    for (int i = 0; i < 4; i++) {
        if (grid[i] == 2) {
            grid[i] = num;
        }
    }
}

bool applyDiagonalTheoryWith0Slave(char cell, Coord* grid) {

    if (cell == 1) {
        if (grid->y == 1 || grid->x == 1) {
            return false;
        } else if (grid->y == 2 || grid->x == 2) {
            grid->y = 0;
            grid->x = 0;
        }

    } else if (cell == 3) {
        if (grid->y == 0 || grid->x == 0) {
            return false;
        } else if (grid->y == 2 || grid->x == 2) {
            grid->y = 1;
            grid->x = 1;
        }
    }

    return true;
}

static const int applyDiagonalTheoryWith1Slave[8] = {
    2, 3, 1, 2, 0, 3, 0, 1,
};

void applyDiagonalTheoryWith2Slave(int idx, char *grid) {

    if (grid[idx * 4] == 0 && grid[idx * 4 + 1] == 0) {
        int opp = (idx + 2) % 4;
        grid[opp * 4] = 0;
        grid[opp * 4 + 1] = 0;
    }
}

bool applyCrossTheoryWith3Slave(int idx, char cell, char* grid) {

    if (cell == 3) {
        if (grid[idx % 2] == 0 || grid[idx % 2 + 1] == 0) {
            return false;
        }

        grid[idx % 2] = 1;
        grid[idx % 2 + 1] = 1;
    }

    return true;
}

}

bool searchCell(const Matrix &cell, char num, Coord* coord) {

    int row = cell.rows();
    int col = cell.cols();

    coord->x++;

    if (coord->y == -1 && num == 0) {
        if (coord->x == -1) {
            return true;
        }

        if (coord->x == 0) {
            coord->x = col;
            return true;
        }

        coord->x = 0;
        coord->y = 0;
    } else if (coord->y == -1) {
        coord->y = 0;
    }

    // x : horizontal, y: vertical
    int i = coord->y, j = coord->x;

    for ( ; i < row; i++) {
        for ( ; j < col; j++) {
            if (cell.get(i, j) == num) {
                coord->y = i;
                coord->x = j;
                return true;
            }
        }

        j = 0;
    }

    if (num == 0) {
        if (coord->y < row) {
            coord->x = -1;
            coord->y = row;
            return true;
        }

        if (coord->x == -1) {
            coord->x = col;
            return true;
        }
    }

    return false;
}

bool setGridWithCell(const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord;
    char grid[4];
    int cnt0, cnt1;

    for (char i = 0; i < 4; i++) {
        coord.x = -1;
        coord.y = 0;

        while (searchCell(cell, i, &coord)) {
            getCellGrid(coord, *rgrid, *cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);

            if (cnt1 > i) {
                return false;
            }
            if (cnt0 > 4 - i) {
                return false;
            }

            if (cnt1 != i && cnt0 == 4 - i) {
                setGridValue(1, grid);
            } else if (cnt0 != 4 - i && cnt1 == i) {
                setGridValue(0, grid);
            }

            applyCellGrid(coord, rgrid, cgrid, grid);
        }
    }

    return true;
}

bool setGridWithVertex(Matrix *rgrid, Matrix *cgrid) {

    Coord coord;
    int row = rgrid->rows();
    int col = cgrid->cols();
    char grid[4];
    int cnt0, cnt1;

    for (coord.y = 0; coord.y < row; coord.y++) {
        for (coord.x = 0; coord.x < col; coord.x++) {
            getVertexGrid(coord, *rgrid, *cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);

            if (cnt1 > 2 || (cnt0 == 3 && cnt1 == 1)) {
                return false;
            }

            if ((cnt0 == 3 && cnt1 == 0) || (cnt1 == 2 && cnt0 != 2)) {
                setGridValue(0, grid);
            } else if (cnt0 == 2 && cnt1 == 1) {
                setGridValue(1, grid);
            }

            applyVertexGrid(coord, rgrid, cgrid, grid);
        }
    }

    return true;
}

bool applyDiagonalTheoryWith0(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord(-1, 0);
    char grid[8];
    char dcell[4];
    Coord pair;
    int i;

    while (searchCell(cell, 0, &coord)) {
        getDiagonalCell(coord, cell, dcell);
        getDiagonalGrid(coord, cell, *rgrid, *cgrid, grid);

        for (i = 0; i < 4; i++) {
            pair.x = grid[i * 4];
            pair.y = grid[i * 4 + 1];
            if (!applyDiagonalTheoryWith0Slave(dcell[i], &pair)) {
                return false;
            }
        }

        applyDiagonalGrid(coord, rgrid, cgrid, grid);
    }

    return true;
}

bool applyDiagonalTheoryWith1(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord(-1, 0);
    char grid[8];
    char dgrid[4];
    Coord pair;
    int i;

    while (searchCell(cell, 1, &coord)) {
        getDiagonalGrid(coord, cell, *rgrid, *cgrid, grid);
        getCellGrid(coord, *rgrid, *cgrid, dgrid);

        for (i = 0; i < 4; i++) {
            if ((grid[i * 4] == 1 && grid[i * 4 + 1] == 0)
                    || (grid[i * 4] == 0 && grid[i * 4 + 1] == 1)) {

                if (dgrid[applyDiagonalTheoryWith1Slave[i * 4]] == 1 ||
                        dgrid[applyDiagonalTheoryWith1Slave[i * 4 + 1]] == 1) {
                    return false;
                }

                dgrid[applyDiagonalTheoryWith1Slave[i * 4]] = 0;
                dgrid[applyDiagonalTheoryWith1Slave[i * 4 + 1]] = 0;
            }
        }

        applyCellGrid(coord, rgrid, cgrid, dgrid);
    }

    return true;
}

bool applyDiagonalTheoryWith2(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord(-1, 0);
    char grid[8];
    int i, cnt;

    while (searchCell(cell, 2, &coord)) {
        getDiagonalGrid(coord, cell, *rgrid, *cgrid, grid);

        for (i = 0; i < 4; i++) {
            applyDiagonalTheoryWith2Slave(i, grid);
        }

        cnt = 0;
        for (i = 0; i < 8; i++) {
            if (grid[i] == 0) {
                cnt++;
            }
        }

        if (cnt == 8) {
            return false;
        }
    }

    return true;
}

bool applyDiagonalTheoryWith3(
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord(-1, 0);
    char grid[8];
    char dgrid[4];
    char vgrid[4];
    Coord vertex;
    char dcell[4];
    int i;

    while (searchCell(cell, 3, &coord)) {
        getDiagonalGrid(coord, cell, *rgrid, *cgrid, grid);
        getCellGrid(coord, *rgrid, *cgrid, dgrid);
        getDiagonalCell(coord, cell, dcell);

        for (i = 0; i < 4; i++) {
            if (dcell[i] == 3 || grid[i * 4] == 1 || grid[i * 4 + 1] == 1) {
                if (dgrid[applyDiagonalTheoryWith1Slave[i * 4]] == 0 ||
                        dgrid[applyDiagonalTheoryWith1Slave[i * 4 + 1]] == 0) {
                    return false;
                }

                dgrid[applyDiagonalTheoryWith1Slave[i * 4]] = 1;
                dgrid[applyDiagonalTheoryWith1Slave[i * 4 + 1]] = 1;
            }

            if (grid[i * 4] == 0 && grid[i * 4 + 1] == 0) {
                vertex.x = coord.x + (i % 2);
                vertex.y = coord.y + (i - (i % 2)) / 2;
                getVertexGrid(vertex, *rgrid, *cgrid, vgrid);
                setGridValue(1, vgrid);
                applyVertexGrid(vertex, rgrid, cgrid, vgrid);
            }

            applyCellGrid(coord, rgrid, cgrid, dgrid);
        }
    }

    return true;
}

bool applyCrossTheoryWith3(
        const Matrix &cell, Matrix* rgrid, Matrix *cgrid) {

    Coord coord(-1, 0);
    char ccell[4];
    char grid[4];
    int i;

    while (searchCell(cell, 3, &coord)) {
        getCrossCell(coord, cell, ccell);
        getCellGrid(coord, *rgrid, *cgrid, grid);

        for (i = 0; i < 4; i++) {
            if (!applyCrossTheoryWith3Slave(i, ccell[i], grid)) {
                return false;
            }
        }

        applyCellGrid(coord, rgrid, cgrid, grid);
    }

    return true;
}
