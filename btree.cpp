#include "btree.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include "defs.h"
#include "helpers.h"

bool hasSpace(uint8 &n) {
    return n < INDEX_PER_BTREE;
}



void btree::findDistributeSibling(btree_node &node, btree_node &sibling, btree_node *parent) {

    if(node.leftSibling != 0) {
        fstream siblingFile(generateIndexFileName(typeName, node.leftSibling), INOUTBIN);

        sibling.read(siblingFile);

        if(sibling == parent) {
            errno = sibling.n > BTREE_D ? 0: 1 ; //same parent, however not have extra

            return;
        }

    }

    if(node.rightSibling != 0) {
        fstream siblingFile(generateIndexFileName(typeName, node.leftSibling), INOUTBIN);

        sibling.read(siblingFile);

        if(sibling.parent == parent) {
            errno = sibling.n > BTREE_D ? 0: 1 ; //same parent, however not have extra

            return;
        }
    }

    errno = -1;
}

child_entry redistributeNodes(btree_node &main_node, btree_node &sibling_node, btree_node* parent) {

}

child_entry* mergeNodes(btree_node &main_node, btree_node &sibling_node, btree_node* parent) {

}

void bindSiblings(btree_node &left_node, btree_node &right_node) {
    right_node.rightSibling = left_node.rightSibling;
    left_node.rightSibling = right_node.id;
    right_node.leftSibling = left_node.id;
}

void popIndexByIndex(btree_node &node, const struct child_entry &entry) {
    int k = 0;

    for(auto i = node.indices.begin(); i != node.indices.end(); ++i, ++k) {
        if(entry.index.value == i->value) {
            node.indices.erase(i);
            break;
        }
    }

    for (int j = k; j < node.n; ++j) {
        node.pointers[j] = node.pointers[j + 1];
    }

    node.n--;
}

void pushIndex(btree_node &node, const struct child_entry &entry) {
    //insert into the position
    int k = 0;

    for(auto i = node.indices.begin(); i != node.indices.end(); ++i, ++k) {
        if(entry.index.value < i->value) {
            node.indices.insert(i, entry.index);
            break;
        }

    }

    if(k == node.n) {
        node.indices.push_back(entry.index);
    }

    //Shift the pointers
    for (int j = node.n + 1; j > k; --j) {
        node.pointers[j] = node.pointers[j - 1];
    }

    node.pointers[k + 1] = entry.pointer;

    node.n++;
}

struct child_entry* splitNode(btree_node &main_node, btree_node &split_node) {
    split_node.indices.clear();
    split_node.indices.resize(BTREE_D);

    auto entry = new child_entry {split_node.id, main_node.indices[BTREE_D]};
    int i = BTREE_D + 1, k = 0;
    for (; i < INDEX_PER_BTREE; ++i, ++k) {
        split_node.indices[k] = main_node.indices[i];
        split_node.pointers[k] = main_node.pointers[i];
    }

    split_node.pointers[k] = main_node.pointers[i];

    split_node.n = BTREE_D;
    main_node.n = BTREE_D;

    return entry;
}

void createAndWriteFile(const btree_node &node, string &typeName) {
    fstream node_file(generateIndexFileName(typeName, node.id), OUTBIN);

    node.write(node_file);

    node_file.close();
}

index btree::search(int32 primaryKey) {
    return this->tree_search(this->root_pointer, primaryKey);
}

btree::btree(const std::string &typeName, uint32 root_pointer) {
    this->typeName = typeName;
    this->root_pointer = root_pointer;
}

index btree::tree_search(uint32 pointer, int32 primaryKey) {
    std::fstream nodeFile(generateIndexFileName(typeName, pointer));

    btree_node btreeNode(0);

    btreeNode.read(nodeFile);

    nodeFile.close();

    errno = 0;

    if (btreeNode.leaf) {
        for(auto &i : btreeNode.indices) {
            if (primaryKey == i.value) {
                return i;
            }
        }

        errno = -1;

        return {};
    }
    int k = 0;

    for (auto &i: btreeNode.indices) {
        if(primaryKey < i.value) {
            return this->tree_search(btreeNode.pointers[k], primaryKey);
        } else if(primaryKey == i.value) {
            return i;
        }

        k++;
    }

    return this->tree_search(btreeNode.pointers[k], primaryKey);
}

void btree::insert(uint32 pointer, index &index, struct child_entry * &entry) {

    fstream nodeFile(generateIndexFileName(typeName, pointer), INOUTBIN);

    btree_node btreeNode(0);

    btreeNode.read(nodeFile);

    if(btreeNode.leaf) {
        struct child_entry new_entry = {0, index};

        if(hasSpace(btreeNode.n)) {
            pushIndex(btreeNode, new_entry);
            entry = nullptr;
        } else {
            btree_node split_child(generateIndexFilePostfix(typeName));
            entry = splitNode(btreeNode, split_child);

            if(index.value < entry->index.value) {
                pushIndex(btreeNode, new_entry);
            } else {
                pushIndex(split_child, new_entry);
            }

            bindSiblings(btreeNode, split_child);

            createAndWriteFile(split_child, typeName);
        }
    } else {
        auto iterator = btreeNode.indices.begin();
        int iterator_index = 0;
        for (; iterator != btreeNode.indices.end(); ++iterator, ++iterator_index) {
            if(index.value < iterator->value) {
                this->insert(btreeNode.pointers[iterator_index], index, entry);
                break;
            }
        }

        if(iterator == btreeNode.indices.end()) {
            this->insert(btreeNode.pointers[iterator_index], index, entry);
        }

        if(entry != nullptr) {
            if(hasSpace(btreeNode.n)) {
                pushIndex(btreeNode, *entry);
                delete entry;
                entry = nullptr;
            } else {
                btree_node split_sibling(generateIndexFilePostfix(typeName));
                split_sibling.leaf = false;

                auto new_entry = splitNode(btreeNode, split_sibling);

                if(entry->index.value < new_entry->index.value) {
                    pushIndex(btreeNode, *entry);
                } else {
                    pushIndex(split_sibling, *entry);
                }

                bindSiblings(btreeNode, split_sibling);

                delete entry;

                entry = new_entry;

                createAndWriteFile(split_sibling, typeName);
            }
        }
    }

    if(entry != nullptr && this->root_pointer == btreeNode.id) {
        btree_node new_root(generateIndexFilePostfix(typeName));

        new_root.indices.push_back(entry->index);
        new_root.pointers[0] = btreeNode.id;
        new_root.pointers[1] = entry->pointer;
        new_root.leaf = false;
        new_root.n = 1;
        this->root_pointer = new_root.id;

        fstream siblingFile(generateIndexFileName(typeName, entry->pointer), INOUTBIN);
        btree_node sibling(0);
        sibling.read(siblingFile);
        sibling.write(siblingFile);
        siblingFile.close();
        createAndWriteFile(new_root, typeName);
    }

    btreeNode.write(nodeFile);
    nodeFile.close();
};

void btree::traverse() {
    queue<uint32> nodes;


    nodes.push(this->root_pointer);

    while(!nodes.empty()) {
        btree_node node(nodes.front());

        nodes.pop();

        fstream nodeFile(generateIndexFileName(typeName, node.id), INOUTBIN);

        node.read(nodeFile);

        nodeFile.close();

        for(auto &i: node.indices) {
            std::cout << node.id  << "\t" << i.file_id << "\t" << (int) i.page_id << "\t" << (int) i.record_id << "\t" << i.value << std::endl;
        }

        if(!node.leaf) {

            for (int i = 0; i < node.n + 1; ++i) {
                nodes.push(node.pointers[i]);
            }
        }

    }

}

void btree::traversePointers() {
    queue<uint32> nodes;

    nodes.push(this->root_pointer);

    while(!nodes.empty()) {
        btree_node node(nodes.front());

        nodes.pop();

        fstream nodeFile(generateIndexFileName(typeName, node.id), INOUTBIN);

        node.read(nodeFile);

        if(!node.leaf) {

            int i = 0;

            for(auto &in: node.indices) {

                std::cout << node.pointers[i] << "\t" << in.value << "\t";
                nodes.push(node.pointers[i]);
                i++;
            }

            std::cout << node.pointers[i];

            std::cout << std::endl;
        }

        nodeFile.close();

    }
}

void btree::insertIndex(index index) {
    struct child_entry *node = nullptr;

    this->insert(this->root_pointer, index, node);
}

void btree::deleteIndex(index index) {
    struct child_entry *node = nullptr;

    btree_node *parent = nullptr;

    this->_delete(parent, this->root_pointer, index, node);
}

void btree::_delete(btree_node *parent, uint32 pointer, index &index, child_entry *&old_entry) {
    fstream nodeFile(generateIndexFileName(typeName, pointer), INOUTBIN);

    btree_node btreeNode(0);

    btreeNode.read(nodeFile);

    if(btreeNode.leaf) {
        child_entry entry = {0, index};
        if(btreeNode.n > BTREE_D) {
            popIndexByIndex(btreeNode, entry);
            old_entry = nullptr;
        } else {
            btree_node sibling(0);

            findDistributeSibling(btreeNode, sibling, parent);

            if(sibling.n > BTREE_D) {
                redistributeNodes(btreeNode,sibling, parent);
            } else {
                old_entry = mergeNodes(btreeNode,sibling, parent);
            }
        }
    } else {

    }
}

void btree::updateIndex(btree_node &btreeNode, index &index) {

}

