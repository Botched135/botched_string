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
         std::cout << dereferened[i] << '\n';
    }
    
    botched_utils::BStringSplit moved_stuff = std::move(*stuff);
    
    for (int i = 0; i < stuff->split_count(); i++)
    {
         std::cout << (*stuff)[i] << '\n';
    }
    delete stuff;
    for (int i = 0; i < moved_stuff.split_count(); i++)
    {
         std::cout << moved_stuff[i] << '\n';
    }
    return 0;
}