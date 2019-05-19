#include "page.h"
#include <iostream>

page::~page() {
    delete[] this->slots;
}

page::page(uint8_t pageID, uint32_t recordSize, uint8_t slotSize) {
    this->pageID = pageID;
    this->recordSize = recordSize;
    this->numRecords = 0;
    this->slotSize = slotSize;
    this->slots = new uint8_t[slotSize];
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

std::vector<record> page::getRecords(std::fstream &stream, int offset, uint8_t numFields) {

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

page::page(const page &p) {
    pageID = p.pageID;
    numRecords = p.numRecords;
    slotSize = p.slotSize;
    recordSize = p.recordSize;

    slots = new uint8_t[slotSize];

    for (int i = 0; i < slotSize; ++i) {
        slots[i] = p.slots[i];
    }
}

page &page::operator=(const page &p) {
    if(this != &p) {
        this->~page();

        pageID = p.pageID;
        numRecords = p.numRecords;
        slotSize = p.slotSize;
        recordSize = p.recordSize;

        slots = new uint8_t[slotSize];

        for (int i = 0; i < slotSize; ++i) {
            slots[i] = p.slots[i];
        }
    }

    return *this;
}

