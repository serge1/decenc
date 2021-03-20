# DECENC

![CMake](https://github.com/serge1/decenc/workflows/CMake/badge.svg)

Binary-to-text encoding algorithms implemented in C++

The header-only C++ library implements arbitrary binary data encoding and decoding
to a number of ASCII formats in accordance to binary-to-text encoding schemes.

---

## The Project Purpose

- One-stop implementation for different binary-to-text schemes
- Encoding/Decoding algorithms for both Internet and computer/EEPROM/Flash memory initialization
- Modern realization in C++ language
- Ease of use
- Permissive licensing
- Portability

## The Project Priorities

- Correctness
- Usage simplicity
- Adherence to C++ standards
- Efficiency

## The Project Name

The project name is built from words "Decoding" and "Encoding". But, I like to
pronounce the name of the project as **"decens"** - a Latin word
meaning "fitting", "appropriate", "worthy", "decent", "well-formed".

## DECENC Design

The project implements a number of well-known encoding schemes.
Due to the "header-only" and "template" nature of the implementation, only those
algorithms that used in an application will be part of compiled executable file.

The main class *decenc* instantiates algorithm passed as a template parameter
providing static polymorphism.

## DECENC Usage

    #include <decenc/decenc.hpp>

    using namespace DECENC;               // Use DECENC namespace

    ...
    std::string in("Hello");
    std::string out;
    ...
    decenc<base64> coder;                 // Choose encoding scheme
    error err = coder.encode(in, out);    // Do encoding
    if (!err.is_ok()) ...                 // Check for errors
    ...
    coder.decode(out, in);                // Do decoding
    if (!err.is_ok()) ...                 // Check for errors
    ...

---

## Supported Schemes

| Scheme | Class Name | Reference |
| --- | --- | --- |
| Base16 | base16 | - [RFC 4648](https://tools.ietf.org/html/rfc4648)
| Base32 | base32 | - [RFC 4648](https://tools.ietf.org/html/rfc4648)
| Base32Hex | base32hex | - [RFC 4648](https://tools.ietf.org/html/rfc4648)
| Base64 | base64 | - [RFC 4648](https://tools.ietf.org/html/rfc4648)
| Base64URL | base64url | - [RFC 4648](https://tools.ietf.org/html/rfc4648)
| Base91 | base91 | - [basE91 encoding](http://base91.sourceforge.net/)<br/> - [A Proposal of Substitute for Base85/64 – Base91](http://www.iiis.org/CDs2010/CD2010SCI/CCCT_2010/PapersPdf/TB100QM.pdf)

---

## Other links

[Wikipedia "Binary-to-text encoding" article](https://en.wikipedia.org/wiki/Binary-to-text_encoding)

[Wikipedia: Base64 variants](https://en.wikipedia.org/wiki/Base64)

[Wikipedia: Base32 variants](https://en.wikipedia.org/wiki/Base32)
