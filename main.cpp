#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "system_catalog.h"
#include "ddl.h"
#include "dml.h"
#include "defs.h"

int main(int argc, char **argv) {
    if(argc < 3) {
        std::cout << "ERROR, expected 2 arguments"<< std::endl;
        return 1;
    }

    auto *sysCatalog = new SystemCatalog;

    auto *ddl = new class ddl(sysCatalog);

    auto *dml = new class dml(sysCatalog);

    if(argv[1][1] =='f'){

        ifstream inputFile(argv[2]);

        ofstream outputFile(argv[3]);

        string line, command, command_type, typeName;

        while(getline(inputFile, line)) {

            istringstream iss(line);

            iss >> command >> command_type >> typeName;

            typeName.resize(TYPE_NAME, (char) 0x20);

            if(command == "create" && command_type == "type") {

                int numFields;

                iss >> numFields;

                auto fields = new string[numFields];

                for (int i = 0; i < numFields; ++i) {
                    iss >> fields[i];
                    fields[i].resize(FIELD_NAME, (char) 0x20);
                }

                ddl->createType(typeName, numFields, fields);

                delete[] fields;

            } else if(command == "delete" && command_type == "type"){

                ddl->deleteType(typeName);

            } else if(command == "list" && command_type == "type") {

                ddl->listTypes(outputFile);

            } else if(command == "create" && command_type == "record") {

                auto type = sysCatalog->getType(typeName);

                if(type != sysCatalog->types.end()) {
                    int *fields = new int32[type->numFields];

                    for(int i = 0; i < type->numFields; i++) {
                        iss >> fields[i];
                    }

                    dml->createRecord(typeName, fields);

                    delete[] fields;
                }

            } else if(command == "delete" && command_type == "record") {
                int32 primaryKey;

                iss >> primaryKey;

                dml->deleteRecord(typeName, primaryKey);

            } else if(command == "update" && command_type == "record") {
                int32 primaryKey;

                iss >> primaryKey;

                auto type = sysCatalog->getType(typeName);

                if(type != sysCatalog->types.end()) {
                    auto *fields = new int32[type->numFields - 1];

                    for (int i = 0; i < type->numFields - 1; ++i) {
                        iss >> fields[i];
                    }

                    dml->updateRecord(typeName, primaryKey, fields);

                    delete[] fields;
                }

            } else if(command == "search" && command_type == "record") {

                int32 primaryKey;

                iss >> primaryKey;

                auto record = dml->searchRecord(typeName, primaryKey);

                if(record != nullptr) {

                    for (int i = 0; i < record->numFields; ++i) {
                        outputFile << record->fields[i] << " ";
                    }

                    delete record;

                    outputFile << endl;

                }

            } else if(command == "list" && command_type == "record") {

                dml->listRecords(typeName, outputFile);

            }
        }

    } else if(argv[1][1] == 'c') {
        string *fields = new string[argc - 3];

        for (int i = 3; i < argc; ++i) {
            fields[i - 3] = string(argv[i]);
            fields[i - 3].resize(FIELD_NAME, (char) 0x20);
        }
        string createType = string(argv[2]);

        createType.resize(TYPE_NAME, (char) 0x20);

        ddl->createType(createType, argc - 3, fields);

    } else if(argv[1][1] == 'd') {
        string deleteFile = string(argv[2]);

        deleteFile.resize(TYPE_NAME, (char) 0x20);

        ddl->deleteType(string(deleteFile));
    } else if(argv[1][1] == 'l' && argv[1][2] == 't') {

        string listType = string(argv[2]);

        listType.resize(TYPE_NAME, (char) 0x20);

        dml->listRecords(listType, cout);

    } else if(argv[1][1] == 'l' && argv[1][2] == 'i') {
        string listType = string(argv[2]);

        listType.resize(TYPE_NAME, (char) 0x20);

        auto indexes = sysCatalog->listIndex(listType);

        for (auto i = indexes.begin(); i != indexes.end(); ++i) {
            std::cout << i->file_id << " " << (int) i->page_id << " "
                << (int) i->record_id << " " << i->value << std::endl;
        }

    } else if(argv[1][1] == 'l') {
        sysCatalog->listTypes(cout);
    } else if(argv[1][1] == 'r' && argv[1][2] == 'c') {
        int *fields = new int[argc - 3];

        for(int i = 3; i < argc; i++) {
            fields[i - 3] = std::stoi(argv[i]);
        }

        string typeName = string(argv[2]);

        typeName.resize(TYPE_NAME, (char) 0x20);

        dml->createRecord(typeName, fields);
    } else if(argv[1][1] == 's') {
        string searchType = string(argv[2]);

        searchType.resize(8, (char) 0x20);

        int32 searchValue = stoi(string(argv[3]));

        auto record = dml->searchRecord(searchType, searchValue);

        std::cout << (int) record->recordID << std::endl;

        for (int i = 0; i < record->numFields; ++i) {
            std::cout << "\t" << record->fields[i] << "\t";
        }

        std::cout << endl;

        delete record;

    } else if(argv[1][1] == 'r' && argv[1][2] == 'd') {
        string deleteType = string(argv[2]);

        deleteType.resize(8, (char) 0x20);

        int32 searchValue = stoi(string(argv[3]));

        dml->deleteRecord(deleteType, searchValue);

    } else if(argv[1][1] == 'u') {
        string updateType = string(argv[2]);

        updateType.resize(8, (char) 0x20);

        int32 updateKey = stoi(string(argv[3]));

        int32 *fields = new int32[argc - 4];

        for (int i = 4; i < argc; ++i) {
            fields[i - 4] = stoi(string(argv[i]));
        }

        dml->updateRecord(updateType, updateKey, fields);
    }

    delete sysCatalog;

    delete dml;

    delete ddl;

    return 0;
}