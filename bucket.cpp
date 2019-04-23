#include "bucket.h"

void bucket::clear() {
    indices.clear();
    size = 0;
}

void bucket::insert(index index) {
    indices.push_back(index);
    size++;
}

void bucket::remove(index index) {
    for (auto i = indices.begin(); i != indices.end(); ++i) {
        if(index.value == i->value) {
            indices.erase(i);
            size--;
            break;
        }
    }
}

index bucket::search(index index) {
    errno = 0;

    for (auto &i: indices) {
        if(index.value == i.value) {
            return i;
        }
    }

    errno = -1;
    return {};
}

bool bucket::isFull() {
    return size == INDEX_PER_BUCKET;
}

bool bucket::isEmpty() {
    return indices.empty();
}

int bucket::getDepth() {
    return depth;
}

void bucket::increaseDepth() {
    depth++;
}

void bucket::decreaseDepth() {
    depth--;
}

void bucket::read(std::fstream &stream) {
    stream.seekg(0);

    stream.read((char *) &id, BUCKET_ID);
    stream.read((char *) &size, BUCKET_SIZE_FIELD);
    stream.read((char *) &depth, DEPTHS);

    indices = std::vector<index>(size);

    for (int i = 0; i < size; ++i) {
        indices[i].read(stream);
    }
}

void bucket::write(std::fstream &stream) const {
    stream.seekp(0);

    stream.write((char *) &id, BUCKET_ID);
    stream.write((char *) &size, BUCKET_SIZE_FIELD);
    stream.write((char *) &depth, DEPTHS);

    for (int i = 0; i < size; ++i) {
        indices[i].write(stream);
    }

    index index(0,0,0,0);

    for (int j = size; j < INDEX_PER_BUCKET; ++j) {
        index.write(stream);
    }
}
