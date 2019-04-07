#include "bucket.h"

bucket::bucket(std::fstream &stream) {
    this->read(stream);
}

void bucket::clear() {
    indices.clear();
}

void bucket::insert(int32 key) {

}

void bucket::remove(int32 key) {

}

index bucket::search(int32 key) {
    return {};
}

bool bucket::isFull() {
    return indices.size() == INDEX_PER_BUCKET;
}

bool bucket::isEmpty() {
    return indices.empty();
}

int bucket::getDepth() {
    return depth;
}

void bucket::increateDepth() {
    depth++;
}

void bucket::decreaseDepth() {
    depth--;
}

void bucket::read(std::fstream &stream) {

}

void bucket::write(std::fstream &stream) const {

}
