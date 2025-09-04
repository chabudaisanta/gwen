#pragma once

#include <cstdio>
#include <cstddef>
#include <cctype>
#include <string>
#include <vector>
#include <concepts>

#include "gwen/types.hpp"

namespace gwen {

class input_stream {
private:
    static constexpr u32 INPUT_BUF_SIZE = 1 << 17;
    u32 p = INPUT_BUF_SIZE;
    inline static char buf[INPUT_BUF_SIZE];

public:
    using os_type = input_stream;
    char seek_char() {
        if(p == INPUT_BUF_SIZE) {
            u32 len = std::fread(buf, 1, INPUT_BUF_SIZE, stdin);
            if(len != INPUT_BUF_SIZE) buf[len] = '\0';
            p = 0;
        }
        return buf[p];
    }

    void skip_space() {
        while(isspace(seek_char())) ++p;
    }

    u32 get_u32() {
        skip_space();
        u32 ret = 0;
        while(true) {
            char tmp = seek_char();
            if(tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i32 get_i32() {
        skip_space();
        if(seek_char() == '-') {
            ++p;
            return -(static_cast<i32>(get_u32()));
        }
        return static_cast<i32>(get_u32());
    }

    u64 get_u64() {
        skip_space();
        u64 ret = 0;
        while(true) {
            char tmp = seek_char();
            if(tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i64 get_i64() {
        skip_space();
        if(seek_char() == '-') {
            ++p;
            return -(static_cast<i64>(get_u64()));
        }
        return static_cast<i64>(get_u64());
    }

    u128 get_u128() {
        skip_space();
        u128 ret = 0;
        while(true) {
            char tmp = seek_char();
            if(tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i128 get_i128() {
        skip_space();
        if(seek_char() == '-') {
            ++p;
            return -(static_cast<i128>(get_u128()));
        }
        return static_cast<i128>(get_u128());
    }

    char get_char() {
        skip_space();
        char ret = seek_char();
        ++p;
        return ret;
    }

    std::string get_token() {
        skip_space();
        std::string ret;
        while(true) {
            char tmp = get_char();
            if(isspace(tmp) || tmp == '\0') break;
            ret.push_back(tmp);
            ++p;
        }
        return ret;
    }

    os_type& operator>>(i32& dest) { dest = get_i32(); return *this; }
    os_type& operator>>(u32& dest) { dest = get_u32(); return *this; }
    os_type& operator>>(i64& dest) { dest = get_i64(); return *this; }
    os_type& operator>>(u64& dest) { dest = get_u64(); return *this; }
    os_type& operator>>(i128& dest) { dest = get_i128(); return *this; }
    os_type& operator>>(u128& dest) { dest = get_u128(); return *this; }
    os_type& operator>>(char& dest) { dest = get_char(); return *this; }
    os_type& operator>>(std::string& dest) { dest = get_token(); return *this; }

    template<std::signed_integral INT>
    os_type& operator>>(INT& dest) { dest = static_cast<INT>(get_i64()); return *this; }
    template<std::unsigned_integral UINT>
    os_type& operator>>(UINT& dest) { dest = static_cast<UINT>(get_u64()); return *this; }

    template<typename T>
    os_type& operator>>(std::vector<T>& vec) {
        for(T& dest : vec) this >> dest;
        return *this;
    }
} cin;



} // namespace gwen