#include "../include/bstring_utils.hpp"

namespace botched_utils 
{

    BStringSplit::BStringSplit(const BStringSplit &other)
    {
        #ifdef DEBUG
        puts("Calling copy ctor");
        #endif 
        size_t split_count = other._split_count;
        if(split_count > 0)
        {
            _split_array = new BString*[split_count];
            memcpy(_split_array, other._split_array, sizeof(BString*)*split_count);
            _split_count = split_count;
        }

    }

    BStringSplit::BStringSplit(BStringSplit && other) noexcept
    {
        #ifdef DEBUG
        puts("Calling move ctor");
        #endif 
        _reset_split_array();
        _split_array = other._split_array;
        _split_count = other._split_count;
        other._split_array = nullptr;
        other._split_count = 0;

    }

    BStringSplit::~BStringSplit()
    {
        #ifdef DEBUG
        puts("Calling dtor");
        #endif 
        _reset_split_array();
    }

    BStringSplit& BStringSplit::operator = (BStringSplit other)
    {
        #ifdef DEBUG
        puts("Calling assignment operator");
        #endif 
        swap(other);
        return *this;   
    }
    
    BString& BStringSplit::operator[](int index)
    {
        if(index >= _split_count || index < 0)
        {
            puts("Index out of bounds for split string. Returning first split entry\n");
            return (*_split_array[0]);
        }
        return (*_split_array[index]);
    }

    BStringSplit split(const BString& src, const char * match)
    {
       return split(src, match, -1);

    }

    BStringSplit split(const BString& src, const char match)
    {
        const char match_s[2] = {match,0};
        return split(src, match_s, -1);

    }
    BStringSplit split(const BString& src, const char * match, int max_split)
    {
        size_t src_str_len = src._str_len;
        if(src_str_len < 1 ) return BStringSplit();
        if(max_split < 0) max_split = _bons_str_max_split;

        size_t match_len = strnlen(match, _bons_str_max_len);
        if( match_len >= _bons_str_max_len || match_len > src_str_len) return BStringSplit();


        char * match_index;
        char * str_ptr = src._str;
        size_t split_count = 0;
        // Consider using move sematics to solve the issues with strs being garbage values
        BString** temp_split_array = new BString*[(src_str_len/2)+1]();
        while((match_index = strstr(str_ptr, match)) && --max_split)
        {
            if(match_index != str_ptr)
            {
                size_t lhs_size = match_index - str_ptr;
                char* cstr_lhs = new char[lhs_size+1](); // temporary buffer
                memcpy(cstr_lhs, str_ptr, lhs_size);             
                temp_split_array[split_count] = new BString(cstr_lhs);
                ++split_count;
                delete [] cstr_lhs;
                str_ptr += lhs_size;
            }
            str_ptr += match_len;
        }
      
        if(*str_ptr != '\0')
        {
            temp_split_array[split_count] = new BString(str_ptr);
            ++split_count;
        }
        
        
        BStringSplit split;
        split._split_array = new BString*[split_count]();
        split._split_count = split_count;
        
        memcpy(split._split_array, temp_split_array, sizeof(BString*)*split_count);

        return split;
    }
    
    void BStringSplit::_reset_split_array() const 
    {
        if(_split_count)
        {
            //dtor the lemenets in the array
            while(_split_count)
            {
                _split_array[--_split_count]->reset();
            }
            _split_count = 0;
        }
        delete [] _split_array;
    }

    void BStringSplit::swap(BStringSplit& other)
    {
        std::swap(_split_array, other._split_array);
        std::swap(_split_count, other._split_count);
    }
    
}