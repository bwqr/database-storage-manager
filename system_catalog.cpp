#include "system_catalog.h"
#include <fstream>
#include <iostream>
#include <set>
#include "defs.h"
#include "type.h"
#include "index.h"
#include "helpers.h"

using namespace std;

SystemCatalog::SystemCatalog() {
    catalogFile = fstream(ROOT + "SystemCatalog", INOUTBIN);

    if(!catalogFile){
        cout << "SystemCatalog file could not be found or read, creating a new one" << endl;

        this->createCatalogFile();

        catalogFile = fstream(ROOT + "SystemCatalog", INOUTBIN);
    }

    catalogFile.read((char *)&numTypes, NUM_TYPES);

    types = set<type>();

    int offset = NUM_TYPES;

    for (int i = 0; i < numTypes; ++i) {

        type _type = type();

        _type.read(catalogFile, offset);

        types.insert(_type);

        offset += calcTypeSize(_type.numFields);
    }
}

void SystemCatalog::createCatalogFile() {
    ofstream _catalogFile(ROOT + "SystemCatalog", OUTBIN);

    int num_types = 0;

    _catalogFile.write((char *)&num_types, NUM_TYPES);

    _catalogFile.close();
}

void SystemCatalog::writeNumTypes() {
    catalogFile.seekp(0);

    catalogFile.write((char *)&numTypes, NUM_TYPES);
}

SystemCatalog::~SystemCatalog() {
    write();

    catalogFile.close();
}

void SystemCatalog::listTypes(ostream &stream) {

    for (const auto &type : this->types) {
        stream << type.typeName << endl;
    }
}

set<type>::iterator SystemCatalog::getType(string &typeName) {
    set<type>::iterator i;

    for ( i = types.begin(); i != types.end(); ++i) {
        if(i->typeName.compare(typeName) == 0) { break; }
    }

    return i;
}

set<index>::iterator SystemCatalog::searchKey(const type &type, int32 primaryKey) {
    readIndex(type);

    index index(0,0,0,primaryKey);

    auto i = type.indexes.find(index);

    return i;
}

void SystemCatalog::readIndex(const type &type) {

    if(type.is_index_read) {
        return;
    }

    type.is_index_read = true;

    fstream indexFile(ROOT + truncateName(type.typeName) + INFIX + "index", INOUTBIN);

    type.indexes = set<index>();

    for (int i = 0; i < type.cardinality; ++i) {
        index index;
        indexFile.read((char *) &index.file_id, FILE_ID );
        indexFile.read((char *) &index.page_id, PAGE_ID);
        indexFile.read((char *) &index.record_id, RECORD_ID);
        indexFile.read((char *) &index.value, FIELD);

        type.indexes.insert(index);
    }

    indexFile.close();
}

void SystemCatalog::insertIndex(const type &type, uint32 file_id, uint8 page_id, uint8 record_id, int32 value) {
    readIndex(type);

    type.indexes.insert(index(file_id, page_id, record_id, value));
}

void SystemCatalog::write() {

    writeNumTypes();

    catalogFile.seekp(NUM_TYPES);

    for (auto k: types) {
        k.write(catalogFile, (int) catalogFile.tellp());
        if(k.is_index_read) {
            fstream indexFile(ROOT + truncateName(k.typeName) + INFIX + "index", OUTBIN);
            k.writeIndex(indexFile);
            indexFile.close();
        }
    }
}

set<index> SystemCatalog::listIndex(string &typeName) {

    auto type = this->getType(typeName);

    if(type == types.end()) { return set<index>(); }

    readIndex(*type);

    return type->indexes;
}

bool SystemCatalog::checkExist(const type &type, int32 primaryKey) {

    auto index = this->searchKey(type, primaryKey);

    return index != type.indexes.end();
}
