#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>

#include <boost/test/minimal.hpp>

#include "stp.hpp"

int test_main(int, char *[])
{
    using namespace stp;

    int arr[] = {1, 2, 3, 4, 5};
    std::vector<int> int_vec(5, 5);
    std::vector<float> float_vec(5, 5.1f);
    std::vector<char> char_vec(5, 'a');
    std::vector<bool> bool_vec(5, true);
    std::vector<std::string> string_vec({"s1 ", "s2  ", "s3   ", "s4    "});

    std::vector<int> ordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    std::vector<int> unordered_ints({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(unordered_ints.begin(), unordered_ints.end(), std::default_random_engine(seed));

    //-----BEGIN SEQUENCE SOURCES-----//
    BOOST_CHECK( Transform(FromArray(arr), ToVector()) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(FromContainer(int_vec), ToVector()) == int_vec );
    BOOST_CHECK( Transform(FromIterators(int_vec.begin(), int_vec.end()), ToVector()) == int_vec );

    BOOST_CHECK( Transform(FromRange(1, 5), ToVector()) == std::vector<int>({1, 2, 3, 4}) );
    BOOST_CHECK( Transform(FromRange(5, 1), ToVector()) == std::vector<int>({5, 4, 3, 2}) );
    BOOST_CHECK( Transform(FromRange(-1, 1), ToVector()) == std::vector<int>({-1, 0}) );
    BOOST_CHECK( Transform(FromRange(1, -1), ToVector()) == std::vector<int>({1, 0}) );
    BOOST_CHECK( Transform(FromRange(1, 1), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(-1, -1), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(0, 0), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(0, 1), ToVector()) == std::vector<int>({0}) );
    BOOST_CHECK( Transform(FromRange(1, 0), ToVector()) == std::vector<int>({1}) );

    BOOST_CHECK( Transform(FromRange(1, 5, 2), ToVector()) == std::vector<int>({1, 3}) );
    BOOST_CHECK( Transform(FromRange(5, 1, 2), ToVector()) == std::vector<int>({5, 3}) );
    BOOST_CHECK( Transform(FromRange(1, 5, 0), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(1, 5, 6), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(1, 10, 3), ToVector()) == std::vector<int>({1, 4, 7}) );
    BOOST_CHECK( Transform(FromRange(10, 1, 3), ToVector()) == std::vector<int>({10, 7, 4}) );

    BOOST_CHECK( Transform(FromRange(-1, -5, 2), ToVector()) == std::vector<int>({-1, -3}) );
    BOOST_CHECK( Transform(FromRange(-5, -1, 2), ToVector()) == std::vector<int>({-5, -3}) );
    BOOST_CHECK( Transform(FromRange(-1, -5, 0), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(-1, -5, 6), ToVector()) == std::vector<int>() );
    BOOST_CHECK( Transform(FromRange(-1, -10, 3), ToVector()) == std::vector<int>({-1, -4, -7}) );
    BOOST_CHECK( Transform(FromRange(-10, -1, 3), ToVector()) == std::vector<int>({-10, -7, -4}) );
    //-----END SEQUENCE SOURCES-----//


    //-----BEGIN STL TRANSFORMATIONS-----//
    //Simple tovector
    BOOST_CHECK( Transform(FromContainer(int_vec), ToVector()) == int_vec );
    BOOST_CHECK( Transform(FromContainer(float_vec), ToVector()) == float_vec );
    BOOST_CHECK( Transform(FromContainer(char_vec), ToVector()) == char_vec );
    BOOST_CHECK( Transform(FromContainer(bool_vec), ToVector()) == bool_vec );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), ToVector()) == ordered_ints );
    BOOST_CHECK( Transform(FromContainer(string_vec), ToVector()) == string_vec );

    //Simple tolist
    BOOST_CHECK( Transform(FromContainer(int_vec), ToList()) == std::list<int>(5, 5) );
    BOOST_CHECK( Transform(FromContainer(float_vec), ToList()) == std::list<float>(5, 5.1f) );
    BOOST_CHECK( Transform(FromContainer(char_vec), ToList()) == std::list<char>(5, 'a') );
    BOOST_CHECK( Transform(FromContainer(bool_vec), ToList()) == std::list<bool>(5, true) );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), ToList()) == std::list<int>(ordered_ints.begin(), ordered_ints.end()) );
    BOOST_CHECK( Transform(FromContainer(string_vec), ToList()) == std::list<std::string>(string_vec.begin(), string_vec.end()) );

    //TODO: COMPOSITES

    //-----END STL TRANSFORMATIONS-----//

    //Simple sum
    BOOST_CHECK( Transform(FromContainer(float_vec), Sum()) == 25.5 );
    BOOST_CHECK( Transform(FromContainer(int_vec), Sum()) == 25 );
    BOOST_CHECK( Transform(FromContainer(bool_vec), Sum()) == 1 );
    BOOST_CHECK( Transform(FromContainer(string_vec), Sum()) == "s1 s2  s3   s4    " );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Sum()) == 66 );
    BOOST_CHECK( Transform(FromContainer(unordered_ints), Sum()) == 66 );

    //Simple take
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Take(5), ToVector()) == std::vector<int>({1, 2, 3, 4, 5}) );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Take(6), ToVector()) == std::vector<int>({1, 2, 3, 4, 5, 6}) );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Take(1000), ToVector()) == ordered_ints );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Take(0), ToVector()) == std::vector<int>({}) );

    //TODO: WHERE

    //Simple count
    BOOST_CHECK( Transform(FromContainer(float_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromContainer(int_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromContainer(bool_vec), Count()) == 5 );
    BOOST_CHECK( Transform(FromContainer(string_vec), Count()) == 4 );
    BOOST_CHECK( Transform(FromContainer(ordered_ints), Count()) == 11 );
    BOOST_CHECK( Transform(FromContainer(unordered_ints), Count()) == 11 );

    /*for(int i : Transform(FromRange(8, 1), ToVector()))
    {
        std::cout << i << std::endl;
    }*/

    return 0;
}

//Query(from_array({1, 2, 3}), take(2), sum());