#include "directory.h"

string generateDirectoryFileName(string &typeName) {
    return ROOT  + truncateName(typeName) + INFIX + "directory";
}

string generateBucketFileName(string &typeName, uint32 bucket_id) {
    return ROOT + truncateName(typeName) + INFIX + "bucket" + INFIX + to_string(bucket_id);
}

uint32 generateBucketId(string &typeName);

void directory::insert(int32 key) {
    int bucket_no = hash(key);

    fstream bucketFile(generateBucketFileName(typeName, bucket_ids[bucket_no]), INOUTBIN);

    bucket bucket(bucketFile);

    if(bucket.isFull()) {
        split(bucket, bucket_no);
    }

    int bucket_no_splitted = hash(key);

    if(bucket_no == bucket_no_splitted) {
        bucket.insert(key);
    } else {
        insert(key);
    }

    bucket.write(bucketFile);
    bucketFile.close();
}

void directory::remove(int32 key) {
    int bucket_no = hash(key);

    fstream bucketFile(generateBucketFileName(typeName, bucket_ids[bucket_no]), INOUTBIN);

    bucket bucket(bucketFile);

    bucket.remove(key);

    if(bucket.isEmpty() && bucket.getDepth() > 1) {
        merge(bucket);
    }

    bucket.write(bucketFile);

    bucketFile.close();
}

index directory::search(int32 key) {

    int bucket_no = hash(key);

    fstream bucketFile(generateBucketFileName(typeName, bucket_ids[bucket_no]), INOUTBIN);

    bucket bucket(bucketFile);

    return bucket.search(key);
}

void directory::read(std::fstream &stream) {
    stream.seekg(0);

    stream.read((char *) &depth, DEPTHS);
    stream.read((char *) &bucket_size, BUCKET_SIZE);

    for (int i = 0; i < bucket_size; ++i) {
        stream.read((char *) &bucket_ids[i], BUCKET_ID);
    }
}

void directory::write(std::fstream &stream) const {
    stream.seekp(0);

    stream.write((char *) &depth, DEPTHS);
    stream.write((char *) &bucket_size, BUCKET_SIZE);

    for (int i = 0; i < bucket_size; ++i) {
        stream.write((char *) &bucket_ids[i], BUCKET_ID);
    }
}

directory::directory(std::string typeName) {

    this->typeName = typeName;

    fstream directoryFile(generateDirectoryFileName(typeName), INOUTBIN);
    
    this->read(directoryFile);
}

int directory::hash(int32 key) {
    return key&((1<<depth)-1);
}

void directory::split(bucket &bucket, int bucket_no) {
    int local_depth;

    bucket.increateDepth();

    local_depth = bucket.getDepth();

    if(local_depth > depth) {
        grow();
    }

    class bucket split_bucket;

    split_bucket.id = generateBucketId(typeName);

    bucket_ids[bucket_no + 1] = split_bucket.id;

    vector<index> copy = vector<index>(bucket.indices.begin(), bucket.indices.end());

    bucket.clear();

    for(auto &i : copy) {
        insert(i.value);
    }
}

void directory::merge(bucket &bucket) {

}

int directory::pairIndex(int bucket_no, int depth) {
    return bucket_no^(1<<(depth-1));
}

void directory::grow() {
    int double_size = 1 << depth;

    bucket_ids.resize(double_size);

    for (int i = double_size - 1; i >= 0; --i) {
        bucket_ids[i] = bucket_ids[i / 2];
    }

    depth++;
}
