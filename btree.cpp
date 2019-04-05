#include "btree.h"
#include <fstream>
#include <iostream>
#include <queue>
#include "defs.h"
#include "helpers.h"

index btree::search(int32 primaryKey) {
    return this->tree_search(this->root_pointer, primaryKey);
}

btree::btree(const std::string &typeName, uint32 root_pointer) {
    this->typeName = typeName;
    this->root_pointer = root_pointer;
}

index btree::tree_search(uint32 pointer, int32 primaryKey) {
    std::fstream nodeFile(ROOT + truncateName(typeName) + "index_" + to_string(pointer));

    btree_node btreeNode(0);

    btreeNode.read(nodeFile);

    nodeFile.close();

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
        }

        k++;
    }

    return this->tree_search(btreeNode.pointers[k], primaryKey);
}

void btree::insert(uint32 pointer, index &index, btree_node *&new_child_entry) {
    fstream nodeFile(generateIndexFileName(typeName, pointer), INOUTBIN);

    btree_node btreeNode(0);

    btreeNode.read(nodeFile);

    if (!btreeNode.leaf) {
        int i = 0;

        for (auto &in: btreeNode.indices) {
            if(index.value < in.value) {
                this->insert(btreeNode.pointers[i], index, new_child_entry);
                break;
            }

            i++;
        }

        if(i == btreeNode.n) {
            this->insert(btreeNode.pointers[i], index, new_child_entry);
        }

        if (new_child_entry != nullptr) {
            if (btreeNode.n < INDEX_PER_BTREE) {
                int j = i;
                for (; j < btreeNode.n; ++j) {
                    btreeNode.pointers[j + 1] = btreeNode.pointers[j];
                }
                btreeNode.pointers[j + 1] = btreeNode.pointers[j];

                btreeNode.pointers[i + 1] = new_child_entry->id;
                btreeNode.pointers[i] = new_child_entry->leftSibling;
                btreeNode.indices.insert(*new_child_entry->indices.begin());
                new_child_entry = nullptr;
                btreeNode.n++;
            } else {
                auto new_sibling = new btree_node(generateIndexFilePostfix(typeName));

                for (int i = BTREE_D, k = 0; i < INDEX_PER_BTREE; ++i, k++) {

                    auto in = btreeNode.indices.rbegin();
                    new_sibling->indices.insert(*in);
                    btreeNode.indices.erase(*in);
                    new_sibling->pointers[k] = btreeNode.pointers[i + 1];
                }


                btreeNode.n = BTREE_D;
                new_sibling->n = INDEX_PER_BTREE - BTREE_D;

                if(new_child_entry->indices.begin()->value < new_sibling->indices.begin()->value) {
                    btreeNode.indices.insert(*new_child_entry->indices.begin());
                    btreeNode.pointers[btreeNode.n] = new_child_entry->id;
                    btreeNode.n++;
                } else {
                    new_sibling->indices.insert(*new_child_entry->indices.begin());
                    new_sibling->pointers[new_sibling->n] = new_child_entry->id;
                    new_sibling->n++;
                }

                new_sibling->rightSibling = btreeNode.rightSibling;
                btreeNode.rightSibling= new_sibling->id;
                new_sibling->leftSibling = btreeNode.id;

                if (btreeNode.id == this->root_pointer) {
                    btree_node newRoot(generateIndexFilePostfix(typeName));

                    newRoot.pointers[0] = btreeNode.id;
                    newRoot.pointers[1] = new_sibling->id;
                    newRoot.indices.insert(*new_sibling->indices.begin());

                    newRoot.leaf = false;
                    this->root_pointer = newRoot.id;
                    fstream newRootFile(generateIndexFileName(typeName, newRoot.id), OUTBIN);

                    newRoot.write(newRootFile);

                    newRootFile.close();

                    this->root_pointer = newRoot.id;
                } else {
                    new_child_entry = new_sibling;
                }
                fstream newSiblingFile(generateIndexFileName(typeName, new_sibling->id), OUTBIN);

                new_sibling->write(newSiblingFile);

                newSiblingFile.close();
            }

            btreeNode.write(nodeFile);
        }
        nodeFile.close();
        return;
    } else {
        if(btreeNode.n < INDEX_PER_BTREE) {
            btreeNode.indices.insert(index);
            btreeNode.n++;
            new_child_entry = nullptr;
            btreeNode.write(nodeFile);
            nodeFile.close();
            return;
        } else {
            new_child_entry = new btree_node(generateIndexFilePostfix(typeName));

            auto iterator = btreeNode.indices.begin();

            for (int i = 0; i < BTREE_D; ++i) {
                i++;
            }

            new_child_entry->indices = set<class index>(btreeNode.indices.begin(), iterator);

            btreeNode.indices = set<class index>(++iterator, btreeNode.indices.end());

            for (int i = BTREE_D, k = 0; i < INDEX_PER_BTREE; ++i, k++) {
                new_child_entry->pointers[k] = btreeNode.pointers[i + 1];
            }


            btreeNode.n = BTREE_D;
            new_child_entry->n = INDEX_PER_BTREE - BTREE_D;

            if(index.value < new_child_entry->indices.begin()->value) {
                btreeNode.indices.insert(index);
                btreeNode.n++;
            } else {
                new_child_entry->indices.insert(index);
                new_child_entry->n++;
            }

            btreeNode.rightSibling = new_child_entry->id;
            new_child_entry->leftSibling = btreeNode.id;

            fstream newChildEntryFile(generateIndexFileName(typeName, new_child_entry->id), OUTBIN);

            new_child_entry->write(newChildEntryFile);

            newChildEntryFile.close();

            btreeNode.write(nodeFile);

            nodeFile.close();

            return;
        }
    }
}

void btree::traverse() {
    queue<uint32> nodes;


    nodes.push(this->root_pointer);

    while(!nodes.empty()) {
        btree_node node(nodes.front());

        nodes.pop();

        fstream nodeFile(generateIndexFileName(typeName, node.id), INOUTBIN);

        node.read(nodeFile);

        nodeFile.close();

        if(node.leaf) {

            for(auto &i: node.indices) {
                std::cout << node.id  << "\t" << i.file_id << "\t" << (int) i.page_id << "\t" << (int) i.record_id << "\t" << i.value << std::endl;
            }
        } else {
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

            std::cout << std::endl;
        }

        nodeFile.close();

    }
}
