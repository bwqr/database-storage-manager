#ifndef SRC_BUCKET_H
#define SRC_BUCKET_H

#include <vector>
#include <fstream>
#include "index.h"

class bucket {
public:
    uint32 id;
    uint8 size = 0;
    uint32 overflow_page_id = 0;
    int32 depth = 1;
    std::vector<index> indices;
    bucket() = default;

    void insert(index index);
    void remove(index index);
    index search(index index);
    bool isFull();
    bool isEmpty();
    int getDepth();
    void increaseDepth();
    void decreaseDepth();
    void clear();

    void read(std::fstream &stream);
    void write(std::fstream &stream) const;
};


#endif //SRC_BUCKET_H
