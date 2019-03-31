//
// Created by fmk on 12.03.2019.
//

#ifndef SRC_DDL_H
#define SRC_DDL_H

#include <string>
#include "system_catalog.h"

class ddl {
private:
    SystemCatalog * systemCatalog;

public:
    ddl(SystemCatalog *systemCatalog);

    void createType(std::string typeName, int numFields, std::string *fields);

    void deleteType(std::string typeName);

    void listTypes(ostream &stream);
};


#endif //SRC_DDL_H
