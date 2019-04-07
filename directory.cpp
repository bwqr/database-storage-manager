#include "directory.h"
#include <iostream>
#include <unistd.h>

void directory::insert(index index) {
    int bucket_no = hash(index.value);

    string bucketName = generateBucketFileName(typeName, bucket_ids[bucket_no]);
    if(bucket_no > (1 << depth) - 1) {
        cout << "bucket no overflow";
    }
    fstream bucketFile(bucketName, INOUTBIN);

    bucket bucket;
    bucket.read(bucketFile);

    if(bucket.isFull()) {
        split(bucket, bucket_no);
        bucket.write(bucketFile);
        bucketFile.close();
        insert(index);
    } else {
        bucket.insert(index);
        bucket.write(bucketFile);
        bucketFile.close();
    }

}

void directory::remove(index index) {
    int bucket_no = hash(index.value);

    fstream bucketFile(generateBucketFileName(typeName, bucket_ids[bucket_no]), INOUTBIN);

    bucket bucket;
    bucket.read(bucketFile);
    bucket.remove(index);

    if(bucket.isEmpty() && bucket.getDepth() > 1) {
        bucketFile.close();
        merge(bucket, bucket_no);
    } else {
        bucket.write(bucketFile);

        bucketFile.close();
    }
}

index directory::search(index index) {

    int bucket_no = hash(index.value);

    fstream bucketFile(generateBucketFileName(typeName, bucket_ids[bucket_no]), INOUTBIN);

    bucket bucket;
    bucket.read(bucketFile);

    return bucket.search(index);
}

void directory::read(std::fstream &stream) {
    stream.seekg(0);

    stream.read((char *) &depth, DEPTHS);
    stream.read((char *) &bucket_size, BUCKET_SIZE);
    this->bucket_ids = vector<uint32>(bucket_size);

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
    int local_depth, dir_depth = depth, pair_index;

    bucket.increaseDepth();

    local_depth = bucket.getDepth();

    class bucket split_bucket;

    split_bucket.id = generateBucketId(typeName);
    split_bucket.depth = local_depth;

    pair_index = pairIndex(bucket_no, local_depth);

    if(local_depth > depth) {
        grow();
        bucket_ids[bucket_no + (1 << dir_depth) ] = split_bucket.id;
    } else {
        if(bucket_no + (1 << (dir_depth - 1)) >= bucket_size) {

            bucket_ids[bucket_no ] = split_bucket.id;
        } else {

            bucket_ids[bucket_no + (1 << (dir_depth - 1)) ] = split_bucket.id;
        }
    }


    vector<index> copy = vector<index>(bucket.indices.begin(), bucket.indices.end());

    bucket.clear();

    for(auto &i: copy) {
        int _bucket_no = hash(i.value);

        if(bucket_ids[_bucket_no ] == split_bucket.id) {
            split_bucket.insert(i);
        } else if(bucket_ids[_bucket_no ] == bucket.id){
            bucket.insert(i);
        } else {
            cout << "error";
        }
    }

    fstream splitBucketFile(generateBucketFileName(typeName, split_bucket.id), OUTBIN);

    split_bucket.write(splitBucketFile);
}

void directory::merge(bucket &bucket, int bucket_no) {
    int pair_index;

    pair_index = pairIndex(bucket_no, bucket.depth);

    std::remove(generateBucketFileName(typeName, bucket_ids[bucket_no]).c_str());

    bucket_ids[bucket_no] = bucket_ids[pair_index];

    for (int i = 0; i < bucket_size; ++i) {
        if(bucket_ids[i] == bucket.id) {
            bucket_ids[i] = bucket_ids[pair_index];
        }
    }

    fstream pairBucketFile(generateBucketFileName(typeName, bucket_ids[pair_index]), INOUTBIN);

    class bucket pair;

    pair.read(pairBucketFile);

    pair.decreaseDepth();

    pair.write(pairBucketFile);

    pairBucketFile.close();

    shrink();
}

void directory::grow() {

    int double_size = 1 << depth + 1;

    bucket_ids.resize(double_size);

    for (int i = 0; i < bucket_size; ++i) {
        bucket_ids[i + (1 << depth)] = bucket_ids[i];
    }

    bucket_size = bucket_size << 1;

    depth++;
}

void directory::shrink() {
    for (int i = 0; i < bucket_size; ++i) {
        fstream bucketFile(generateBucketFileName(typeName, bucket_ids[i]), INOUTBIN);

        bucket bucket;

        bucket.read(bucketFile);

        if(bucket.getDepth() == depth) {
            return;
        }
    }

    depth--;

    for (int j = 0; j < 1 << depth; ++j) {
        bucket_ids.pop_back();
    }

    bucket_size = bucket_size >> 1;
}

void directory::print() {

    vector<uint32> visited;

    for (int i = 0; i < bucket_size; ++i) {
        bool cont = false;

        for(auto &visit: visited) {
            if(visit == bucket_ids[i]) {
                cont = true;
                break;
            }
        }

        if(cont) {
            continue;
        }

        visited.push_back(bucket_ids[i]);

        fstream bucketFile(generateBucketFileName(typeName, bucket_ids[i]), INOUTBIN);

        bucket bucket;

        bucket.read(bucketFile);

        for(auto &index: bucket.indices) {
            std::cout << bucket_ids[i] << "\t" << index.file_id << "\t" << (int) index.page_id << "\t" << (int) index.record_id << "\t" << index.value << std::endl;
        }
    }
}

int directory::pairIndex(int bucket_no, int depth) {
    return bucket_no^(1 << (depth - 1));
}
