#include "search.h"

namespace {
void getVertexGrid (const GridSearchDataStore data, char *grid) {

    grid[0] = data.rgrid.get(data.coord.y, data.coord.x - 1);
    grid[1] = data.cgrid.get(data.coord.y - 1, data.coord.x);
    grid[2] = data.rgrid.get(data.coord.y, data.coord.x);
    grid[3] = data.cgrid.get(data.coord.y, data.coord.x);
}
void applyVertexGrid (GridSearchDataStore* data, char *grid) {

    data->rgrid.set(data->coord.y, data->coord.x - 1, grid[0]);
    data->cgrid.set(data->coord.y - 1, data->coord.x, grid[1]);
    data->rgrid.set(data->coord.y, data->coord.x, grid[2]);
    data->cgrid.set(data->coord.y, data->coord.x, grid[3]);
}

bool setNextDirection (GridSearchDataStore* curr) {

    char grid[4];

    getVertexGrid(*curr, grid);
    for (;curr->direction < 4; ++(curr->direction)) {
        if (grid[curr->direction] == 2) {
            grid[curr->direction] = 1;
            break;
        }
    }
    if (curr->direction == 4) {
        return false;
    }
    applyVertexGrid(curr, grid);
    return true;
}

}

void executeSearch (const Matrix &cell, Matrix* rgrid, Matrix* cgrid) {

    rgrid->setExceptValue(static_cast<char>(0));
    cgrid->setExceptValue(static_cast<char>(0));

    std::vector<GridSearchDataStore> stored;
    GridSearchDataStore curr(*rgrid, *cgrid, Coord(-1, 0), -1);

    if (!searchEdge(curr.rgrid, curr.cgrid, &(curr.coord))) {
        return;
    }
    stored.push_back(curr);

    while (true) {
        if (!setNextDirection(&curr)) {
            curr.direction = stored.back().direction + 1;
            curr.coord = stored.back().coord;
            stored.pop_back();
            curr.rgrid.copy(stored.back().rgrid);
            curr.cgrid.copy(stored.back().cgrid);
            continue;
        }

        if (!applyDefinite(cell, &(curr.rgrid), &(curr.cgrid))) {
            curr.rgrid.copy(stored.back().rgrid);
            curr.cgrid.copy(stored.back().cgrid);
            ++(curr.direction);
            continue;
        }

        stored.push_back(curr);

        curr.coord = Coord(-1, 0);
        curr.direction = 0;
        if (!searchEdge(curr.rgrid, curr.cgrid, &(curr.coord))) {
            break;
        }
    }
    rgrid->copy(stored.back().rgrid);
    cgrid->copy(stored.back().cgrid);
}

void executeSearchWithLog (
        const Matrix &cell, Matrix* rgrid, Matrix* cgrid,
        const std::string logdirpath) {

    rgrid->setExceptValue(static_cast<char>(0));
    cgrid->setExceptValue(static_cast<char>(0));

    std::vector<GridSearchDataStore> stored;
    GridSearchDataStore curr(*rgrid, *cgrid, Coord(-1, 0), -1);

    if (!searchEdge(curr.rgrid, curr.cgrid, &(curr.coord))) {
        return;
    }
    stored.push_back(curr);

    ImageBuilder img("graphics.dat");
    const Matrix vertex(cell.rows() + 1, cell.cols() + 1, 0);
    std::ostringstream oss;
    int cnt = 0;

    while (true) {
        std::cout << "Coord: " << curr.coord.x << ' '
            << curr.coord.y << std::endl;
        if (!setNextDirection(&curr)) {
            curr.direction = stored.back().direction + 1;
            curr.coord = stored.back().coord;
            stored.pop_back();
            curr.rgrid.copy(stored.back().rgrid);
            curr.cgrid.copy(stored.back().cgrid);
            std::cout << "No direction found: stack = " <<
                stored.size() << std::endl;
            continue;
        }

        std::cout << "Direction: " << curr.direction << std::endl;
        if (!applyDefinite(cell, &(curr.rgrid), &(curr.cgrid))) {
            std::cout << "Invalid grid" << std::endl;
            curr.rgrid.copy(stored.back().rgrid);
            curr.cgrid.copy(stored.back().cgrid);
            ++(curr.direction);
            continue;
        }

        oss.str("");
        oss << "/" << std::setw(5) << std::setfill('0') << cnt++ << ".png";
        img.write(logdirpath + oss.str(), cell, curr.rgrid, curr.cgrid, vertex);
        std::cout << "save image to \"" << oss.str() << "\"" << std::endl;
        std::cout << "New grid added: stack = " << stored.size() <<
            std::endl;
        stored.push_back(curr);

        curr.coord = Coord(-1, 0);
        curr.direction = 0;
        if (!searchEdge(curr.rgrid, curr.cgrid, &(curr.coord))) {
            break;
        }
    }

    rgrid->copy(stored.back().rgrid);
    cgrid->copy(stored.back().cgrid);
}
