//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <leveldb/db.h>
#include <pycpp/cache/kv_backend.h>
#include <stdexcept>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

void kv_open(void* db, const path_t& path, kv_options options, comparator_callback cb)
{
    // set options
    leveldb::Options opts;
//    if (options & kv_logs_mask == kv_reuse_logs) {
//        opts.reuse_logs = true;
//    }

    // create database
    auto status = leveldb::DB::Open(opts, path_to_string(path), (leveldb::DB**) &db);
    if (!status.ok()) {
        throw std::runtime_error(status.ToString().data());
    }
}


void kv_close(void* db)
{
    delete (leveldb::DB*) db;
}

PYCPP_END_NAMESPACE
