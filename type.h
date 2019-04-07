#ifndef SRC_TYPE_H
#define SRC_TYPE_H

#include <tiff.h>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include "index.h"
#include "directory.h"

class type {
public:
    std::string typeName;
    uint8 numFields = 0;
    mutable uint64 cardinality;
    uint8 primaryKey;
    std::vector<std::string> fieldsName;
    mutable std::set<index> indexes;
    directory *dir = nullptr;

    mutable bool is_index_read = false;

    type();

    type(std::string &typeName, uint8 numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);

    void writeIndex(std::fstream &stream);

    bool operator<(const type &type) const {
        return this->typeName < type.typeName;
    };
};

#endif //SRC_TYPE_H
