#include "record.h"
#include <iostream>
record::record() {

}

record::record(uint8 recordID, uint8 numFields) {
    this->recordID = recordID;
    this->fields = new int32[numFields];
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
