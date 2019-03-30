#include "system_catalog.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <set>

#define NUM_TYPES 4
#define ROOT string("data/")
#define INFIX "_"
#define TYPE_NAME 8
#define NUM_PAGES 1
#define PAGE_PER_FILE 256
#define PAGE 2048
#define SLOT 1

#define INOUTBIN ios::in | ios::out | ios::binary
#define OUTBIN ios::out | ios::binary
#define INBIN ios::in | ios::binary

#define RECORD_ID 1
#define FIELD 4

#include "type.h"
#include "file.h"
#include "index.h"
#include "page.h"
#include "record.h"

using namespace std;

string truncateName(const string name) {

    for (int i = 0; i < TYPE_NAME; ++i) {
        if(name[i] == 0x20) { return name.substr(0, i);}
    }

    return name;
}

void linkFiles(const string typeName, uint32 prevFileLink, uint32 nextFileLink) {
    if(prevFileLink != 0) {
        fstream prevFile(ROOT + truncateName(typeName) + INFIX + to_string(prevFileLink), INOUTBIN);

        prevFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE);
        prevFile.write((char *) &nextFileLink, LINK_TO_FILE);

        prevFile.close();
    }

    if(nextFileLink != 0) {
        fstream nextFile(ROOT + truncateName(typeName) + INFIX + to_string(nextFileLink), INOUTBIN);

        nextFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS);
        nextFile.write((char *) &prevFileLink, LINK_TO_FILE);

        nextFile.close();
    }
}

void linkFiles(const string typeName, uint32 currFileLink, uint32 prevFileLink, uint32 nextFileLink) {
    if(prevFileLink != 0) {
        fstream prevFile(ROOT + truncateName(typeName) + INFIX + to_string(prevFileLink), INOUTBIN);

        prevFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE);
        prevFile.write((char *) &currFileLink, LINK_TO_FILE);

        prevFile.close();
    }

    if(nextFileLink != 0) {
        fstream nextFile(ROOT + truncateName(typeName) + INFIX + to_string(nextFileLink), INOUTBIN);

        nextFile.seekp(IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS);
        nextFile.write((char *) &currFileLink, LINK_TO_FILE);

        nextFile.close();
    }
}

void createIndex(string typeName) {
    ofstream indexFile(ROOT + truncateName(typeName) + INFIX + "index", OUTBIN);

    indexFile.close();
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

int calcSize(int numFields) {
    return TYPE_NAME + NUM_FIELDS + CARDINALITY + PRIMARY_KEY + numFields * FIELD_NAME;
}

SystemCatalog::SystemCatalog() {
    catalogFile = fstream(ROOT + "SystemCatalog", INOUTBIN);

    if(!catalogFile){
        cout << "SystemCatalog file could not be found or read, creating a new one" << endl;

        this->createCatalogFile();

        catalogFile = fstream(ROOT + "SystemCatalog", INOUTBIN);
    }

    catalogFile.read((char *)&numTypes, NUM_TYPES);

    types = vector<type>(numTypes);

    int offset = NUM_TYPES;

    for (int i = 0; i < numTypes; ++i) {

        types[i].read(catalogFile, offset);

        offset += calcSize(types[i].numFields);
    }
}

void SystemCatalog::createCatalogFile() {
    ofstream catalogFile(ROOT + "SystemCatalog", OUTBIN);

    int num_types = 0;

    catalogFile.write((char *)&num_types, NUM_TYPES);

    catalogFile.close();
}

void SystemCatalog::createType(string typeName, int numFields, string *fields) {
    auto i = this->getType(typeName);

    if(i != this->types.end()) {
        cout << "type already exists, type: " << typeName << endl;
    }

    type type(typeName, (int8) numFields);

    for (int i = 0; i < numFields; ++i) {
        type.fieldsName[i] = fields[i];
    }

    int pos = NUM_TYPES;

    //Get the end of file;
    for (int i = 0; i < numTypes; ++i) {
        pos += calcSize(types[i].numFields);
    }

    numTypes++;

    types.push_back(type);

    writeNumTypes();

    type.write(catalogFile, pos);

    createAndLinkFiles(typeName, 1, 0, 0);

    createIndex(typeName);
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

void SystemCatalog::deleteType(string typeName) {

    vector<type>::iterator i;

    int pos = NUM_TYPES;

    for ( i = types.begin(); i != types.end(); ++i) {
        if(i->typeName.compare(typeName) == 0) { break; }
        pos += calcSize(i->numFields);
    }

    if(i == types.end()) {
        cout << "type could not be found, typeName: " << typeName << endl;

        return;
    }

    int end = pos, currEnd = pos + calcSize(i->numFields);

    for (auto j = i; j != types.end(); ++j) {
        end += calcSize(j->numFields);
    }

    string path = string(ROOT + "SystemCatalog");

    if(truncate((char *) path.c_str(), end - (currEnd - pos)) == -1) {
        cout << "file cannot be shrinked" << endl;

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

    numTypes--;

    types.erase(i);
}

void SystemCatalog::createRecord(string typeName, int32 *fields) {

    auto type = this->getType(typeName);

    if(type == types.end()) { return; }

    if(this->checkExist(*type, fields[0])) {
        cout << "Duplicate primary key: " << fields[0] << endl;

        return;
    }

    uint32 currFileLink = 1;

    fstream currFile(ROOT + truncateName(typeName) + INFIX + to_string(currFileLink), INOUTBIN);

    file file;
    file.read(currFile);

    while(file.isFull) {
        currFile.close();

        if(file.nextFile == 0) {
            currFileLink++;
            file = createAndLinkFiles(typeName, currFileLink, currFileLink - 1, 0);
            currFile = fstream(ROOT + truncateName(typeName) + INFIX + to_string(currFileLink + 1), INOUTBIN);
        } else {
            currFileLink = file.nextFile;
            currFile = fstream(ROOT + truncateName(typeName) + INFIX + to_string(currFileLink), INOUTBIN);
            file.read(currFile);
        }
    }

    int offset = IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE + LINK_TO_FILE - (PAGE);

    auto recordSize = (uint8) (RECORD_ID + type->numFields * FIELD);

    auto slotSize = (uint8) min((PAGE - PAGE_ID - NUM_RECORDS) / (recordSize+ 1), 255);

    page page(0, recordSize, slotSize);

    do {
        offset += PAGE;
        page.read(currFile, offset);
    } while (page.numRecords >= page.slotSize);

    int j;

    for ( j = 0; j < page.slotSize; ++j) {
        if (page.slots[j] == 0){ break; }
    }

    page.numRecords++;

    page.slots[j] = page.numRecords;

    record record(j, type->numFields);

    for (int k = 0; k < type->numFields; ++k) {
        record.fields[k] = fields[k];
    }

    page.write(currFile, offset);

    int record_offset = offset + PAGE_ID + PAGE_NUM_RECORDS + page.slotSize + (page.numRecords - 1) * (RECORD_ID + type->numFields * FIELD);

    record.write(currFile, record_offset);

    file.numRecords++;

    if(file.numRecords >= slotSize * PAGE_PER_FILE) {
        file.isFull = true;
    }

    file.write(currFile);

    currFile.close();

    insertIndex(*type, currFileLink, page.pageID, record.recordID, record.fields[0]);

    type->cardinality++;
}

void SystemCatalog::listRecords(string typeName, ostream &stream) {

    auto type = this->getType(typeName);

    if(type == types.end()) { return; }

    auto records = vector<record>(0);

    int currFileLink = 1;

    auto recordSize = (uint8) (RECORD_ID + type->numFields * FIELD);

    auto slotSize = (uint8) min((PAGE - PAGE_ID - NUM_RECORDS) / (recordSize+ 1), 255);

    do {
        fstream currFile(ROOT + truncateName(typeName) + INFIX + to_string(currFileLink), INBIN);

        file file;
        file.read(currFile);

        int offset = IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE + LINK_TO_FILE;

        for (int i = file.numRecords; i > 0; --i) {
            page page(0, recordSize, slotSize);

            page.read(currFile, offset);

            vector<record> pageRecords = page.getRecords(currFile, offset, type->numFields);

            records.insert(records.end(), pageRecords.begin(), pageRecords.end());

            offset += PAGE;
        }

        currFileLink = file.nextFile;
    } while(currFileLink != 0);

    for (const auto &record: records) {

        for (int i = 0; i < record.numFields; ++i) {
            stream << record.fields[i] << " ";
        }

        stream << endl;
    }
}

vector<type>::iterator SystemCatalog::getType(string &typeName) {
    vector<type>::iterator i;

    for ( i = types.begin(); i != types.end(); ++i) {
        if(i->typeName.compare(typeName) == 0) { break; }
    }

    return i;
}

void SystemCatalog::deleteRecord(string typeName, int32 primaryKey) {

    auto type = this->getType(typeName);

    if(type == this->types.end()) { return; }

    auto index = this->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return;
    }

    readIndex(*type);

    fstream searchFile(ROOT + truncateName(typeName) + INFIX + to_string(index->file_id), INOUTBIN);

    //find the slot and set ZERO

    file file;

    file.read(searchFile);

    file.numRecords--;

    type->cardinality--;

    type->indexes.erase(index);

    if (file.numRecords == 0 && file.prevFile != 0) {
        remove((ROOT + truncateName(typeName) + INFIX + to_string(index->file_id)).c_str());

        linkFiles(typeName, file.prevFile, file.nextFile);

        return;
    }

    file.isFull = false;

    auto recordSize = (uint8) (RECORD_ID + type->numFields * FIELD);

    auto slotSize = (uint8) min((PAGE - PAGE_ID - NUM_RECORDS) / (recordSize+ 1), 255);

    page page(0, recordSize, slotSize);

    int offset = (IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE + LINK_TO_FILE) + (index->page_id * PAGE);

    page.read(searchFile, offset);

    page.numRecords--;

    int i;

    if (page.numRecords != 0) {
        record record(0, type->numFields);

        record.read(searchFile, offset + (PAGE_ID + PAGE_NUM_RECORDS) + page.slotSize + page.numRecords * recordSize);

        page.slots[record.recordID] = page.slots[index->record_id];

        record.write(searchFile, offset + (PAGE_ID + PAGE_NUM_RECORDS) + page.slotSize + (page.slots[index->record_id] -1 ) * recordSize);
    }

    page.slots[index->record_id] = 0;

    file.write(searchFile);

    page.write(searchFile, offset);

    searchFile.close();
}

set<index>::iterator SystemCatalog::searchKey(type &type, int32 primaryKey) {
    readIndex(type);

    index index(0,0,0,primaryKey);

    auto i = type.indexes.find(index);

    return i;
}

void SystemCatalog::readIndex(type &type) {

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

void SystemCatalog::insertIndex(type &type, uint32 file_id, uint8 page_id, uint8 record_id, int32 value) {
    readIndex(type);

    index index(file_id, page_id, record_id, value);

    type.indexes.insert(index);
}

void SystemCatalog::write() {

    writeNumTypes();

    catalogFile.seekp(NUM_TYPES);

    for (auto k: types) {
        k.write(catalogFile, (int) catalogFile.tellp());
        if(k.is_index_read) {
            fstream indexFile(ROOT + truncateName(k.typeName) + INFIX + "index", INOUTBIN);
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

record* SystemCatalog::searchByPrimaryKey(string typeName, int32 primaryKey) {

    auto type = this->getType(typeName);

    if(type == this->types.end()) { return nullptr; }

    auto index = this->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return nullptr;
    }

    readIndex(*type);

    fstream searchFile(ROOT + truncateName(typeName) + INFIX + to_string(index->file_id), INOUTBIN);

    auto recordSize = (uint8) (RECORD_ID + type->numFields * FIELD);

    auto slotSize = (uint8) min((PAGE - PAGE_ID - NUM_RECORDS) / (recordSize+ 1), 255);

    page page(0, recordSize, slotSize);

    int offset = (IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE + LINK_TO_FILE) + (index->page_id * PAGE);

    page.read(searchFile, offset);

    auto record = new class record(0, type->numFields);

    record->read(searchFile, offset + (PAGE_ID + PAGE_NUM_RECORDS) + page.slotSize + page.recordSize * (page.slots[index->record_id] - 1));

    return record;
}

void SystemCatalog::updateRecord(string typeName, int32 primaryKey, int32 *fields) {

    auto type = this->getType(typeName);

    if(type == this->types.end()) { return; }

    auto index = this->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return;
    }

    readIndex(*type);

    fstream searchFile(ROOT + truncateName(typeName) + INFIX + to_string(index->file_id), INOUTBIN);

    auto recordSize = (uint8) (RECORD_ID + type->numFields * FIELD);

    auto slotSize = (uint8) min((PAGE - PAGE_ID - NUM_RECORDS) / (recordSize+ 1), 255);

    page page(0, recordSize, slotSize);

    int offset = (IS_FULL + TYPE_NAME + NUM_PAGES + NUM_RECORDS + LINK_TO_FILE + LINK_TO_FILE) + (index->page_id * PAGE);

    page.read(searchFile, offset);

    record record(0, type->numFields);

    record.read(searchFile, offset + (PAGE_ID + PAGE_NUM_RECORDS) + page.slotSize + page.recordSize * (page.slots[index->record_id] - 1));

    for (int i = 1; i < record.numFields; ++i) {
        record.fields[i] = fields[i - 1];
    }

    record.write(searchFile, offset + (PAGE_ID + PAGE_NUM_RECORDS) + page.slotSize + page.recordSize * (page.slots[index->record_id] - 1));
}

bool SystemCatalog::checkExist(type &type, int32 primaryKey) {

    auto index = this->searchKey(type, primaryKey);

    return index != type.indexes.end();
}
