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
bool Theory::run() {
    if (!theory0()) return false;
    if (!theory3()) return false;

    ArrayD<short> row_temp, col_temp, vertex_temp;
    buf_row_grid->exportArray(&row_temp);
    buf_col_grid->exportArray(&col_temp);
    buf_vertex->exportArray(&vertex_temp);

    if (!checker->checkVertex(&row_temp, &col_temp)) return false;
    if (!checker->checkCell(&row_temp, &col_temp)) return false;
    checker->reloadVertex(row_temp, col_temp, &vertex_temp);

    buf_row_grid->clone(row_temp);
    buf_col_grid->clone(col_temp);
    buf_vertex->clone(vertex_temp);
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
bool Theory::slave3_2(int row, int col, int cross) {
    int coord[2];
    coord[0] = row + cross_node[cross][0];
    coord[1] = col + cross_node[cross][1];
    if (buf_cell->get(coord[0], coord[1], 0) != 3) return true;

    short grid[4];
    if (cross_node[cross][0] != 0) {
        grid[0] = buf_row_grid->get(coord[0], coord[1], 0);
        grid[1] = buf_row_grid->get(row, col, 0);
        grid[2] = buf_row_grid->get(row + 1, col, 0);
        grid[3] = buf_row_grid->get(coord[0] + 1, coord[1], 0);
        for (int i = 0; i < 4; i++) {
            if (grid[i] == 0) return false;
            else if (grid[i] == 2) grid[i] = 1;
        }
        buf_row_grid->set(coord[0], coord[1], grid[0]);
        buf_row_grid->set(row, col, grid[1]);
        buf_row_grid->set(row + 1, col, grid[2]);
        buf_row_grid->set(coord[0] + 1, coord[1], grid[3]);
    } else {
        grid[0] = buf_col_grid->get(coord[0], coord[1], 0);
        grid[1] = buf_col_grid->get(row, col, 0);
        grid[2] = buf_col_grid->get(row, col + 1, 0);
        grid[3] = buf_col_grid->get(coord[0], coord[1] + 1, 0);
        for (int i = 0; i < 4; i++) {
            if (grid[i] == 0) return false;
            else if (grid[i] == 2) grid[i] = 1;
        }
        buf_col_grid->set(coord[0], coord[1], grid[0]);
        buf_col_grid->set(row, col, grid[1]);
        buf_col_grid->set(row, col + 1, grid[2]);
        buf_col_grid->set(coord[0], coord[1] + 1, grid[3]);
    }
    return true;
}
bool Theory::slave3_3(int row, int col, int vertex) {
    int coord[2] = {
        row + vertex_node[vertex][0][0],
        col + vertex_node[vertex][0][1]
    };
    short grid[4] = {
        buf_row_grid->get(coord[0], coord[1] - 1, 0),
        buf_col_grid->get(coord[0] - 1, coord[1], 0),
        buf_row_grid->get(coord[0], coord[1], 0),
        buf_col_grid->get(coord[0], coord[1], 0),
    };
    short cellGrid[4] = {
        buf_col_grid->get(row, col, 0),
        buf_row_grid->get(row, col, 0),
        buf_row_grid->get(row, col + 1, 0),
        buf_row_grid->get(row + 1, col, 0)
    };
    int idx[6] = {
        vertex_node[vertex][1][0],
        vertex_node[vertex][1][1],
        vertex_node[vertex][2][0],
        vertex_node[vertex][2][1],
        vertex_node[vertex][3][0],
        vertex_node[vertex][3][1]
    };

    if (grid[idx[0]] == 1 || grid[idx[1]] == 1) {
        if (cellGrid[idx[2]] == 0) return false;
        if (cellGrid[idx[3]] == 0) return false;
        cellGrid[idx[2]] = 1;
        cellGrid[idx[3]] = 1;
    } else if (grid[idx[0]] == 0 && grid[idx[1]] == 0) {
        if (cellGrid[idx[4]] == 0) return false;
        if (cellGrid[idx[5]] == 0) return false;
        cellGrid[idx[4]] = 1;
        cellGrid[idx[5]] = 1;
    }

    buf_row_grid->set(row, col, cellGrid[0]);
    buf_col_grid->set(row, col, cellGrid[1]);
    buf_row_grid->set(row, col + 1, cellGrid[2]);
    buf_col_grid->set(row + 1, col, cellGrid[3]);

    return true;
}
bool Theory::theory3() {
    int coord[2] = {0, -1};
    while (checker->searchCell(3, coord)) {
        for (int i = 0; i < 4; i++) {
            if (!slave3_1(coord[0], coord[1], i)) return false;
            if (!slave3_2(coord[0], coord[1], i)) return false;
            if (!slave3_3(coord[0], coord[1], i)) return false;
        }
    }
    return true;
}
