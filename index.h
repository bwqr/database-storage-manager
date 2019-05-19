#ifndef SRC_INDEX_H
#define SRC_INDEX_H

#include <fstream>
#include "defs.h"

class index {
public:
    uint32_t file_id;
    uint8_t page_id;
    uint8_t record_id;
    int32_t value;
    index() = default;
    index(uint32_t file_id, uint8_t page_id, uint8_t record_id, int32_t value);

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
