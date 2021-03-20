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

#ifndef _DECENC_BASE32_HPP
#define _DECENC_BASE32_HPP

namespace DECENC {

class base32_data
{
  public:
    const uint8_t* get_enctab() const
    {
        static const uint8_t enctab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

        return (const uint8_t*)enctab;
    };

    const uint8_t* get_dectab() const
    {
        static const uint8_t dectab[256] = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
            0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
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

template <typename T_DATA> class base32_algo
{
  protected:
    T_DATA data;

  public:
    error encode(const std::string in, std::string& out) const
    {
        error          err;
        const uint8_t* enctab = data.get_enctab();

        size_t len = in.length();
        out.reserve((len + 4) / 5 * 8);
        out = "";
        for (size_t i = 0; i < len; i += 5) {
            uint8_t b0 = 0, b1 = 0, b2 = 0, b3 = 0, b4 = 0;

            b0 = in[i];
            if (i + 1 < len) {
                b1 = in[i + 1];
                if (i + 2 < len) {
                    b2 = in[i + 2];
                    if (i + 3 < len) {
                        b3 = in[i + 3];
                        if (i + 4 < len) {
                            b4 = in[i + 4];
                        }
                    }
                }
            }

            if (i + 4 < len) {
                out += enctab[(b0 >> 3) & 0x1F];
                out += enctab[((b0 << 2) | ((b1 >> 6) & 0x03)) & 0x1F];
                out += enctab[(b1 >> 1) & 0x1F];
                out += enctab[((b1 << 4) | ((b2 >> 4) & 0x0F)) & 0x1F];
                out += enctab[((b2 << 1) | ((b3 >> 7) & 0x01)) & 0x1F];
                out += enctab[(b3 >> 2) & 0x1F];
                out += enctab[((b3 << 3) | ((b4 >> 5) & 0x07)) & 0x1F];
                out += enctab[(b4)&0x1F];
            }
            else if (i + 3 < len) {
                out += enctab[(b0 >> 3) & 0x1F];
                out += enctab[((b0 << 2) | ((b1 >> 6) & 0x03)) & 0x1F];
                out += enctab[(b1 >> 1) & 0x1F];
                out += enctab[((b1 << 4) | ((b2 >> 4) & 0x0F)) & 0x1F];
                out += enctab[((b2 << 1) | ((b3 >> 7) & 0x01)) & 0x1F];
                out += enctab[(b3 >> 2) & 0x1F];
                out += enctab[((b3 << 3) | ((b4 >> 5) & 0x07)) & 0x1F];
                out += '=';
            }
            else if (i + 2 < len) {
                out += enctab[(b0 >> 3) & 0x1F];
                out += enctab[((b0 << 2) | ((b1 >> 6) & 0x03)) & 0x1F];
                out += enctab[(b1 >> 1) & 0x1F];
                out += enctab[((b1 << 4) | ((b2 >> 4) & 0x0F)) & 0x1F];
                out += enctab[((b2 << 1) | ((b3 >> 7) & 0x01)) & 0x1F];
                out += '=';
                out += '=';
                out += '=';
            }
            else if (i + 1 < len) {
                out += enctab[(b0 >> 3) & 0x1F];
                out += enctab[((b0 << 2) | ((b1 >> 6) & 0x03)) & 0x1F];
                out += enctab[(b1 >> 1) & 0x1F];
                out += enctab[((b1 << 4) | ((b2 >> 4) & 0x0F)) & 0x1F];
                out += '=';
                out += '=';
                out += '=';
                out += '=';
            }
            else {
                out += enctab[(b0 >> 3) & 0x1F];
                out += enctab[((b0 << 2) | ((b1 >> 6) & 0x03)) & 0x1F];
                out += '=';
                out += '=';
                out += '=';
                out += '=';
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
        if ((len % 8 == 1) || (len % 8 == 3) || (len % 8 == 6)) {
            err = "Invalid input length";
            return err;
        }

        out.reserve((len + 7) * 5 / 8);
        out = "";
        for (size_t i = 0; i < len; i += 8) {
            if (dectab[in[i + 0]] == 0xFF || ((i + 1 < len) && dectab[in[i + 1]] == 0xFF) ||
                ((i + 2 < len) && dectab[in[i + 2]] == 0xFF) ||
                ((i + 3 < len) && dectab[in[i + 3]] == 0xFF) ||
                ((i + 4 < len) && dectab[in[i + 4]] == 0xFF) ||
                ((i + 5 < len) && dectab[in[i + 5]] == 0xFF) ||
                ((i + 6 < len) && dectab[in[i + 6]] == 0xFF) ||
                ((i + 7 < len) && dectab[in[i + 7]] == 0xFF)) {
                err = "Invalid character found in the input";
                return err;
            }
            out += (dectab[in[i + 0]] << 3) | (dectab[in[i + 1]] >> 2);
            if (i + 3 < len) {
                if (in[i + 3] != '=')
                    out += (dectab[in[i + 1]] << 6) | (dectab[in[i + 2]] << 1) | (dectab[in[i + 3]] >> 4);
                if (i + 4 < len) {
                    if (in[i + 4] != '=')
                        out += (dectab[in[i + 3]] << 4) | (dectab[in[i + 4]] >> 1);
                    if (i + 6 < len) {
                        if (in[i + 6] != '=')
                            out += (dectab[in[i + 4]] << 7) | (dectab[in[i + 5]] << 2) |
                                   (dectab[in[i + 6]] >> 3);
                        if (i + 7 < len) {
                            if (in[i + 7] != '=')
                                out += (dectab[in[i + 6]] << 5) | (dectab[in[i + 7]] << 0);
                        }
                    }
                }
            }
        }

        return err;
    };
}; // namespace DECENC

class base32 : public base32_algo<base32_data>
{
};

} // namespace DECENC

#endif // _DECENC_BASE32_HPP
