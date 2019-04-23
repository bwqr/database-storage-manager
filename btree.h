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

    void insert(uint32 pointer, index &index, struct child_entry* &entry);

    void updateIndex(btree_node &btreeNode, index &index);

    void findDistributeSibling(btree_node &node, btree_node &sibling, btree_node *parent);

    void _delete(btree_node *parent, uint32 pointer, index &index, child_entry *&old_entry);
public:
    uint32 root_pointer;

    std::string typeName;

    btree(const std::string &typeName, uint32 root_pointer);

    index search(int32 primaryKey);


    void insertIndex(index index);

    void deleteIndex(index index);
//    void insert2(uint32 pointer, index &index, btree_node* &new_child_entry);

    void traverse();

    void traversePointers();
};


#endif //SRC_BTREE_H