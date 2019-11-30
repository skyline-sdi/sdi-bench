/*-
 * Copyright (c) 2019 Rui Liu and Dominique Li <dominique.li@univ-tours.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: log.h 998 2014-12-18 12:07:14Z li $
 */

#ifndef SDI_SORT_H
#define SDI_SORT_H

#include "sdi-types.h"

namespace sdibench {

template<class _T>
void msort(_T *, size_t);
template<class _T>
void msort(_T *, size_t, bool);
template<class _T>
void msort(_T *, size_t, size_t, _T *);
template<class _T>
void merge(_T *, size_t, size_t, size_t, _T *);
template<class _T>
void merge(_T *, size_t, _T *, size_t, _T *);

template<class _T>
void msort(_T *a, size_t length) {
  msort(a, length, false);
}

template<class _T>
void msort(_T *a, size_t length, bool unique) {
  if (!length)
    return;
  _T *tmp = new _T[length];
  msort<_T>(a, 0, length - 1, tmp);
  if (unique) {
    for (size_t i = 0; i < length; ++i)
      *tmp++ = *a++;
  }
  delete[] tmp;
}

template<class _T>
void msort(_T *a, size_t first, size_t last, _T *tmp) {
  if (first < last) {
    size_t middle = (first + last) / 2;
    msort(a, first, middle, tmp);
    msort(a, middle + 1, last, tmp);
    merge(a, first, middle, last, tmp);
  }
}

/**
 * Merge two sorted sub-blocks x[lo, mi] and x(mi, hi] to the block z.
 * The size of block z should be at least hi - lo.
 */
template<class _T>
void merge(_T *x, size_t lo, size_t mi, size_t hi, _T *z) {
  merge(x + lo, mi - lo + 1, x + mi + 1, hi - mi, z);
  for (size_t i = 0; i <= hi - lo; ++i)
    *(x + lo + i) = *(z + i);
}

/**
 * Merge two sorted blocks x of size m and y of size n to the block z.
 * The size of block z should be at least m + n.
 */
template<class _T>
void merge(_T *x, size_t m, _T *y, size_t n, _T *z) {
  size_t i = 0;
  size_t j = 0;
  for (; i < m && j < n;) {
    if (*x < *y) {
      *z++ = *x++;
      ++i;
    } else {
      *z++ = *y++;
      ++j;
    }
  }
  for (; i < m; ++i)
    *z++ = *x++;
  for (; j < n; ++j)
    *z++ = *y++;
}

}

#endif //SDI_SORT_H
