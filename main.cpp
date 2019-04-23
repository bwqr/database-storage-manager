#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "system_catalog.h"
#include "ddl.h"
#include "dml.h"
#include "defs.h"
#include "btree.h"

int main(int argc, char **argv) {

    if (argc < 3) {
        std::cout << "ERROR, expected 2 arguments" << std::endl;
        return 1;
    }

    auto *sysCatalog = new SystemCatalog;

    auto *ddl = new class ddl(sysCatalog);

    auto *dml = new class dml(sysCatalog);

    ifstream inputFile(argv[1]);

    ofstream outputFile(argv[2]);

    string line, command, command_type, typeName;

    while (getline(inputFile, line)) {

        istringstream iss(line);

        iss >> command >> command_type >> typeName;

        typeName.resize(TYPE_NAME, (char) 0x20);

        if (command == "create" && command_type == "type") {

            int numFields;

            iss >> numFields;

            if(numFields >= MAX_FIELDS) {
                continue;
            }

            auto fields = new string[numFields];

            for (int i = 0; i < numFields; ++i) {
                iss >> fields[i];
                fields[i].resize(FIELD_NAME, (char) 0x20);
            }

            ddl->createType(typeName, numFields, fields);

            delete[] fields;

        } else if (command == "delete" && command_type == "type") {

            ddl->deleteType(typeName);

        } else if (command == "list" && command_type == "type") {

            ddl->listTypes(outputFile);

        } else if (command == "create" && command_type == "record") {

            auto type = sysCatalog->getType(typeName);

            if (type != sysCatalog->types.end()) {
                int *fields = new int32[type->numFields];

                for (int i = 0; i < type->numFields; i++) {
                    iss >> fields[i];
                }

                dml->createRecord(typeName, fields);

                delete[] fields;
            }

        } else if (command == "delete" && command_type == "record") {
            int32 primaryKey;

            iss >> primaryKey;

            dml->deleteRecord(typeName, primaryKey);

        } else if (command == "update" && command_type == "record") {
            int32 primaryKey;

            iss >> primaryKey;

            auto type = sysCatalog->getType(typeName);

            if (type != sysCatalog->types.end()) {
                auto *fields = new int32[type->numFields - 1];

                for (int i = 0; i < type->numFields - 1; ++i) {
                    iss >> fields[i];
                }

                dml->updateRecord(typeName, primaryKey, fields);

                delete[] fields;
            }

        } else if (command == "search" && command_type == "record") {

            int32 primaryKey;

            iss >> primaryKey;

            auto record = dml->searchRecord(typeName, primaryKey);

            if (record != nullptr) {

                for (int i = 0; i < record->numFields; ++i) {
                    outputFile << record->fields[i] << " ";
                }

                delete record;

                outputFile << endl;

            }

        } else if (command == "list" && command_type == "record") {

            dml->listRecords(typeName, outputFile);

        }

    }

    delete sysCatalog;

    delete dml;

    delete ddl;

    return 0;
}