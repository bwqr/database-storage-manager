#include "file.h"
#include <string>

file::file() {
    typeName = std::string(8, ' ');
}

file::file(std::string &typeName, int prevFile, int nextFile) {
    this->isFull = false;
    this->numRecords = 0;
    this->numPages = 0;
    this->typeName = typeName;
    this->prevFile = prevFile;
    this->nextFile = nextFile;
}

void file::read(std::fstream &stream) {
    stream.seekg(0);

    stream.read((char *) &this->isFull, IS_FULL);
    stream.read((char *) this->typeName.c_str(), TYPE_NAME);
    stream.read((char *) &this->numPages, NUM_PAGES);
    stream.read((char *) &this->numRecords, NUM_RECORDS);
    stream.read((char *) &this->prevFile, LINK_TO_FILE);
    stream.read((char *) &this->nextFile, LINK_TO_FILE);
}

void file::write(std::fstream &stream) {
    stream.seekp(0);

    stream.write((char *) &this->isFull, IS_FULL);
    stream.write((char *) this->typeName.c_str(), TYPE_NAME);
    stream.write((char *) &this->numPages, NUM_PAGES);
    stream.write((char *) &this->numRecords, NUM_RECORDS);
    stream.write((char *) &this->prevFile, LINK_TO_FILE);
    stream.write((char *) &this->nextFile, LINK_TO_FILE);
}
