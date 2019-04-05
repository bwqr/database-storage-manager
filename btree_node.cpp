#include "btree_node.h"
#include <iostream>
#include "helpers.h"

void btree_node::read(std::fstream &stream) {
    stream.seekg(0);

    stream.read((char *) &id, BTREE_ID);
    stream.read((char *) &leaf, BTREE_LEAF);
    stream.read((char *) &n, NUM_BTREE);

    index index;

    for (int i = 0; i < n; ++i) {
        index.read(stream);
        indices.insert(index);
    }

    stream.seekg(BTREE_ID + BTREE_LEAF + NUM_BTREE + INDEX_PER_BTREE*(FILE_ID + RECORD_ID + PAGE_ID + PRIMARY_KEY));

    stream.read((char *) pointers, (INDEX_PER_BTREE + 1) * (BTREE_POINTER));
    stream.read((char *) &leftSibling, BTREE_POINTER);
    stream.read((char *) &rightSibling, BTREE_POINTER);
}

void btree_node::write(std::fstream &stream) {
    stream.seekp(0);

    stream.write((char *) &id, BTREE_ID);
    stream.write((char *) &leaf, BTREE_LEAF);
    stream.write((char *) &n, NUM_BTREE);

    int k = 0;
    for(auto &i: indices) {
        i.write(stream);
        k++;
    }
    index index(0, 0, 0, 0);

    for (; k < INDEX_PER_BTREE; ++k) {
        index.write(stream);
    }

    stream.write((char *) pointers, (INDEX_PER_BTREE + 1) * (BTREE_POINTER));
    stream.write((char *) &leftSibling, BTREE_POINTER);
    stream.write((char *) &rightSibling, BTREE_POINTER);
}

btree_node::btree_node(uint32 id) {

    indices = std::set<index>();

    for (int i = 0;i < INDEX_PER_BTREE + 1; ++i) {
        pointers[i] = 0;
    }

    this->id = id;
}
