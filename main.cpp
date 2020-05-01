#include "include/bstring.hpp"
#include "include/bstring_utils.hpp"
#include <iostream>

int main(int argc, const char** argv)
{
    botched_utils::BString test = "Country_roads_take__me_home_to_a_place___I__be_lo_";

    auto stuff = botched_utils::split(test,'_');
   
    botched_utils::BStringSplit dereferened = *stuff;
    for (int i = 0; i < stuff->split_count(); i++)
    {
         std::cout << (*stuff)[i] << '\n';
    }
    
    return 0;
}