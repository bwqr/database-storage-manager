#ifndef SRC_SYSTEMCATALOG_H
#define SRC_SYSTEMCATALOG_H

#include <tiff.h>
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
    int32 numTypes;

    vector<type> types;

    fstream catalogFile;

    SystemCatalog();

    void createCatalogFile();

    void createType(string typeName, int numFields, string* fields);

    void listTypes(ostream &stream);

    void writeNumTypes();

    void write();

    vector<type>::iterator getType(string &typeName);

    void deleteType(string typeName);

    void deleteRecord(string typeName, int32 primaryKey);

    record* searchByPrimaryKey(string typeName, int32 primaryKey);

    set<index>::iterator searchKey(type &type, int32 primaryKey);

    void createRecord(string typeName, int32 *fields);

    void updateRecord(string typeName, int32 primaryKey, int32 *fields);

    void insertIndex(type &type, uint32 file_id, uint8 page_id, uint8 record_id, int32 value);

    void readIndex(type &type);

    bool checkExist(type &type, int32 primaryKey);

    void listRecords(string typeName, ostream &stream);

    set<index> listIndex(string &typeName);

    ~SystemCatalog();
};


#endif //SRC_SYSTEMCATALOG_H
