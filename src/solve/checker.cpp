#include "solve.h"

Checker::Checker(const ArrayD<short> &givenCell) {
    cell.clone(givenCell);
}

void Checker::reloadVertex(const ArrayD<short> &row_grid, const ArrayD<short> &col_grid, ArrayD<short> *vertex) const {
    int row, col;
    short grid[4];
    bool isValueChanged;
    for (row = 0; row < vertex->getlen1(); row++) {
        for (col = 0; col < vertex->getlen2(); col++) {
            isValueChanged = false;
            grid[0] = row_grid.getArray(row, col - 1);
            grid[1] = col_grid.getArray(row - 1, col);
            grid[2] = row_grid.getArray(row, col);
            grid[3] = col_grid.getArray(row, col);

            for (int i = 1; i < 7; i++) {
                if (grid[vertex_map[i][0]] == 1 && grid[vertex_map[i][1]] == 1) {
                    vertex->setArray(row, col, i);
                    isValueChanged = true;
                    break;
                }
            }
            if (!isValueChanged) vertex->setArray(row, col, 0);
        }
    }
}
bool Checker::searchCell(int num, int *resume) const {
    int row_size = cell.getlen1();
    int col_size = cell.getlen2();

    if (num == 0) {
        if (resume[0] < 0 || resume[0] >= row_size) {
            return searchCell0(resume);
        }
    }

    int row = resume[0], col = resume[1] + 1;

    for ( ; row < row_size; row++) {
        for ( ; col < col_size; col++) {
            if (cell.getArray(row, col) == num) {
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
bool Checker::searchCell0(int *resume) const {
    if (resume[0] == -1) {

        if (resume[1] == -1) {
            //(-1, -1) -> (-1, colSize)
            resume[1] = cell.getlen2();
        } else {
            //(-1, colSize) -> (colSize, rowSize)
            resume[0] = cell.getlen1();
        }

    } else if (resume[0] == cell.getlen1()) {

        if (resume[1] == -1) {
            //(rowSize, -1) -> (-1, -1)
            resume[0] = -1;
        } else {
            //(rowSize, colSize) -> end
            return false;
        }

    } else {
        //set (rowSize, -1)
        resume[0] = cell.getlen1();
        resume[1] = -1;
    }
    return true;
}
bool Checker::checkVertex(ArrayD<short> *row_grid, ArrayD<short> *col_grid) const {
    int row, col;
    int count0, count1;
    short grid[4];

    for (row = 0; row < row_grid->getlen1(); row++) {
        for (col = 0; col < col_grid->getlen2(); col++) {
            grid[0] = row_grid->getArray(row, col - 1);
            grid[1] = col_grid->getArray(row - 1, col);
            grid[2] = row_grid->getArray(row, col);
            grid[3] = col_grid->getArray(row, col);

            for (int i = 0; i < 4; i++) {
                if (grid[i] == 0) count0++;
                else if (grid[i] == 1) count1++;
            }

            if (count1 > 2) return false;
            if (count0 == 3 && count1 == 1) return false;

            if ((count0 == 3 && count1 == 0) || (count0 == 2 && count0 != 2)) {
                setVertexGrid(0, grid);
            }

            if (count0 == 2 && count1 == 1) {
                setVertexGrid(1, grid);
            }
            row_grid->setArray(row, col - 1, grid[0]);
            col_grid->setArray(row - 1, col, grid[1]);
            row_grid->setArray(row, col, grid[2]);
            col_grid->setArray(row, col, grid[3]);

        }
    }
    return true;
}
bool Checker::checkCell(const ArrayD<short> &row_grid, const ArrayD<short> &col_grid) const {
    int cell_point[2];
    short grid[4];
    for (int i = 0; i <= 4; i++) {
        cell_point[0] = 0;
        cell_point[1] = -1;
        while (searchCell(i, cell_point)) {
            grid[0] = row_grid.getArray(cell_point[0], cell_point[1] - 1);
            grid[1] = col_grid.getArray(cell_point[0] - 1, cell_point[1]);
            grid[2] = row_grid.getArray(cell_point[0], cell_point[1]);
            grid[3] = col_grid.getArray(cell_point[0], cell_point[1]);

            int count = 0;
            for (int j = 0; j < 4; j++)
                if (grid[j] == 1) count++;

            if (count != i) return false;
        }
    }
    return false;
}

void Checker::setVertexGrid(int value, short *grid) const {
    for(int i = 0; i < 4; i++) {
        if (grid[i] == 2) {
            grid[i] = value;
        }
    }
}
