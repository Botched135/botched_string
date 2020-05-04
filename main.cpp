#include "include/bstring.hpp"
#include "include/bstring_utils.hpp"
#include <iostream>

int main(int argc, const char** argv)
{
    botched_utils::BString test = "Country_roads_take__me_home_to_a_place___I__be_lo_";
    botched_utils::BString test2 = "a_a_a_a__c__s__a";


    auto stuff = botched_utils::split(test,'_');
    auto stuff2 = botched_utils::split(test, "Contry");


   
    for (int i = 0; i < stuff2.split_count(); i++)
    {
         std::cout << stuff2[i] << '\n';
    }

    botched_utils::BString replaced = test2.replace_first("a", "b");
    std::cout << replaced << '\n';

    replaced = test2.replace_count("a", "b",2);
    std::cout << replaced << '\n';

    replaced = test2.replace_all("a", "b");
    std::cout << replaced << '\n';
    return 0;
}