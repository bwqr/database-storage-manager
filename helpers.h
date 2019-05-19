#ifndef SRC_HELPERS_H
#define SRC_HELPERS_H

#include <string>
#include <unistd.h>
#include "file.h"
#include "directory.h"

using namespace std;

void createAndLinkFiles();

class directory* createIndex(string typeName);

string truncateName(string typeName);

void linkFiles(const string typeName, uint32_t prevFileLink, uint32_t nextFileLink);

void linkFiles(const string typeName, uint32_t currFileLink, uint32_t prevFileLink, uint32_t nextFileLink);

file createAndLinkFiles(string typeName, uint32_t currFile, uint32_t prevFile, uint32_t nextFile);

int calcTypeSize(int numFields);

string generateDirectoryFileName(string &typeName);

string generateBucketFileName(string &typeName, uint32_t bucket_id);

uint32_t generateBucketId(string &typeName);

bool checkFileExist(const std::string &name);

#endif //SRC_HELPERS_H
