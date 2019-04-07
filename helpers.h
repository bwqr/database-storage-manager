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

void linkFiles(const string typeName, uint32 prevFileLink, uint32 nextFileLink);

void linkFiles(const string typeName, uint32 currFileLink, uint32 prevFileLink, uint32 nextFileLink);

file createAndLinkFiles(string typeName, uint32 currFile, uint32 prevFile, uint32 nextFile);

int calcTypeSize(int numFields);

string generateDirectoryFileName(string &typeName);

string generateBucketFileName(string &typeName, uint32 bucket_id);

uint32 generateBucketId(string &typeName);

bool checkFileExist(const std::string &name);

#endif //SRC_HELPERS_H
