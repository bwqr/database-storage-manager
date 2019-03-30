#ifndef SRC_TYPE_H
#define SRC_TYPE_H

#include <tiff.h>
#include <string>
#include <fstream>
#include <set>
#include "index.h"

#define NUM_FIELDS 1
#define TYPE_NAME 8
#define CARDINALITY 8
#define PRIMARY_KEY 1
#define FIELD_NAME 8
#define FIELD 4

class type {
public:
    std::string typeName;
    uint8 numFields;
    uint64 cardinality;
    uint8 primaryKey;
    std::string* fieldsName;
    std::set<index> indexes;

    bool is_index_read = false;

    type();

    type(std::string &typeName, uint8 numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);

    void writeIndex(std::fstream &stream);
//    type(char *typeName, int8 numFields, int64 cardinality, int8 primaryKey, char** fieldsName);
};

#endif //SRC_TYPE_H
