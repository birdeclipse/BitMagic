#ifndef BMSSE_UTIL__H__INCLUDED__
#define BMSSE_UTIL__H__INCLUDED__
/*
Copyright(c) 2002-2017 Anatoliy Kuznetsov(anatoliy_kuznetsov at yahoo.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

For more information please visit:  http://bitmagic.io
*/



namespace bm
{

/** @defgroup SSE2 SSE2 functions
    Processor specific optimizations for SSE2 instructions (internals)
    @internal
    @ingroup bvector
 */


/*! 
  @brief SSE2 reinitialization guard class

  SSE2 requires to call _mm_empty() if we are intermixing
  MMX integer commands with floating point arithmetics.
  This class guards critical code fragments where SSE2 integer
  is used.

  As of 2015 _mm_empty() is considered deprecated, and not even recognised
  by some compilers (like MSVC) in 64-bit mode.
  As MMX instructions gets old, we here deprecate and comment out 
  use of _mm_empty()

  @ingroup SSE2
*/
class sse_empty_guard
{
public:
    BMFORCEINLINE sse_empty_guard() 
    {
        //_mm_empty();
    }

    BMFORCEINLINE ~sse_empty_guard() 
    {
        //_mm_empty();
    }
};



/*! 
    @brief XOR array elements to specified mask
    *dst = *src ^ mask

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_xor_arr_2_mask(__m128i* BMRESTRICT dst, 
                         const __m128i* BMRESTRICT src, 
                         const __m128i* BMRESTRICT src_end,
                         bm::word_t mask)
{
     __m128i xmm2 = _mm_set_epi32(mask, mask, mask, mask);
     do
     {
        __m128i xmm1 = _mm_load_si128(src);

        xmm1 = _mm_xor_si128(xmm1, xmm2);
        _mm_store_si128(dst, xmm1);
        ++dst;
        ++src;

     } while (src < src_end);
}


/*! 
    @brief Inverts array elements and NOT them to specified mask
    *dst = ~*src & mask

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_andnot_arr_2_mask(__m128i* BMRESTRICT dst, 
                            const __m128i* BMRESTRICT src, 
                            const __m128i* BMRESTRICT src_end,
                            bm::word_t mask)
{
     __m128i xmm2 = _mm_set_epi32(mask, mask, mask, mask);
     do
     {
        //_mm_prefetch((const char*)(src)+1024, _MM_HINT_NTA);
        //_mm_prefetch((const char*)(src)+1088, _MM_HINT_NTA);

        __m128i xmm1 = _mm_load_si128(src);

        xmm1 = _mm_andnot_si128(xmm1, xmm2); // xmm1 = (~xmm1) & xmm2 
        _mm_store_si128(dst, xmm1);
        ++dst;
        ++src;

     } while (src < src_end);
}

/*! 
    @brief AND array elements against another array
    *dst &= *src

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_and_arr(__m128i* BMRESTRICT dst, 
                  const __m128i* BMRESTRICT src, 
                  const __m128i* BMRESTRICT src_end)
{
    __m128i xmm1, xmm2;
    do
    {
        _mm_prefetch((const char*)(src)+512,  _MM_HINT_NTA);
    
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_and_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);
        
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_and_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_and_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_and_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

    } while (src < src_end);

}


/*! 
    @brief OR array elements against another array
    *dst |= *src

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_or_arr(__m128i* BMRESTRICT dst, 
                 const __m128i* BMRESTRICT src, 
                 const __m128i* BMRESTRICT src_end)
{
    __m128i xmm1, xmm2;
    do
    {
        _mm_prefetch((const char*)(src)+512,  _MM_HINT_NTA);
    
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_or_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);
        
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_or_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_or_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_or_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

    } while (src < src_end);
}


/*! 
    @brief OR array elements against another array
    *dst ^= *src

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_xor_arr(__m128i* BMRESTRICT dst, 
                  const __m128i* BMRESTRICT src, 
                  const __m128i* BMRESTRICT src_end)
{
    __m128i xmm1, xmm2;
    do
    {
        _mm_prefetch((const char*)(src)+512,  _MM_HINT_NTA);
    
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_xor_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);
        
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_xor_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_xor_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_xor_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

    } while (src < src_end);
}



/*! 
    @brief AND-NOT (SUB) array elements against another array
    *dst &= ~*src

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_sub_arr(__m128i* BMRESTRICT dst, 
                 const __m128i* BMRESTRICT src, 
                 const __m128i* BMRESTRICT src_end)
{
    __m128i xmm1, xmm2;
    do
    {
        _mm_prefetch((const char*)(src)+512,  _MM_HINT_NTA);
    
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_andnot_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);
        
        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_andnot_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_andnot_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

        xmm1 = _mm_load_si128(src++);
        xmm2 = _mm_load_si128(dst);
        xmm1 = _mm_andnot_si128(xmm1, xmm2);
        _mm_store_si128(dst++, xmm1);

    } while (src < src_end);    
}

/*! 
    @brief SSE2 block memset
    *dst = value

    @ingroup SSE2
*/

BMFORCEINLINE 
void sse2_set_block(__m128i* BMRESTRICT dst, 
                    __m128i* BMRESTRICT dst_end, 
                    bm::word_t value)
{
    __m128i xmm0 = _mm_set_epi32 (value, value, value, value);
    do
    {            
        _mm_store_si128(dst, xmm0);
/*        
        _mm_store_si128(dst+1, xmm0);
        _mm_store_si128(dst+2, xmm0);
        _mm_store_si128(dst+3, xmm0);

        _mm_store_si128(dst+4, xmm0);
        _mm_store_si128(dst+5, xmm0);
        _mm_store_si128(dst+6, xmm0);
        _mm_store_si128(dst+7, xmm0);

        dst += 8;
*/        
    } while (++dst < dst_end);
    
    _mm_sfence();
}



/*! 
    @brief SSE2 block copy
    *dst = *src

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_copy_block(__m128i* BMRESTRICT dst, 
                     const __m128i* BMRESTRICT src, 
                     const __m128i* BMRESTRICT src_end)
{
    __m128i xmm0, xmm1, xmm2, xmm3;
    do
    {
        _mm_prefetch((const char*)(src)+512,  _MM_HINT_NTA);
    
        xmm0 = _mm_load_si128(src+0);
        xmm1 = _mm_load_si128(src+1);
        xmm2 = _mm_load_si128(src+2);
        xmm3 = _mm_load_si128(src+3);
        
        _mm_store_si128(dst+0, xmm0);
        _mm_store_si128(dst+1, xmm1);
        _mm_store_si128(dst+2, xmm2);
        _mm_store_si128(dst+3, xmm3);
        
        xmm0 = _mm_load_si128(src+4);
        xmm1 = _mm_load_si128(src+5);
        xmm2 = _mm_load_si128(src+6);
        xmm3 = _mm_load_si128(src+7);
        
        _mm_store_si128(dst+4, xmm0);
        _mm_store_si128(dst+5, xmm1);
        _mm_store_si128(dst+6, xmm2);
        _mm_store_si128(dst+7, xmm3);
        
        src += 8;
        dst += 8;
        
    } while (src < src_end);    
}

/*! 
    @brief Invert array elements
    *dst = ~*dst
    or
    *dst ^= *dst 

    @ingroup SSE2
*/
BMFORCEINLINE 
void sse2_invert_arr(bm::word_t* first, bm::word_t* last)
{
    __m128i xmm1 = _mm_set_epi32(0xFFFFFFFF, 0xFFFFFFFF, 
                                 0xFFFFFFFF, 0xFFFFFFFF);
    __m128i* wrd_ptr = (__m128i*)first;

    do 
    {
        _mm_prefetch((const char*)(wrd_ptr)+512,  _MM_HINT_NTA);
        
        __m128i xmm0 = _mm_load_si128(wrd_ptr);
        xmm0 = _mm_xor_si128(xmm0, xmm1);
        _mm_store_si128(wrd_ptr, xmm0);
        ++wrd_ptr;
    } while (wrd_ptr < (__m128i*)last);
}

BMFORCEINLINE 
__m128i sse2_and(__m128i a, __m128i b)
{
    return _mm_and_si128(a, b);
}

BMFORCEINLINE 
__m128i sse2_or(__m128i a, __m128i b)
{
    return _mm_or_si128(a, b);
}


BMFORCEINLINE 
__m128i sse2_xor(__m128i a, __m128i b)
{
    return _mm_xor_si128(a, b);
}

BMFORCEINLINE 
__m128i sse2_sub(__m128i a, __m128i b)
{
    return _mm_andnot_si128(b, a);
}


/*!
    @brief Gap block population count (array sum) utility
    @param pbuf - unrolled, aligned to 1-start GAP buffer
    @param sse_vect_waves - number of SSE vector lines to process
    @param sum - result acumulator
    @return tail pointer

    @internal
*/
inline
const bm::gap_word_t* sse2_gap_sum_arr(
    const bm::gap_word_t* BMRESTRICT pbuf,
    unsigned sse_vect_waves,
    unsigned* sum)
{
    __m128i xcnt = _mm_setzero_si128();

    for (unsigned i = 0; i < sse_vect_waves; ++i)
    {
        __m128i mm0 = _mm_loadu_si128((__m128i*)(pbuf - 1));
        __m128i mm1 = _mm_loadu_si128((__m128i*)(pbuf + 8 - 1));
        __m128i mm_s2 = _mm_add_epi16(mm1, mm0);
        xcnt = _mm_add_epi16(xcnt, mm_s2);
        pbuf += 16;
    }
    xcnt = _mm_sub_epi16(_mm_srli_epi32(xcnt, 16), xcnt);

    unsigned short* cnt8 = (unsigned short*)&xcnt;
    *sum += (cnt8[0]) + (cnt8[2]) + (cnt8[4]) + (cnt8[6]);
    return pbuf;
}


} // namespace



#endif
