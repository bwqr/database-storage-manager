//
// Created by fmk on 12.03.2019.
//

#ifndef SRC_DML_H
#define SRC_DML_H

#include <string>
#include "defs.h"
#include "system_catalog.h"

using namespace std;

class dml {
private:
    SystemCatalog *systemCatalog;

public:
    dml(SystemCatalog *systemCatalog);

    void createRecord(string typeName, int32_t *fields);

    void deleteRecord(string typeName, int32_t primaryKey);

    record* searchRecord(string typeName, int32_t primaryKey);

    void listRecords(string typeName, ostream &stream);

    void updateRecord(string typeName, int32_t primaryKey, int32_t *fields);
};


#endif //SRC_DML_H
