#include "page.h"
#include <iostream>
page::page() {
}

page::page(uint8 pageID, uint32 recordSize, uint8 slotSize) {
    this->pageID = pageID;
    this->recordSize = recordSize;
    this->numRecords = 0;
    this->slotSize = slotSize;
    this->slots = new uint8[slotSize];
}

void page::read(std::fstream &stream, int offset) {
    stream.seekg(offset);

    stream.read((char *) &this->pageID, PAGE_ID);
    stream.read((char *) &this->numRecords, PAGE_NUM_RECORDS);
    stream.read((char *) this->slots, this->slotSize);
}

void page::write(std::fstream &stream, int offset) {
    stream.seekp(offset);

    stream.write((char *) &this->pageID, PAGE_ID);
    stream.write((char *) &this->numRecords, PAGE_NUM_RECORDS);
    stream.write((char *) this->slots, this->slotSize);
}

std::vector<record> page::getRecords(std::fstream &stream, int offset, uint8 numFields) {

    auto records = std::vector<record>(this->numRecords);
    int k = 0;
    for (int i = 0; i < this->slotSize && k < this->numRecords; ++i) {
        if(this->slots[i] == 0) { continue; }
        record record(0, numFields);
        record.read(stream, offset + PAGE_ID + PAGE_NUM_RECORDS + this->slotSize + (this->slots[i] - 1) * this->recordSize);
        records[k] = record;
        k++;
    }

    return records;
}

