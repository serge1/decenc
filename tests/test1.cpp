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

#define BOOST_TEST_MODULE Decenc_Test
#include <boost/test/included/unit_test.hpp>

#include <decenc/decenc.hpp>

using namespace DECENC;

BOOST_AUTO_TEST_CASE(test_base16)
{
    decenc<base16> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "66"},
        {"fo", "666F"},
        {"foo", "666F6F"},
        {"foob", "666F6F62"},
        {"fooba", "666F6F6261"},
        {"foobar", "666F6F626172"},
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

BOOST_AUTO_TEST_CASE(test_base16_1)
{
    decenc<base16> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }
}

BOOST_AUTO_TEST_CASE(test_base16_2)
{
    decenc<base16> coder;

    std::string in;
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in = std::to_string(i);
        coder.encode(in, out);

        std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return std::tolower(c); });

        coder.decode(out, in);
        BOOST_CHECK_EQUAL(std::stoi(in), i);
    }
}

BOOST_AUTO_TEST_CASE(test_base16_3)
{
    decenc<base16> coder;

    std::string in;
    std::string out;

    in        = "123";
    error err = coder.decode(in, out);
    BOOST_CHECK_EQUAL(err.is_ok(), false);

    in  = "12w3";
    err = coder.decode(in, out);
    BOOST_CHECK_EQUAL(err.is_ok(), false);
}

BOOST_AUTO_TEST_CASE(test_base32)
{
    decenc<base32> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "MY======"},
        {"fo", "MZXQ===="},
        {"foo", "MZXW6==="},
        {"foob", "MZXW6YQ="},
        {"fooba", "MZXW6YTB"},
        {"foobar", "MZXW6YTBOI======"},
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

BOOST_AUTO_TEST_CASE(test_base32_with_no_pad)
{
    decenc<base32> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"f", "MY"}, {"fo", "MZXQ"}, {"foo", "MZXW6"}, {"foob", "MZXW6YQ"}, {"foobar", "MZXW6YTBOI"},
    };

    for (auto test : tests) {
        std::string out;
        error       err = coder.decode(test.second, out);
        BOOST_REQUIRE_EQUAL(err.is_ok(), true);
        BOOST_CHECK_EQUAL(out, test.first);
    }
}

BOOST_AUTO_TEST_CASE(test_base32_1)
{
    decenc<base32> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }
}

BOOST_AUTO_TEST_CASE(test_base32hex)
{
    decenc<base32hex> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "CO======"},
        {"fo", "CPNG===="},
        {"foo", "CPNMU==="},
        {"foob", "CPNMUOG="},
        {"fooba", "CPNMUOJ1"},
        {"foobar", "CPNMUOJ1E8======"},
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

BOOST_AUTO_TEST_CASE(test_base32hex_with_no_pad)
{
    decenc<base32hex> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"f", "CO"}, {"fo", "CPNG"}, {"foo", "CPNMU"}, {"foob", "CPNMUOG"}, {"foobar", "CPNMUOJ1E8"},
    };

    for (auto test : tests) {
        std::string out;
        error       err = coder.decode(test.second, out);
        BOOST_REQUIRE_EQUAL(err.is_ok(), true);
        BOOST_CHECK_EQUAL(out, test.first);
    }
}

BOOST_AUTO_TEST_CASE(test_base32hex_1)
{
    decenc<base32hex> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }
}

BOOST_AUTO_TEST_CASE(test_base64)
{
    decenc<base64> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "Zg=="},
        {"fo", "Zm8="},
        {"foo", "Zm9v"},
        {"foob", "Zm9vYg=="},
        {"fooba", "Zm9vYmE="},
        {"foobar", "Zm9vYmFy"},
        {"Man is distinguished, not only by his reason, but by this singular passion from other animals, "
         "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
         "generation of knowledge, exceeds the short vehemence of any carnal pleasure.",
         "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
         "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
         "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
         "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
         "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4="},
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

BOOST_AUTO_TEST_CASE(test_base64_with_no_pad)
{
    decenc<base64> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"f", "Zg"},
        {"fo", "Zm8"},
        {"foob", "Zm9vYg"},
        {"fooba", "Zm9vYmE"},
    };

    for (auto test : tests) {
        std::string out;
        error       err = coder.decode(test.second, out);
        BOOST_REQUIRE_EQUAL(err.is_ok(), true);
        BOOST_CHECK_EQUAL(out, test.first);
    }
}

BOOST_AUTO_TEST_CASE(test_base64_1)
{
    decenc<base64> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }
}

BOOST_AUTO_TEST_CASE(test_base64_2)
{
    decenc<base64> coder;

    std::string in;
    std::string out;
    std::string result;

    for (unsigned int i = 0; i < 256; i++) {
        in.append(1, (unsigned char)i);
    }

    coder.encode(in, out);
    coder.decode(out, result);
    BOOST_CHECK_EQUAL(in, result);
}

BOOST_AUTO_TEST_CASE(test_base64_3)
{
    decenc<base64> coder;

    std::string in;
    std::string out;

    in        = "12[3";
    error err = coder.decode(in, out);
    BOOST_CHECK_EQUAL(err.is_ok(), false);
}

BOOST_AUTO_TEST_CASE(test_base64url)
{
    decenc<base64url> coder;

    std::string in;
    std::string out;

    std::vector<std::pair<std::string, std::string>> tests = {
        {"", ""},
        {"f", "Zg=="},
        {"fo", "Zm8="},
        {"foo", "Zm9v"},
        {"foob", "Zm9vYg=="},
        {"fooba", "Zm9vYmE="},
        {"foobar", "Zm9vYmFy"},
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

BOOST_AUTO_TEST_CASE(test_base64url_1)
{
    decenc<base64url> coder;

    std::string in = " ";
    std::string out;

    for (unsigned int i = 0; i < 256; i++) {
        in[0] = i;
        coder.encode(in, out);
        coder.decode(out, in);
        BOOST_CHECK_EQUAL((unsigned char)in[0], i);
    }
}
