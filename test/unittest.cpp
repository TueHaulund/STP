#define BOOST_TEST_MODULE STP_UT

#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <cmath>
#include <cctype>

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

BOOST_FIXTURE_TEST_SUITE(filter_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(drop_test)
    {
        auto drop_5 = drop(5);
        auto drop_6 = drop(6);
        auto drop_10 = drop(10);
        auto drop_1000 = drop(1000);
        auto drop_0 = drop(0);

        std::vector<std::vector<int>> input(5, ordered_ints);

        BOOST_CHECK( drop_5(input[0])    == std::vector<int>({6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_6(input[1])    == std::vector<int>({7, 8, 9, 10}) );
        BOOST_CHECK( drop_10(input[2])   == std::vector<int>({}) );
        BOOST_CHECK( drop_1000(input[3]) == std::vector<int>({}) );
        BOOST_CHECK( drop_0(input[4])    == ordered_ints );
    }

    BOOST_AUTO_TEST_CASE(drop_while_test)
    {
        auto drop_lt5 = drop_while([](const int &i){return i < 5;});
        auto drop_lte5 = drop_while([](const int &i){return i <= 5;});
        auto drop_e0 = drop_while([](const int &i){return i == 0;});
        auto drop_e1000 = drop_while([](const int &i){return i == 1000;});
        auto drop_lt1000 = drop_while([](const int &i){return i < 1000;});
        auto drop_e1 = drop_while([](const int &i){return i == 1;});

        std::vector<std::vector<int>> input(6, ordered_ints);

        BOOST_CHECK( drop_lt5(input[0])    == std::vector<int>({5, 6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_lte5(input[1])   == std::vector<int>({6, 7, 8, 9, 10}) );
        BOOST_CHECK( drop_e0(input[2])     == ordered_ints );
        BOOST_CHECK( drop_e1000(input[3])  == ordered_ints );
        BOOST_CHECK( drop_lt1000(input[4]) == std::vector<int>({}) );
        BOOST_CHECK( drop_e1(input[5])     == std::vector<int>({2, 3, 4, 5, 6, 7, 8, 9, 10}) );
    }

    BOOST_AUTO_TEST_CASE(take_test)
    {
        auto take_5 = take(5);
        auto take_6 = take(6);
        auto take_10 = take(10);
        auto take_1000 = take(1000);
        auto take_0 = take(0);

        std::vector<std::vector<int>> input(5, ordered_ints);

        BOOST_CHECK( take_5(input[0])    == std::vector<int>({1, 2, 3, 4, 5}) );
        BOOST_CHECK( take_6(input[1])    == std::vector<int>({1, 2, 3, 4, 5, 6}) );
        BOOST_CHECK( take_10(input[2])   == ordered_ints );
        BOOST_CHECK( take_1000(input[3]) == ordered_ints );
        BOOST_CHECK( take_0(input[4])    == std::vector<int>({}) );
    }

    BOOST_AUTO_TEST_CASE(take_while_test)
    {
        auto take_lt5 = take_while([](const int &i){return i < 5;});
        auto take_lte5 = take_while([](const int &i){return i <= 5;});
        auto take_e0 = take_while([](const int &i){return i == 0;});
        auto take_e1000 = take_while([](const int &i){return i == 1000;});
        auto take_lt1000 = take_while([](const int &i){return i < 1000;});
        auto take_e1 = take_while([](const int &i){return i == 1;});

        std::vector<std::vector<int>> input(6, ordered_ints);

        BOOST_CHECK( take_lt5(input[0])    == std::vector<int>({1, 2, 3, 4}) );
        BOOST_CHECK( take_lte5(input[1])   == std::vector<int>({1, 2, 3, 4, 5}) );
        BOOST_CHECK( take_e0(input[2])     == std::vector<int>({}) );
        BOOST_CHECK( take_e1000(input[3])  == std::vector<int>({}) );
        BOOST_CHECK( take_lt1000(input[4]) == ordered_ints );
        BOOST_CHECK( take_e1(input[5])     == std::vector<int>({1}) );
    }

    //DELETE ME
    template <typename T>
    struct tell;

    BOOST_AUTO_TEST_CASE(where_test)
    {
        auto where_stp = make_stp(where([](const int &i){return i % 2 == 0;}));
        BOOST_CHECK( where_stp(ordered_ints) == std::vector<int>({2, 4, 6, 8, 10}) );

        auto test = make_stp(where([](const int &i){return i % 2 == 0;}), take(2));
        test(ordered_ints);

        auto test2 = make_stp(take(8), take(2), take(1), sum());
        test2(ordered_ints);

        //TODO: MAKE ME
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
BOOST_AUTO_TEST_SUITE_END() //generator_tests

BOOST_FIXTURE_TEST_SUITE(reduction_tests, test_fixture)
    BOOST_AUTO_TEST_CASE(all_test)
    {
        auto all_5 = all([](const int &i){return i == 5;});
        auto all_false = all([](const bool &i){return i == false;});
        auto all_true = all([](const bool &i){return i == true;});
        auto all_alpha = all([](const char &i){return isalpha(i);});

        BOOST_CHECK( all_5(ordered_ints) == false );
        BOOST_CHECK( all_5(int_vec)      == true  );
        BOOST_CHECK( all_false(bool_vec) == false );
        BOOST_CHECK( all_true(bool_vec)  == true  );
        BOOST_CHECK( all_alpha(char_vec) == true );
    }

    BOOST_AUTO_TEST_CASE(any_test)
    {
        auto any_6 = any([](const int &i){return i == 6;});
        auto any_false = any([](const bool &i){return i == false;});
        auto any_digit = any([](const char &i){return isdigit(i);});

        BOOST_CHECK( any_6(ordered_ints) == true  );
        BOOST_CHECK( any_6(int_vec)      == false );
        BOOST_CHECK( any_false(bool_vec) == false );
        BOOST_CHECK( any_digit(char_vec) == false );
    }

    BOOST_AUTO_TEST_CASE(count_test)
    {
        auto count_5 = count(5);
        auto count_0 = count(0);
        auto count_1000 = count(1000);
        auto count_a = count('a');
        auto count_str = count(std::string("s1 "));

        BOOST_CHECK( count_5(int_vec)      == 5 );
        BOOST_CHECK( count_0(int_vec)      == 0 );
        BOOST_CHECK( count_1000(int_vec)   == 0 );
        BOOST_CHECK( count_a(char_vec)     == 5 );
        BOOST_CHECK( count_str(string_vec) == 1 );
    }

    BOOST_AUTO_TEST_CASE(equal_test)
    {
        auto equal_oi = equal(ordered_ints);
        auto equal_uoi = equal(unordered_ints);
        auto equal_empty = equal(std::vector<int>());
        auto equal_cv = equal(char_vec);

        BOOST_CHECK( equal_oi(ordered_ints)                       == true );
        BOOST_CHECK( equal_uoi(unordered_ints)                    == true );
        BOOST_CHECK( equal_oi(std::vector<int>({1, 2, 3}))        == false );
        BOOST_CHECK( equal_uoi(std::vector<int>({1, 2, 3}))       == false );
        BOOST_CHECK( equal_empty(std::vector<int>())              == true );
        BOOST_CHECK( equal_empty(std::vector<int>({1, 2, 3}))     == false );
        BOOST_CHECK( equal_cv(char_vec)                           == true );
        BOOST_CHECK( equal_cv(std::vector<char>({'a', 'b', 'c'})) == false );
        BOOST_CHECK( equal_cv(ordered_ints)                       == false );
    }

    BOOST_AUTO_TEST_CASE(foldl_test)
    {
        auto foldl_plus_5 = fold_left(std::plus<int>(), 5);
        auto foldl_plus_0 = fold_left(std::plus<int>(), 0);
        auto foldl_minus_5 = fold_left(std::minus<int>(), 5);
        auto foldl_minus_0 = fold_left(std::minus<int>(), 0);

        BOOST_CHECK( foldl_plus_5(ordered_ints) == 60 );
        BOOST_CHECK( foldl_plus_0(ordered_ints) == 55 );
        BOOST_CHECK( foldl_minus_5(ordered_ints) == -50 );
        BOOST_CHECK( foldl_minus_0(ordered_ints) == -55 );
    }

    BOOST_AUTO_TEST_CASE(foldr_test)
    {
        auto foldr_plus_5 = fold_right(std::plus<int>(), 5);
        auto foldr_plus_0 = fold_right(std::plus<int>(), 0);
        auto foldr_minus_5 = fold_right(std::minus<int>(), 5);
        auto foldr_minus_0 = fold_right(std::minus<int>(), 0);

        BOOST_CHECK( foldr_plus_5(ordered_ints) == 60 );
        BOOST_CHECK( foldr_plus_0(ordered_ints) == 55 );
        BOOST_CHECK( foldr_minus_5(ordered_ints) == 0 );
        BOOST_CHECK( foldr_minus_0(ordered_ints) == -5 );
    }

    BOOST_AUTO_TEST_CASE(size_test)
    {
        auto size_obj = size();
        BOOST_CHECK( size_obj(int_vec)        == 5 );
        BOOST_CHECK( size_obj(float_vec)      == 5 );
        BOOST_CHECK( size_obj(char_vec)       == 5 );
        BOOST_CHECK( size_obj(bool_vec)       == 5 );
        BOOST_CHECK( size_obj(string_vec)     == 4 );
        BOOST_CHECK( size_obj(ordered_ints)   == 10 );
        BOOST_CHECK( size_obj(unordered_ints) == 10 );
    }

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
BOOST_AUTO_TEST_SUITE_END() //reduction_tests

BOOST_FIXTURE_TEST_SUITE(stl_converter_tests, test_fixture)
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
BOOST_AUTO_TEST_SUITE_END() //stl_converter_tests