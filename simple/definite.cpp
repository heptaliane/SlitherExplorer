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

void setGridValue(char num, char *grid) {

    for (int i = 0; i < 4; i++) {
        if (grid[i] == 2) {
            grid[i] = num;
        }
    }
}

void applyVertexGrid(
        const Coord &coord,
        Matrix *rgrid, Matrix *cgrid, char *grid) {

    rgrid->set(coord.y, coord.x - 1, grid[0]);
    cgrid->set(coord.y - 1, coord.x, grid[1]);
    rgrid->set(coord.y, coord.x, grid[2]);
    cgrid->set(coord.y, coord.x, grid[3]);
}

void checkGridStatus(char *grid, int *count0, int *count1) {

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

void getCellGrid(
        const Coord &coord, const Matrix &rgrid,
        const Matrix &cgrid, char *grid) {

    grid[0] = rgrid.get(coord.y, coord.x);
    grid[1] = cgrid.get(coord.y, coord.x);
    grid[2] = rgrid.get(coord.y + 1, coord.x);
    grid[3] = cgrid.get(coord.y, coord.x + 1);
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

bool checkCell(const Matrix &cell, const Matrix &rgrid, const Matrix &cgrid) {

    Coord coord;
    int row = cell.rows();
    int col = cell.cols();
    char grid[4];
    int cnt0, cnt1;

    for (char i = 0; i < 4; i++) {
        coord.x = -1;
        coord.y = 0;

        while (searchCell(cell, i, &coord)) {
            getCellGrid(coord, rgrid, cgrid, grid);
            checkGridStatus(grid, &cnt0, &cnt1);

            if (cnt1 > i) {
                return false;
            }
            if (cnt0 > 4 - i) {
                return false;
            }
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
