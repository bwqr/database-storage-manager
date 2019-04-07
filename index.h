#ifndef SRC_INDEX_H
#define SRC_INDEX_H

#include <tiff.h>
#include <fstream>
#include "defs.h"

class index {
public:
    uint32 file_id;
    uint8 page_id;
    uint8 record_id;
    int32 value;
    index() = default;
    index(uint32 file_id, uint8 page_id, uint8 record_id, int32 value);

    void read(std::fstream &stream);
    void write(std::fstream &stream) const;

    bool operator<(const index &index) const {
        return this->value < index.value;
    };

    bool operator==(index &index) {
        return this->value == index.value;
    };
};

#endif //SRC_INDEX_H
