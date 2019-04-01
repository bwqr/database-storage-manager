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
        cout << "type already exists, type: " << typeName << endl;

        return;
    }

    type type(typeName, (int8) numFields);

    for (int i = 0; i < numFields; ++i) {
        type.fieldsName[i] = fields[i];
    }

    int pos = NUM_TYPES;

    //Get the end of file;
    for(auto & _i : systemCatalog->types) {
        pos += calcTypeSize(_i.numFields);

    }

    systemCatalog->numTypes++;

    systemCatalog->types.insert(type);

    systemCatalog->writeNumTypes();

    type.write(systemCatalog->catalogFile, pos);

    createAndLinkFiles(typeName, 1, 0, 0);

    createIndex(typeName);
}

void ddl::deleteType(std::string typeName) {
    set<type>::iterator i;

    int pos = NUM_TYPES;

    for ( i = systemCatalog->types.begin(); i != systemCatalog->types.end(); ++i) {
        if(i->typeName.compare(typeName) == 0) { break; }
        pos += calcTypeSize(i->numFields);
    }

    if(i == systemCatalog->types.end()) {
        cout << "type could not be found, typeName: " << typeName << endl;

        return;
    }

    int end = pos, currEnd = pos + calcTypeSize(i->numFields);

    for (auto j = i; j != systemCatalog->types.end(); ++j) {
        end += calcTypeSize(j->numFields);
    }

    string path = string(ROOT + "SystemCatalog");

    if(truncate((char *) path.c_str(), end - (currEnd - pos)) == -1) {
        cout << "file cannot be shrank" << endl;

        return;
    }

    string truncatedName = truncateName(typeName);

    remove((ROOT + truncatedName + INFIX + "index").c_str());

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
