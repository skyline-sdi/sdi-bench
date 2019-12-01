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
 * $Id: sdi-index.cpp 497 2019-12-01 08:36:53Z li $
 */

#include <iostream>
#include "sdi-db.h"
#include "sdi-entry.h"
#include "sdi-index.h"
#include "sort.h"

namespace sdibench {

index::index(db &db) : D_(db), cardinality_(db.height()), dimensionality_(db.width()) {
  I_ = new block<entry>(dimensionality_, cardinality_);
  O_ = new block<size_t>(cardinality_, dimensionality_ + 2);
  S_ = new block<V *>(dimensionality_, cardinality_);
  skyline_ = new size_t[dimensionality_];
  for (size_t d = 0; d < dimensionality_; ++d) {
    skyline_[d] = 0;
  }
  stop_ = new bool[dimensionality_];
  for (size_t d = 0; d < dimensionality_; ++d) {
    stop_[d] = false;
  }
}

index::~index() {
  delete I_;
  delete O_;
  delete S_;
  delete[] skyline_;
  delete[] stop_;
}

size_t index::best() {
  size_t b = 0;
  for (size_t d = 0; d < dimensionality_; ++d) {
    if (stop_[d]) {
      continue;
    }
    if (skyline_[d] < skyline_[b]) {
      b = d;
    }
  }
  return b;
}

void index::build() {
  auto &I = *I_;
  auto &O = *O_;
  auto &S = *S_;
  for (size_t i = 0; i < cardinality_; ++i) {
    auto row = D_(i);
    for (size_t d = 0; d < dimensionality_; ++d) {
      I(d, i).key = i;
      I(d, i).value = row[d];
    }
  }
  for (size_t d = 0; d < dimensionality_; ++d) {
    msort(I(d), cardinality_);
  }
  for (size_t d = 0; d < dimensionality_; ++d) {
    for (size_t i = 0; i < cardinality_; ++i) {
      auto row = O(I(d, i).key);
      auto &max = row[dimensionality_];
      auto &mean = row[dimensionality_ + 1];
      row[d] = i;
      if (max < i) {
        max = i;
      }
      mean += i;
    }
  }
}

bool index::dominate(size_t d, K key) {
  auto &&s = (*S_)(d);
  for (size_t i = 0; i < cardinality_; ++i) {
    if (!s[i]) {
      return false;
    }
    if (D_.dominate(s[i], key)) {
      return true;
    }
  }
  return false;
}

void index::dump(std::ostream &out) {
  auto &I = *I_;
  for (size_t i = 0; i < cardinality_; ++i) {
    out << I(0, i).key << ":" << I(0, i).value;
    for (size_t d = 1; d < dimensionality_; ++d) {
      out << " " << I(d, i).key << ":" << I(d, i).value;
    }
    out << std::endl;
  }
}

size_t index::height() const {
  return cardinality_;
}

size_t *index::offsets(K key) const {
  return (*O_)(key);
}

void index::skyline(size_t d, K key) {
  (*S_)(d)[skyline_[d]++] = D_(key);
}

void index::stop() {
  for (size_t d = 0; d < dimensionality_; ++d) {
    stop_[d] = false;
  }
}

size_t index::stop(size_t dimension) {
  stop_[dimension] = true;
  size_t s = 0;
  for (size_t d = 0; d < dimensionality_; ++d) {
    if (stop_[d]) {
      ++s;
    }
  }
  return s;
}

size_t index::width() const {
  return dimensionality_;
}

entry *index::operator()(size_t dimension) {
  return (*I_)(dimension);
}

entry *index::operator()(size_t dimension) const {
  return (*I_)(dimension);
}

}
