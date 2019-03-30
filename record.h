#ifndef SRC_RECORD_H
#define SRC_RECORD_H

#include <tiff.h>
#include <fstream>

#define RECORD_ID 1
#define FIELD 4

class record {
public:
    uint8 recordID;
    int32 *fields;
    uint8 numFields;

    record();

    record(uint8 recordID, uint8 numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);
};


#endif //SRC_RECORD_H
