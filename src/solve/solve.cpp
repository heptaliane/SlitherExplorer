#include "solve.h"

Solve::Solve(const Utilarray &kRow, const Utilarray &kCol, const ArrayD<short> &kCell) {
    int row = kCell.getlen1() + 1, col = kCell.getlen2() + 1;
    row_grid = new Utilarray(kRow);
    col_grid = new Utilarray(kCol);
    cell = new ArrayD<short>(kCell);
    checker = new Checker(*cell);

    edge_map = new ArrayD<int>(row, col);
    setEdge();

    is_edge_visited.reserve(edge_list->getlen1());
    coord_log.reserve(row * col);
    node_log.reserve(row * col);
    is_skipped.reserve(row * col);

    coord[0] = edge_list->getArray(0, 0);
    coord[1] = edge_list->getArray(0, 1);
}
Solve::~Solve() {
    delete row_grid;
    row_grid = NULL;
    delete col_grid;
    col_grid = NULL;
    delete cell;
    cell = NULL;
    delete checker;
    checker = NULL;
    delete edge_map;
    edge_map = NULL;
    delete edge_list;
    edge_list = NULL;
}
//--- private ---
void Solve::connectEdge(int *edge) const {
    int prev = -1, next = -1;
    int count;
    short grid[4];
    while (true) {
        grid[0] = row_grid->get(edge[0], edge[1] - 1, 0);
        grid[1] = col_grid->get(edge[0] - 1, edge[1], 0);
        grid[2] = row_grid->get(edge[0], edge[1], 0);
        grid[3] = col_grid->get(edge[0], edge[1], 0);
        count = 0;
        for (int i = 0; i < 4; i++) {
            if (grid[i] == 1) {
                count++;
                if (!isReturned(prev, i)) next = i;
            }
        }

        if (count == 1 && prev != -1) break;
        if (next == -1) break;

        edge[0] += node_map[next][2][0];
        edge[1] += node_map[next][2][1];
        prev = next;
        next = -1;
    }
}
int Solve::getEdgeDirection(int row, int col) const {
    short grid[4] = {
        row_grid->get(row, col - 1, 0),
        col_grid->get(row - 1, col, 0),
        row_grid->get(row, col, 0),
        col_grid->get(row, col, 0)
    };
    int node = 0;
    for (int i = 0; i < 4; i++) {
        if (grid[i] == 1) node = i;
    }
    return direction[node];
}
int Solve::searchEdge() {
    int edge_count = 0;
    int count;
    short grid[4];
    for (int i = 0; i < edge_map->getlen1(); i++) {
        for (int j = 0; j < edge_map->getlen2(); j++) {
            grid[0] = row_grid->get(i, j - 1, 0);
            grid[1] = col_grid->get(i - 1, j, 0);
            grid[2] = row_grid->get(i, j, 0);
            grid[3] = col_grid->get(i, j, 0);
            for (int k = 0; k < 4; k++) {
                if (grid[k] == 1) count++;
            }
            if (count == 1) {
                edge_map->setArray(i, j, -1);
                edge_count++;
            } else {
                edge_map->setArray(i, j, -2);
            }
        }
    }
    return edge_count / 2;
}
void Solve::setEdge() {
    edge_list = new ArrayD<int>(searchEdge(), 6);
    int idx = 0;
    for (int i = 0; i < edge_map->getlen1(); i++) {
        for (int j = 0; j < edge_map->getlen2(); j++) {
            if (edge_map->getArray(i, j) != -1) continue;
            int edge[2] = {i, j};
            connectEdge(edge);
            edge_map->setArray(i, j, idx);
            edge_map->setArray(edge[0], edge[1], idx);

            edge_list->setArray(idx, 0, i);
            edge_list->setArray(idx, 0, j);
            edge_list->setArray(idx, 0, getEdgeDirection(i, j));
            edge_list->setArray(idx, 0, edge[0]);
            edge_list->setArray(idx, 0, edge[1]);
            edge_list->setArray(idx, 0, getEdgeDirection(edge[0], edge[1]));
            idx++;
        }
    }
}
void Solve::getOtherEdge(int *edge) const {
    int idx = edge_map->getArray(edge[0], edge[1]);
    if (edge_list->getArray(idx, 0) == edge[0] ||
            edge_list->getArray(idx, 1) == edge[1]) {
        edge[0] = edge_list->getArray(idx, 0);
        edge[1] = edge_list->getArray(idx, 1);
    } else {
        edge[0] = edge_list->getArray(idx, 3);
        edge[1] = edge_list->getArray(idx, 4);
    }
}
bool Solve::isReturned(int direction1, int direction2) const {
    // (arg1, arg2) == (0, 2), (1, 3), (2, 0), (3, 1)
    if (direction1 % 4 == direction2 % 4) return false;
    if ((direction1 + direction2) % 2 == 0) return true;
    return false;
}
bool Solve::next() {
    int idx = edge_map->getArray(coord[0], coord[1]);

    if (!idx != -2 && !is_edge_visited[idx]) {
        int next[2] = {coord[0],  coord[1]};
        getOtherEdge(next);

        coord_log.push_back(make_pair(coord[0], coord[1]));

        coord[0] = next[0];
        coord[1] = next[1];

        is_edge_visited[idx] = true;
        is_skipped.push_back(true);

        if (edge_list->getArray(idx, 0) == coord[0] &&
                edge_list->getArray(idx, 1) == coord[1]) {
            node_log.push_back(edge_list->getArray(idx, 2));
        } else {
            node_log.push_back(edge_list->getArray(idx, 5));
        }

        cycle++;
        return true;
    }

    short next_node = node_log[cycle] + 1;
    short prev_node = node_log[cycle - 1];

    if (isReturned(next_node, prev_node))
        next_node += 1;

    if (next_node > 3) {
        node_log[cycle] = -1;
        back();
        return false;
    }

    node_log[cycle] = next_node;
    return setNextCoord();
}
bool Solve::setNextCoord() {
    int row = coord[0] + node_map[node_log[cycle]][1][0];
    int col = coord[1] + node_map[node_log[cycle]][1][1];

    if (node_map[node_log[cycle]][0][0] == 0) {

        if (row_grid->get(row, col, 0) == 0) return false;
        row_grid->set(row, col, 1);

    } else if (node_map[node_log[cycle]][0][0] == 1) {

        if (col_grid->get(row, col, 0) == 0) return false;
        col_grid->set(row, col, 1);

    }

    coord_log.push_back(make_pair(coord[0], coord[1]));

    coord[0] += node_map[node_log[cycle]][2][0];
    coord[1] += node_map[node_log[cycle]][2][1];

    is_skipped.push_back(false);
    cycle++;

    return true;
}
void Solve::back() {
    cycle -= 1;

    if (cycle == 0) {
        is_all_case_considered = true;
        return;
    }

    if (is_skipped[cycle]) {
        is_edge_visited[edge_map->getArray(coord[0], coord[1])] = false;
        node_log[cycle] = -1;
        cycle -= 1;
    }

    if (node_map[node_log[cycle]][0][0] == 0)
        row_grid->undo();
    else if (node_map[node_log[cycle]][0][0] == 1)
        col_grid->undo();

    coord[0] = coord_log[cycle].first;
    coord[1] = coord_log[cycle].second;
    coord_log.pop_back();
}
bool Solve::checkVertex() {
    if (cycle == 1) return true;
    if (is_all_case_considered) return true;

    int row_size = cell->getlen1() + 1;
    int col_size = cell->getlen2() + 1;
    short grid[4];
    int cnt1, cnt0;

    for (int row = 0; row < row_size; row++) {
        for (int col = 0; col < col_size; col++) {
            grid[0] = row_grid->get(row, col - 1, 0);
            grid[1] = col_grid->get(row - 1, col, 0);
            grid[2] = row_grid->get(row, col, 0);
            grid[3] = col_grid->get(row, col, 0);

            cnt1 = 0;
            cnt0 = 0;

            for (int i = 0; i < 4; i++) {
                if (grid[i] == 0)
                    cnt0 += 1;
                else if (grid[i] == 1)
                    cnt1 += 1;
            }

            if (cnt1 > 2)
                return false;
            if (cnt1 == 1 && cnt0 == 3)
                return false;
        }
    }

    return true;
}
bool Solve::accelerator1() {
    int coord[2] = {0, -1};
    int vnode[4][2];
    short grid[4];
    int cnt0, cnt2, cnt;
    int is_grid;

    while (checker->searchCell(3, coord)) {
        vnode[0][0] = coord[0];
        vnode[0][1] = coord[1];
        vnode[1][0] = coord[0];
        vnode[1][1] = coord[1] + 1;
        vnode[2][0] = coord[0] + 1;
        vnode[2][1] = coord[1];
        vnode[3][0] = coord[0] + 1;
        vnode[3][1] = coord[1] + 1;

        cnt0 = 0;
        cnt2 = 0;

        for (int i = 0; i < 4; i++) {
            grid[0] = row_grid->get(vnode[i][0], vnode[i][1] - 1, 0);
            grid[1] = col_grid->get(vnode[i][0] - 1, vnode[i][1], 0);
            grid[2] = row_grid->get(vnode[i][0], vnode[i][1], 0);
            grid[3] = col_grid->get(vnode[i][0], vnode[i][1], 0);

            cnt = 0;
            for (int j = 0; j < 4; j++) {
                if (grid[j] == 1)
                    cnt += 1;
            }

            if (cnt = 2) 
                cnt2 += 1;
            else if (cnt == 0)
                cnt0 += 1;
        }

        if (cnt2 + cnt0 == 4 && cnt0 != 4) {
            grid[0] = row_grid->get(coord[0], coord[1], 0);
            grid[1] = col_grid->get(coord[0], coord[1], 0);
            grid[2] = row_grid->get(coord[0] + 1, coord[1], 0);
            grid[3] = col_grid->get(coord[0], coord[1] + 1, 0);

            is_grid = 0;
            for (int k = 0; k < 4; k++) {
                if (grid[k] == 1)
                    is_grid += 1;
            }

            if (is_grid != 3)
                return false;
        }

    }

    return true;
}
