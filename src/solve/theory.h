#ifndef THEORY_H_
#define THEORY_H_

#include "../util/utilarray.h"

class Theory {
public:
    Theory(const ArrayD<short> &cell);
    Theory(const Utilarray &cell);
    Theory();
    ~Theory();
    bool run();
    void cpyRowGrid(ArrayD<short> *obj) const;
    void cpyColGrid(ArrayD<short> *obj) const;
    void cpyVertex(ArrayD<short> *obj) const;
private:
    const int vertex_map[7][2] = {
        {}, {0, 2}, {1, 3}, {0, 1}, {1, 2}. {2, 3}, {3, 0}
    };
    const int diag_node[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    const int cross_node[4][2] = {
        {-1, 0}, {0, -1}, {1, 0}, {0, 1}
    };
    const int vertex_node[4][4][2] = {
        {{0, 0}, {0, 1}, {2, 3}, {0, 1}},
        {{0, 1}, {1, 2}, {3, 0}, {1, 2}},
        {{1, 0}, {3, 0}, {1, 2}, {3, 0}},
        {{1, 1}, {2, 3}, {0, 1}, {2, 3}}
    };
    const int diag_0[4][2][2] = {
        {{0, -1}, {-1, 0}},
        {{0, 1}, {-1, 1}},
        {{1, -1}, {1, 0}},
        {{1, 1}, {1, 1}}
    };
    const int diag_3[4][4][2] = {
        {{-1, -1}, {1, 0}, {-1, -1}, {0, 1}},
        {{-1, 1}, {1, 0}, {-1, 2}, {3, 0}},
        {{0, 0}, {2, -1}, {0, 1}, {1, -1}},
        {{0, 0}, {2, 1}, {0, 0}, {1, 2}}
    };

    void init(const ArrayD<short> &cell);
    void reloadVertex(int row, int col);
    bool serachCell(int num, int *resume);
    bool serachCell0(int *resume);
    bool checkVertex(int row, int col);
    bool checkCell();
    bool theory0();
    bool theory3();

    Utilarray *buf_row_grid;
    Utilarray *buf_col_grid;
    Utilarray *buf_vertex;
    Utilarray *buf_cell;
};

#endif /* THEORY__H_ */
