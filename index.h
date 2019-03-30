#ifndef SRC_INDEX_H
#define SRC_INDEX_H

#define FILE_ID 4
#define PAGE_ID 1
#define RECORD_ID 1

#include <tiff.h>

class index {
public:
    uint32 file_id;
    uint8 page_id;
    uint8 record_id;
    int32 value;
    index() = default;
    index(uint32 file_id, uint8 page_id, uint8 record_id, int32 value);

    bool operator<(const index &index) const {
        return this->value < index.value;
    };

    bool operator==(index &index) {
        return this->value == index.value;
    };
};

#endif //SRC_INDEX_H
