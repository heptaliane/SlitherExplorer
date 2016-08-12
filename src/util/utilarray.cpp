#include "utilarray.h"

//---constructor--
Utilarray::Utilarray(const ArrayD<short> &obj) {
    init(obj.getlen1(), obj.getlen2());
    array->clone(obj);
}
Utilarray::Utilarray(int row_size, int col_size) {
    init(row_size, col_size);
}
Utilarray::Utilarray(int row_size, int col_size, int value) {
    init(row_size, col_size);
    clear(value);
}
Utilarray::Utilarray(int row_size, int col_size, int value, int reserve_size) {
    init(row_size, col_size);
    clear(value);
    log_reserved_size = reserve_size;
    is_log_reserved = true;
    log.reserve(reserve_size);
}
Utilarray::Utilarray(const Utilarray &obj) {
    init(obj.getlen1(), obj.getlen2());
    clone(obj);
}

//---destructor--
Utilarray::~Utilarray() {
    delete array;
    array = NULL;
}

//---public---
short Utilarray::get(int row, int col, int exception) const {
    if (row >= array->getlen1() || col >= array->getlen2() || row < 0 || col < 0) {
        return static_cast<short>(exception);
    }

    return array->getArray(row, col);
}
void Utilarray::set(int row, int col, int value) {
    if (row >= array->getlen1() || col >= array->getlen2() || row < 0 || col < 0) {
        return;
    }
    //store the value
    setLog(row, col);

    array->setArray(row, col, value);
}
void Utilarray::undo() {
    //in case log has no data
    if (log.empty()) {
        cout << "NullPointerException" << endl;
        return;
    }
    //apply newest log
    //array->setArray(log[log.size()][0][0], log[log.size()][0][1], log[log.size()][1]);
    array->setArray(log[log.size()].first.first, log[log.size()].first.second, log[log.size()].second);
}
void Utilarray::clone(const Utilarray &obj) {
    //adjust array size
    if (obj.getlen1() != getlen1() || obj.getlen2() != getlen2()) {
        delete array;
        array = new ArrayD<short>(obj.getlen1(), obj.getlen2());
    }

    log.clear();

    //if "log" vector is reserved, set reserved state.
    if (obj.getLogReservedProperty()) {
        if (!is_log_reserved) {
            is_log_reserved = true;
        } else {
            log.shrink_to_fit();
        }
        log_reserved_size = obj.getLogReservedSize();
        log.reserve(obj.getLogReservedSize());
    }

    //copy array
    for (int i = 0; i < array->getlen1(); i++) {
        for (int j = 0; j < array->getlen2(); j++) {
            array->setArray(i, j, obj.get(i, j, 0));
        }
    }
    exportLog(&log);
}
void Utilarray::clone(const ArrayD<short> &obj) {
    array->clone(obj);
    log.clear();
}
void Utilarray::clear(int value) {
    for (int i = 0; i < array->getlen1(); i++) {
        for (int j = 0; j < array->getlen2(); j++) {
            array->setArray(i, j, static_cast<short>(value));
        }
    }
    log.clear();
}
bool Utilarray::equals(const Utilarray &obj) const {
    for (int i = 0; i < obj.getlen1(); i++) {
        for (int j = 0; j < obj.getlen2(); j++) {
            if(get(i, j, 0) != obj.get(i, j, 0)) return false;
        }
    }
    return true;
}
void Utilarray::exportLog(vector<pair<pair<int, int>, short> > *vec) const {
    vec->clear();
    for (auto itr = log.cbegin(); itr != log.cend(); ++itr) {
        vec->push_back(*itr);
    }
}
void Utilarray::exportArray(ArrayD<short> *obj) const {
    obj->clone(*array);
}

//---private---
void Utilarray::init(int row_size, int col_size) {
    array = new ArrayD<short>(row_size, col_size);
}
void Utilarray::setLog(int row, int col) {
    //when log is full, delete oldest log if log reserved
    if (is_log_reserved) {
        if (log.size() == log_reserved_size) {
            killLogStack();
        }
    }

    //store coodinate and value
    log.push_back(make_pair(make_pair(row, col), array->getArray(row, col)));
}
void Utilarray::killLogStack() {
    log.erase(log.begin());
}
