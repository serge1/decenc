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

#ifndef _DECENC_BASE91_HPP
#define _DECENC_BASE91_HPP

/*
    This is a C++ port of Joachim Henke's C language implementation located at:
    https://sourceforge.net/projects/base91/
*/

namespace DECENC {

class base91_data
{
  public:
    const uint8_t* get_enctab() const
    {
        static const uint8_t enctab[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!#$%&()*+,./:;<=>?@[]^_`{|}~\"";

        return (const uint8_t*)enctab;
    };

    const uint8_t* get_dectab() const
    {
        static const uint8_t dectab[256] = {
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0X3E, 0X5A, 0X3F, 0X40, 0X41, 0X42, 0XFF, 0X43, 0X44, 0X45, 0X46, 0X47, 0XFF, 0X48, 0X49,
            0X34, 0X35, 0X36, 0X37, 0X38, 0X39, 0X3A, 0X3B, 0X3C, 0X3D, 0X4A, 0X4B, 0X4C, 0X4D, 0X4E, 0X4F,
            0X50, 0X00, 0X01, 0X02, 0X03, 0X04, 0X05, 0X06, 0X07, 0X08, 0X09, 0X0A, 0X0B, 0X0C, 0X0D, 0X0E,
            0X0F, 0X10, 0X11, 0X12, 0X13, 0X14, 0X15, 0X16, 0X17, 0X18, 0X19, 0X51, 0XFF, 0X52, 0X53, 0X54,
            0X55, 0X1A, 0X1B, 0X1C, 0X1D, 0X1E, 0X1F, 0X20, 0X21, 0X22, 0X23, 0X24, 0X25, 0X26, 0X27, 0X28,
            0X29, 0X2A, 0X2B, 0X2C, 0X2D, 0X2E, 0X2F, 0X30, 0X31, 0X32, 0X33, 0X56, 0X57, 0X58, 0X59, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
            0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF,
        };

        return dectab;
    };
};

template <typename T_DATA> class base91_algo
{
  protected:
    T_DATA data;

  public:
    error encode(const std::string in, std::string& out) const
    {
        unsigned long  queue  = 0;
        unsigned int   nbits  = 0;
        const uint8_t* enctab = data.get_enctab();

        size_t len = in.length();
        out.reserve((len + 3) / 4 * 5);
        out = "";

        for (size_t i = 0; i < len; i++) {
            queue |= (uint8_t)in[i] << nbits;
            nbits += 8;
            if (nbits > 13) { /* enough bits in queue */
                unsigned int val = queue & 8191;

                if (val > 88) {
                    queue >>= 13;
                    nbits -= 13;
                }
                else { /* we can take 14 bits */
                    val = queue & 16383;
                    queue >>= 14;
                    nbits -= 14;
                }
                out += enctab[val % 91];
                out += enctab[val / 91];
            }
        }
        if (nbits != 0) {
            out += enctab[queue % 91];
            if (nbits > 7 || queue > 90) {
                out += enctab[queue / 91];
            }
        }

        return error();
    };

    error decode(const std::string in, std::string& out) const
    {
        error          err;
        const uint8_t* dectab = data.get_dectab();

        size_t len = in.length();
        out.reserve((len + 4) / 5 * 4);
        out = "";

        unsigned int c     = 0;
        int          val   = -1;
        unsigned int queue = 0;
        size_t       nbits = 0;
        for (int i = 0; i < len; i++) {
            c = dectab[in[i]];
            if (c == 0xFF) {
                err = "Invalid character found in the input";
                break;
            }
            if (val < 0) {
                val = c;
            }
            else {
                val += c * 91;
                queue |= val << nbits;
                nbits += (val & 8191) > 88 ? 13 : 14;
                do {
                    out += (uint8_t)(queue & 0xFF);
                    queue >>= 8;
                    nbits -= 8;
                } while (nbits > 7);
                val = -1;
            }
        }
        if (val + 1 != 0) {
            out += (uint8_t)((queue | val << nbits) & 0xFF);
        }

        return err;
    };
};

class base91 : public base91_algo<base91_data>
{
};

} // namespace DECENC

#endif // _DECENC_BASE91_HPP
