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

namespace riak
{

namespace easy
{

class Object
{
    std::string get_key();
    std::string get_bucket();
    riak_metadata get_metadata();

};

class Client
{
    Client(const std::string& host, const std::string& port);
    ~Client();


    bool ping();
    void set_client_id(int id);
    uint32_t get_client_id();
    string_vector list_buckets();
    string_vector list_keys(const std::string& bucket);
    void put(const std::string& bucket, const std::string& key, const std::string& value);
    void put_params(const std::string& bucket, const std::string& key, const std::string& value, int w, int dw);

    response<result_ptr> get(const std::string& bucket, const std::string& key, int r)
    std::string get_value(const std::string& bucket, const std::string& key, int r)
    void set_bucket_properties(const std::string& bucket, const bucket_properties& prop);


};


} // end ns easy
} // end ns riak
