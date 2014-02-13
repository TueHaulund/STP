#define BOOST_TEST_MODULE STP_UT

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

#include <boost/test/included/unit_test.hpp>

#include "stp.hpp"
using namespace stp;

struct TestFixture
{
    TestFixture()
    {
        int_vec = std::vector<int>(5, 5);
        float_vec = std::vector<float>(5, 5.1f);
        char_vec = std::vector<char>(5, 'a');
        bool_vec = std::vector<bool>(5, true);
        string_vec = std::vector<std::string>({"s1 ", "s2  ", "s3   ", "s4    "});
        ordered_ints = std::vector<int> ({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        unordered_ints = std::vector<int> ({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(unordered_ints.begin(), unordered_ints.end(), std::default_random_engine(seed));
    }

    std::vector<int> int_vec;
    std::vector<float> float_vec;
    std::vector<char> char_vec;
    std::vector<bool> bool_vec;
    std::vector<std::string> string_vec;
    std::vector<int> ordered_ints;
    std::vector<int> unordered_ints;
};

BOOST_FIXTURE_TEST_SUITE(SequenceSourceTests, TestFixture)
BOOST_AUTO_TEST_CASE(RangeIntTest)
{
    BOOST_CHECK( Range(1, 5)       == std::vector<int>({1, 2, 3, 4}) );
    BOOST_CHECK( Range(5, 1)       == std::vector<int>({5, 4, 3, 2}) );
    BOOST_CHECK( Range(-1, 1)      == std::vector<int>({-1, 0}) );
    BOOST_CHECK( Range(1, -1)      == std::vector<int>({1, 0}) );
    BOOST_CHECK( Range(1, 1)       == std::vector<int>() );
    BOOST_CHECK( Range(-1, -1)     == std::vector<int>() );
    BOOST_CHECK( Range(0, 0)       == std::vector<int>() );
    BOOST_CHECK( Range(0, 1)       == std::vector<int>({0}) );
    BOOST_CHECK( Range(1, 0)       == std::vector<int>({1}) );

    BOOST_CHECK( Range(1, 5, 2)    == std::vector<int>({1, 3}) );
    BOOST_CHECK( Range(5, 1, 2)    == std::vector<int>({5, 3}) );
    BOOST_CHECK( Range(1, 5, 0)    == std::vector<int>() );
    BOOST_CHECK( Range(1, 5, 6)    == std::vector<int>() );
    BOOST_CHECK( Range(1, 10, 3)   == std::vector<int>({1, 4, 7}) );
    BOOST_CHECK( Range(10, 1, 3)   == std::vector<int>({10, 7, 4}) );

    BOOST_CHECK( Range(-1, -5, 2)  == std::vector<int>({-1, -3}) );
    BOOST_CHECK( Range(-5, -1, 2)  == std::vector<int>({-5, -3}) );
    BOOST_CHECK( Range(-1, -5, 0)  == std::vector<int>() );
    BOOST_CHECK( Range(-1, -5, 6)  == std::vector<int>() );
    BOOST_CHECK( Range(-1, -10, 3) == std::vector<int>({-1, -4, -7}) );
    BOOST_CHECK( Range(-10, -1, 3) == std::vector<int>({-10, -7, -4}) );
}

BOOST_AUTO_TEST_CASE(RangeFloatTest)
{
    //TODO: MAKE ME
    BOOST_CHECK( Range(1.0, 5.0) == std::vector<double>({1.0, 2.0, 3.0, 4.0}));
}

BOOST_AUTO_TEST_CASE(RangeDoubleTest)
{
    //TODO: MAKE ME
    BOOST_CHECK( Range(1.0f, 5.0f) == std::vector<float>({1.0f, 2.0f, 3.0f, 4.0f}));
}

BOOST_AUTO_TEST_CASE(RangeCharTest)
{
    //TODO: MAKE ME
    BOOST_CHECK( Range('a', 'e') == std::vector<char>({'a', 'b', 'c', 'd'}));
}

BOOST_AUTO_TEST_CASE(RepeatTest)
{
    BOOST_CHECK( Repeat(1, 3)                  == std::vector<int>(3, 1) );
    BOOST_CHECK( Repeat(10, 5)                 == std::vector<int>(5, 10) );
    BOOST_CHECK( Repeat(std::string("hej"), 3) == std::vector<std::string>(3, "hej") );
    BOOST_CHECK( Repeat(false, 10)             == std::vector<bool>(10, false) );
}
BOOST_AUTO_TEST_SUITE_END() //SequenceSourceTests

BOOST_FIXTURE_TEST_SUITE(ConversionTests, TestFixture)
BOOST_AUTO_TEST_CASE(ToVectorTest)
{
    BOOST_CHECK( Transform(int_vec, ToVector())        == int_vec );
    BOOST_CHECK( Transform(float_vec, ToVector())      == float_vec );
    BOOST_CHECK( Transform(char_vec, ToVector())       == char_vec );
    BOOST_CHECK( Transform(bool_vec, ToVector())       == bool_vec );
    BOOST_CHECK( Transform(string_vec, ToVector())     == string_vec );
    BOOST_CHECK( Transform(ordered_ints, ToVector())   == ordered_ints );
    BOOST_CHECK( Transform(unordered_ints, ToVector()) == unordered_ints );
}

BOOST_AUTO_TEST_CASE(ToListTest)
{
    BOOST_CHECK( Transform(int_vec, ToList())        == std::list<int>(5, 5) );
    BOOST_CHECK( Transform(float_vec, ToList())      == std::list<float>(5, 5.1f) );
    BOOST_CHECK( Transform(char_vec, ToList())       == std::list<char>(5, 'a') );
    BOOST_CHECK( Transform(bool_vec, ToList())       == std::list<bool>(5, true) );
    BOOST_CHECK( Transform(string_vec, ToList())     == std::list<std::string>(string_vec.begin(), string_vec.end()) );
    BOOST_CHECK( Transform(ordered_ints, ToList())   == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( Transform(unordered_ints, ToList()) == std::list<int>(unordered_ints.begin(), unordered_ints.end()) );
}
BOOST_AUTO_TEST_SUITE_END() //ConversionTests

BOOST_FIXTURE_TEST_SUITE(AggregationTests, TestFixture)
BOOST_AUTO_TEST_CASE(SumTest)
{
    BOOST_CHECK( Transform(int_vec, Sum())        == 25 );
    BOOST_CHECK( Transform(float_vec, Sum())      == 25.5 );
    BOOST_CHECK( Transform(bool_vec, Sum())       == 1  );
    BOOST_CHECK( Transform(string_vec, Sum())     == "s1 s2  s3   s4    " );
    BOOST_CHECK( Transform(ordered_ints, Sum())   == 55 );
    BOOST_CHECK( Transform(unordered_ints, Sum()) == 55 );
}

BOOST_AUTO_TEST_CASE(CountTest)
{
    BOOST_CHECK( Transform(int_vec, Count())        == 5 );
    BOOST_CHECK( Transform(float_vec, Count())      == 5 );
    BOOST_CHECK( Transform(char_vec, Count())       == 5 );
    BOOST_CHECK( Transform(bool_vec, Count())       == 5 );
    BOOST_CHECK( Transform(string_vec, Count())     == 4 );
    BOOST_CHECK( Transform(ordered_ints, Count())   == 10 );
    BOOST_CHECK( Transform(unordered_ints, Count()) == 10 );
}
BOOST_AUTO_TEST_SUITE_END() //AggregationTests

BOOST_FIXTURE_TEST_SUITE(FilterTests, TestFixture)
BOOST_AUTO_TEST_CASE(TakeTest)
{
    BOOST_CHECK( Transform(ordered_ints, Take(5))    == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(ordered_ints, Take(6))    == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Transform(ordered_ints, Take(10))   == ordered_ints );
    BOOST_CHECK( Transform(ordered_ints, Take(1000)) == ordered_ints );
    BOOST_CHECK( Transform(ordered_ints, Take(0))    == std::vector<int>({}) );
}

BOOST_AUTO_TEST_CASE(DropTest)
{
    //TODO: MAKE ME
}

BOOST_AUTO_TEST_CASE(WhereTest)
{
    //TODO: MAKE ME
}
BOOST_AUTO_TEST_SUITE_END() //FilterTests