#include "include/bstring.hpp"
#include <iostream>

int main(int argc, const char** argv)
{
    botched_utils::BString test = "Country roads";
    std::cout << test.format("%s %s","one", "two") << '\n'; 
    return 0;
}