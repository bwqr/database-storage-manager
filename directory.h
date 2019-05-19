#ifndef SRC_DIRECTORY_H
#define SRC_DIRECTORY_H

#include <vector>
#include "bucket.h"
#include "helpers.h"

using namespace std;

class directory {
private:
    uint32_t depth = 1, bucket_size = 2;


    int hash(int32_t key);
    void grow();
    void shrink();
    int pairIndex(int bucket_no, int depth);
    void split(bucket &bucket, int bucket_no);
    void merge(bucket &bucket, int bucket_no);

public:
    string typeName;
    vector<uint32_t> bucket_ids;

    directory(string typeName);

    directory() = default;

    void insert(index index);
    void remove(index index);
    index search(index index);

    void read(fstream &stream);
    void write(fstream &stream) const;

    void print();
};


#endif //SRC_DIRECTORY_H
