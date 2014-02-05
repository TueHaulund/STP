#include <iostream>
#include <vector>
#include <string>

#include "stp.hpp"

int main()
{
    using namespace stp;

    std::vector<int> intvec(5, 5);
    std::vector<float> floatvec(5, 5.1f);
    std::vector<char> charvec(5, 'a');
    std::vector<bool> boolvec(5, true);
    std::vector<int> numbers({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
    std::vector<std::string> stringvec({"hej", "lang", "langere", "langesteeee"});

    std::cout << Query(FromVector(floatvec), Sum()) << std::endl;
    std::cout << std::endl;
    std::cout << Query(FromVector(intvec), Sum()) << std::endl;
    std::cout << std::endl;
    std::cout << Query(FromVector(charvec), Sum()) << std::endl;
    std::cout << std::endl;

    for(char i : Query(FromVector(charvec), ToVector()))
    {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;

    std::cout << Query(FromVector(boolvec), Sum()) << std::endl;
    std::cout << std::endl;

    std::cout << Query(FromVector(numbers), Take(5), Sum()) << std::endl;
    std::cout << std::endl;

    for(int i : Query(FromVector(numbers), Take(5), ToList()))
    {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;

    for(int i : Query(FromVector(numbers), Take(7), Take(5), Take(6), ToList()))
    {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;

    std::cout << Query(FromVector(numbers), Where([](int i){return i < 8;}), Sum()) << std::endl;
    std::cout << std::endl;

    for(int i : Query(FromVector(numbers), Where([](int i){return i < 8;}), ToList()))
    {
        std::cout << i << std::endl;
    }
    std::cout << std::endl;

    std::cout << Query(FromVector(stringvec), Where([](std::string i) {return i.size() < 10;}), Sum()) << std::endl;
    std::cout << std::endl;

    std::cout << Query(FromVector(numbers), Take(4), Count()) << std::endl;
    std::cout << std::endl;

    return 0;
}

//Query(from_array({1, 2, 3}), take(2), sum());