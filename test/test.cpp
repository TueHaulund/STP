#define BOOST_TEST_MODULE STP_UT

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <limits>
#include <map>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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
        string_vec = std::vector<std::string>({"s1 ", "s2  ", "s3   ", "s4    ", "s5     "});
        ordered_ints = std::vector<int> ({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        unordered_ints = std::vector<int> ({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(unordered_ints.begin(), unordered_ints.end(), std::default_random_engine(seed));
        BOOST_TEST_MESSAGE( "Setup test fixture" );
    }

    ~test_fixture()
    {
        BOOST_TEST_MESSAGE( "Teardown test fixture" );
    }

    template<typename T>
    bool comp_real(const T &a, const T &b, const T &epsilon)
    {
        return (std::fabs(a - b) < epsilon);
    }

    template<typename T>
    bool compare_range(const std::vector<T> &a, const std::vector<T> &b, const T epsilon = std::numeric_limits<T>::epsilon())
    {
        if (a.size() == b.size())
        {
            for (size_t i = 0; i < a.size(); i++)
            {
                if (!comp_real(a[i], b[i], epsilon))
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
    std::vector<int> empty_int_vec;
};

BOOST_FIXTURE_TEST_SUITE(bool_reduction_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(all_test)
    {
        auto all_5 = all([](const int &i){return i == 5;});
        auto all_false = all([](const bool &i){return i == false;});
        auto all_true = all([](const bool &i){return i == true;});
        auto all_alpha = all([](const char &i){return isalpha(i);});

        BOOST_CHECK( all_5(ordered_ints)  == false );
        BOOST_CHECK( all_5(int_vec)       == true );
        BOOST_CHECK( all_false(bool_vec)  == false );
        BOOST_CHECK( all_true(bool_vec)   == true );
        BOOST_CHECK( all_alpha(char_vec)  == true );
        BOOST_CHECK( all_5(empty_int_vec) == true );
    }

    BOOST_AUTO_TEST_CASE(any_test)
    {
        auto any_6 = any([](const int &i){return i == 6;});
        auto any_false = any([](const bool &i){return i == false;});
        auto any_digit = any([](const char &i){return isdigit(i);});

        BOOST_CHECK( any_6(ordered_ints)  == true );
        BOOST_CHECK( any_6(int_vec)       == false );
        BOOST_CHECK( any_false(bool_vec)  == false );
        BOOST_CHECK( any_digit(char_vec)  == false );
        BOOST_CHECK( any_6(empty_int_vec) == false );
    }

    BOOST_AUTO_TEST_CASE(contains_test)
    {
        auto contains_5 = contains(5);
        auto contains_4 = contains(4);
        auto contains_a = contains('a');
        auto contains_str = contains(std::string("s1 "));

        BOOST_CHECK( contains_5(int_vec)       == true );
        BOOST_CHECK( contains_4(int_vec)       == false );
        BOOST_CHECK( contains_a(int_vec)       == false );
        BOOST_CHECK( contains_a(char_vec)      == true );
        BOOST_CHECK( contains_str(string_vec)  == true );
        BOOST_CHECK( contains_5(empty_int_vec) == false );
    }

    BOOST_AUTO_TEST_CASE(equal_test)
    {
        auto equal_oi = equal(ordered_ints);
        auto equal_uoi = equal(unordered_ints);
        auto equal_empty = equal(empty_int_vec);
        auto equal_cv = equal(char_vec);

        BOOST_CHECK( equal_oi(ordered_ints)                       == true );
        BOOST_CHECK( equal_uoi(unordered_ints)                    == true );
        BOOST_CHECK( equal_oi(std::vector<int>({1, 2, 3}))        == false );
        BOOST_CHECK( equal_uoi(std::vector<int>({1, 2, 3}))       == false );
        BOOST_CHECK( equal_empty(std::vector<int>())              == true );
        BOOST_CHECK( equal_empty(std::vector<int>({1, 2, 3}))     == false );
        BOOST_CHECK( equal_cv(char_vec)                           == true );
        BOOST_CHECK( equal_cv(std::vector<char>({'a', 'b', 'c'})) == false );
    }
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(filter_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(drop_test)
    {
        auto drop_5 = drop(5);
        auto drop_6 = drop(6);
        auto drop_10 = drop(10);
        auto drop_1000 = drop(1000);
        auto drop_0 = drop(0);

        BOOST_CHECK( drop_5(ordered_ints)    == std::vector<int>({6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_6(ordered_ints)    == std::vector<int>({7, 8, 9, 10}) );
        BOOST_CHECK( drop_10(ordered_ints)   == std::vector<int>({}) );
        BOOST_CHECK( drop_1000(ordered_ints) == std::vector<int>({}) );
        BOOST_CHECK( drop_0(ordered_ints)    == ordered_ints );
        BOOST_CHECK( drop_0(empty_int_vec)   == empty_int_vec );
        BOOST_CHECK( drop_10(empty_int_vec)  == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(drop_while_test)
    {
        auto drop_lt5 = drop_while([](const int &i){return i < 5;});
        auto drop_lte5 = drop_while([](const int &i){return i <= 5;});
        auto drop_e0 = drop_while([](const int &i){return i == 0;});
        auto drop_e1000 = drop_while([](const int &i){return i == 1000;});
        auto drop_lt1000 = drop_while([](const int &i){return i < 1000;});
        auto drop_e1 = drop_while([](const int &i){return i == 1;});

        BOOST_CHECK( drop_lt5(ordered_ints)    == std::vector<int>({5, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_lte5(ordered_ints)   == std::vector<int>({6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_e0(ordered_ints)     == ordered_ints );
        BOOST_CHECK( drop_e1000(ordered_ints)  == ordered_ints );
        BOOST_CHECK( drop_lt1000(ordered_ints) == std::vector<int>({}) );
        BOOST_CHECK( drop_e1(ordered_ints)     == std::vector<int>({2, 3, 4, 5, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_e0(empty_int_vec)    == empty_int_vec );
        BOOST_CHECK( drop_lt5(empty_int_vec)   == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(take_test)
    {
        auto take_5 = take(5);
        auto take_6 = take(6);
        auto take_10 = take(10);
        auto take_1000 = take(1000);
        auto take_0 = take(0);

        BOOST_CHECK( take_5(ordered_ints)    == std::vector<int>({1, 2, 3, 4, 5}) );
        BOOST_CHECK( take_6(ordered_ints)    == std::vector<int>({1, 2, 3, 4, 5, 6}) );
        BOOST_CHECK( take_10(ordered_ints)   == ordered_ints );
        BOOST_CHECK( take_1000(ordered_ints) == ordered_ints );
        BOOST_CHECK( take_0(ordered_ints)    == std::vector<int>({}) );
        BOOST_CHECK( take_0(empty_int_vec)   == empty_int_vec );
        BOOST_CHECK( take_10(empty_int_vec)  == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(take_while_test)
    {
        auto take_lt5 = take_while([](const int &i){return i < 5;});
        auto take_lte5 = take_while([](const int &i){return i <= 5;});
        auto take_e0 = take_while([](const int &i){return i == 0;});
        auto take_e1000 = take_while([](const int &i){return i == 1000;});
        auto take_lt1000 = take_while([](const int &i){return i < 1000;});
        auto take_e1 = take_while([](const int &i){return i == 1;});

        BOOST_CHECK( take_lt5(ordered_ints)    == std::vector<int>({1, 2, 3, 4}) );
        BOOST_CHECK( take_lte5(ordered_ints)   == std::vector<int>({1, 2, 3, 4, 5}) );
        BOOST_CHECK( take_e0(ordered_ints)     == std::vector<int>({}) );
        BOOST_CHECK( take_e1000(ordered_ints)  == std::vector<int>({}) );
        BOOST_CHECK( take_lt1000(ordered_ints) == ordered_ints );
        BOOST_CHECK( take_e1(ordered_ints)     == std::vector<int>({1}) );
        BOOST_CHECK( take_e0(empty_int_vec)    == empty_int_vec );
        BOOST_CHECK( take_lt5(empty_int_vec)   == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(where_test)
    {
        auto where_not5 = where([](int i){return i != 5;});
        auto where_5 = where([](int i){return i == 5;});
        auto where_odd = where([](int i){return i % 2 != 0;});
        auto where_even = where([](int i){return i % 2 == 0;});
        auto where_lt5 = where([](int i){return i < 5;});
        auto where_len_lt4 = where([](std::string i){return i.size() < 4;});

        BOOST_CHECK( where_not5(int_vec)       == std::vector<int>() );
        BOOST_CHECK( where_not5(ordered_ints)  == std::vector<int>({1, 2, 3, 4, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( where_5(int_vec)          == int_vec );
        BOOST_CHECK( where_5(ordered_ints)     == std::vector<int>({5}) );
        BOOST_CHECK( where_odd(int_vec)        == int_vec );
        BOOST_CHECK( where_odd(ordered_ints)   == std::vector<int>({1, 3, 5, 7, 9}) );
        BOOST_CHECK( where_even(int_vec)       == std::vector<int>() );
        BOOST_CHECK( where_even(ordered_ints)  == std::vector<int>({2, 4, 6, 8, 10}) );
        BOOST_CHECK( where_lt5(int_vec)        == std::vector<int>() );
        BOOST_CHECK( where_lt5(ordered_ints)   == std::vector<int>({1, 2, 3, 4}) );
        BOOST_CHECK( where_len_lt4(string_vec) == std::vector<std::string>({"s1 "}) );
        BOOST_CHECK( where_lt5(empty_int_vec)  == empty_int_vec );

    }
BOOST_AUTO_TEST_SUITE_END() //filter_tests

BOOST_FIXTURE_TEST_SUITE(generator_tests, test_fixture)
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
        BOOST_CHECK( compare_range(range(1.0f, -1.0f, 0.33f),  std::vector<float>({1.0f, 0.66f, 0.33f, 0.00f, -0.33f, -0.66f, -0.99f}), 0.01f) );
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
        BOOST_CHECK( compare_range(range(1.0, -1.0, 0.33),  std::vector<double>({1.0, 0.66, 0.33, 0.00, -0.33, -0.66, -0.99}), 0.01) );
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
BOOST_AUTO_TEST_SUITE_END() //generator_tests

BOOST_FIXTURE_TEST_SUITE(misc_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(map_test)
    {
        auto map_str = map([](int i){return std::string("n", i);});
        auto map_square = map([](int i){return i * i;});
        auto map_not = map([](bool i){return !i;});
        auto map_vec = map([](int i){std::vector<int> vec; vec.push_back(i); return vec;});

        std::vector<std::string> map_str_vec;
        for(unsigned int i = 1; i <= 10; ++i)
        {
            map_str_vec.push_back(std::string("n", i));
        }

        std::vector<std::vector<int>> map_vec_int;
        for(unsigned int i = 1; i <= 10; ++i)
        {
            std::vector<int> vec;
            vec.push_back(i);
            map_vec_int.push_back(vec);
        }

        BOOST_CHECK( map_str(ordered_ints)     == map_str_vec );
        BOOST_CHECK( map_square(int_vec)       == std::vector<int>(5, 25) );
        BOOST_CHECK( map_not(bool_vec)         == std::vector<bool>(5, false) );
        BOOST_CHECK( map_square(empty_int_vec) == empty_int_vec );
        BOOST_CHECK( map_vec(ordered_ints)     == map_vec_int );
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
        BOOST_CHECK( tl_obj(empty_int_vec)  == std::list<int>() );
    }

    BOOST_AUTO_TEST_CASE(to_map_test)
    {
        auto tm_obj = to_map();
        std::vector<std::pair<std::string, int>> pair_vec = {std::pair<std::string, int>("one", 1), std::pair<std::string, int>("two", 2), std::pair<std::string, int>("three", 3)};
        std::map<std::string, int> num_map;
        num_map["one"] = 1;
        num_map["two"] = 2;
        num_map["three"] = 3;

        BOOST_CHECK( tm_obj(pair_vec) == num_map );

        num_map["four"] = 4;

        BOOST_CHECK( tm_obj(pair_vec) != num_map );

        pair_vec.push_back(std::pair<std::string, int>("four", 4));
        pair_vec.push_back(std::pair<std::string, int>("two", 2));

        BOOST_CHECK( tm_obj(pair_vec) == num_map );

        std::vector<std::pair<std::string, int>> empty_pair_vec;
        std::map<std::string, int> empty_num_map;

        BOOST_CHECK( tm_obj(empty_pair_vec) == empty_num_map );
    }

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
        BOOST_CHECK( tv_obj(empty_int_vec)  == std::vector<int>() );
    }

    BOOST_AUTO_TEST_CASE(unique_test)
    {
        auto unique_obj = unique();
        BOOST_CHECK( unique_obj(int_vec)        == std::vector<int>({5}) );
        BOOST_CHECK( unique_obj(float_vec)      == std::vector<float>({5.1f}) );
        BOOST_CHECK( unique_obj(char_vec)       == std::vector<char>({'a'}) );
        BOOST_CHECK( unique_obj(bool_vec)       == std::vector<bool>({true}) );
        BOOST_CHECK( unique_obj(string_vec)     == string_vec );
        BOOST_CHECK( unique_obj(ordered_ints)   == ordered_ints );
        BOOST_CHECK( unique_obj(unordered_ints) == unordered_ints );
        BOOST_CHECK( unique_obj(empty_int_vec)  == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(zip_test)
    {
        std::vector<std::pair<int, char>> int_char_zip;
        std::vector<std::pair<float, char>> float_char_zip;
        std::vector<std::pair<std::string, char>> str_char_zip;
        std::vector<std::pair<int, char>> empty_char_zip;
        for(unsigned int i = 0; i < 5; ++i)
        {
            int_char_zip.push_back(std::make_pair(5, 'a'));
            float_char_zip.push_back(std::make_pair(5.1f, 'a'));
            str_char_zip.push_back(std::make_pair(string_vec[i], 'a'));
        }

        auto zip_obj = zip(char_vec);
        BOOST_CHECK( zip_obj(int_vec)       == int_char_zip );
        BOOST_CHECK( zip_obj(float_vec)     == float_char_zip );
        BOOST_CHECK( zip_obj(string_vec)    == str_char_zip );
        BOOST_CHECK( zip_obj(empty_int_vec) == empty_char_zip ) ;
    }
BOOST_AUTO_TEST_SUITE_END() //misc_tests

BOOST_FIXTURE_TEST_SUITE(num_reduction_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(avg_test)
    {
        auto avg_obj = avg();
        BOOST_CHECK( comp_real(avg_obj(ordered_ints),                  5.5, std::numeric_limits<double>::epsilon()) );
        BOOST_CHECK( comp_real(avg_obj(unordered_ints),                5.5, std::numeric_limits<double>::epsilon()) );
        BOOST_CHECK( comp_real(avg_obj(int_vec),                       5.0, std::numeric_limits<double>::epsilon()) );
        BOOST_CHECK( comp_real(static_cast<float>(avg_obj(float_vec)), 5.1f, std::numeric_limits<float>::epsilon()) );
        BOOST_CHECK_THROW( avg_obj(empty_int_vec), std::range_error );
    }

    BOOST_AUTO_TEST_CASE(count_test)
    {
        auto count_5 = count(5);
        auto count_0 = count(0);
        auto count_1000 = count(1000);
        auto count_a = count('a');
        auto count_str = count(std::string("s1 "));

        BOOST_CHECK( count_5(int_vec)       == 5 );
        BOOST_CHECK( count_0(int_vec)       == 0 );
        BOOST_CHECK( count_1000(int_vec)    == 0 );
        BOOST_CHECK( count_a(char_vec)      == 5 );
        BOOST_CHECK( count_str(string_vec)  == 1 );
        BOOST_CHECK( count_0(empty_int_vec) == 0 );
        BOOST_CHECK( count_5(empty_int_vec) == 0 );
    }

    BOOST_AUTO_TEST_CASE(fold_test)
    {
        auto fold_plus_5 = fold(std::plus<int>(), 5);
        auto fold_plus_0 = fold(std::plus<int>(), 0);
        auto fold_minus_5 = fold(std::minus<int>(), 5);
        auto fold_minus_0 = fold(std::minus<int>(), 0);
        auto fold_str = fold([](std::string a, std::string b){return a + b;}, std::string(""));

        BOOST_CHECK( fold_plus_5(ordered_ints)  == 60 );
        BOOST_CHECK( fold_plus_0(ordered_ints)  == 55 );
        BOOST_CHECK( fold_minus_5(ordered_ints) == -50 );
        BOOST_CHECK( fold_minus_0(ordered_ints) == -55 );
        BOOST_CHECK( fold_str(string_vec)       == std::string("s1 s2  s3   s4    s5     ") );
        BOOST_CHECK( fold_plus_5(empty_int_vec) == 5 );
    }

    BOOST_AUTO_TEST_CASE(max_test)
    {
        auto max_obj = max();
        BOOST_CHECK( max_obj(int_vec)        == 5 );
        BOOST_CHECK( max_obj(float_vec)      == 5.1f );
        BOOST_CHECK( max_obj(char_vec)       == 'a' );
        BOOST_CHECK( max_obj(bool_vec)       == true );
        BOOST_CHECK( max_obj(string_vec)     == std::string("s5     ") );
        BOOST_CHECK( max_obj(ordered_ints)   == 10 );
        BOOST_CHECK( max_obj(unordered_ints) == 10 );
        BOOST_CHECK_THROW( max_obj(empty_int_vec), std::range_error );
    }

    BOOST_AUTO_TEST_CASE(min_test)
    {
        auto min_obj = min();
        BOOST_CHECK( min_obj(int_vec)        == 5 );
        BOOST_CHECK( min_obj(float_vec)      == 5.1f );
        BOOST_CHECK( min_obj(char_vec)       == 'a' );
        BOOST_CHECK( min_obj(bool_vec)       == true );
        BOOST_CHECK( min_obj(string_vec)     == std::string("s1 ") );
        BOOST_CHECK( min_obj(ordered_ints)   == 1 );
        BOOST_CHECK( min_obj(unordered_ints) == 1 );
        BOOST_CHECK_THROW( min_obj(empty_int_vec), std::range_error );
    }

    BOOST_AUTO_TEST_CASE(size_test)
    {
        auto size_obj = size();
        BOOST_CHECK( size_obj(int_vec)        == 5 );
        BOOST_CHECK( size_obj(float_vec)      == 5 );
        BOOST_CHECK( size_obj(char_vec)       == 5 );
        BOOST_CHECK( size_obj(bool_vec)       == 5 );
        BOOST_CHECK( size_obj(string_vec)     == 5 );
        BOOST_CHECK( size_obj(ordered_ints)   == 10 );
        BOOST_CHECK( size_obj(unordered_ints) == 10 );
        BOOST_CHECK( size_obj(empty_int_vec)  == 0 );
    }

    BOOST_AUTO_TEST_CASE(sum_test)
    {
        auto sum_obj = sum();
        BOOST_CHECK( sum_obj(int_vec)        == 25 );
        BOOST_CHECK( sum_obj(bool_vec)       == 1  );
        BOOST_CHECK( sum_obj(string_vec)     == "s1 s2  s3   s4    s5     " );
        BOOST_CHECK( sum_obj(ordered_ints)   == 55 );
        BOOST_CHECK( sum_obj(unordered_ints) == 55 );
        BOOST_CHECK( sum_obj(empty_int_vec)  == 0 );
    }
BOOST_AUTO_TEST_SUITE_END() //reduction_tests

BOOST_FIXTURE_TEST_SUITE(order_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(reverse_test)
    {
        auto reverse_obj = reverse();
        BOOST_CHECK( reverse_obj(int_vec)       == int_vec );
        BOOST_CHECK( reverse_obj(float_vec)     == float_vec );
        BOOST_CHECK( reverse_obj(bool_vec)      == bool_vec );
        BOOST_CHECK( reverse_obj(string_vec)    == std::vector<std::string>({"s5     ", "s4    ", "s3   ", "s2  ", "s1 "}) );
        BOOST_CHECK( reverse_obj(ordered_ints)  == std::vector<int>({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}) );
        BOOST_CHECK( reverse_obj(empty_int_vec) == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(sort_test)
    {
        auto sort_obj = sort();
        auto reverse_obj = reverse();
        BOOST_CHECK( sort_obj(int_vec)                   == int_vec );
        BOOST_CHECK( sort_obj(float_vec)                 == float_vec );
        BOOST_CHECK( sort_obj(bool_vec)                  == bool_vec );
        BOOST_CHECK( sort_obj(string_vec)                == string_vec );
        BOOST_CHECK( sort_obj(ordered_ints)              == ordered_ints );
        BOOST_CHECK( sort_obj(unordered_ints)            == ordered_ints );
        BOOST_CHECK( sort_obj(empty_int_vec)             == empty_int_vec );
        BOOST_CHECK( sort_obj(reverse_obj(ordered_ints)) == ordered_ints );
    }

    BOOST_AUTO_TEST_CASE(sort_with_test)
    {
        auto sort_with_int_comp = sort_with([](int i, int j){return i < j;});
        auto sort_with_str_comp = sort_with([](std::string i, std::string j){return i.size() < j.size();});
        auto reverse_obj = reverse();
        BOOST_CHECK( sort_with_int_comp(int_vec)                   == int_vec );
        BOOST_CHECK( sort_with_int_comp(ordered_ints)              == ordered_ints );
        BOOST_CHECK( sort_with_int_comp(unordered_ints)            == ordered_ints );
        BOOST_CHECK( sort_with_int_comp(empty_int_vec)             == empty_int_vec );
        BOOST_CHECK( sort_with_int_comp(reverse_obj(ordered_ints)) == ordered_ints );
        BOOST_CHECK( sort_with_str_comp(string_vec)                == string_vec );
        BOOST_CHECK( sort_with_str_comp(reverse_obj(string_vec))   == string_vec );
    }
BOOST_AUTO_TEST_SUITE_END() //order tests

BOOST_FIXTURE_TEST_SUITE(set_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(difference_test)
    {
        auto difference_obj = difference(int_vec);
        BOOST_CHECK( difference_obj(int_vec) == empty_int_vec );
        BOOST_CHECK( difference_obj(empty_int_vec) == empty_int_vec );
        BOOST_CHECK( difference_obj(ordered_ints) == std::vector<int>({1, 2, 3, 4, 6, 7, 8, 9, 10}) );
    }

    BOOST_AUTO_TEST_CASE(intersect_test)
    {
        auto intersect_obj = intersect(int_vec);
        BOOST_CHECK( intersect_obj(int_vec) == int_vec );
        BOOST_CHECK( intersect_obj(ordered_ints) == std::vector<int>(1, 5) );
        BOOST_CHECK( intersect_obj(empty_int_vec) == empty_int_vec );
    }

    BOOST_AUTO_TEST_CASE(join_test)
    {
        auto join_obj = join(std::vector<int>({4, 4}));
        BOOST_CHECK( join_obj(int_vec)       == std::vector<int>({5, 5, 5, 5, 5, 4, 4}) );
        BOOST_CHECK( join_obj(ordered_ints)  == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 4, 4}) );
        BOOST_CHECK( join_obj(empty_int_vec) == std::vector<int>({4, 4}) );
    }
BOOST_AUTO_TEST_SUITE_END() //set tests

BOOST_FIXTURE_TEST_SUITE(stp_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(stp_level1_test)
    {
        auto all_5 = make_stp(all([](const int &i){return i == 5;}));
        auto any_6 = make_stp(any([](const int &i){return i == 6;}));
        auto contains_5 = make_stp(contains(5));
        auto equal_oi = make_stp(equal(ordered_ints));

        BOOST_CHECK( all_5(ordered_ints)                   == false );
        BOOST_CHECK( all_5(int_vec)                        == true );
        BOOST_CHECK( all_5(empty_int_vec)                  == true );
        BOOST_CHECK( any_6(ordered_ints)                   == true );
        BOOST_CHECK( any_6(int_vec)                        == false );
        BOOST_CHECK( any_6(empty_int_vec)                  == false );
        BOOST_CHECK( contains_5(int_vec)                   == true );
        BOOST_CHECK( contains_5(empty_int_vec)             == false );
        BOOST_CHECK( equal_oi(ordered_ints)                == true );
        BOOST_CHECK( equal_oi(std::vector<int>({1, 2, 3})) == false );

        auto drop_5 = make_stp(drop(5));
        auto drop_lt5 = make_stp(drop_while([](const int &i){return i < 5;}));
        auto take_5 = make_stp(take(5));
        auto take_lt5 = make_stp(take_while([](const int &i){return i < 5;}));
        auto where_lt5 = make_stp(where([](int i){return i < 5;}));
        auto where_not5 = make_stp(where([](int i){return i != 5;}));

        BOOST_CHECK( drop_5(ordered_ints)     == std::vector<int>({6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_5(int_vec)          == empty_int_vec );
        BOOST_CHECK( drop_5(empty_int_vec)    == empty_int_vec );
        BOOST_CHECK( drop_lt5(ordered_ints)   == std::vector<int>({5, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_lt5(empty_int_vec)  == empty_int_vec );
        BOOST_CHECK( drop_lt5(int_vec)        == int_vec );
        BOOST_CHECK( take_5(ordered_ints)     == std::vector<int>({1, 2, 3, 4, 5}) );
        BOOST_CHECK( take_5(int_vec)          == int_vec );
        BOOST_CHECK( take_5(empty_int_vec)    == empty_int_vec );
        BOOST_CHECK( take_lt5(ordered_ints)   == std::vector<int>({1, 2, 3, 4}) );
        BOOST_CHECK( take_lt5(empty_int_vec)  == empty_int_vec );
        BOOST_CHECK( where_lt5(int_vec)       == empty_int_vec );
        BOOST_CHECK( where_lt5(ordered_ints)  == std::vector<int>({1, 2, 3, 4}) );
        BOOST_CHECK( where_lt5(empty_int_vec) == empty_int_vec );
        BOOST_CHECK( where_not5(int_vec)      == empty_int_vec );
        BOOST_CHECK( where_not5(ordered_ints) == std::vector<int>({1, 2, 3, 4, 6, 7, 8, 9, 10}) );

        auto map_square = make_stp(map([](int i){return i * i;}));
        auto tl_obj = make_stp(to_list());
        auto tm_obj = make_stp(to_map());
        auto tv_obj = make_stp(to_vector());
        auto unique_obj = make_stp(unique());
        auto zip_obj = make_stp(zip(char_vec));

        std::vector<std::pair<int, char>> int_char_zip;
        std::vector<std::pair<float, char>> float_char_zip;
        std::vector<std::pair<std::string, char>> str_char_zip;
        std::vector<std::pair<int, char>> empty_char_zip;
        for(unsigned int i = 0; i < 5; ++i)
        {
            int_char_zip.push_back(std::make_pair(5, 'a'));
            float_char_zip.push_back(std::make_pair(5.1f, 'a'));
            str_char_zip.push_back(std::make_pair(string_vec[i], 'a'));
        }
        std::vector<std::pair<std::string, int>> pair_vec = {std::pair<std::string, int>("one", 1), std::pair<std::string, int>("two", 2), std::pair<std::string, int>("three", 3)};
        std::map<std::string, int> num_map;
        num_map["one"] = 1;
        num_map["two"] = 2;
        num_map["three"] = 3;
        std::vector<std::pair<std::string, int>> empty_pair_vec;
        std::map<std::string, int> empty_num_map;

        BOOST_CHECK( map_square(int_vec)        == std::vector<int>(5, 25) );
        BOOST_CHECK( map_square(empty_int_vec)  == empty_int_vec );
        BOOST_CHECK( tl_obj(unordered_ints)     == std::list<int>(unordered_ints.begin(), unordered_ints.end()) );
        BOOST_CHECK( tl_obj(empty_int_vec)      == std::list<int>() );
        BOOST_CHECK( tm_obj(pair_vec)           == num_map );
        BOOST_CHECK( tm_obj(empty_pair_vec)     == empty_num_map );
        BOOST_CHECK( tv_obj(unordered_ints)     == unordered_ints );
        BOOST_CHECK( tv_obj(empty_int_vec)      == std::vector<int>() );
        BOOST_CHECK( unique_obj(unordered_ints) == unordered_ints );
        BOOST_CHECK( unique_obj(empty_int_vec)  == empty_int_vec );
        BOOST_CHECK( zip_obj(int_vec)           == int_char_zip );
        BOOST_CHECK( zip_obj(float_vec)         == float_char_zip );
        BOOST_CHECK( zip_obj(string_vec)        == str_char_zip );
        BOOST_CHECK( zip_obj(empty_int_vec)     == empty_char_zip ) ;

        auto avg_obj = make_stp(avg());
        auto count_5 = make_stp(count(5));
        auto count_0 = make_stp(count(0));
        auto fold_plus_5 = make_stp(fold(std::plus<int>(), 5));
        auto fold_plus_0 = make_stp(fold(std::plus<int>(), 0));
        auto max_obj = make_stp(max());
        auto min_obj = make_stp(min());
        auto size_obj = make_stp(size());
        auto sum_obj = make_stp(sum());

        BOOST_CHECK( comp_real(avg_obj(unordered_ints), 5.5, std::numeric_limits<double>::epsilon()) );
        BOOST_CHECK( comp_real(avg_obj(int_vec),        5.0, std::numeric_limits<double>::epsilon()) );
        BOOST_CHECK( count_5(int_vec)           == 5 );
        BOOST_CHECK( count_5(empty_int_vec)     == 0 );
        BOOST_CHECK( count_0(int_vec)           == 0 );
        BOOST_CHECK( count_0(empty_int_vec)     == 0 );
        BOOST_CHECK( fold_plus_5(ordered_ints)  == 60 );
        BOOST_CHECK( fold_plus_5(empty_int_vec) == 5 );
        BOOST_CHECK( fold_plus_0(ordered_ints)  == 55 );
        BOOST_CHECK( max_obj(string_vec)        == std::string("s5     ") );
        BOOST_CHECK( max_obj(ordered_ints)      == 10 );
        BOOST_CHECK( max_obj(unordered_ints)    == 10 );
        BOOST_CHECK( min_obj(string_vec)        == std::string("s1 ") );
        BOOST_CHECK( min_obj(ordered_ints)      == 1 );
        BOOST_CHECK( min_obj(unordered_ints)    == 1 );
        BOOST_CHECK( size_obj(string_vec)       == 5 );
        BOOST_CHECK( size_obj(ordered_ints)     == 10 );
        BOOST_CHECK( size_obj(unordered_ints)   == 10 );
        BOOST_CHECK( sum_obj(int_vec)           == 25 );
        BOOST_CHECK( sum_obj(bool_vec)          == 1  );
        BOOST_CHECK( sum_obj(string_vec)        == "s1 s2  s3   s4    s5     " );

        auto reverse_obj = make_stp(reverse());
        auto sort_obj = make_stp(sort());
        auto sort_with_int_comp = make_stp(sort_with([](int i, int j){return i < j;}));

        BOOST_CHECK( reverse_obj(bool_vec)                         == bool_vec );
        BOOST_CHECK( reverse_obj(string_vec)                       == std::vector<std::string>({"s5     ", "s4    ", "s3   ", "s2  ", "s1 "}) );
        BOOST_CHECK( reverse_obj(ordered_ints)                     == std::vector<int>({10, 9, 8, 7, 6, 5, 4, 3, 2, 1}) );
        BOOST_CHECK( sort_obj(unordered_ints)                      == ordered_ints );
        BOOST_CHECK( sort_obj(empty_int_vec)                       == empty_int_vec );
        BOOST_CHECK( sort_obj(reverse_obj(ordered_ints))           == ordered_ints );
        BOOST_CHECK( sort_with_int_comp(ordered_ints)              == ordered_ints );
        BOOST_CHECK( sort_with_int_comp(unordered_ints)            == ordered_ints );
        BOOST_CHECK( sort_with_int_comp(empty_int_vec)             == empty_int_vec );
        BOOST_CHECK( sort_with_int_comp(reverse_obj(ordered_ints)) == ordered_ints );

        auto difference_obj = make_stp(difference(int_vec));
        auto intersect_obj = make_stp(intersect(int_vec));
        auto join_obj = make_stp(join(std::vector<int>({4, 4})));

        BOOST_CHECK( difference_obj(int_vec)       == empty_int_vec );
        BOOST_CHECK( difference_obj(empty_int_vec) == empty_int_vec );
        BOOST_CHECK( difference_obj(ordered_ints)  == std::vector<int>({1, 2, 3, 4, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( intersect_obj(int_vec)        == int_vec );
        BOOST_CHECK( intersect_obj(ordered_ints)   == std::vector<int>(1, 5) );
        BOOST_CHECK( intersect_obj(empty_int_vec)  == empty_int_vec );
        BOOST_CHECK( join_obj(int_vec)             == std::vector<int>({5, 5, 5, 5, 5, 4, 4}) );
        BOOST_CHECK( join_obj(ordered_ints)        == std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 4, 4}) );
        BOOST_CHECK( join_obj(empty_int_vec)       == std::vector<int>({4, 4}) );
    }

    BOOST_AUTO_TEST_CASE(stp_level2_test)
    {
        auto even_sum = make_stp(where([](const int &i){return i % 2 == 0;}), sum());
        auto odd_avg = make_stp(where([](const int &i){return i % 2 != 0;}), avg());
        auto drop_count = make_stp(drop(5), count(3));
        auto take_size = make_stp(take(4), size());
        auto map_min = make_stp(map([](const int &i){return i*i;}), min());
        auto unique_max = make_stp(unique(), max());
        auto reverse_fold = make_stp(reverse(), fold(std::plus<int>(), 5));
        auto sort_equal = make_stp(sort(), equal(ordered_ints));
        auto difference_contains = make_stp(difference(int_vec), contains(5));
        auto intersect_all = make_stp(intersect(int_vec), all([](const int &i){return i == 5;}));
        auto join_any = make_stp(join(std::vector<int>({11, 11})), any([](const int &i){return i == 11;}));
        auto join_tolist = make_stp(join(std::vector<int>({4, 4})), to_list());
        auto join_tovector = make_stp(join(std::vector<int>({4, 4})), to_vector());
        auto zip_map = make_stp(zip(range(10, 0)), map([](const std::pair<int, int> &i){return i.first + i.second;}));
        auto zip_to_map = make_stp(zip(range(10, 0)), to_map());

        std::map<int, int> int_map;
        int_map[0] = 10;
        int_map[1] = 9;
        int_map[2] = 8;
        int_map[3] = 7;
        int_map[4] = 6;
        int_map[5] = 5;
        int_map[6] = 4;
        int_map[7] = 3;
        int_map[8] = 2;
        int_map[9] = 1;

        BOOST_CHECK( even_sum(ordered_ints)                     == 30 );
        BOOST_CHECK( drop_count(ordered_ints)                   == 0 );
        BOOST_CHECK( take_size(ordered_ints)                    == 4 );
        BOOST_CHECK( map_min(int_vec)                           == 25 );
        BOOST_CHECK( unique_max(std::vector<int>({3, 4, 5, 5})) == 5 );
        BOOST_CHECK( reverse_fold(ordered_ints)                 == 60 );
        BOOST_CHECK( join_tolist(empty_int_vec)                 == std::list<int>({4, 4}) );
        BOOST_CHECK( join_tovector(empty_int_vec)               == std::vector<int>({4, 4}) );
        BOOST_CHECK( zip_map(ordered_ints)                      == std::vector<int>({11, 11, 11, 11, 11, 11, 11, 11, 11, 11}) );
        BOOST_CHECK( zip_to_map(range(0, 10))                   == int_map );

        BOOST_CHECK( sort_equal(unordered_ints) );
        BOOST_CHECK( intersect_all(ordered_ints) );
        BOOST_CHECK( !difference_contains(ordered_ints) );
        BOOST_CHECK( join_any(ordered_ints) );
        BOOST_CHECK( comp_real(odd_avg(ordered_ints), 5.0, std::numeric_limits<double>::epsilon()) );
    }

    BOOST_AUTO_TEST_CASE(stp_level3_test)
    {
        auto take_take = make_stp(take(8), take(2), take(1), sum());
        BOOST_CHECK( take_take(ordered_ints) == 1 );

        auto zip_fib = make_stp(where([](const int &i){return i % 2 == 0;}), map([](const int &i){return i * i;}), zip(range(1, 7)));
        std::vector<int> fib_vec({0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89});
        std::vector<std::pair<int, int>> zip_fib_result({std::make_pair(0,1), std::make_pair(4,2), std::make_pair(64,3), std::make_pair(1156,4)});
        BOOST_CHECK( zip_fib(fib_vec) == zip_fib_result );

        auto zip_map_sum = make_stp(zip(range(10, 0)), map([](const std::pair<int, int> &i){return i.first + i.second;}), sum());
        BOOST_CHECK( zip_map_sum(ordered_ints) == 110 );
    }
BOOST_AUTO_TEST_SUITE_END() //stp tests