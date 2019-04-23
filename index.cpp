#include "index.h"

index::index(uint32 file_id, uint8 page_id, uint8 record_id, int32 value) {
    this->file_id = file_id;
    this->page_id = page_id;
    this->record_id = record_id;
    this->value = value;
}

void index::read(std::fstream &stream) {
    stream.read((char *) &file_id, FILE_ID );
    stream.read((char *) &page_id, PAGE_ID);
    stream.read((char *) &record_id, RECORD_ID);
    stream.read((char *) &value, FIELD);

}

void index::write(std::fstream &stream) const {
    stream.write((char *) &file_id, FILE_ID );
    stream.write((char *) &page_id, PAGE_ID);
    stream.write((char *) &record_id, RECORD_ID);
    stream.write((char *) &value, FIELD);

}
