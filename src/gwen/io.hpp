#pragma once

// https://github.com/NachiaVivias/cp-library/blob/main/Cpp/Include/nachia/misc/fastio.hpp

#include <cctype>
#include <concepts>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

#include "gwen/types.hpp"

#define D_GWEN_IO_BUF_SIZE 1 << 20

namespace gwen {

class input_stream {
private:
    static constexpr u32 INPUT_BUF_SIZE = D_GWEN_IO_BUF_SIZE;
    u32 p = INPUT_BUF_SIZE;
    inline static char buf[INPUT_BUF_SIZE];

public:
    using is_type = input_stream;
    char seek_char() {
        if (p == INPUT_BUF_SIZE) {
            u32 len = std::fread(buf, 1, INPUT_BUF_SIZE, stdin);
            if (len != INPUT_BUF_SIZE) buf[len] = '\0';
            p = 0;
        }
        return buf[p];
    }

    void skip_space() {
        while (isspace(seek_char())) ++p;
    }

    u32 get_u32() {
        skip_space();
        u32 ret = 0;
        while (true) {
            char tmp = seek_char();
            if (tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i32 get_i32() {
        skip_space();
        if (seek_char() == '-') {
            ++p;
            return -(static_cast<i32>(get_u32()));
        }
        return static_cast<i32>(get_u32());
    }

    u64 get_u64() {
        skip_space();
        u64 ret = 0;
        while (true) {
            char tmp = seek_char();
            if (tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i64 get_i64() {
        skip_space();
        if (seek_char() == '-') {
            ++p;
            return -(static_cast<i64>(get_u64()));
        }
        return static_cast<i64>(get_u64());
    }

    u128 get_u128() {
        skip_space();
        u128 ret = 0;
        while (true) {
            char tmp = seek_char();
            if (tmp < '0' || '9' < tmp) break;
            ret = ret * 10 + (tmp - '0');
            ++p;
        }
        return ret;
    }

    i128 get_i128() {
        skip_space();
        if (seek_char() == '-') {
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
        while (true) {
            char tmp = seek_char();
            if (isspace(tmp) || tmp == '\0') break;
            ret.push_back(tmp);
            p++;
        }
        return ret;
    }

    is_type& operator>>(i32& dest) {
        dest = get_i32();
        return *this;
    }
    is_type& operator>>(u32& dest) {
        dest = get_u32();
        return *this;
    }
    is_type& operator>>(i64& dest) {
        dest = get_i64();
        return *this;
    }
    is_type& operator>>(u64& dest) {
        dest = get_u64();
        return *this;
    }
    is_type& operator>>(i128& dest) {
        dest = get_i128();
        return *this;
    }
    is_type& operator>>(u128& dest) {
        dest = get_u128();
        return *this;
    }
    is_type& operator>>(char& dest) {
        dest = get_char();
        return *this;
    }
    is_type& operator>>(std::string& dest) {
        dest = get_token();
        return *this;
    }

    template <std::signed_integral INT>
    is_type& operator>>(INT& dest) {
        dest = static_cast<INT>(get_i64());
        return *this;
    }
    template <std::unsigned_integral UINT>
    is_type& operator>>(UINT& dest) {
        dest = static_cast<UINT>(get_u64());
        return *this;
    }

    template <typename T>
    is_type& operator>>(std::vector<T>& vec) {
        for (T& dest : vec) (*this) >> dest;
        return *this;
    }
} cin;

struct output_precalc_table {
    char LZ[1000][4] = {};
    char NLZ[1000][4] = {};
    constexpr output_precalc_table() {
        for (u32 d = 0; d < 1000; d++) {
            LZ[d][0] = ('0' + d / 100 % 10);
            LZ[d][1] = ('0' + d / 10 % 10);
            LZ[d][2] = ('0' + d / 1 % 10);
            LZ[d][3] = '\0';
        }
        for (u32 d = 0; d < 1000; d++) {
            u32 i = 0;
            if (d >= 100) NLZ[d][i++] = ('0' + d / 100 % 10);
            if (d >= 10) NLZ[d][i++] = ('0' + d / 10 % 10);
            if (d >= 1) NLZ[d][i++] = ('0' + d / 1 % 10);
            NLZ[d][i++] = '\0';
        }
    }
};

class output_stream {
private:
    static constexpr u32 OUTPUT_BUF_SIZE = D_GWEN_IO_BUF_SIZE;
    inline static char buf[OUTPUT_BUF_SIZE];
    static constexpr output_precalc_table TB = output_precalc_table();

    u32 p = 0;
    static constexpr u32 P10(u32 d) { return d ? P10(d - 1) * 10 : 1; }
    static constexpr u64 P10L(u32 d) { return d ? P10L(d - 1) * 10 : 1; }

    template <class T, class U>
    static void fil(T& m, U& l, U x) noexcept {
        m = l / x;
        l -= m * x;
    }

    void push_dig9(u32 x) {
        u32 y;
        fil(y, x, P10(6));
        push_token(TB.LZ[y]);
        fil(y, x, P10(3));
        push_token(TB.LZ[y]);
        push_token(TB.LZ[x]);
    }

public:
    using os_type = output_stream;
    void push_1byte(char c) {
        buf[p++] = c;
        if (p == OUTPUT_BUF_SIZE) {
            fwrite(buf, p, 1, stdout);
            p = 0;
        }
    }

    void push_ln() { push_1byte('\n'); }

    void push_token(const char* s) {
        while (*s) push_1byte(*(s++));
    }

    void push_u32(u32 x) {
        u32 y = 0;
        if (x >= P10(9)) {
            fil(y, x, P10(9));
            push_token(TB.NLZ[y]);
            push_dig9(x);
        }
        else if (x >= P10(6)) {
            fil(y, x, P10(6));
            push_token(TB.NLZ[y]);
            fil(y, x, P10(3));
            push_token(TB.LZ[y]);
            push_token(TB.LZ[x]);
        }
        else if (x >= P10(3)) {
            fil(y, x, P10(3));
            push_token(TB.NLZ[y]);
            push_token(TB.LZ[x]);
        }
        else if (x >= 1)
            push_token(TB.NLZ[x]);
        else
            push_1byte('0');
    }

    void push_i32(i32 x) {
        if (0 <= x)
            push_u32(static_cast<u32>(x));
        else {
            push_1byte('-');
            push_u32(static_cast<u32>(-x));
        }
    }

    void push_u64(u64 x) {
        u32 y = 0;
        if (x >= P10L(18)) {
            fil(y, x, P10L(18));
            push_u32(y);
            fil(y, x, P10L(9));
            push_dig9(y);
            push_dig9(static_cast<u32>(x));
        }
        else if (x >= P10L(9)) {
            fil(y, x, P10L(9));
            push_u32(y);
            push_dig9(static_cast<u32>(x));
        }
        else
            push_u32(x);
    }

    void push_i64(i64 x) {
        if (0 <= x)
            push_u64(x);
        else {
            push_1byte('-');
            push_u64(static_cast<u64>(-x));
        }
    }

    void write_to_file(bool flush = false) {
        fwrite(buf, p, 1, stdout);
        if (flush) std::fflush(stdout);
        p = 0;
    }

    output_stream() { buf[0] = '\0'; }
    ~output_stream() { write_to_file(); }

    os_type& operator<<(i32 tg) {
        push_i32(tg);
        return *this;
    }
    os_type& operator<<(u32 tg) {
        push_u32(tg);
        return *this;
    }
    os_type& operator<<(i64 tg) {
        push_i64(tg);
        return *this;
    }
    os_type& operator<<(u64 tg) {
        push_u64(tg);
        return *this;
    }
    // TODO: i128, u128
    os_type& operator<<(const std::string& tg) {
        push_token(tg.c_str());
        return *this;
    }
    os_type& operator<<(const char* tg) {
        push_token(tg);
        return *this;
    }
    os_type& operator<<(char tg) {
        push_1byte(tg);
        return *this;
    }

    template <std::signed_integral INT>
    os_type& operator<<(INT tg) {
        push_i64(tg);
        return *this;
    }
    template <std::unsigned_integral UINT>
    os_type& operator<<(UINT tg) {
        push_u64(tg);
        return *this;
    }

    template <typename T>
    os_type& operator<<(const std::vector<T>& vec) {
        auto it = vec.begin(), end = vec.end();
        if (it == end) return *this;
        *this << *(it++);
        for (; it != end; ++it) (*this) << ' ' << *it;
        return *this;
    }
} cout;

}  // namespace gwen