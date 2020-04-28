#include "../include/bstring.hpp"

namespace botched_utils
{
    BString::BString()
    {
    }

    BString::BString(const char * str)
    {
        copy_str(str);
    }

    BString::BString(const BString & other)
    {
        copy_str(other);
    }

    BString::BString(BString && other) noexcept 
    {
        reset();
        // since the pointer and size_t are very small, we simply copy them over.
        _str = other._str;
        _str_len = other._str_len;
        other._str = nullptr;
        other._str_len = 0;
        other.reset(); // this might not be needed if we dont use split_array
    }

    BString::~BString()
    {
        reset();
    }

    void BString::reset()
    {
        _reset_split_array();
        if(_str)
        {
            delete [] _str;
            _str = nullptr;
            _str_len = 0;
        }

    }

    // NON-MEMBER OPERATOR
    BString operator+ ( const BString & lhs, const BString & rhs)
    {
        BString rs = lhs;
        rs += rhs;
        return rs;
    }



    // OPERATORS 
    BString::operator const char *() const 
    {
        return _str;
    }

    BString & BString::operator = ( BString other)
    {
        swap(other);
        return *this;
    }

    BString & BString::operator += (const char * rhs)
    {
    if(rhs)
    {
        size_t newlen = _str_len+ strnlen(rhs, _bons_str_max_len);
        if (newlen > _bons_str_max_len) newlen = _bons_str_max_len;

        size_t rhs_len = newlen - _str_len;
        if(rhs_len < 1) return *this;

        char * buffer = new char[newlen+1]();
        if(_str && _str_len) memcpy(buffer, _str, _str_len);
        memcpy(buffer+_str_len, rhs, rhs_len);
        copy_str(buffer);
        delete [] buffer;
    }
    return *this;
    }

    BString & BString::operator += (const BString & rhs)
    {
    
    const char * rhs_str = rhs._str;
    if(rhs_str)
    {
        size_t newlen = _str_len+ rhs._str_len;
        if (newlen > _bons_str_max_len) newlen = _bons_str_max_len;

        size_t rhs_len = newlen - _str_len;
        if(rhs_len < 1) return *this;

        char * buffer = new char[newlen+1]();
        if(_str && _str_len) memcpy(buffer, _str, _str_len);
        memcpy(buffer+_str_len, rhs_str, rhs_len);
        copy_str(buffer);
        delete [] buffer;
    }
    return *this;
    }

    const char BString::operator[] (const int index) const
    {
        if(index <0 )return 0;
        if(index>= (int)_str_len) return 0;
        else return _str[index];
    }

    bool BString::operator== (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) == 0) return true;
        
        return false;
    }

    bool BString::operator!= (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) != 0) return true;
        
        return false;
    }

    bool BString::operator > (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) > 0) return true;
        
        return false;
    }
    bool BString::operator >= (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) >= 0) return true;
        
        return false;
    }
    bool BString::operator < (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) < 0) return true;
        
        return false;
    }
    bool BString::operator <= (const BString & rhs) const
    {
        if(std::strncmp(*this, rhs, _bons_str_max_len) <= 0) return true;
        
        return false;
    }



    // PRIVATE METHODS

    void BString::swap(BString & other)
    {
        std::swap(_str, other._str);
        std::swap(_str_len, other._str_len);
    }

    const char * BString::copy_str( const char * str)
    {
        if(str)
        {
            size_t len = strnlen(str, _bons_str_max_len);
            alloc_str(len);
            strncpy((char*)_str, str, len);
            _str_len = len;
        }
        return _str;
    }

    const char * BString::alloc_str(size_t size)
    {
        if(_str) reset();
        _str_len = (size > _bons_str_max_len) ? _bons_str_max_len : size;
        _str = new char[_str_len+1](); // fills new char with 0;
        return _str;
    }



    void BString::_reset_split_array() const 
    {
        if(_split_count)
        {
            //dtor the lemenets in the array
            while(_split_count)
            {
                _split_array[--_split_count].reset();
            }
            _split_array.reset();
            _split_count = 0;
        }
    }

    void BString::_append_split_array(const BString & str) const
    {
        if(_split_count >= _bons_str_max_split) return;
        if(!_split_count)
            _split_array.reset(new _bsp[_bons_str_max_split+1]);
        
        _split_array[_split_count] = std::make_shared<BString>(str);
        ++_split_count;
    }

    // UTILITY METHODS

    bool BString::empty() const
    {
        if(_str) return true;
        return false;
    }

    BString & BString::format( const char * format, ...)
    {
        char * buffer;

        va_list args;
        va_start(args, format);

        vasprintf(&buffer, format, args);
        copy_str(buffer);
        free(buffer);
        va_end(args);
        return *this;
    }

    // trim leading and trailing spaces
    BString & BString::trim()
    {
        constexpr const char* whitespace = "\x20\x1b\t\r\n\v\b\f\a";

        if(empty()) return *this;

        size_t begin = 0;
        size_t end = _str_len-1;

        for(begin = 0; begin <= end; ++begin)
        {
            if (strchr(whitespace, _str[end]) == nullptr)
                break;
        }

        for( ; end > begin; --end)
        {
            if(strchr(whitespace, _str[end]) == nullptr)
                break;
            else 
                _str[end] = '\0';
        }

        if (begin)
        {
            for (size_t i = 0; _str[i]; i++)
            {
                _str[i] = _str[begin++];
            }
            
        }

        _str_len = strlen(_str);
        return *this;

    }

    BString BString::lower() const 
    {
        BString rs = *this;
        for (size_t i = 0; rs._str[i]; ++i)
        {
            rs._str[i] = tolower(rs._str[i]);
        }
        return rs;
        
    }

    BString BString::upper() const 
    {
        BString rs = *this;
        for (size_t i = 0; rs._str[i]; ++i)
        {
            rs._str[i] = toupper(rs._str[i]);
        }
        return rs;
        
    }

    const char & BString::back() const
    {
        return _str[_str_len-1];
    }

    const char & BString::front() const
    {
        return _str[0];
    }

    long int BString::char_find( const char & match) const 
    {
        for (size_t i = 0; _str[i]; i++)
        {
            if(_str[i] == match) return i;
        }
        return -1;
    }

    const BString & BString::char_replace(const char & match, const char & replace)
    {
        for (size_t i = 0; _str[i]; i++)
        {
            if(_str[i] == match) _str[i] = replace;
        }
        return *this;
    }

    BString BString::sub_string(size_t start, size_t length)
    {
        BString rs;
        char * buffer;
        if((length+1) > _bons_str_max_len || (start + length) > _bons_str_max_len) return rs;
        if(length > _str_len-start) return rs;
        if(!_str) return rs;

        buffer = new char[length+1]();
        memcpy(buffer, _str+start, length);
        rs = buffer;
        delete []buffer;

        return rs;
    }

    long BString::find(const BString & match) const
    {
        char * pos = strstr(_str, match);
        if(pos) return (long) (pos - _str);
        return -1;
    }

    const BString BString::replace_first(const BString & match, const BString & replace)
    {
        // this ONLY takes the first match. This should loop, if we want to replace all
        BString rs;
        long found_match = find(match);
        if(found_match >= 0)
        {
            size_t pos_start = (size_t) found_match;
            size_t pos_end = pos_start + match._str_len;
            BString lh_string = pos_start > 0 ? sub_string(0, pos_start) : "";
            BString rh_string= sub_string(pos_end, this->_str_len - pos_end);
            rs = lh_string + replace + rh_string;
        } 
        return rs;
    }

    const BString BString::replace_count(const BString & match, const BString & replace, size_t)
    {
        // TODO: Implement
        // this ONLY takes the first match. This should loop, if we want to replace all
        BString rs;
        long found_match = find(match);
        if(found_match >= 0)
        {
            size_t pos_start = (size_t) found_match;
            size_t pos_end = pos_start + match._str_len;
            BString lh_string = pos_start > 0 ? sub_string(0, pos_start) : "";
            BString rh_string= sub_string(pos_end, this->_str_len - pos_end);
            rs = lh_string + replace + rh_string;
        } 
        return rs;
    }

    const BString BString::replace_all(const BString & match, const BString & replace)
    {
        // TODO: Implement
        BString rs;
        long found_match = find(match);
        if(found_match >= 0)
        {
            size_t pos_start = (size_t) found_match;
            size_t pos_end = pos_start + match._str_len;
            BString lh_string = pos_start > 0 ? sub_string(0, pos_start) : "";
            BString rh_string= sub_string(pos_end, this->_str_len - pos_end);
            rs = lh_string + replace + rh_string;
        } 
        return rs;
    }

    const BString::split_ptr & BString::split(const char * match) const
    {
        return split(match, -1);

    }
    const BString::split_ptr & BString::split(const char match) const
    {
        const char match_s[2] = {match,0};
        return split(match_s, -1);

    }
    const BString::split_ptr & BString::split(const char * match, int max_split) const
    {
        _reset_split_array();
        if(_str_len < 1 ) return _split_array;
        if(max_split < 0) max_split = _bons_str_max_split;

        size_t match_len = strnlen(match, _bons_str_max_len);
        if( match_len >= _bons_str_max_len) return _split_array;


        char * match_index;
        char * str_ptr = _str;
        while((match_index = strstr(str_ptr, match)) && --max_split)
        {
            if(match_index != str_ptr)
            {
                size_t lhsz = match_index - str_ptr;
                char* cslhs = new char[lhsz+1](); // temporary buffer
                memcpy(cslhs, str_ptr, lhsz);
                _append_split_array(cslhs);
                delete [] cslhs;
                str_ptr += lhsz;
            }
            str_ptr += match_len;
        }

        if(*str_ptr != '\0')
            _append_split_array(str_ptr);

        return _split_array;

    }

    const BString & BString::split_item(size_t index) const
    {
        if (_split_count > index) return *_split_array[index];
        return *this; //I dont think I like this...
    }
       
}
#ifdef _MSC_VER

int vasprintf(char ** ret, const char * format, va_list ap)
{
    int len;
    char * buffer;

    len = _vscprintf(format, ap) +1;
    buffer = (char *) malloc(len * sizeof(char));
    if (!buffer) return 0;
    vsprintf_s(buffer, len, format, ap);
    *ret = buffer;
    return len -1;
}

#endif // _MSC_VER
