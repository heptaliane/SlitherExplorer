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
bool Theory::serachCell(int num, int *resume) const {
    int row_size = buf_cell->getlen1();
    int col_size = buf_cell->getlen2();

    if (num == 0) {
        if (resume[0] < 0 || resume[0] >= row_size) {
            return searchCell0(resume);
        }
    }

    int row = resume[0], col = resume[1] + 1;

    for ( ; row < row_size; row++) {
        for ( ; col < col_size; col++) {
            if (buf_cell->getArray(row, col, 4) == num) {
                resume[0] = row;
                resume[1] = col;
                return true;
            }
        }
    }

    if (num == 0) {
        return searchCell0(resume);
    }

    return false;
}
bool Theory::searchCell0(int *resume) const {
    if (resume[0] == -1) {

        if (resume[1] == -1) {
            //(-1, -1) -> (-1, colSize)
            resume[1] = buf_cell->getlen2();
        } else {
            //(-1, rowSize) -> (colSize, rowSize)
            resume[0] = buf_cell->getlen1();
        }

    } else if (resume[0] == buf_cell->getlen1()) {

        if (resume[1] == -1) {
            //(rowSize, -1) -> (-1, -1)
            resume[0] = -1;
        } else {
            //(rowSize, colSize) -> end
            return false;
        }

    } else {
        //begin -> (rowSize, -1)
        resume[0] = buf_cell->getlen1();
        resume[1] = -1;
    }

    return true;
}
