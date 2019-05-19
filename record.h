#ifndef SRC_RECORD_H
#define SRC_RECORD_H

#include <fstream>

#define RECORD_ID 1
#define FIELD 4

class record {
public:
    uint8_t recordID;
    int32_t *fields;
    uint8_t numFields = 0;

    record() = default;

    ~record();

    record(uint8_t recordID, uint8_t numFields);

    record(const record& r);

    record& operator=(const record& r);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);

    bool operator<(const record &record) const {
        return this->fields[0] < record.fields[0];
    };
};

#endif //SRC_RECORD_H
