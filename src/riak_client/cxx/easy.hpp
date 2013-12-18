/*
 Copyright 2013 Pedro Larroy Tovar

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#pragma once
#include <string>


#include "riak_client/cxx/object/riak_metadata.hpp"
#include "riak_client/cxx/basic.hpp"

namespace riak
{

namespace easy
{

/**
 * Easy Client interface for riak
 *
 * All member functions throw instances of riak::exception when there is an error
 *
 */
class Client
{
public:
    Client(const std::string& host, const std::string& port);
    ~Client();

    void set_def_metadata(const riak_metadata& metadata)
    {
        m_def_metadata = metadata;
    }

    void set_def_store_params(const store_params& sparams)
    {
        m_def_store_params = sparams;
    }

    void set_def_bucket_properties(const bucket_properties& bprop)
    {
        m_def_bucket_properties = bprop;
    }

    void set_def_r(int r)
    {
        m_def_r = r;
    }

    bool ping();
    void set_client_id(int id);
    uint32_t get_client_id();
    string_vector list_buckets();
    string_vector list_keys(const std::string& bucket);
    void put(const std::string& bucket, const std::string& key, const std::string& value);
    void put_params(const std::string& bucket, const std::string& key, const std::string& value, int w, int dw);

    response<result_ptr> fetch(const std::string& bucket, const std::string& key, int r);
    std::string fetch_value(const std::string& bucket, const std::string& key);
    riak_metadata fetch_metadata(const std::string& bucket, const std::string& key, int r);
    void set_bucket_properties(const std::string& bucket, const bucket_properties& prop);

private:
    std::string m_host;
    std::string m_port;
    client_ptr m_client;
    store_params m_def_store_params;
    bucket_properties m_def_bucket_properties;
    riak_metadata m_def_metadata;
    int m_def_r;
};


} // end ns easy
} // end ns riak
