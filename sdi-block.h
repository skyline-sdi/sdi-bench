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
 * $Id: sdi-block.h 566 2019-12-23 15:12:34Z li $
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
  auto height() const -> size_t;
  auto width() const -> size_t;
  auto operator()(size_t) -> _T *;
  auto operator()(size_t) const -> _T *;
  auto operator()(size_t, size_t) -> _T &;
  auto operator()(size_t, size_t) const -> _T &;
  auto operator[](size_t) -> _T &;
  auto operator[](size_t) const -> _T &;
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
auto block<_T>::height() const -> size_t {
  return height_;
}

template<class _T>
auto block<_T>::width() const -> size_t {
  return width_;
}

template<class _T>
auto block<_T>::operator()(size_t row) -> _T * {
  return &block_[row * width_];
}

template<class _T>
auto block<_T>::operator()(size_t row) const -> _T * {
  return &block_[row * width_];
}

template<class _T>
auto block<_T>::operator()(size_t row, size_t offset) -> _T & {
  return block_[row * width_ + offset];
}

template<class _T>
auto block<_T>::operator()(size_t row, size_t offset) const -> _T & {
  return block_[row * width_ + offset];
}

template<class _T>
auto block<_T>::operator[](size_t index) -> _T & {
  return block_[index];
}

template<class _T>
auto block<_T>::operator[](size_t index) const -> _T & {
  return block_[index];
}

}

#endif //SDI_BLOCK_H
