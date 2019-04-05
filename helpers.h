#ifndef SRC_HELPERS_H
#define SRC_HELPERS_H

#include <string>
#include <unistd.h>
#include "file.h"
using namespace std;

void createAndLinkFiles();

void createIndex(string typeName);

string truncateName(string typeName);

void linkFiles(const string typeName, uint32 prevFileLink, uint32 nextFileLink);

void linkFiles(const string typeName, uint32 currFileLink, uint32 prevFileLink, uint32 nextFileLink);

file createAndLinkFiles(string typeName, uint32 currFile, uint32 prevFile, uint32 nextFile);

int calcTypeSize(int numFields);

bool checkFileExist(const std::string &name);

std::string generateIndexFileName(const std::string &typeName, uint32 postfix);

uint32 generateIndexFilePostfix(const std::string &typeName);
#endif //SRC_HELPERS_H
