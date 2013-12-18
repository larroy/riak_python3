#include <boost/python.hpp>
#include "riak_client/cxx/easy.hpp"
using namespace boost::python;

using namespace riak::easy;

BOOST_PYTHON_MODULE_INIT(riak3k)
{
    class_<Client>("Client", "Riak Client class", init<std::string, std::string>())
        .def("put", &Client::put)
        .def("fetch_value", &Client::fetch_value)
    ;
}

