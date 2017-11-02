#ifndef PUFFS_FLATE_H
#define PUFFS_FLATE_H

// Code generated by puffs-c. DO NOT EDIT.

#ifndef PUFFS_BASE_HEADER_H
#define PUFFS_BASE_HEADER_H

// Copyright 2017 The Puffs Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// Puffs requires a word size of at least 32 bits because it assumes that
// converting a u32 to usize will never overflow. For example, the size of a
// decoded image is often represented, explicitly or implicitly in an image
// file, as a u32, and it is convenient to compare that to a buffer size.
//
// Similarly, the word size is at most 64 bits because it assumes that
// converting a usize to u64 will never overflow.
#if __WORDSIZE < 32
#error "Puffs requires a word size of at least 32 bits"
#elif __WORDSIZE > 64
#error "Puffs requires a word size of at most 64 bits"
#endif

// PUFFS_VERSION is the major.minor version number as a uint32. The major
// number is the high 16 bits. The minor number is the low 16 bits.
//
// The intention is to bump the version number at least on every API / ABI
// backwards incompatible change.
//
// For now, the API and ABI are simply unstable and can change at any time.
//
// TODO: don't hard code this in base-header.h.
#define PUFFS_VERSION (0x00001)

// PUFFS_USE_NO_OP_PERFORMANCE_HACKS enables code paths that look like
// redundant no-ops, but for reasons to be investigated, can have dramatic
// performance effects with gcc 4.8.4 (e.g. 1.2x on some benchmarks).
//
// TODO: investigate; delete these hacks (without regressing performance).
// The order matters here. Clang also defines "__GNUC__".
#if defined(__clang__)
#elif defined(__GNUC__)
#define PUFFS_USE_NO_OP_PERFORMANCE_HACKS 1
#endif

// puffs_base__slice_u8 is a 1-dimensional buffer (a pointer and length).
//
// A value with all fields NULL or zero is a valid, empty slice.
typedef struct {
  uint8_t* ptr;
  size_t len;
} puffs_base__slice_u8;

// puffs_base__buf1 is a 1-dimensional buffer (a pointer and length), plus
// additional indexes into that buffer, plus an opened / closed flag.
//
// A value with all fields NULL or zero is a valid, empty buffer.
typedef struct {
  uint8_t* ptr;  // Pointer.
  size_t len;    // Length.
  size_t wi;     // Write index. Invariant: wi <= len.
  size_t ri;     // Read  index. Invariant: ri <= wi.
  bool closed;   // No further writes are expected.
} puffs_base__buf1;

#ifdef PUFFS_USE_NO_OP_PERFORMANCE_HACKS
typedef struct {
  void* always_null0;
  void* always_null1;
} puffs_base__paired_nulls;
#endif

typedef struct {
  // TODO: move buf into private_impl? As it is, it looks like users can modify
  // the buf field to point to a different buffer, which can turn the limit and
  // mark fields into dangling pointers.
  puffs_base__buf1* buf;
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    uint8_t* limit;
    uint8_t* mark;
    // use_limit is redundant, in that it always equals (limit != NULL), but
    // having a separate bool can have a significant performance effect with
    // gcc 4.8.4 (e.g. 1.1x on some benchmarks).
    bool use_limit;
#ifdef PUFFS_USE_NO_OP_PERFORMANCE_HACKS
    struct {
      puffs_base__paired_nulls* noph0;
      uint32_t noph1;
    } * no_op_performance_hacks;
#endif
  } private_impl;
} puffs_base__reader1;

typedef struct {
  // TODO: move buf into private_impl? As it is, it looks like users can modify
  // the buf field to point to a different buffer, which can turn the limit and
  // mark fields into dangling pointers.
  puffs_base__buf1* buf;
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so.
  struct {
    uint8_t* limit;
    uint8_t* mark;
    // use_limit is redundant, in that it always equals (limit != NULL), but
    // having a separate bool can have a significant performance effect with
    // gcc 4.8.4 (e.g. 1.1x on some benchmarks).
    bool use_limit;
  } private_impl;
} puffs_base__writer1;

#endif  // PUFFS_BASE_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- Status Codes

// Status codes are int32_t values:
//  - the sign bit indicates a non-recoverable status code: an error
//  - bits 10-30 hold the packageid: a namespace
//  - bits 8-9 are reserved
//  - bits 0-7 are a package-namespaced numeric code
//
// Do not manipulate these bits directly. Use the API functions such as
// puffs_flate__status__is_error instead.
typedef int32_t puffs_flate__status;

#define puffs_flate__packageid 967230  // 0x000ec23e

#define PUFFS_FLATE__STATUS_OK 0                               // 0x00000000
#define PUFFS_FLATE__ERROR_BAD_PUFFS_VERSION -2147483647       // 0x80000001
#define PUFFS_FLATE__ERROR_BAD_RECEIVER -2147483646            // 0x80000002
#define PUFFS_FLATE__ERROR_BAD_ARGUMENT -2147483645            // 0x80000003
#define PUFFS_FLATE__ERROR_INITIALIZER_NOT_CALLED -2147483644  // 0x80000004
#define PUFFS_FLATE__ERROR_CLOSED_FOR_WRITES -2147483643       // 0x80000005
#define PUFFS_FLATE__ERROR_UNEXPECTED_EOF -2147483642          // 0x80000006
#define PUFFS_FLATE__SUSPENSION_SHORT_READ 7                   // 0x00000007
#define PUFFS_FLATE__SUSPENSION_SHORT_WRITE 8                  // 0x00000008

#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_CODE_OVER_SUBSCRIBED \
  -1157040128  // 0xbb08f800
#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_CODE_UNDER_SUBSCRIBED \
  -1157040127  // 0xbb08f801
#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_CODE_LENGTH_COUNT \
  -1157040126  // 0xbb08f802
#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_CODE_LENGTH_REPETITION \
  -1157040125                                            // 0xbb08f803
#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_CODE -1157040124  // 0xbb08f804
#define PUFFS_FLATE__ERROR_BAD_HUFFMAN_MINIMUM_CODE_LENGTH \
  -1157040123                                                   // 0xbb08f805
#define PUFFS_FLATE__ERROR_BAD_DISTANCE -1157040122             // 0xbb08f806
#define PUFFS_FLATE__ERROR_BAD_DISTANCE_CODE_COUNT -1157040121  // 0xbb08f807
#define PUFFS_FLATE__ERROR_BAD_FLATE_BLOCK -1157040120          // 0xbb08f808
#define PUFFS_FLATE__ERROR_BAD_LITERAL_LENGTH_CODE_COUNT \
  -1157040119                                             // 0xbb08f809
#define PUFFS_FLATE__ERROR_CHECKSUM_MISMATCH -1157040118  // 0xbb08f80a
#define PUFFS_FLATE__ERROR_INCONSISTENT_STORED_BLOCK_LENGTH \
  -1157040117  // 0xbb08f80b
#define PUFFS_FLATE__ERROR_INTERNAL_ERROR_INCONSISTENT_HUFFMAN_DECODER_STATE \
  -1157040116  // 0xbb08f80c
#define PUFFS_FLATE__ERROR_INTERNAL_ERROR_INCONSISTENT_N_BITS \
  -1157040115                                                     // 0xbb08f80d
#define PUFFS_FLATE__ERROR_MISSING_END_OF_BLOCK_CODE -1157040114  // 0xbb08f80e
#define PUFFS_FLATE__ERROR_NO_HUFFMAN_CODES -1157040113           // 0xbb08f80f
#define PUFFS_FLATE__ERROR_INVALID_ZLIB_COMPRESSION_METHOD \
  -1157040112  // 0xbb08f810
#define PUFFS_FLATE__ERROR_INVALID_ZLIB_COMPRESSION_WINDOW_SIZE \
  -1157040111                                                     // 0xbb08f811
#define PUFFS_FLATE__ERROR_INVALID_ZLIB_PARITY_CHECK -1157040110  // 0xbb08f812
#define PUFFS_FLATE__ERROR_TODO_UNSUPPORTED_ZLIB_PRESET_DICTIONARY \
  -1157040109  // 0xbb08f813

bool puffs_flate__status__is_error(puffs_flate__status s);

const char* puffs_flate__status__string(puffs_flate__status s);

// ---------------- Public Consts

// ---------------- Structs

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so. Instead, use the
  // puffs_flate__flate_decoder__etc functions.
  //
  // In C++, these fields would be "private", but C does not support that.
  //
  // It is a struct, not a struct*, so that it can be stack allocated.
  struct {
    puffs_flate__status status;
    uint32_t magic;

    uint32_t f_bits;
    uint32_t f_n_bits;
    uint32_t f_huffs[2][1234];
    uint32_t f_n_huffs_bits[2];
    uint8_t f_history[32768];
    uint32_t f_history_index;
    uint8_t f_code_lengths[320];

    struct {
      uint32_t coro_susp_point;
      puffs_flate__status v_z;
      uint64_t v_n_copied;
      uint32_t v_already_full;
    } c_decode[1];
    struct {
      uint32_t coro_susp_point;
      uint32_t v_final;
      uint32_t v_type;
    } c_decode_blocks[1];
    struct {
      uint32_t coro_susp_point;
      uint32_t v_length;
      uint32_t v_n_copied;
      uint64_t scratch;
    } c_decode_uncompressed[1];
    struct {
      uint32_t coro_susp_point;
      uint32_t v_bits;
      uint32_t v_n_bits;
      uint32_t v_table_entry;
      uint32_t v_table_entry_n_bits;
      uint32_t v_lmask;
      uint32_t v_dmask;
      uint32_t v_redir_top;
      uint32_t v_redir_mask;
      uint32_t v_length;
      uint32_t v_distance;
      uint32_t v_n_copied;
      uint32_t v_hlen;
      uint32_t v_hdist;
    } c_decode_huffman[1];
    struct {
      uint32_t coro_susp_point;
      uint32_t v_i;
    } c_init_fixed_huffman[1];
    struct {
      uint32_t coro_susp_point;
      uint32_t v_bits;
      uint32_t v_n_bits;
      uint32_t v_n_lit;
      uint32_t v_n_dist;
      uint32_t v_n_clen;
      uint32_t v_i;
      uint32_t v_mask;
      uint32_t v_table_entry;
      uint32_t v_table_entry_n_bits;
      uint32_t v_n_extra_bits;
      uint8_t v_rep_symbol;
      uint32_t v_rep_count;
    } c_init_dynamic_huffman[1];
  } private_impl;
} puffs_flate__flate_decoder;

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so. Instead, use the
  // puffs_flate__adler32__etc functions.
  //
  // In C++, these fields would be "private", but C does not support that.
  //
  // It is a struct, not a struct*, so that it can be stack allocated.
  struct {
    puffs_flate__status status;
    uint32_t magic;

    uint32_t f_state;

  } private_impl;
} puffs_flate__adler32;

typedef struct {
  // Do not access the private_impl's fields directly. There is no API/ABI
  // compatibility or safety guarantee if you do so. Instead, use the
  // puffs_flate__zlib_decoder__etc functions.
  //
  // In C++, these fields would be "private", but C does not support that.
  //
  // It is a struct, not a struct*, so that it can be stack allocated.
  struct {
    puffs_flate__status status;
    uint32_t magic;

    puffs_flate__flate_decoder f_flate;
    puffs_flate__adler32 f_adler;

    struct {
      uint32_t coro_susp_point;
      uint16_t v_x;
      uint32_t v_checksum;
      puffs_flate__status v_z;
      uint64_t scratch;
    } c_decode[1];
  } private_impl;
} puffs_flate__zlib_decoder;

// ---------------- Public Initializer Prototypes

// puffs_flate__flate_decoder__initialize is an initializer function.
//
// It should be called before any other puffs_flate__flate_decoder__* function.
//
// Pass PUFFS_VERSION and 0 for puffs_version and for_internal_use_only.
void puffs_flate__flate_decoder__initialize(puffs_flate__flate_decoder* self,
                                            uint32_t puffs_version,
                                            uint32_t for_internal_use_only);

// puffs_flate__zlib_decoder__initialize is an initializer function.
//
// It should be called before any other puffs_flate__zlib_decoder__* function.
//
// Pass PUFFS_VERSION and 0 for puffs_version and for_internal_use_only.
void puffs_flate__zlib_decoder__initialize(puffs_flate__zlib_decoder* self,
                                           uint32_t puffs_version,
                                           uint32_t for_internal_use_only);

// ---------------- Public Function Prototypes

puffs_flate__status puffs_flate__flate_decoder__decode(
    puffs_flate__flate_decoder* self,
    puffs_base__writer1 a_dst,
    puffs_base__reader1 a_src);

puffs_flate__status puffs_flate__zlib_decoder__decode(
    puffs_flate__zlib_decoder* self,
    puffs_base__writer1 a_dst,
    puffs_base__reader1 a_src);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // PUFFS_FLATE_H
