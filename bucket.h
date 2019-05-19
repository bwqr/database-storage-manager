#ifndef SRC_BUCKET_H
#define SRC_BUCKET_H

#include <vector>
#include <fstream>
#include "index.h"

class bucket {
public:
    uint32_t id;
    uint8_t  size = 0;
    int32_t depth = 1;
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
