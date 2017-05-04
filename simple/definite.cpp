#include "definite.h"
#include <iostream>

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
        const Coord &coord, const Matrix &rgrid,
        const Matrix &cgrid, char* grid) {

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

bool applyDiagonalTheoryWith2Slave(int idx, char* grid) {

    if (grid[idx * 2] != 2 && grid[idx * 2] == grid[idx * 2 + 1]) {
        return false;
    }

    if (grid[idx * 2] == 0 && grid[idx * 2 + 1] == 2) {
        grid[idx * 2 + 1] = 1;
    } else if (grid[idx * 2 + 1] == 0 && grid[idx * 2] == 2) {
        grid[idx * 2] = 1;
    }

    return true;
}

bool applyDiagonalTheoryWith3Slave(
        const Coord &coord, const Matrix &cell, const Matrix &rgrid,
        const Matrix &cgrid, int idx) {

    char dgrid[8];
    Coord crd(coord.x + (1 & idx) - (1 & ~idx), coord.y + (2 & idx) - 1);

    if (cell.get(crd.y, crd.x) == 2) {
        getDiagonalGrid(crd, rgrid, cgrid, dgrid);
        int opp1 = (1 & idx) + (2 & ~idx), opp2 = (1 & ~idx) + (2 & idx);

        return (dgrid[opp1 * 2] == 0 && dgrid[opp1 * 2 + 1] == 0) ||
            (dgrid[opp2 * 2] == 0 && dgrid[opp2 * 2 + 1] == 0);
    }

    return false;
}

static const int applyCrossTheoryWith1SlaveArray[16] = {
    0, -1, 0, 1, -1, 0, 1, 0, 1, -1, 1, 1, -1, 1, 1, 1
};

bool applyCrossTheoryWith1Slave(
        const Coord &coord, const Matrix &grid,
        char cell, int idx, char* crsgrid) {

    if (cell != 1) {
        return true;
    }

    if (grid.get(coord.y + applyCrossTheoryWith1SlaveArray[idx * 4],
                coord.x + applyCrossTheoryWith1SlaveArray[idx * 4 + 1]) == 0 ||
            grid.get(coord.x + applyCrossTheoryWith1SlaveArray[idx * 4 + 2],
                coord.x + applyCrossTheoryWith1SlaveArray[idx * 4 + 3]) == 0) {
        if (*crsgrid == 1) {
            return false;
        }

        *crsgrid = 0;
    }
    return true;
}

bool applyCrossTheoryWith3Slave(int idx, char cell, char* grid) {

    if (cell == 3) {
        if (grid[idx % 2] == 0 || grid[idx % 2 + 2] == 0) {
            return false;
        }

        grid[idx % 2] = 1;
        grid[idx % 2 + 2] = 1;
    }

    return true;
}

static const int connectEdgePairSlave[8] = {
    -1, 0, 0, -1, 1, 0, 0, 1
};

int getDirection(int prev, char* grid) {

    for (int i = 0; i < 4; ++i) {
        if (grid[i] == 1 && i != prev) {
            return i;
        }
    }

    return -1;
}

// for debug
void seeMatrix(const Matrix &mat) {
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            std::cout << +mat.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
}

void seeGrid8(char* grid) {
    for (int i = 0; i < 8; ++i) {
        if (grid[i] == -1) 
            std::cout << '-' << ' ';
        else
            std::cout << +grid[i] << ' ';
    }
    std::cout << std::endl;
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

bool searchEdge(
        const Matrix &rgrid, const Matrix &cgrid, Coord* coord) {

    int row = rgrid.rows();
    int col = cgrid.cols();

    coord->x++;
    char grid[4];
    int cnt0, cnt1;

    for ( ; coord->y < row; coord->y++) {
        for ( ; coord->x < col; coord->x++) {
            getVertexGrid(*coord, rgrid, cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);

            if (cnt1 == 1) {
                return true;
            }
        }

        coord->x = 0;
    }

    return false;
}

bool connectEdgePair(
        const Coord &coord, const Matrix &rgrid,
        const Matrix &cgrid, Coord* edge) {

    int direction = -1;
    char grid[4];
    edge->x = coord.x;
    edge->y = coord.y;

    while (true) {
        getVertexGrid(*edge, rgrid, cgrid, grid);
        direction = getDirection(direction, grid);

        if (direction == -1) {
            return true;
        }

        edge->x += connectEdgePairSlave[direction * 2];
        edge->y += connectEdgePairSlave[direction * 2 + 1];
        direction = (2 & ~direction) + (1 & direction);

        if (coord.x == edge->x && coord.y == edge->y) {
            break;
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

bool setGridWithEdgePair(const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    Coord coord(-1, 0);
    Coord edge;
    char grid[4];
    bool flg = isSatisfiedAboutCell(cell, *rgrid, *cgrid);
    flg &= isSatisfiedAboutEdge(*rgrid, *cgrid);

    while (searchEdge(*rgrid, *cgrid, &coord)) {
        if (!connectEdgePair(coord, *rgrid, *cgrid, &edge)) {
            return false;
        }

        if (edge.x == coord.x + 1 && edge.y == coord.y) {
            if (!flg && rgrid->get(coord.y, coord.x) != 1) {
                rgrid->set(coord.y, coord.x, 0);
            } else if (flg) {
                rgrid->set(coord.y, coord.x, 1);
            }

        } else if (edge.y == coord.y + 1 && edge.x == coord.x) {
            if (!flg && cgrid->get(coord.y, coord.x) != 1) {
                cgrid->set(coord.y, coord.x, 0);
            } else if (flg) {
                cgrid->set(coord.y, coord.x, 1);
            }
        }
    }

    return true;
}

bool isSatisfiedAboutCell(
        const Matrix &cell, const Matrix &rgrid, const Matrix &cgrid) {

    Coord coord;
    char grid[4];
    int cnt0, cnt1;

    for (coord.y = 0; coord.y < cell.rows(); coord.y++) {
        for (coord.x = 0; coord.x < cell.cols(); coord.x++) {
            if (cell.get(coord.y, coord.x) == 4) {
                continue;
            }

            getCellGrid(coord, rgrid, cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);
            if (cnt1 != cell.get(coord.y, coord.x)) {
                return false;
            }
        }
    }

    return true;
}

bool isSatisfiedAboutEdge(const Matrix &rgrid, const Matrix &cgrid) {

    Coord coord;
    char grid[4];
    int cnt0, cnt1;
    int cnt = 0;
    int row = rgrid.rows();
    int col = cgrid.cols();

    for (coord.y = 0; coord.y < row; coord.y++) {
        for (coord.x = 0; coord.x < col; coord.x++) {
            getVertexGrid(coord, rgrid, cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);

            if (cnt1 == 1) {
                ++cnt;
            }
        }
    }

    return cnt < 3;
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
        getDiagonalGrid(coord, *rgrid, *cgrid, grid);

        for (i = 0; i < 4; i++) {
            pair.x = static_cast<int>(grid[i * 2]);
            pair.y = static_cast<int>(grid[i * 2 + 1]);
            if (!applyDiagonalTheoryWith0Slave(dcell[i], &pair)) {
                return false;
            }
            grid[i * 2] = pair.x;
            grid[i * 2 + 1] = pair.y;
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
        getDiagonalGrid(coord, *rgrid, *cgrid, grid);
        getCellGrid(coord, *rgrid, *cgrid, dgrid);

        for (i = 0; i < 4; i++) {
            if ((grid[i * 2] == 1 && grid[i * 2 + 1] == 0)
                    || (grid[i * 2] == 0 && grid[i * 2 + 1] == 1)) {

                if (dgrid[applyDiagonalTheoryWith1Slave[i * 2]] == 1 ||
                        dgrid[applyDiagonalTheoryWith1Slave[i * 2 + 1]] == 1) {
                    return false;
                }

                dgrid[applyDiagonalTheoryWith1Slave[i * 2]] = 0;
                dgrid[applyDiagonalTheoryWith1Slave[i * 2 + 1]] = 0;
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
    int i;
    bool flg = true;

    while (searchCell(cell, 2, &coord)) {
        getDiagonalGrid(coord, *rgrid, *cgrid, grid);
        for (i = 0; i < 4; ++i) {
            if (grid[i * 2] == 0 && grid[i * 2 + 1] == 0) {
                flg &= applyDiagonalTheoryWith2Slave((2 & i) + (1 & ~i), grid);
                flg &= applyDiagonalTheoryWith2Slave((2 & ~i) + (1 & i), grid);
            }

            if (!flg) {
                return false;
            }
        }
        applyDiagonalGrid(coord, rgrid, cgrid, grid);
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
        getDiagonalGrid(coord, *rgrid, *cgrid, grid);
        getCellGrid(coord, *rgrid, *cgrid, dgrid);
        getDiagonalCell(coord, cell, dcell);

        for (i = 0; i < 4; i++) {
            if (dcell[i] == 3 || grid[i * 2] == 1 || grid[i * 2 + 1] == 1 ||
                    applyDiagonalTheoryWith3Slave(
                        coord, cell, *rgrid, *cgrid, i)) {
                if (dgrid[applyDiagonalTheoryWith1Slave[i * 2]] == 0 ||
                        dgrid[applyDiagonalTheoryWith1Slave[i * 2 + 1]] == 0) {
                    return false;
                }

                dgrid[applyDiagonalTheoryWith1Slave[i * 2]] = 1;
                dgrid[applyDiagonalTheoryWith1Slave[i * 2 + 1]] = 1;
            }

            if (grid[i * 2] == 0 && grid[i * 2 + 1] == 0) {
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

bool applyCrossTheoryWith1(
        const Matrix &cell, Matrix* rgrid, Matrix *cgrid) {

    Coord coord(-1, 0);
    char ccell[4];
    char grid[4];
    int i;
    bool flg = true;

    while (searchCell(cell, 1, &coord)) {
        getCrossCell(coord, cell, ccell);
        getCellGrid(coord, *rgrid, *cgrid, grid);

        flg &= applyCrossTheoryWith1Slave(coord, *rgrid, ccell[0], 0, &grid[0]);
        flg &= applyCrossTheoryWith1Slave(coord, *cgrid, ccell[1], 1, &grid[1]);
        flg &= applyCrossTheoryWith1Slave(coord, *rgrid, ccell[2], 2, &grid[2]);
        flg &= applyCrossTheoryWith1Slave(coord, *cgrid, ccell[3], 3, &grid[3]);

        applyCellGrid(coord, rgrid, cgrid, grid);
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

bool applyDefinite(const Matrix &cell, Matrix* rgrid, Matrix *cgrid) {

    rgrid->setExceptValue(0);
    cgrid->setExceptValue(0);

    Matrix rprev(*rgrid), cprev(*cgrid);
    bool flg = true;

    while (true) {
        flg &= setGridWithCell(cell, rgrid, cgrid);
        flg &= setGridWithVertex(rgrid, cgrid);
        flg &= applyDiagonalTheoryWith0(cell, rgrid, cgrid);
        flg &= applyDiagonalTheoryWith1(cell, rgrid, cgrid);
        flg &= applyDiagonalTheoryWith2(cell, rgrid, cgrid);
        flg &= applyDiagonalTheoryWith3(cell, rgrid, cgrid);
        flg &= applyCrossTheoryWith1(cell, rgrid, cgrid);
        flg &= applyCrossTheoryWith3(cell, rgrid, cgrid);
        flg &= setGridWithEdgePair(cell, rgrid, cgrid);

        if (!flg) {
            return false;
        }
        if (rprev.equals(*rgrid) && cprev.equals(*cgrid)) {
            break;
        }

        rprev.copy(*rgrid);
        cprev.copy(*cgrid);
    }

    return true;
}
