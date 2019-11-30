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

#ifndef SDI_BLOCK_H
#define SDI_BLOCK_H

#define SDI_DB_BUFFER 4096
#define SDI_DB_PRECISION 8

#include <iostream>
#include "sdi-types.h"

namespace sdibench {

template<class _T>
class block {
public:
  explicit block(size_t, size_t);
  virtual ~block();
  size_t height() const;
  size_t width() const;
  _T *operator()(size_t);
  _T *operator()(size_t) const;
  _T &operator()(size_t, size_t);
  _T &operator()(size_t, size_t) const;
  _T &operator[](size_t);
  _T &operator[](size_t) const;
private:
  _T *block_ = nullptr;
  size_t height_ = 0;
  size_t width_ = 0;
};

template<class _T>
block<_T>::block(size_t height, size_t width) : height_(height), width_(width) {
  block_ = new _T[height_ * width_];
  memset(block_, 0, sizeof(_T) * height_ * width_);
}

template<class _T>
block<_T>::~block() {
  delete[] block_;
}

template<class _T>
size_t block<_T>::height() const {
  return height_;
}

template<class _T>
size_t block<_T>::width() const {
  return width_;
}

template<class _T>
_T *block<_T>::operator()(size_t row) {
  return &block_[row * width_];
}

template<class _T>
_T *block<_T>::operator()(size_t row) const {
  return &block_[row * width_];
}

template<class _T>
_T &block<_T>::operator()(size_t row, size_t offset) {
  return block_[row * width_ + offset];
}

template<class _T>
_T &block<_T>::operator()(size_t row, size_t offset) const {
  return block_[row * width_ + offset];
}

template<class _T>
_T &block<_T>::operator[](size_t index) {
  return block_[index];
}

template<class _T>
_T &block<_T>::operator[](size_t index) const {
  return block_[index];
}

}

#endif //SDI_BLOCK_H
