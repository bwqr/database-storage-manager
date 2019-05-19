#ifndef SRC_PAGE_H
#define SRC_PAGE_H

#include <fstream>
#include <vector>
#include "record.h"

#define PAGE_ID 1
#define PAGE_NUM_RECORDS 1

class page {
public:
    uint8_t pageID;
    uint8_t numRecords;
    uint8_t slotSize;
    int32_t recordSize;
    uint8_t *slots;

    page() = default;

    ~page();

    page(uint8_t pageID, uint32_t recordSize, uint8_t slotSize);

    page(const page& p);

    page& operator=(const page& p);

    std::vector<record> getRecords(std::fstream &stream, int offset, uint8_t numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);
};


#endif //SRC_PAGE_H
