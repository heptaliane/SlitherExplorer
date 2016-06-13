#include "theory.h"

// --- constructor ---
Theory::Theory(const ArrayD<short> &cell) {
    init(cell);
}
Theory::Theory(const Utilarray &cell) {
    ArrayD<short> temp;
    cell->exportArray(&temp);
    init(temp);
}
Theory::Theory(){

}
Theory::~Theory(){
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
    buf_row_grid->(obj);
}
void Theory::cpyColGrid(ArrayD<short> *obj) const {
    buf_col_grid->(obj);
}
void Theory::cpyVertex(ArrayD<short> *obj) const {
    buf_vertex->(obj);
}
// --- private ---
void Theory::init(const ArrayD<short> &cell) {
    const int cell_row_size = cell.getlen1();
    const int cell_col_size = cell.getlen2();

    buf_row_grid = new Utilarray(cell_row_size + 1, cell_col_size, 2);
    buf_col_grid = new Utilarray(cell_row_size, cell_col_size + 1, 2);
    buf_vertex = new Utilarray(cell_row_size + 1, cell_col_size + 1, 0);
    buf_cell = new Utilarray(cell_row_size, cell_col_size, 4);

    buf_cell->clone(cell);
}
