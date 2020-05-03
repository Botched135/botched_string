#ifndef __BSTRING_UTILS_HPP
#define __BSTRING_UTILS_HPP
#include "bstring.hpp"

#define DEBUG
namespace botched_utils 
{

    constexpr size_t _bons_str_max_split = 1023;
    class BStringSplit
    {
        mutable BString** _split_array = nullptr;
        mutable size_t _split_count = 0;
        void _reset_split_array() const;

        void swap(BStringSplit& other);

        BStringSplit(){} // private constructor, so they cannot be created empty

        friend BStringSplit* split(const BString& src, const char * match, int max_split);
    public:
        BStringSplit(const BStringSplit &other); // COPY CTOR
        BStringSplit(BStringSplit && other) noexcept; // MOVE CTOR 
        ~BStringSplit();

        BStringSplit& operator = (BStringSplit other); // copy-and-swap
        BString& operator[](int);

        size_t split_count() const { return _split_count;}
    };

    // split methods 
    BStringSplit* split(const BString& src, const char * match);
    BStringSplit* split(const BString& src, const char match);
    BStringSplit* split(const BString& src, const char * match, int max_split);
}
#endif // __BSTRING_UTILS_HPP