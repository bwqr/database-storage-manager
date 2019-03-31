#include "helpers.h"
#include <fstream>
#include "defs.h"

std::string truncateName(std::string name) {

    for (int i = 0; i < TYPE_NAME; ++i) {
        if (name[i] == 0x20) { return name.substr(0, i); }
    }

    return name;
}

void linkFiles(const string typeName, uint32 prevFileLink, uint32 nextFileLink) {
    if (prevFileLink != 0) {
        fstream prevFile(ROOT + truncateName(typeName) + INFIX + to_string(prevFileLink), INOUTBIN);

        prevFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE);
        prevFile.write((char *) &nextFileLink, LINK_TO_FILE);

        prevFile.close();
    }

    if (nextFileLink != 0) {
        fstream nextFile(ROOT + truncateName(typeName) + INFIX + to_string(nextFileLink), INOUTBIN);

        nextFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS);
        nextFile.write((char *) &prevFileLink, LINK_TO_FILE);

        nextFile.close();
    }
}

int calcTypeSize(int numFields) {
    return TYPE_NAME + NUM_FIELDS + CARDINALITY + PRIMARY_KEY + numFields * FIELD_NAME;
}

void createIndex(std::string typeName) {
    std::ofstream indexFile(ROOT + truncateName(typeName) + INFIX + "index", OUTBIN);

    indexFile.close();
}

void linkFiles(const string typeName, uint32 currFileLink, uint32 prevFileLink, uint32 nextFileLink) {
    if (prevFileLink != 0) {
        fstream prevFile(ROOT + truncateName(typeName) + INFIX + to_string(prevFileLink), INOUTBIN);

        prevFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE);
        prevFile.write((char *) &currFileLink, LINK_TO_FILE);

        prevFile.close();
    }

    if (nextFileLink != 0) {
        fstream nextFile(ROOT + truncateName(typeName) + INFIX + to_string(nextFileLink), INOUTBIN);

        nextFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS);
        nextFile.write((char *) &currFileLink, LINK_TO_FILE);

        nextFile.close();
    }
}

file createAndLinkFiles(string typeName, uint32 currFile, uint32 prevFile, uint32 nextFile) {
    fstream typeFile(ROOT + truncateName(typeName) + INFIX + to_string(currFile), OUTBIN);

    file file(typeName, prevFile, nextFile);

    file.write(typeFile);

    for (int i = 0; i < PAGE_PER_FILE; ++i) {
        int fill[PAGE - PAGE_ID] = {0};
        typeFile.write((char *) &i, PAGE_ID);
        typeFile.write((char *) &fill, PAGE - PAGE_ID);
    }

    typeFile.close();

    linkFiles(typeName, currFile, prevFile, nextFile);

    return file;
}
