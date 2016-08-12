#ifndef SOLVE_H_
#define SOLVE_H_

#include "../util/utilarray.h"

class Checker {
public:
    Checker(const ArrayD<short> &givenCell);
    Checker() {};
    ~Checker() {};
    void reloadVertex(
            const ArrayD<short> &row_grid,
            const ArrayD<short> &col_grid,
            ArrayD<short> *vertex) const;
    bool searchCell(int num, int *resume) const;
    bool searchCell0(int *resume) const;
    bool checkVertex(
            ArrayD<short> *row_grid,
            ArrayD<short> *col_grid
            ) const;
    bool checkCell(
            ArrayD<short> *row_grid,
            ArrayD<short> *col_grid
            ) const;
private:
    const int vertex_map[7][2] = {
        {}, {0, 2}, {1, 3}, {0, 1}, {1, 2}. {2, 3}, {3, 0}
    };
    ArrayD<short> cell;
    void setVertexGrid(int value, short *grid) const;
};

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
    bool theory0();
    bool slave0(int row, int col, int diag);
    bool theory3();
    bool slave3_1(int row, int col, int diag);
    bool slave3_2(int row, int col, int cross);
    bool slave3_3(int row, int col, int vertex);

    Utilarray *buf_row_grid = NULL;
    Utilarray *buf_col_grid = NULL;
    Utilarray *buf_vertex = NULL;
    Utilarray *buf_cell = NULL;
    Checker *checker = NULL;
};

class Solve {
public:
    Solve(const Utilarray &givenRow,
          const Utilarray &givenCol,
          const Utilarray &givenCell);
    ~Solve();
    void run();
private:
    const int node_map[4][3][2] = {
        {{0}, {0, -1}, {0, -1}},
        {{1}, {-1, 0}, {-1, 0}},
        {{0}, {0, 0}, {0, 1}},
        {{0}, {0, 0}, {1, 0}}
    };

    void connectEdge(int *edge) const; // search_edge
    int getEdgeDirection(int row, int col) const; // edge_dir
    int searchEdge(); // mk_edge_map
    void setEdge(); //set_edge_list

    void getOtherEdge(int *edge) const; // connectEdge
    bool isReturned(int direction1, int direction2) const; //isReturn

    bool next();
    bool setNextCoord(); // set_node
    void back();

    bool isCompleted() const;
    bool isAnswer() const;

    Utilarray *row_grid;
    Utilarray *col_grid;
    Urilarray *cell;
    ArrayD<int> *edge_list;
    ArrayD<int> *edge_map;
    vector<bool> is_edge_visited;
    vector<bool> is_skipped;
    vector<short> node_log;
    int coord[2];
    bool is_all_case_considered = false;
    int cycle = 0;
    int answer = 0;
};
#endif /* SOLVE__H_ */
