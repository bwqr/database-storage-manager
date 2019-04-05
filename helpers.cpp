#include "helpers.h"
#include <fstream>
#include <random>
#include <sys/stat.h>
#include "defs.h"
#include "btree_node.h"

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
    return TYPE_NAME + NUM_FIELDS + CARDINALITY + PRIMARY_KEY + numFields * FIELD_NAME + BTREE_ID;
}

void createIndex(std::string typeName) {
    std::fstream rootFile(ROOT + truncateName(typeName) + INFIX + "index" + INFIX + "1", OUTBIN);
    std::fstream leftFile(ROOT + truncateName(typeName) + INFIX + "index" + INFIX + "2", OUTBIN);
    std::fstream rightFile(ROOT + truncateName(typeName) + INFIX + "index" + INFIX + "3", OUTBIN);

    btree_node root(1);
    btree_node left(2);
    btree_node right(3);

    root.pointers[0] = left.id;
    root.pointers[1] = right.id;
    root.indices.insert(index(0,0,0,0));
    root.leaf = false;
    root.n++;

    left.rightSibling = right.id;
    right.leftSibling = left.id;

    root.write(rootFile);
    left.write(leftFile);
    right.write(rightFile);

    rootFile.close();
    leftFile.close();
    rightFile.close();
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


bool checkFileExist(const std::string &name) {
    struct stat buffer;

    return (stat(name.c_str(), &buffer) == 0);
}

std::string generateIndexFileName(const std::string &typeName, uint32 postfix) {
    return ROOT + truncateName(typeName) + INFIX + "index" + INFIX + to_string(postfix);
}

uint32 generateIndexFilePostfix(const std::string &typeName) {

    bool file_exist;

    uint32 postfix;

    do {
        postfix = random();

        file_exist = checkFileExist(generateIndexFileName(typeName, postfix));

    } while(file_exist);

    return postfix;
}
