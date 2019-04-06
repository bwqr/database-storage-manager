#ifndef SRC_BTREE_H
#define SRC_BTREE_H

#include <string>
#include "index.h"
#include "btree_node.h"

struct child_entry {
    uint32 pointer;
    class index index;
};

class btree {
private:
    index tree_search(uint32 pointer, int32 primaryKey);
public:
    uint32 root_pointer;

    std::string typeName;

    btree(const std::string &typeName, uint32 root_pointer);

    index search(int32 primaryKey);

    void insert(uint32 pointer, index &index, struct child_entry* &entry);
//    void insert2(uint32 pointer, index &index, btree_node* &new_child_entry);

    void traverse();

    void traversePointers();
};


#endif //SRC_BTREE_H
