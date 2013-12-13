#include <boost/python.hpp>
#include "riak_client/cxx/riak_client.hpp"
using namespace boost::python;


class RiakClient
{
public:
    RiakClient(const std::string& host, const std::string& port)
        m_cli(riak::new_client(host, port))
    {
        
    }

    void put(

private:
    riak::client_ptr m_cli;
};

BOOST_PYTHON_MODULE_INIT(riak3)
{

}

