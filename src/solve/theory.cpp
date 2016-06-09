#include "theory.h"

// --- constructor ---
Theory::Theory(ArrayD<short> &cell) {
}

// --- public ---


// --- private ---
void Theory::init(ArrayD<short> &cell) {
    buf_row_grid = new Utilarray(cell.getlen1() + 1, cell.getlen2(), 2);
    buf_col_grid = new Utilarray(cell.getlen1(), cell.col_len2() + 1, 2);
    buf_vertex = new Utilarray(cell.getlen1() + 1, cell.getlen2() + 1, 0);
    buf_cell = new Utilarray(cell.getlen1(), cell.getlen2(), 4);

    buf_cell->clone(cell);
}
