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

#include <boost/test/unit_test.hpp>

#include <decenc/decenc.hpp>

using namespace DECENC;

BOOST_AUTO_TEST_CASE(test_base91)
{
    decenc<base91> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "LB"},
        {"fo", "drD"},
        {"foo", "dr.J"},
        {"foob", "dr/2Y"},
        {"fooba", "dr/2s)A"},
        {"foobar", "dr/2s)uC"},
        {"This is Base91 encoding/decoding", "nX,<:WRT$F,ue9QUz\"|,|irMZ2(.xX<!7&8P;mMB"},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;<=>?@[]^_",
         "fG^F%w_o%5qOdwQbFrzd[5eYAP;gMP+f#G(Ic,5ph#77&xrmlrjgs@DZ7UB>xQGrgw_,$k_i$Js@Tj$MaRDa7dq)L1<[3vwV[|"
         "O/7%q{F"},
    };

    for (auto test : tests) {
        std::string out;
        error       err = coder.encode(test.first, out);
        BOOST_REQUIRE_EQUAL(err.is_ok(), true);
        BOOST_CHECK_EQUAL(out, test.second);
    }

    for (auto test : tests) {
        std::string out;
        error       err = coder.decode(test.second, out);
        BOOST_REQUIRE_EQUAL(err.is_ok(), true);
        BOOST_CHECK_EQUAL(out, test.first);
    }
}

BOOST_AUTO_TEST_CASE(test_base91_1)
{
    decenc<base91> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }

    in = "  ";
    for (unsigned int i = 0; i < 256; i++) {
        for (unsigned int j = 0; j < 256; j++) {
            in[0] = i;
            in[1] = j;
            coder.encode(in, out);
            coder.decode(out, in);
            BOOST_CHECK_EQUAL((unsigned char)in[0], i);
            BOOST_CHECK_EQUAL((unsigned char)in[1], j);
        }
    }
}

BOOST_AUTO_TEST_CASE(test_base91_2)
{
    decenc<base91> coder;

    std::string in;
    std::string out;

    for (unsigned int i = 0; i < 1000; i++) {
        in = std::to_string(i);
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL(std::stoi(in), i);
    }
}
