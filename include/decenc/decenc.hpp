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

#ifndef _DECENC_HPP
#define _DECENC_HPP

#include <decenc/decenc_version.hpp>

namespace DECENC {

class error
{
  private:
    std::string text_;

  public:
    bool        is_ok() const { return text_.empty(); };
    void        operator=(const std::string& msg) { text_ = msg; };
    std::string text() const { return text_; };
};

template <typename T> class decenc
{
  private:
    T algo;

  public:
    error encode(const std::string in, std::string& out) const { return algo.encode(in, out); };
    error decode(const std::string in, std::string& out) const { return algo.decode(in, out); };
};

} // namespace DECENC

#include <decenc/decenc_base16.hpp>
#include <decenc/decenc_base32.hpp>
#include <decenc/decenc_base32hex.hpp>
#include <decenc/decenc_base64.hpp>
#include <decenc/decenc_base64url.hpp>
#include <decenc/decenc_base91.hpp>

#endif // _DECENC_HPP
