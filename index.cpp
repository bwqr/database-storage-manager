#include "index.h"

index::index(uint32 file_id, uint8 page_id, uint8 record_id, int32 value) {
    this->file_id = file_id;
    this->page_id = page_id;
    this->record_id = record_id;
    this->value = value;
}

void index::read(std::fstream &stream) {
    stream.read((char *) &this->file_id, FILE_ID );
    stream.read((char *) &this->page_id, PAGE_ID);
    stream.read((char *) &this->record_id, RECORD_ID);
    stream.read((char *) &this->value, FIELD);
}

void index::write(std::fstream &stream) const {
    stream.write((char *) &this->file_id, FILE_ID );
    stream.write((char *) &this->page_id, PAGE_ID);
    stream.write((char *) &this->record_id, RECORD_ID);
    stream.write((char *) &this->value, FIELD);
}
