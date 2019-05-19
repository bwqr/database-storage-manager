#ifndef SRC_SYSTEMCATALOG_H
#define SRC_SYSTEMCATALOG_H

#include "type.h"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include "file.h"
#include "record.h"

using namespace std;

class SystemCatalog {
private:
public:
    int32_t numTypes;

    set<type> types;

    fstream catalogFile;

    SystemCatalog();

    void createCatalogFile();

    void listTypes(ostream &stream);

    void writeNumTypes();

    void write();

    set<type>::iterator getType(string &typeName);

    index searchKey(const type &type, int32_t primaryKey);

    void insertIndex(const type &type, uint32_t file_id, uint8_t page_id, uint8_t record_id, int32_t value);

    void readIndex(const type &type);

    bool checkExist(const type &type, int32_t primaryKey);

    set<index> listIndex(string &typeName);

    ~SystemCatalog();
};


#endif //SRC_SYSTEMCATALOG_H
