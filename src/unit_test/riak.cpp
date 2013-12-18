// http://www.boost.org/doc/libs/1_47_0/libs/test/doc/html/utf/testing-tools/reference.html

#include <boost/test/unit_test.hpp>
#include "riak_client/cxx/riak_client.hpp"
#include "riak_client/cxx/easy.hpp"


#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


using namespace std;


namespace std
{
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

/// Formatted string, allows to use stream operators and returns a std::string with the resulting format
#define fs(a) \
   (static_cast<const std::ostringstream&>(((*make_unique<std::ostringstream>().get()) << a)).str ())


namespace
{
int err_sys(std::string s)
{
    std::string err;
    char *err_str = std::strerror(errno);
    if (err_str)
        err = s + " error: " + std::strerror(errno)  + "\n";
    else
        err = s + " error: unknown (strerror returned NULL)\n";
    throw std::runtime_error(err);
}
}

/**
 * @author larroy
 * @addtogroup unit_tests
 * @{
 */


struct RiakTextFixture
{
    RiakTextFixture():
        m_pid(-1)
    {
        if ( (m_pid = fork()) > 0)
        {
            // waiting for the server to be started and stabilized
            sleep(1);
        }
        else if (m_pid == 0)
        {
            vector<char*> arglist;
            arglist.push_back(const_cast<char*>(s_riak_test_server_bin.c_str()));
            arglist.push_back(const_cast<char*>(s_port.c_str())); // 8087 is the one used by the real riak
            arglist.push_back(nullptr);

            cout << "Starting riak test server on port: " << s_port << endl;
            execvp(s_riak_test_server_bin.c_str(), &arglist[0]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            throw std::runtime_error(fs("fork() error: " << m_pid));
        }
    }

    ~RiakTextFixture()
    {
        cout << "Sending SIGTERM to riak test server pid: " << m_pid << " ..." << flush;
        if (kill(m_pid, SIGTERM))
            err_sys(fs("kill" << m_pid << " failed"));
        int status = 0;
        if( waitpid(m_pid, &status, 0) < 0 )
        {
            perror("wait failed");
            exit(EXIT_FAILURE);
        }
        cout << "Done." << endl;
    }

    int m_pid;
    static const std::string s_port;
    static const std::string s_host;
    static const std::string s_riak_test_server_bin;
};

const std::string RiakTextFixture::s_port = "8086";
const std::string RiakTextFixture::s_host = "127.0.0.1"; // avoid using localhost as it might fail depending on resolver and interface status in Linux

#ifdef DEBUG
const std::string RiakTextFixture::s_riak_test_server_bin = "build/debug/riak_test_server";
#else
const std::string RiakTextFixture::s_riak_test_server_bin = "build/release/riak_test_server";
#endif

static const std::string TEST_BUCKET("riak-cxx-test");
static const std::string TEST_KEY("riak-cxx-test");


BOOST_GLOBAL_FIXTURE(RiakTextFixture);

BOOST_AUTO_TEST_CASE(riak_test_01)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    bool ping = c->ping();
    BOOST_REQUIRE(ping);
    c->client_id(42);
    BOOST_REQUIRE(c->client_id() == 42);
}

BOOST_AUTO_TEST_CASE (test_set_bucket)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    riak::bucket_properties properties;
    properties.allow_mult(true);
    properties.n_val(3);
    BOOST_REQUIRE(c->set_bucket(TEST_BUCKET, properties) == true);
}

BOOST_AUTO_TEST_CASE (test_fetch_bucket)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    riak::bucket_properties result = c->fetch_bucket(TEST_BUCKET);
    BOOST_REQUIRE(result.allow_mult());
    BOOST_REQUIRE(result.n_val() == 3);
}

BOOST_AUTO_TEST_CASE (test_put)
{
    //std::cout << riak::tss_client_id() << std::endl;
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    c->client_id(42);
    riak::store_params sp;
    sp.w(3).dw(3).return_body(true);
    riak::result_ptr fetch_result = c->fetch(TEST_BUCKET, TEST_KEY, 2);
    riak::object_ptr o;
    if (fetch_result->not_found())
    {
        o = riak::make_object(TEST_BUCKET, TEST_KEY, TEST_KEY);
        riak::link_vector v = o->update_content().links();
        v.push_back(riak::link("foo", "bar", "baz"));
        o->update_content().links(v);
    }
    else
        o = fetch_result->choose_sibling(0);
    riak::string_map usermeta(o->update_metadata().usermeta());
    usermeta["foo"] = "bar";
    riak::riak_metadata md(usermeta);
    o->update_metadata(md);
    riak::result_ptr r(c->store(o, sp));
    riak::object_ptr o2(r->choose_sibling(0));
}

BOOST_AUTO_TEST_CASE (test_fetch)
{
    riak::client_ptr c(riak::new_client("127.0.0.1", RiakTextFixture::s_port));
    riak::result_ptr fr(c->fetch(TEST_BUCKET, TEST_KEY, 3));
    BOOST_REQUIRE(fr->contents()[0].value() == TEST_KEY);
}

BOOST_AUTO_TEST_CASE (test_list_buckets)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    riak::string_vector v = c->list_buckets();
    BOOST_REQUIRE(std::find(v.begin(), v.end(), TEST_BUCKET) != v.end());
}

BOOST_AUTO_TEST_CASE (test_list_keys)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    BOOST_REQUIRE(v.size() > 0);
}

BOOST_AUTO_TEST_CASE (test_del)
{
    riak::client_ptr c = riak::new_client("127.0.0.1", RiakTextFixture::s_port);
    riak::string_vector v = c->list_keys(TEST_BUCKET);
    for (riak::string_vector::size_type i=0;
         i < v.size(); ++i)
    {
        BOOST_REQUIRE(c->del(TEST_BUCKET, v[i], 3) == true);
    }
}

BOOST_AUTO_TEST_CASE (test_client)
{
    riak::cluster cluster(riak::node("127.0.0.1", RiakTextFixture::s_port));
    riak::client client(cluster.make_client());
    riak::basic_bucket<std::string> bucket = client.bucket<std::string>("bucket");
    bucket.del("foo").rw(2)();
    std::string value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "");
    value = bucket.store("foo", "bar")
        .r(2)
        .w(2)
        .dw(2)();
    value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "bar");
}


BOOST_AUTO_TEST_CASE(test_easy1)
{
    riak::easy::Client client(RiakTextFixture::s_host, RiakTextFixture::s_port);
}

BOOST_AUTO_TEST_CASE(test_easy2)
{
    riak::easy::Client client(RiakTextFixture::s_host, RiakTextFixture::s_port);
    string bucket = "test_easy2";
    string key = "key";
    string value = "value";
    BOOST_CHECK_EQUAL("", client.fetch_value(bucket, key));
}


BOOST_AUTO_TEST_CASE(test_easy3)
{
    riak::easy::Client client(RiakTextFixture::s_host, RiakTextFixture::s_port);
    string bucket = "test_easy3";
    string key = "key";
    string value = "value";
    client.put(bucket, key, value);
    BOOST_CHECK_EQUAL(value, client.fetch_value(bucket, key));
}



// TODO
#if 0
BOOST_AUTO_TEST_CASE (test_client2)
{
    riak::cluster cluster(riak::node("127.0.0.1", "8087"));
    riak::client client(cluster.make_client());
    riak::basic_bucket<std::string> bucket = client.bucket<std::string>("bucket");
    bucket.del("foo").rw(2)();
    std::string value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "");
    value = bucket.store("foo", "bar").w(1)();
    value = bucket.fetch("foo").r(3)();
    BOOST_REQUIRE(value == "bar");
}

#endif






/// @}
