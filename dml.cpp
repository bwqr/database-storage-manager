#include <iostream>
#include <algorithm>
#include "dml.h"
#include "helpers.h"
#include "page.h"

dml::dml(SystemCatalog *systemCatalog) {
    this->systemCatalog = systemCatalog;
}

void dml::createRecord(string typeName, int32 *fields) {
    auto type = systemCatalog->getType(typeName);

    if(type == systemCatalog->types.end()) { return; }

//    if(systemCatalog->checkExist(*type, fields[0])) {
//        cout << "Duplicate primary key: " << fields[0] << endl;
//
//        return;
//    }

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

    systemCatalog->insertIndex(*type, currFileLink, page.pageID, record.recordID, record.fields[0]);

    type->cardinality++;
}

void dml::deleteRecord(string typeName, int32 primaryKey) {
    auto type = systemCatalog->getType(typeName);

    if(type == systemCatalog->types.end()) { return; }

    auto index = systemCatalog->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return;
    }

    systemCatalog->readIndex(*type);

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

void dml::listRecords(string typeName, ostream &stream) {
    auto type = systemCatalog->getType(typeName);

    if(type == systemCatalog->types.end()) { return; }

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

    sort(records.begin(), records.end());

    for (const auto &record: records) {

        for (int i = 0; i < record.numFields; ++i) {
            stream << record.fields[i] << " ";
        }

        stream << endl;
    }
}

record* dml::searchRecord(string typeName, int32 primaryKey) {
    auto type = systemCatalog->getType(typeName);

    if(type == systemCatalog->types.end()) { return nullptr; }

    auto index = systemCatalog->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return nullptr;
    }

    systemCatalog->readIndex(*type);

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

void dml::updateRecord(string typeName, int32 primaryKey, int32 *fields) {
    auto type = systemCatalog->getType(typeName);

    if(type == systemCatalog->types.end()) { return; }

    auto index = systemCatalog->searchKey(*type, primaryKey);

    if(index == type->indexes.end()) {
        cout << "Record could not be found, type: " << truncateName(typeName) << ", key: " << primaryKey << endl;

        return;
    }

    systemCatalog->readIndex(*type);

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
