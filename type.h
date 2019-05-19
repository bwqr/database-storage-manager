#ifndef SRC_TYPE_H
#define SRC_TYPE_H

#include <string>
#include <fstream>
#include <set>
#include <vector>
#include "index.h"
#include "directory.h"

class type {
public:
    std::string typeName;
    uint8_t  numFields = 0;
    mutable uint64_t cardinality;
    uint8_t  primaryKey;
    std::vector<std::string> fieldsName;
    directory *dir = nullptr;

    type();

    type(std::string &typeName, uint8_t  numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);

    bool operator<(const type &type) const {
        return this->typeName < type.typeName;
    };
};

#endif //SRC_TYPE_H
