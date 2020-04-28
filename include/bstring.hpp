#ifndef __BSTRING_HPP
#define __BSTRING_HPP

#ifdef _MSC_VER
// disable microsoft compiler warnings
#ifndef _CRT_SECURE_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS
#endif //_CRT_SECURE_NO_WARNINGS
// uncomment to disable pragma warnings
// #pragma warning( disable : 4068 )
// standard-ish function missing from MS Library

#include <cstdarg>
int vasprintf(char** ret, const char * format, va_list ap);
#endif // _MSC_VER

#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cctype>
#include <memory>

namespace botched_utils 
{
    constexpr const char * _bons_str_version = "0.0.1";
    constexpr size_t _bons_str_max_len = 65535;
    constexpr size_t _bons_str_max_split = 1023;
    
    class BString
    {
        char * _str = nullptr;
        size_t _str_len = 0;

        // here he has a mutable unique ptr to an array of shared pointers of string objects
        // lets make it with raw pointers.
        // maybe consider whether or not we should store the split in the BonsString or whether
        // they simply should be move the receiver 
        typedef std::shared_ptr<BString> _bsp;
        // unique ptr is the only one that allows for storing arrays
        typedef std::unique_ptr<_bsp[]> _split_ptr;
        // these are mutable to make sure string split methods can operator on const level
        mutable _split_ptr _split_array;
        mutable size_t _split_count = 0;

        void _reset_split_array() const;
        void _append_split_array(const BString & str) const;
        void swap(BString & other);
        const char *  copy_str(const char * str);
        const char * alloc_str(size_t size);
    public: 
        typedef _split_ptr split_ptr;

        BString();
        BString(const char * str);
        BString(const BString & other); //copy constructor
        BString(BString && other) noexcept; // move constructor
        ~BString();
        void reset();

        operator const  char * () const;

        // assingment operators
        BString & operator = ( BString other); // copy-and-swap assignement
        BString & operator += (const char * rhs);
        BString & operator += (const BString & rhs);
        const char operator[] (const int index) const;

        //comparison operators

        bool operator == (const BString & rhs) const;
        bool operator!= (const BString & rhs) const;
        bool operator > (const BString & rhs) const;
        bool operator >= (const BString & rhs) const;
        bool operator < (const BString & rhs) const;
        bool operator <= (const BString & rhs) const;

        // utility methods
        static const char * version() { return _bons_str_version;}
        bool empty() const;
        size_t length() const { return _str_len;}
        size_t size() const { return _str_len;}
        BString & format( const char * format, ...);
        BString & trim();
        BString lower() const;
        BString upper() const;
        const char & back() const;
        const char & front() const;

        // find and replace
        long int char_find(const char & match) const;
        const BString & char_replace(const char & match, const char & replace);
        BString sub_string(size_t start, size_t length);
        long find (const BString & match) const;
        const BString replace_first(const BString & match, const BString & replace);
        const BString replace_count(const BString & match, const BString & replace, size_t count);
        const BString replace_all(const BString & match, const BString & replace);

        // split methods 
        const split_ptr & split(const char * match) const;
        const split_ptr & split(const char match) const;
        const split_ptr & split(const char * match, int max_split) const;
        const BString & split_item(size_t index) const;
        size_t split_count() const { return _split_count;}
    };
    // non-memember operator overload for appending
    BString operator+ (const BString & lhs, const BString & rhs);
}
#endif //__BONS_STR_HPP