/*
MIT License

Copyright (c) 2020 Serge Lamikhov-Center

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <string>
#include <experimental/string_view>
#include <chrono>

#include <boost/test/unit_test.hpp>

#include <decenc/decenc.hpp>

using namespace DECENC;

/* 
   This file is dedicated to performance measurements.
   As the Boost was not able to process 256MB, the constant 128 * 1024 * 1024
   is chosen.
   Uncomment if required
*/

/*

BOOST_AUTO_TEST_CASE(test_perf_base64)
{
    decenc<base64> coder;
    std::string    in(std::string(128 * 1024 * 1024, '1'));
    std::string    out;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    coder.encode(in, out);
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> time_span =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for decenc<base64> ______" << std::endl;
}

std::string base64_decode(const std::experimental::string_view in)
{
    // table from '+' to 'z'
    const uint8_t lookup[] = {62,  255, 62,  255, 63,  52,  53, 54, 55, 56, 57, 58, 59, 60, 61, 255,
                              255, 0,   255, 255, 255, 255, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
                              10,  11,  12,  13,  14,  15,  16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
                              255, 255, 255, 255, 63,  255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
                              36,  37,  38,  39,  40,  41,  42, 43, 44, 45, 46, 47, 48, 49, 50, 51};
    //    static_assert(sizeof(lookup) == 'z' - '+' + 1);

    std::string out;
    int         val = 0, valb = -8;
    for (uint8_t c : in) {
        if (c < '+' || c > 'z')
            break;
        c -= '+';
        if (lookup[c] >= 64)
            break;
        val = (val << 6) + lookup[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

static const std::string base64_chars_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";

static inline bool is_base64_(unsigned char c) { return (isalnum(c) || (c == '+') || (c == '/')); }

std::string base64_encode_(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    std::string   ret;
    int           i = 0;
    int           j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars_[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars_[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string base64_decode_(std::string const& encoded_string)
{
    int           in_len = encoded_string.size();
    int           i      = 0;
    int           j      = 0;
    int           in_    = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string   ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64_(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars_.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars_.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

#include <vector>
#include <string>
typedef unsigned char BYTE;

std::string       base64_encode__(BYTE const* buf, unsigned int bufLen);
std::vector<BYTE> base64_decode__(std::string const&);

#include <iostream>

static const std::string base64_chars__ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                          "abcdefghijklmnopqrstuvwxyz"
                                          "0123456789+/";

static inline bool is_base64__(BYTE c) { return (isalnum(c) || (c == '+') || (c == '/')); }

std::string base64_encode__(BYTE const* buf, unsigned int bufLen)
{
    std::string ret;
    int         i = 0;
    int         j = 0;
    BYTE        char_array_3[3];
    BYTE        char_array_4[4];

    while (bufLen--) {
        char_array_3[i++] = *(buf++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars__[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars__[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::vector<BYTE> base64_decode__(std::string const& encoded_string)
{
    int               in_len = encoded_string.size();
    int               i      = 0;
    int               j      = 0;
    int               in_    = 0;
    BYTE              char_array_4[4], char_array_3[3];
    std::vector<BYTE> ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64__(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars__.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars__.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret.push_back(char_array_3[j]);
    }

    return ret;
}

BOOST_AUTO_TEST_CASE(test_perf_base64__)
{
    decenc<base64> coder;
    std::string    in(std::string(128 * 1024 * 1024, '1'));
    std::string    out3;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    out3                                     = base64_encode__((BYTE const*)in.c_str(), in.length());
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> time_span =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for base64_encode__" << std::endl;
}


#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <algorithm>

std::string boost_decode(std::string input)
{
    using namespace boost::archive::iterators;
    typedef transform_width<binary_from_base64<remove_whitespace<std::string::const_iterator>>, 8, 6>
        ItBinaryT;

    try {
        // If the input isn't a multiple of 4, pad with =
        size_t num_pad_chars((4 - input.size() % 4) % 4);
        input.append(num_pad_chars, '=');

        size_t pad_chars(std::count(input.begin(), input.end(), '='));
        std::replace(input.begin(), input.end(), '=', 'A');
        std::string output(ItBinaryT(input.begin()), ItBinaryT(input.end()));
        output.erase(output.end() - pad_chars, output.end());
        return output;
    }
    catch (std::exception const&) {
        return std::string("");
    }
}

BOOST_AUTO_TEST_CASE(test_perf_base64_decode)
{
    decenc<base64>    coder;
    std::string       in(std::string(128 * 1024 * 1024, '1'));
    std::string       out;
    std::string       out1;
    std::string       out2;
    std::vector<BYTE> out3;
    std::string       out4;

    coder.encode(in, out);
    in = out;

    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    coder.decode(in, out);
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

    std::chrono::duration<double> time_span =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for decenc<base64>.decode ______"
              << std::endl;

    t1   = std::chrono::steady_clock::now();
    out1 = base64_decode(std::experimental::string_view(in));
    t2   = std::chrono::steady_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for base64_decode" << std::endl;

    t1   = std::chrono::steady_clock::now();
    out2 = base64_decode_(in);
    t2   = std::chrono::steady_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for base64_decode_" << std::endl;

    t1   = std::chrono::steady_clock::now();
    out3 = base64_decode__(in);
    t2   = std::chrono::steady_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for base64_decode__" << std::endl;

    t1   = std::chrono::steady_clock::now();
    out4 = boost_decode(in);
    t2   = std::chrono::steady_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds for boost_decode" << std::endl;

    BOOST_CHECK_EQUAL(out, out1);
    BOOST_CHECK_EQUAL(out, out2);
    BOOST_CHECK_EQUAL(out, out4);
}

*/
