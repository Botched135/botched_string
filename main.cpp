#include "include/bstring.hpp"
#include "include/bstring_utils.hpp"

int main(int argc, const char** argv)
{
    botched::BString test = "Country_roads_take__me_home_to_a_place___I__be_lo_";
    botched::BString test2 = "a_a_a_a__c__s__a";
    botched::BString test3 = "ab";

    auto stuff = botched::split(test,'_');
    auto stuff2 = botched::split(test, "Contry");
    auto stuff3 = botched::split(test3,"bbb");
   
    printf("%s \n", stuff3[0].c_str());
    for (int i = 0; i < stuff2.split_count(); i++)
    {
         printf("%s \n",  stuff2[i].c_str());
    }

    botched::BString replaced = test2.replace_first("a", "b");
    printf("%s \n",replaced.c_str());

    replaced = test2.replace_count("a", "b",2);
    printf("%s \n",replaced.c_str());

    replaced = test2.replace_all("a", "b");
    printf("%s \n",replaced.c_str());
    return 0;
}