#include "index.h"

index::index(uint32 file_id, uint8 page_id, uint8 record_id, int32 value) {
    this->file_id = file_id;
    this->page_id = page_id;
    this->record_id = record_id;
    this->value = value;
}
