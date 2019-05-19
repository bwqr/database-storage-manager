#include "record.h"
#include <iostream>

record::record(uint8_t recordID, uint8_t numFields) {
    this->recordID = recordID;
    this->fields = new int32_t[numFields];
    this->numFields = numFields;
}

void record::read(std::fstream &stream, int offset) {
    stream.seekg(offset);

    stream.read((char *) &this->recordID, RECORD_ID);
    stream.read((char *) this->fields, this->numFields * FIELD);
}

void record::write(std::fstream &stream, int offset) {
    stream.seekp(offset);

    stream.write((char *) &this->recordID, RECORD_ID);
    stream.write((char *) this->fields, this->numFields* FIELD);
}

record::~record() {
    if (numFields > 0) {

        delete[] this->fields;
    }
}

record::record(const record &r) {
    recordID = r.recordID;
    numFields = r.numFields;

    fields = new int32_t[numFields];

    for (int i = 0; i < numFields; ++i) {
        fields[i] = r.fields[i];
    }
}

record &record::operator=(const record &r) {
    if(this != &r) {
        this->~record();

        recordID = r.recordID;
        numFields = r.numFields;

        fields = new int32_t[numFields];

        for (int i = 0; i < numFields; ++i) {
            fields[i] = r.fields[i];
        }
    }
    return *this;
}
