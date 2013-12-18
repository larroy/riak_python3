/**
 @file    bucket_properties.hpp
 @brief   Bucket Properties */
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

#include <riak_client/cxx/riak_client_fwd.hpp>

namespace riak {

/// Customizable properties of a Riak Bucket.
///
/// @see http://wiki.basho.com/Basic-Riak-API-Operations.html
class RIAKC_API bucket_properties
{
public:
    bucket_properties()
        : m_allow_mult(false), m_n_val(-1) { }

    /// Return the value of the <dfn>allow_mult</dfn> bucket property.
    bool allow_mult() const
    {
        return m_allow_mult;
    }

    /// Return the <dfn>n-val</dfn> bucket property.
    ///
    /// The n-value is the replication factor of a bucket.
    int n_val() const
    {
        return m_n_val;
    }

    /// Set the <dfn>allow_mult</dfn> bucket property.
    void allow_mult(bool allow)
    {
        m_allow_mult = allow;
    }

    /// Set the <dfn>n_val</dfn> bucket property.
    void n_val(int n)
    {
        m_n_val = n;
    }
private:
    bool m_allow_mult;
    int m_n_val;
};

} // :: riak

