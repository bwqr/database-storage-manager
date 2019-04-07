#ifndef SRC_BUCKET_H
#define SRC_BUCKET_H

#include <vector>
#include <fstream>
#include "index.h"

class bucket {
public:
    uint32 id;
    std::vector<index> indices;
    uint32 overflow_page_id = 0;
    int depth;
    bucket() = default;
    bucket(std::fstream &stream);

    void insert(int32 key);
    void remove(int32 key);
    index search(int32 key);
    bool isFull();
    bool isEmpty();
    int getDepth();
    void increateDepth();
    void decreaseDepth();
    void clear();

    void read(std::fstream &stream);
    void write(std::fstream &stream) const;
};


#endif //SRC_BUCKET_H
