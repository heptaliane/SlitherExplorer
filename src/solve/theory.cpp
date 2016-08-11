#include "solve.h"

// --- constructor ---
Theory::Theory(const ArrayD<short> &cell) {
    init(cell);
}
Theory::Theory(const Utilarray &cell) {
    ArrayD<short> temp;
    cell.exportArray(&temp);
    init(temp);
}
Theory::Theory(){

}
Theory::~Theory(){
    delete checker;
    checker = NULL;
    delete buf_row_grid;
    buf_row_grid = NULL;
    delete buf_col_grid;
    buf_col_grid = NULL;
    delete buf_cell;
    buf_cell = NULL;
    delete buf_vertex;
    buf_vertex = NULL;
}

// --- public ---
void Theory::cpyRowGrid(ArrayD<short> *obj) const {
    buf_row_grid->exportArray(obj);
}
void Theory::cpyColGrid(ArrayD<short> *obj) const {
    buf_col_grid->exportArray(obj);
}
void Theory::cpyVertex(ArrayD<short> *obj) const {
    buf_vertex->exportArray(obj);
}

// --- private ---
void Theory::init(const ArrayD<short> &cell) {
    const int cell_row_size = cell.getlen1();
    const int cell_col_size = cell.getlen2();

    checker = new Checker(cell);
    buf_row_grid = new Utilarray(cell_row_size + 1, cell_col_size, 2);
    buf_col_grid = new Utilarray(cell_row_size, cell_col_size + 1, 2);
    buf_vertex = new Utilarray(cell_row_size + 1, cell_col_size + 1, 0);
    buf_cell = new Utilarray(cell_row_size, cell_col_size, 4);

    buf_cell->clone(cell);
}

bool Theory::slave0(int row, int col, int diag) {
    int rowCoord[2], colCoord[2], cellCoord[2];

    cellCoord[0] = row + diag_node[diag][0];
    cellCoord[1] = col + diag_node[diag][1];
    rowCoord[0] = row + diag_0[diag][0][0];
    rowCoord[1] = col + diag_0[diag][0][1];
    colCoord[0] = row + diag_0[diag][1][0];
    colCoord[1] = col + diag_0[diag][1][1];

    if (buf_cell->get(cellCoord[0], cellCoord[1], 4) == 3) {
        if (buf_row_grid->get(rowCoord[0], rowCoord[1], 0) == 0) return false;
        if (buf_col_grid->get(colCoord[0], colCoord[1], 0) == 0) return false;

        buf_row_grid->set(rowCoord[0], rowCoord[1], 1);
        buf_col_grid->set(colCoord[0], colCoord[1], 1);
    } else if (buf_cell->get(cellCoord[0], cellCoord[1], 4) == 1) {
        if (buf_row_grid->get(rowCoord[0], rowCoord[1], 0) == 1) return false;
        if (buf_col_grid->get(colCoord[0], colCoord[1], 0) == 1) return false;

        buf_row_grid->set(rowCoord[0], rowCoord[1], 0);
        buf_col_grid->set(colCoord[0], colCoord[1], 0);
    }
    return true;
}

bool Theory::theory0() {
    int coord[2] = {0, -1};
    short grid[4];

    while(checker->searchCell0(coord)) {
        grid[0] = buf_row_grid->get(coord[0], coord[1], 0);
        grid[1] = buf_row_grid->get(coord[0] + 1, coord[1], 0);
        grid[2] = buf_col_grid->get(coord[0], coord[1], 0);
        grid[3] = buf_col_grid->get(coord[0], coord[1] + 1, 0);

        for (int i = 0; i < 4; i++) {
            if (grid[i] == 1) return false;
        }

        buf_row_grid->set(coord[0], coord[1], 0);
        buf_row_grid->set(coord[0] + 1, coord[1], 0);
        buf_col_grid->set(coord[0], coord[1], 0);
        buf_col_grid->set(coord[0], coord[1] + 1, 0);

        for (int j = 0; j < 4; j++) {
            if (!slave0(coord[0], coord[1], j)) return false;
        }
    }
    return true;
}
bool Theory::slave3_1(int row, int col, int diag) {
    int cellCoord[2];

    cellCoord[0] = row + diag_node[diag][0];
    cellCoord[1] = col + diag_node[diag][1];
    if (buf_cell->get(cellCoord[0], cellCoord[1], 4) != 3) return true;

    int rowCoord[2][2], colCoord[2][2];
    rowCoord[0][0] = row + diag_3[diag][0][0];
    rowCoord[0][1] = col + diag_3[diag][0][1];
    rowCoord[1][0] = row + diag_3[diag][1][0];
    rowCoord[1][1] = col + diag_3[diag][1][1];
    colCoord[0][0] = row + diag_3[diag][2][0];
    colCoord[0][1] = col + diag_3[diag][2][1];
    colCoord[1][0] = row + diag_3[diag][3][0];
    colCoord[1][1] = col + diag_3[diag][3][1];

    short grid[4] = {
        buf_row_grid->get(rowCoord[0][0], rowCoord[0][1], 0),
        buf_row_grid->get(rowCoord[1][0], rowCoord[1][1], 0),
        buf_col_grid->get(colCoord[0][0], colCoord[0][1], 0),
        buf_col_grid->get(colCoord[1][0], colCoord[1][1], 0)
    };
    for (int i = 0; i < 4; i++) {
        if (grid[i] == 2) grid[i] = 1;
        else if (grid[i] == 0) return false;
    }

    buf_row_grid->set(rowCoord[0][0], rowCoord[0][1], grid[0]);
    buf_row_grid->set(rowCoord[1][0], rowCoord[1][1], grid[1]);
    buf_col_grid->set(colCoord[0][0], colCoord[0][1], grid[2]);
    buf_col_grid->set(colCoord[1][0], colCoord[1][1], grid[3]);
}
bool Theory::theory3() {
    int coord[2] = {0, -1};
    while (checker->searchCell(3, coord)) {
        for (int i = 0; i < 4; i++) {
            slave3_1(coord[0], coord[1], i);
        }
    }
}
