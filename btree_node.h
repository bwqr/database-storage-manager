#ifndef SRC_BTREE_NODE_H
#define SRC_BTREE_NODE_H

#include <tiff.h>
#include <fstream>
#include <vector>
#include "index.h"
#include "defs.h"

class btree_node {
public:
    uint8 n = 0;
    uint32 id;
    bool leaf = true;
    std::vector<index> indices;
    uint32 *pointers = new uint32[INDEX_PER_BTREE + 1];
    uint32 leftSibling = 0;
    uint32 rightSibling = 0;

    btree_node(uint32 id);

    ~btree_node();

    void read(std::fstream &stream);
    void write(std::fstream &stream) const;
};


#endif //SRC_BTREE_NODE_H
