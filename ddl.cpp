#include <iostream>
#include <unistd.h>
#include "ddl.h"
#include "defs.h"
#include "helpers.h"

ddl::ddl(SystemCatalog *systemCatalog) {
    this->systemCatalog = systemCatalog;
}

void ddl::createType(std::string typeName, int numFields, std::string *fields) {
    auto i = systemCatalog->getType(typeName);

    if(i != systemCatalog->types.end()) {
        return;
    }

    type type(typeName, (int8_t ) numFields);

    for (int j = 0; j < numFields; ++j) {
        type.fieldsName[j] = fields[j];
    }

    int pos = NUM_TYPES;

    //Get the end of file;
    for(auto & _i : systemCatalog->types) {
        pos += calcTypeSize(_i.numFields);

    }

    systemCatalog->numTypes++;

    systemCatalog->writeNumTypes();

    type.dir = createIndex(typeName);

    type.write(systemCatalog->catalogFile, pos);

    systemCatalog->types.insert(type);

    createAndLinkFiles(typeName, 1, 0, 0);
}

void ddl::deleteType(std::string typeName) {
    set<type>::iterator i;

    int pos = NUM_TYPES;

    for ( i = systemCatalog->types.begin(); i != systemCatalog->types.end(); ++i) {
        if(i->typeName == typeName) { break; }
        pos += calcTypeSize(i->numFields);
    }

    if(i == systemCatalog->types.end()) {
        return;
    }

    int end = pos, currEnd = pos + calcTypeSize(i->numFields);

    for (auto j = i; j != systemCatalog->types.end(); ++j) {
        end += calcTypeSize(j->numFields);
    }

    string path = string(ROOT + "SystemCatalog");

    truncate((char *) path.c_str(), end - (currEnd - pos));

    string truncatedName = truncateName(typeName);

    for (auto &bucket_id: i->dir->bucket_ids) {
        remove(generateBucketFileName(typeName, bucket_id).c_str());
    }

    remove(generateDirectoryFileName(typeName).c_str());

    //delete the files
    int currFileLink = 1;

    do {
        ifstream currFile(ROOT + truncatedName + INFIX + to_string(currFileLink));

        int nextFileLink = 0;

        currFile.seekg(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS);

        currFile.read((char *) &nextFileLink, LINK_TO_FILE);

        remove((ROOT + truncatedName + INFIX + to_string(currFileLink)).c_str());

        currFileLink = nextFileLink;
    }
    while (currFileLink != 0);

    systemCatalog->numTypes--;

    systemCatalog->types.erase(i);
}

void ddl::listTypes(ostream &stream) {
    systemCatalog->listTypes(stream);
}
