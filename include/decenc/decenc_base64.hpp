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

#ifndef _DECENC_BASE64_HPP
#define _DECENC_BASE64_HPP

namespace DECENC {

class base64_data
{
  public:
    const uint8_t* get_enctab() const
    {
        static const uint8_t enctab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        return (const uint8_t*)enctab;
    };

    const uint8_t* get_dectab() const
    {
        static const uint8_t dectab[256] = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
            0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
            0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

        return dectab;
    };
};

template <typename T_DATA> class base64_algo
{
  protected:
    T_DATA data;

  public:
    error encode(const std::string in, std::string& out) const
    {
        error          err;
        const uint8_t* enctab = data.get_enctab();

        size_t len = in.length();
        out.reserve((len + 2) / 3 * 4);
        out = "";
        for (size_t i = 0; i < len; i += 3) {
            if (i + 2 < len) {
                uint8_t b0 = in[i], b1 = in[i + 1], b2 = in[i + 2];
                out += enctab[(b0 >> 2) & 0x3F];
                out += enctab[((b0 << 4) | ((b1 >> 4) & 0x0F)) & 0x3F];
                out += enctab[((b1 << 2) | ((b2 >> 6) & 0x03)) & 0x3F];
                out += enctab[b2 & 0x3F];
            }
            else if (i + 1 < len) {
                uint8_t b0 = in[i], b1 = in[i + 1];
                out += enctab[(b0 >> 2) & 0x3F];
                out += enctab[((b0 << 4) | ((b1 >> 4) & 0x0F)) & 0x3F];
                out += enctab[((b1 << 2) | (0 >> 6)) & 0x3F];
                out += '=';
            }
            else {
                uint8_t b0 = in[i];
                out += enctab[(b0 >> 2) & 0x3F];
                out += enctab[((b0 << 4) | (0 >> 4)) & 0x3F];
                out += '=';
                out += '=';
            }
        }

        return err;
    };

    error decode(const std::string in, std::string& out) const
    {
        error          err;
        const uint8_t* dectab = data.get_dectab();

        size_t len = in.length();
        if (len % 4 == 1) {
            err = "Invalid input length";
            return err;
        }

        out.reserve(len * 3 / 4);
        out = "";
        for (size_t i = 0; i < len; i += 4) {
            if (dectab[in[i + 0]] == 0xFF || dectab[in[i + 1]] == 0xFF ||
                ((i + 2 < len) && dectab[in[i + 2]] == 0xFF) ||
                ((i + 3 < len) && dectab[in[i + 3]] == 0xFF)) {
                err = "Invalid character found in the input";
                return err;
            }

            out += (dectab[in[i + 0]] << 2) | (dectab[in[i + 1]] >> 4);
            if (i + 2 < len) {
                if ((in[i + 2] != '='))
                    out += (dectab[in[i + 1]] << 4) | (dectab[in[i + 2]] >> 2);
                if (i + 3 < len) {
                    if (in[i + 3] != '=')
                        out += (dectab[in[i + 2]] << 6) | (dectab[in[i + 3]]);
                }
            }
        }

        return err;
    };
};

class base64 : public base64_algo<base64_data>
{
};

} // namespace DECENC

#endif // _DECENC_BASE64_HPP
