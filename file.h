#ifndef SRC_FILE_H
#define SRC_FILE_H

#define IS_FULL 1
#define NUM_PAGES 1
#define NUM_RECORDS 4
#define LINK_TO_FILE 4
#define TYPE_NAME 8

#include <string>
#include <fstream>
class file {
public:
    bool isFull;
    std::string typeName;
    uint8_t numPages;
    uint32_t numRecords;
    uint32_t nextFile;
    uint32_t prevFile;

    file();

    file(std::string &typeName, int prevFile, int nextFile);

    void read(std::fstream &stream);

    void write(std::fstream &stream);
};


#endif //SRC_FILE_H
