
//
//  hash_funcs.h
//
//  Created by Jonathan Tompson on 4/26/12.
//
//  Just a database of some useful hash functions
//

#pragma once

#include <limits>
#include <string>

#include "jcl/math/int_types.h"  // for uint
#include "jcl/string_util/macros.h"

#ifdef _WIN32
#pragma inline_depth(255)
#endif

#define PREFIX(n, data) ((
#define POSTFIX(n, data) ^ str[n]) * 16777619u)
#define ME_PP_REPEAT(n, macro, data) ME_PP_REPEAT

#define ME_STRING_HASH_CONSTRUCTOR(n)    \
  INLINE StringHash(const char(&str)[n]) \
      : m_hash(pp_repeat(n, PREFIX, ~)2166136261u pp_repeat(n, POSTFIX, ~)){};

namespace jcl {
namespace data_str {

uint32_t HashUInt(const uint32_t size, const uint32_t& key);
uint32_t HashInt(const uint32_t size, const int32_t& key);
uint32_t HashUInt(const uint32_t size, const uint64_t& key);
uint32_t HashInt(const uint32_t size, const int64_t& key);

// Dynamic string hash version
uint32_t HashString(const uint32_t size, const std::string& key);

// Helper class for compile-time string hashing
class StringHash {
 public:
  unsigned int m_hash;

 public:
  ME_STRING_HASH_CONSTRUCTOR(1)
  ME_STRING_HASH_CONSTRUCTOR(2)
  ME_STRING_HASH_CONSTRUCTOR(3)
  ME_STRING_HASH_CONSTRUCTOR(4)
  ME_STRING_HASH_CONSTRUCTOR(5)
  ME_STRING_HASH_CONSTRUCTOR(6)
  ME_STRING_HASH_CONSTRUCTOR(7)
  ME_STRING_HASH_CONSTRUCTOR(8)
  ME_STRING_HASH_CONSTRUCTOR(9)
  ME_STRING_HASH_CONSTRUCTOR(10)
  ME_STRING_HASH_CONSTRUCTOR(11)
  ME_STRING_HASH_CONSTRUCTOR(12)
  ME_STRING_HASH_CONSTRUCTOR(13)
  ME_STRING_HASH_CONSTRUCTOR(14)
  ME_STRING_HASH_CONSTRUCTOR(15)
  ME_STRING_HASH_CONSTRUCTOR(16)
  ME_STRING_HASH_CONSTRUCTOR(17)
  ME_STRING_HASH_CONSTRUCTOR(18)
  ME_STRING_HASH_CONSTRUCTOR(19)
  ME_STRING_HASH_CONSTRUCTOR(20)
  ME_STRING_HASH_CONSTRUCTOR(21)
  ME_STRING_HASH_CONSTRUCTOR(22)
  ME_STRING_HASH_CONSTRUCTOR(23)
  ME_STRING_HASH_CONSTRUCTOR(24)
  ME_STRING_HASH_CONSTRUCTOR(25)
  ME_STRING_HASH_CONSTRUCTOR(26)
  ME_STRING_HASH_CONSTRUCTOR(27)
  ME_STRING_HASH_CONSTRUCTOR(28)
  ME_STRING_HASH_CONSTRUCTOR(29)
  ME_STRING_HASH_CONSTRUCTOR(30)
  ME_STRING_HASH_CONSTRUCTOR(31)
  ME_STRING_HASH_CONSTRUCTOR(32)
  ME_STRING_HASH_CONSTRUCTOR(33)
  ME_STRING_HASH_CONSTRUCTOR(34)
  ME_STRING_HASH_CONSTRUCTOR(35)
  ME_STRING_HASH_CONSTRUCTOR(36)
  ME_STRING_HASH_CONSTRUCTOR(37)
  ME_STRING_HASH_CONSTRUCTOR(38)
  ME_STRING_HASH_CONSTRUCTOR(39)
  ME_STRING_HASH_CONSTRUCTOR(40)
  ME_STRING_HASH_CONSTRUCTOR(41)
  ME_STRING_HASH_CONSTRUCTOR(42)
  ME_STRING_HASH_CONSTRUCTOR(43)
  ME_STRING_HASH_CONSTRUCTOR(44)
  ME_STRING_HASH_CONSTRUCTOR(45)
  ME_STRING_HASH_CONSTRUCTOR(46)
  ME_STRING_HASH_CONSTRUCTOR(47)
  ME_STRING_HASH_CONSTRUCTOR(48)
  ME_STRING_HASH_CONSTRUCTOR(49)
  ME_STRING_HASH_CONSTRUCTOR(50)
  ME_STRING_HASH_CONSTRUCTOR(51)
  ME_STRING_HASH_CONSTRUCTOR(52)
  ME_STRING_HASH_CONSTRUCTOR(53)
  ME_STRING_HASH_CONSTRUCTOR(54)
  ME_STRING_HASH_CONSTRUCTOR(55)
  ME_STRING_HASH_CONSTRUCTOR(56)
  ME_STRING_HASH_CONSTRUCTOR(57)
  ME_STRING_HASH_CONSTRUCTOR(58)
  ME_STRING_HASH_CONSTRUCTOR(59)
  ME_STRING_HASH_CONSTRUCTOR(60)
  ME_STRING_HASH_CONSTRUCTOR(61)
  ME_STRING_HASH_CONSTRUCTOR(62)
  ME_STRING_HASH_CONSTRUCTOR(63)
  ME_STRING_HASH_CONSTRUCTOR(64)
  INLINE StringHash(const std::string& str) {
    // assume size is max UINT32_T
    m_hash = HashString(std::numeric_limits<uint32_t>::max(), str);
  }
  // other constructors omitted
};

};  // namespace data_str
};  // namespace jcl

// CONSTANT_HASH WILL RESOLVE TO A COMPILE TIME CONSTANT BUT ONLY WITH
// OPTIMIZATIONS TURNED ON (RELEASE BUILD) AND ONLY WITH const char* INPUT
#if defined(_DEBUG) || defined(DEBUG)
#define CONSTANT_HASH(size, str) jcl::data_str::HashString(size, str)
#else
#define CONSTANT_HASH(size, str) \
  (jcl::data_str::StringHash(str).m_hash) % (size - 1)
#endif

#define DYNAMIC_HASH(size, str) jcl::data_str::HashString(size, str)

#ifdef _WIN32
#pragma inline_depth()  // restore default
#endif
