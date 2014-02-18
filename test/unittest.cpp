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

struct test_fixture
{
    test_fixture()
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
    bool compare_range(const std::vector<T> &a, const std::vector<T> &b)
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

BOOST_FIXTURE_TEST_SUITE(generation_tests, test_fixture)
BOOST_AUTO_TEST_CASE(range_int_test)
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

BOOST_AUTO_TEST_CASE(range_float_test)
{
    BOOST_CHECK( compare_range(range(1.0f, 5.0f),          std::vector<float>({1.0f, 2.0f, 3.0f, 4.0f})) );
    BOOST_CHECK( compare_range(range(1.0f, -1.0f),         std::vector<float>({1.0f, 0.0f})) );
    BOOST_CHECK( compare_range(range(-1.0f, 1.0f),         std::vector<float>({-1.0f, 0.0f})) );
    BOOST_CHECK( compare_range(range(0.0f, 0.0f),          std::vector<float>({})) );
    BOOST_CHECK( compare_range(range(1.0f, 1.0f),          std::vector<float>({})) );
    BOOST_CHECK( compare_range(range(-1.0f, -1.0f),        std::vector<float>({})) );
    BOOST_CHECK( compare_range(range(0.0f, 1.0f),          std::vector<float>({0.0f})) );
    BOOST_CHECK( compare_range(range(1.0f, 0.0f),          std::vector<float>({1.0f})) );
    BOOST_CHECK( compare_range(range(0.0f, 10.0f, 3.14f),  std::vector<float>({0.0f, 3.14f, 6.28f, 9.42f})) );
    BOOST_CHECK( compare_range(range(1.0f, -1.0f, 0.33f),  std::vector<float>({1.0f, 0.67f, 0.34f, 0.01f, -0.32f, -0.65f, -0.98f})) );
    BOOST_CHECK( compare_range(range(0.0f, 10.0f, 11.65f), std::vector<float>({})) );
    BOOST_CHECK( compare_range(range(10.0f, 0.0f, 11.65f), std::vector<float>({})) );
    BOOST_CHECK( compare_range(range(0.0f, 10.0f, 5.65f),  std::vector<float>({0.0f, 5.65f})) );
}

BOOST_AUTO_TEST_CASE(range_double_test)
{
    BOOST_CHECK( compare_range(range(1.0, 5.0),         std::vector<double>({1.0, 2.0, 3.0, 4.0})) );
    BOOST_CHECK( compare_range(range(1.0, -1.0),        std::vector<double>({1.0, 0.0})) );
    BOOST_CHECK( compare_range(range(-1.0, 1.0),        std::vector<double>({-1.0, 0.0})) );
    BOOST_CHECK( compare_range(range(0.0, 0.0),         std::vector<double>({})) );
    BOOST_CHECK( compare_range(range(1.0, 1.0),         std::vector<double>({})) );
    BOOST_CHECK( compare_range(range(-1.0, -1.0),       std::vector<double>({})) );
    BOOST_CHECK( compare_range(range(0.0, 1.0),         std::vector<double>({0.0})) );
    BOOST_CHECK( compare_range(range(1.0, 0.0),         std::vector<double>({1.0})) );
    BOOST_CHECK( compare_range(range(0.0, 10.0, 3.14),  std::vector<double>({0.0, 3.14, 6.28, 9.42})) );
    BOOST_CHECK( compare_range(range(1.0, -1.0, 0.33),  std::vector<double>({1.0, 0.67, 0.34, 0.01, -0.32, -0.65, -0.98})) );
    BOOST_CHECK( compare_range(range(0.0, 10.0, 11.65), std::vector<double>({})) );
    BOOST_CHECK( compare_range(range(10.0, 0.0, 11.65), std::vector<double>({})) );
    BOOST_CHECK( compare_range(range(0.0, 10.0, 5.65),  std::vector<double>({0.0, 5.65})) );
}

BOOST_AUTO_TEST_CASE(range_char_test)
{
    BOOST_CHECK( range('a', 'e')     == std::vector<char>({'a', 'b', 'c', 'd'}) );
    BOOST_CHECK( range('z', 'v')     == std::vector<char>({'z', 'y', 'x', 'w'}) );
    BOOST_CHECK( range('J', 'N')     == std::vector<char>({'J', 'K', 'L', 'M'}) );
    BOOST_CHECK( range('V', 'R')     == std::vector<char>({'V', 'U', 'T', 'S'}) );
    BOOST_CHECK( range('0', '4')     == std::vector<char>({'0', '1', '2', '3'}) );
    BOOST_CHECK( range('$', ' ')     == std::vector<char>({'$', '#', '"', '!'}) );
    BOOST_CHECK( range('a', 'a')     == std::vector<char>({}) );
    BOOST_CHECK( range('a', 'b')     == std::vector<char>({'a'}) );
    BOOST_CHECK( range('1', '5', 2)  == std::vector<char>({'1', '3'}) );
    BOOST_CHECK( range('5', '1', 2)  == std::vector<char>({'5', '3'}) );
    BOOST_CHECK( range('1', '5', 0)  == std::vector<char>() );
    BOOST_CHECK( range('1', '5', 6)  == std::vector<char>() );
    BOOST_CHECK( range('A', '[', 25) == std::vector<char>({'A', 'Z'}) );
    BOOST_CHECK( range('a', '@', 32) == std::vector<char>({'a', 'A'}) );
}

BOOST_AUTO_TEST_CASE(repeat_test)
{
    BOOST_CHECK( repeat(1, 3)                  == std::vector<int>(3, 1) );
    BOOST_CHECK( repeat(10, 5)                 == std::vector<int>(5, 10) );
    BOOST_CHECK( repeat(std::string("hej"), 3) == std::vector<std::string>(3, "hej") );
    BOOST_CHECK( repeat(false, 10)             == std::vector<bool>(10, false) );
}
BOOST_AUTO_TEST_SUITE_END() //generation_tests

BOOST_FIXTURE_TEST_SUITE(stl_conversion_tests, test_fixture)
BOOST_AUTO_TEST_CASE(to_vector_test)
{
    auto tv_obj = to_vector();
    BOOST_CHECK( tv_obj(int_vec)        == int_vec );
    BOOST_CHECK( tv_obj(float_vec)      == float_vec );
    BOOST_CHECK( tv_obj(char_vec)       == char_vec );
    BOOST_CHECK( tv_obj(bool_vec)       == bool_vec );
    BOOST_CHECK( tv_obj(string_vec)     == string_vec );
    BOOST_CHECK( tv_obj(ordered_ints)   == ordered_ints );
    BOOST_CHECK( tv_obj(unordered_ints) == unordered_ints );
}

BOOST_AUTO_TEST_CASE(to_list_test)
{
    auto tl_obj = to_list();
    BOOST_CHECK( tl_obj(int_vec)        == std::list<int>(5, 5) );
    BOOST_CHECK( tl_obj(float_vec)      == std::list<float>(5, 5.1f) );
    BOOST_CHECK( tl_obj(char_vec)       == std::list<char>(5, 'a') );
    BOOST_CHECK( tl_obj(bool_vec)       == std::list<bool>(5, true) );
    BOOST_CHECK( tl_obj(string_vec)     == std::list<std::string>(string_vec.begin(), string_vec.end()) );
    BOOST_CHECK( tl_obj(ordered_ints)   == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( tl_obj(unordered_ints) == std::list<int>(unordered_ints.begin(), unordered_ints.end()) );
}
BOOST_AUTO_TEST_SUITE_END() //stl_conversion_tests

BOOST_FIXTURE_TEST_SUITE(reduction_tests, test_fixture)
BOOST_AUTO_TEST_CASE(sum_test)
{
    auto sum_obj = sum();
    BOOST_CHECK( sum_obj(int_vec)        == 25 );
    BOOST_CHECK( sum_obj(float_vec)      == 25.5 );
    BOOST_CHECK( sum_obj(bool_vec)       == 1  );
    BOOST_CHECK( sum_obj(string_vec)     == "s1 s2  s3   s4    " );
    BOOST_CHECK( sum_obj(ordered_ints)   == 55 );
    BOOST_CHECK( sum_obj(unordered_ints) == 55 );
}

BOOST_AUTO_TEST_CASE(count_test)
{
    auto count_obj = count();
    BOOST_CHECK( count_obj(int_vec)        == 5 );
    BOOST_CHECK( count_obj(float_vec)      == 5 );
    BOOST_CHECK( count_obj(char_vec)       == 5 );
    BOOST_CHECK( count_obj(bool_vec)       == 5 );
    BOOST_CHECK( count_obj(string_vec)     == 4 );
    BOOST_CHECK( count_obj(ordered_ints)   == 10 );
    BOOST_CHECK( count_obj(unordered_ints) == 10 );
}
BOOST_AUTO_TEST_SUITE_END() //reduction_tests

BOOST_FIXTURE_TEST_SUITE(filter_tests, test_fixture)
BOOST_AUTO_TEST_CASE(take_test)
{
    //TODO: Fix sequence ownership and copy/reference behaviour
    /*auto take_5_stp = make_stp(take(5));
    auto take_6_stp = make_stp(take(6));
    auto take_10_stp = make_stp(take(10));
    auto take_1000_stp = make_stp(take(1000));
    auto take_0_stp = make_stp(take(0));

    BOOST_CHECK( take_5_stp(ordered_ints)    == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( take_6_stp(ordered_ints)    == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( take_10_stp(ordered_ints)   == ordered_ints );
    BOOST_CHECK( take_1000_stp(ordered_ints) == ordered_ints );
    BOOST_CHECK( take_0_stp(ordered_ints)    == std::vector<int>({}) );*/
}

BOOST_AUTO_TEST_CASE(drop_test)
{
    //TODO: MAKE ME
}

BOOST_AUTO_TEST_CASE(where_test)
{
    auto where_stp = make_stp(where([](const int &i){return i % 2 == 0;}));
    BOOST_CHECK( where_stp(ordered_ints) == std::vector<int>({2, 4, 6, 8, 10}) );
    //TODO: MAKE ME
}
BOOST_AUTO_TEST_SUITE_END() //filter_tests