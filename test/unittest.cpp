#define BOOST_TEST_MODULE STP_UT

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <cmath>

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

    template<typename T>
    bool Equal(std::vector<T> a, std::vector<T> b)
    {
        if (a.size() == b.size())
        {
            for (size_t i = 0; i < a.size(); i++)
            {
                if (std::fabs(a[i] - b[i]) > std::numeric_limits<T>::epsilon())
                {
                    return false;
                }
            }
            return true;
        }
        return false;
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
    BOOST_CHECK( range(1, 5)       == std::vector<int>({1, 2, 3, 4}) );
    BOOST_CHECK( range(5, 1)       == std::vector<int>({5, 4, 3, 2}) );
    BOOST_CHECK( range(-1, 1)      == std::vector<int>({-1, 0}) );
    BOOST_CHECK( range(1, -1)      == std::vector<int>({1, 0}) );
    BOOST_CHECK( range(1, 1)       == std::vector<int>() );
    BOOST_CHECK( range(-1, -1)     == std::vector<int>() );
    BOOST_CHECK( range(0, 0)       == std::vector<int>() );
    BOOST_CHECK( range(0, 1)       == std::vector<int>({0}) );
    BOOST_CHECK( range(1, 0)       == std::vector<int>({1}) );

    BOOST_CHECK( range(1, 5, 2)    == std::vector<int>({1, 3}) );
    BOOST_CHECK( range(5, 1, 2)    == std::vector<int>({5, 3}) );
    BOOST_CHECK( range(1, 5, 0)    == std::vector<int>() );
    BOOST_CHECK( range(1, 5, 6)    == std::vector<int>() );
    BOOST_CHECK( range(1, 10, 3)   == std::vector<int>({1, 4, 7}) );
    BOOST_CHECK( range(10, 1, 3)   == std::vector<int>({10, 7, 4}) );

    BOOST_CHECK( range(-1, -5, 2)  == std::vector<int>({-1, -3}) );
    BOOST_CHECK( range(-5, -1, 2)  == std::vector<int>({-5, -3}) );
    BOOST_CHECK( range(-1, -5, 0)  == std::vector<int>() );
    BOOST_CHECK( range(-1, -5, 6)  == std::vector<int>() );
    BOOST_CHECK( range(-1, -10, 3) == std::vector<int>({-1, -4, -7}) );
    BOOST_CHECK( range(-10, -1, 3) == std::vector<int>({-10, -7, -4}) );
}

BOOST_AUTO_TEST_CASE(RangeFloatTest)
{
    BOOST_CHECK( Equal( range(1.0f, 5.0f)  , std::vector<float>({1.0f, 2.0f, 3.0f, 4.0f})) );
    BOOST_CHECK( Equal( range(1.0f, -1.0f) , std::vector<float>({1.0f, 0.0f})) );
    BOOST_CHECK( Equal( range(-1.0f, 1.0f) , std::vector<float>({-1.0f, 0.0f})) );
    BOOST_CHECK( Equal( range(0.0f, 0.0f)  , std::vector<float>({})) );
    BOOST_CHECK( Equal( range(1.0f, 1.0f)  , std::vector<float>({})) );
    BOOST_CHECK( Equal( range(-1.0f, -1.0f), std::vector<float>({})) );
    BOOST_CHECK( Equal( range(0.0f, 1.0f)  , std::vector<float>({0.0f})) );
    BOOST_CHECK( Equal( range(1.0f, 0.0f)  , std::vector<float>({1.0f})) );

    BOOST_CHECK( Equal( range(0.0f, 10.0f, 3.14f) , std::vector<float>({0.0f, 3.14f, 6.28f, 9.42f})) );
    BOOST_CHECK( Equal( range(1.0f, -1.0f, 0.33f) , std::vector<float>({1.0f, 0.67f, 0.34f, 0.01f, -0.32f, -0.65f, -0.98f})) );
    BOOST_CHECK( Equal( range(0.0f, 10.0f, 11.65f), std::vector<float>({})) );
    BOOST_CHECK( Equal( range(10.0f, 0.0f, 11.65f), std::vector<float>({})) );
    BOOST_CHECK( Equal( range(0.0f, 10.0f, 5.65f) , std::vector<float>({0.0f, 5.65f})) );
}

BOOST_AUTO_TEST_CASE(RangeDoubleTest)
{
    BOOST_CHECK( Equal( range(1.0, 5.0)  , std::vector<double>({1.0, 2.0, 3.0, 4.0})) );
    BOOST_CHECK( Equal( range(1.0, -1.0) , std::vector<double>({1.0, 0.0})) );
    BOOST_CHECK( Equal( range(-1.0, 1.0) , std::vector<double>({-1.0, 0.0})) );
    BOOST_CHECK( Equal( range(0.0, 0.0)  , std::vector<double>({})) );
    BOOST_CHECK( Equal( range(1.0, 1.0)  , std::vector<double>({})) );
    BOOST_CHECK( Equal( range(-1.0, -1.0), std::vector<double>({})) );
    BOOST_CHECK( Equal( range(0.0, 1.0)  , std::vector<double>({0.0})) );
    BOOST_CHECK( Equal( range(1.0, 0.0)  , std::vector<double>({1.0})) );

    BOOST_CHECK( Equal( range(0.0, 10.0, 3.14) , std::vector<double>({0.0, 3.14, 6.28, 9.42})) );
    BOOST_CHECK( Equal( range(1.0, -1.0, 0.33) , std::vector<double>({1.0, 0.67, 0.34, 0.01, -0.32, -0.65, -0.98})) );
    BOOST_CHECK( Equal( range(0.0, 10.0, 11.65), std::vector<double>({})) );
    BOOST_CHECK( Equal( range(10.0, 0.0, 11.65), std::vector<double>({})) );
    BOOST_CHECK( Equal( range(0.0, 10.0, 5.65) , std::vector<double>({0.0, 5.65})) );
}

BOOST_AUTO_TEST_CASE(RangeCharTest)
{
    BOOST_CHECK( range('a', 'e') == std::vector<char>({'a', 'b', 'c', 'd'}) );
    BOOST_CHECK( range('z', 'v') == std::vector<char>({'z', 'y', 'x', 'w'}) );
    BOOST_CHECK( range('J', 'N') == std::vector<char>({'J', 'K', 'L', 'M'}) );
    BOOST_CHECK( range('V', 'R') == std::vector<char>({'V', 'U', 'T', 'S'}) );
    BOOST_CHECK( range('0', '4') == std::vector<char>({'0', '1', '2', '3'}) );
    BOOST_CHECK( range('$', ' ') == std::vector<char>({'$', '#', '"', '!'}) );
    BOOST_CHECK( range('a', 'a') == std::vector<char>({}) );
    BOOST_CHECK( range('a', 'b') == std::vector<char>({'a'}) );

    BOOST_CHECK( range('1', '5', 2)  == std::vector<char>({'1', '3'}) );
    BOOST_CHECK( range('5', '1', 2)  == std::vector<char>({'5', '3'}) );
    BOOST_CHECK( range('1', '5', 0)  == std::vector<char>() );
    BOOST_CHECK( range('1', '5', 6)  == std::vector<char>() );
    BOOST_CHECK( range('A', '[', 25) == std::vector<char>({'A', 'Z'}) );
    BOOST_CHECK( range('a', '@', 32) == std::vector<char>({'a', 'A'}) );
}

BOOST_AUTO_TEST_CASE(RepeatTest)
{
    BOOST_CHECK( repeat(1, 3)                  == std::vector<int>(3, 1) );
    BOOST_CHECK( repeat(10, 5)                 == std::vector<int>(5, 10) );
    BOOST_CHECK( repeat(std::string("hej"), 3) == std::vector<std::string>(3, "hej") );
    BOOST_CHECK( repeat(false, 10)             == std::vector<bool>(10, false) );
}
BOOST_AUTO_TEST_SUITE_END() //SequenceSourceTests

BOOST_FIXTURE_TEST_SUITE(ConversionTests, TestFixture)
BOOST_AUTO_TEST_CASE(ToVectorTest)
{
    BOOST_CHECK( Transform(int_vec, to_vector())        == int_vec );
    BOOST_CHECK( Transform(float_vec, to_vector())      == float_vec );
    BOOST_CHECK( Transform(char_vec, to_vector())       == char_vec );
    BOOST_CHECK( Transform(bool_vec, to_vector())       == bool_vec );
    BOOST_CHECK( Transform(string_vec, to_vector())     == string_vec );
    BOOST_CHECK( Transform(ordered_ints, to_vector())   == ordered_ints );
    BOOST_CHECK( Transform(unordered_ints, to_vector()) == unordered_ints );
}

BOOST_AUTO_TEST_CASE(ToListTest)
{
    BOOST_CHECK( Transform(int_vec, to_list())        == std::list<int>(5, 5) );
    BOOST_CHECK( Transform(float_vec, to_list())      == std::list<float>(5, 5.1f) );
    BOOST_CHECK( Transform(char_vec, to_list())       == std::list<char>(5, 'a') );
    BOOST_CHECK( Transform(bool_vec, to_list())       == std::list<bool>(5, true) );
    BOOST_CHECK( Transform(string_vec, to_list())     == std::list<std::string>(string_vec.begin(), string_vec.end()) );
    BOOST_CHECK( Transform(ordered_ints, to_list())   == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( Transform(unordered_ints, to_list()) == std::list<int>(unordered_ints.begin(), unordered_ints.end()) );
}
BOOST_AUTO_TEST_SUITE_END() //ConversionTests

BOOST_FIXTURE_TEST_SUITE(AggregationTests, TestFixture)
BOOST_AUTO_TEST_CASE(SumTest)
{
    BOOST_CHECK( Transform(int_vec, sum())        == 25 );
    BOOST_CHECK( Transform(float_vec, sum())      == 25.5 );
    BOOST_CHECK( Transform(bool_vec, sum())       == 1  );
    BOOST_CHECK( Transform(string_vec, sum())     == "s1 s2  s3   s4    " );
    BOOST_CHECK( Transform(ordered_ints, sum())   == 55 );
    BOOST_CHECK( Transform(unordered_ints, sum()) == 55 );
}

BOOST_AUTO_TEST_CASE(CountTest)
{
    BOOST_CHECK( Transform(int_vec, count())        == 5 );
    BOOST_CHECK( Transform(float_vec, count())      == 5 );
    BOOST_CHECK( Transform(char_vec, count())       == 5 );
    BOOST_CHECK( Transform(bool_vec, count())       == 5 );
    BOOST_CHECK( Transform(string_vec, count())     == 4 );
    BOOST_CHECK( Transform(ordered_ints, count())   == 10 );
    BOOST_CHECK( Transform(unordered_ints, count()) == 10 );
}
BOOST_AUTO_TEST_SUITE_END() //AggregationTests

BOOST_FIXTURE_TEST_SUITE(FilterTests, TestFixture)
BOOST_AUTO_TEST_CASE(TakeTest)
{
    BOOST_CHECK( Transform(ordered_ints, take(5))    == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(ordered_ints, take(6))    == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Transform(ordered_ints, take(10))   == ordered_ints );
    BOOST_CHECK( Transform(ordered_ints, take(1000)) == ordered_ints );
    BOOST_CHECK( Transform(ordered_ints, take(0))    == std::vector<int>({}) );
}

BOOST_AUTO_TEST_CASE(DropTest)
{
    //TODO: MAKE ME
}

BOOST_AUTO_TEST_CASE(WhereTest)
{
    BOOST_CHECK( Transform(ordered_ints, where([](const int &i){return i % 2 == 0;})) == std::vector<int>({2, 4, 6, 8, 10}) );
    //TODO: MAKE ME
}
BOOST_AUTO_TEST_SUITE_END() //FilterTests