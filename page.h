#ifndef SRC_PAGE_H
#define SRC_PAGE_H

#include <tiff.h>
#include <fstream>
#include <vector>
#include "record.h"

#define PAGE_ID 1
#define PAGE_NUM_RECORDS 1

class page {
public:
    uint8 pageID;
    uint8 numRecords;
    uint8 slotSize;
    int32 recordSize;
    uint8 *slots;

    page() = default;

    ~page();

    page(uint8 pageID, uint32 recordSize, uint8 slotSize);

    page(const page& p);

    page& operator=(const page& p);

    std::vector<record> getRecords(std::fstream &stream, int offset, uint8 numFields);

    void read(std::fstream &stream, int offset);

    void write(std::fstream &stream, int offset);
};


#endif //SRC_PAGE_H
