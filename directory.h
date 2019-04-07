//https://github.com/nitish6174/extendible-hashing/
#ifndef SRC_DIRECTORY_H
#define SRC_DIRECTORY_H

#include <vector>
#include <tiff.h>
#include "bucket.h"
#include "helpers.h"

using namespace std;

class directory {
private:
    string typeName;
    uint32 depth, bucket_size;

    vector<uint32> bucket_ids;

    int hash(int32 key);
    void grow();
    void shrink();
    int pairIndex(int bucket_no, int depth);
    void split(bucket &bucket, int bucket_no);
    void merge(bucket &bucket);

public:

    directory(string typeName);

    void insert(int32 key);
    void remove(int32 key);
    index search(int32 key);

    void read(fstream &stream);
    void write(fstream &stream) const;
};


#endif //SRC_DIRECTORY_H
