#include "type.h"
#include <string>
#include <iostream>
type::type() {
    typeName = std::string(8, ' ');
}

type::type(std::string &typeName, uint8 numFields): type() {
    this->typeName = typeName;
    this->numFields = numFields;
    this->cardinality = 0;
    this->primaryKey = 0;
    this->index_root_pointer = 1;
    this->fieldsName = std::vector<std::string>(numFields);
}

void type::read(std::fstream &stream, int offset) {
    stream.seekg(offset);

    stream.read((char *) this->typeName.c_str(), TYPE_NAME);

    stream.read((char *) &this->numFields, NUM_FIELDS);
    stream.read((char *) &this->cardinality, CARDINALITY);
    stream.read((char *) &this->primaryKey, PRIMARY_KEY);
    stream.read((char *) &this->index_root_pointer, BTREE_ID);

    this->fieldsName = std::vector<std::string>(this->numFields);

    for (int j = 0; j < this->numFields; ++j) {
        this->fieldsName[j] = std::string(8, 0x20);
        stream.read((char *) this->fieldsName[j].c_str(), FIELD_NAME);
    }
}

void type::write(std::fstream &stream, int offset) {
    stream.seekp(offset);

    stream.write((char *) this->typeName.c_str(), TYPE_NAME);
    stream.write((char *) &this->numFields, NUM_FIELDS);
    stream.write((char *) &this->cardinality, CARDINALITY);
    stream.write((char *) &this->primaryKey, PRIMARY_KEY);
    stream.write((char *) &this->index_root_pointer, BTREE_ID);

    for (int i = 0; i < this->numFields; ++i) {
        stream.write((char *) this->fieldsName[i].c_str(), FIELD_NAME);
    }
}

void type::writeIndex(std::fstream &stream) {
    stream.seekp(0);

    for (auto i : this->indexes) {
        stream.write((char *) &i.file_id, FILE_ID );
        stream.write((char *) &i.page_id, PAGE_ID);
        stream.write((char *) &i.record_id, RECORD_ID);
        stream.write((char *) &i.value, FIELD);

    }
}