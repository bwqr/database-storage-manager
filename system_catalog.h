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

    set<type> types;

    fstream catalogFile;

    SystemCatalog();

    void createCatalogFile();

    void listTypes(ostream &stream);

    void writeNumTypes();

    void write();

    set<type>::iterator getType(string &typeName);

    set<index>::iterator searchKey(const type &type, int32 primaryKey);

    void insertIndex(const type &type, uint32 file_id, uint8 page_id, uint8 record_id, int32 value);

    void readIndex(const type &type);

    bool checkExist(const type &type, int32 primaryKey);

    set<index> listIndex(string &typeName);

    ~SystemCatalog();
};


#endif //SRC_SYSTEMCATALOG_H
