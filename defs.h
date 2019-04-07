#ifndef SRC_DEFS_H
#define SRC_DEFS_H

#define FIELD_NAME 8

#define NUM_TYPES 4
#define ROOT std::string("data/")
#define INFIX "_"
#define TYPE_NAME 8
#define NUM_PAGES 1
#define PAGE_PER_FILE 256
#define PAGE 2048
#define SLOT 1
#define FILE_ID 4

#define INOUTBIN ios::in | ios::out | ios::binary
#define OUTBIN std::ios::out | std::ios::binary
#define INBIN ios::in | ios::binary

#define RECORD_ID 1
#define FIELD 4

#define PAGE_ID 1

#define MAX_FIELDS 64

#define NUM_FIELDS 1
#define TYPE_NAME 8
#define CARDINALITY 8
#define FIELD_NAME 8
#define FIELD 4
#define PRIMARY_KEY 1

#define INDEX_PER_BUCKET 256
#define DEPTHS 4
#define BUCKET_SIZE 4
#define BUCKET_ID 4
#endif //SRC_DEFS_H
