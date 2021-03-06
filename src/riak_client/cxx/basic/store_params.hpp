/*
 Copyright 2011 Basho Technologies, Inc.

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


namespace riak {

class RIAKC_API store_params
{
public:

    enum: uint32_t
    {
        ONE = 4294967295 - 1,
        QUORUM = 4294967295 - 2,
        ALL = 4294967295 - 3,
        DEFAULT = 4294967295 - 4,
    };

    store_params()
        : w_(DEFAULT), dw_(DEFAULT), pw_(DEFAULT), return_body_(false) { }
public: // accessors
    uint32_t w() const { return w_; }
    uint32_t dw() const { return dw_; }
    uint32_t pw() const { return pw_; }
    bool return_body() const { return return_body_; }
public: // mutators
    store_params& w(uint32_t w) { w_ = w; return *this;}
    store_params& dw(uint32_t dw) { dw_ = dw; return *this;}
    store_params& pw(uint32_t pw) { pw_ = pw; return *this; }
    store_params& return_body(bool returnbody)
    {
        return_body_ = returnbody;
        return *this;
    }
private: // intentionally copyable
    uint32_t w_;
    uint32_t dw_;
    uint32_t pw_;
    bool return_body_;
};

} // ::riak

