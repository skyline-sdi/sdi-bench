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
 * $Id: sdi.cpp 497 2019-12-01 08:36:53Z li $
 */

#include <list>
#include <vector>
#include "sdi.h"

namespace sdibench {

sdi::sdi(size_t cardinality, size_t dimensionality) : D_(cardinality, dimensionality), I_(D_) {
  cardinality_ = cardinality;
  dimensionality_ = dimensionality;
  max_ = dimensionality_;
  mean_ = dimensionality_ + 1;
}

void sdi::build(std::istream &in) {
  in >> D_;
  I_.build();
}

void sdi::query() {
  auto &D = D_;
  auto &I = I_;
  auto &S = S_;
  std::vector<entry *> block;
  std::vector<size_t> its(dimensionality_, 0);
  std::vector<V> itv(dimensionality_);
  for (size_t d = 0; d < dimensionality_; ++d) {
    itv[d] = I(d)->value;
  }
  bool stop = false;
#ifndef WITHOUT_STOPLINE
  stop_ = 0;
  stopline_ = nullptr;
  stopped_ = 0;
#endif
  // The main loop.
  for (;;) {
    // Dimension switching loop.
    size_t d = I.best();
#ifndef WITHOUT_STOPLINE
    if (stopped_ >= dimensionality_) {
      break;
    }
#endif
    // Go ahead.
    while (its[d] < cardinality_) {
      auto dp = its[d]++;
      if (its[d] == cardinality_) {
        stop = true;
      }
      auto &&e = I(d)[dp];
      // If current tuple is skipped, just ignore it; however, if it is the
      // last one, do block skyline commit and automatically quit the loop.
      if (D.skipped(e.key)) {
        if (stop) {
          skyline_(block, d);
        }
        continue;
      }
      // Record tested tuples.
      if (!D.tested(e.key)) {
        D.tested(e.key, true);
        ++db::TT;
      }
      // Anyway, if stop, do block skyline commit and quit the loop.
      if (stop) {
        block.push_back(&e);
        skyline_(block, d);
        break;
      }
      // Treat block skyline.
      size_t sky = 0;
      if (e.value == itv[d]) {
        // Add current tuple to the block.
        block.push_back(&e);
      } else {
        // If block changes, do block skyline commit and recreate the block.
        // Roll back current dimension pointer.
        sky = skyline_(block, d);
        block.clear();
        itv[d] = e.value;
        --its[d];
#ifndef WITHOUT_STOPLINE
        if (stopline_ && its[d] > stopline_[d]) {
          stopped_ = I.stop(d);
          break;
        }
#endif
      }
      // If any new skyline tuple is determined, switch dimension.
      if (sky) {
        break;
      }
    }
    if (stop) {
      break;
    }
  }
}

K sdi::better_(K key1, K key2) {
  auto o1 = I_.offsets(key1);
  auto o2 = I_.offsets(key2);
  if (o1[max_] < o2[max_]) {
    return key1;
  } else if (o2[max_] < o1[max_]) {
    return key2;
  }
  if (o1[mean_] < o2[mean_]) {
    return key1;
  } else {
    return key2;
  }
}

size_t sdi::skyline_(std::vector<entry *> &block, size_t d) {
  auto &D = D_;
  auto &I = I_;
  auto &S = S_;
  if (block.size() > 1) {
    for (size_t i = 0; i < block.size() - 1; ++i) {
      if (!block[i]) {
        continue;
      }
      auto ik = block[i]->key;
      if (D.skipped(ik)) {
        block[ik] = nullptr;
        continue;
      }
      auto is = D.skyline(ik);
      for (size_t j = i + 1; j < block.size(); ++j) {
        if (!block[j]) {
          continue;
        }
        auto jk = block[j]->key;
        if (D.skipped(jk)) {
          block[j] = nullptr;
          continue;
        }
        auto js = D.skyline(jk);
        if (is && js) {
          continue;
        } else if (!js && D.dominate(ik, jk)) {
          D.skipped(jk, true);
          block[j] = nullptr;
        } else if (!is && D.dominate(jk, ik)) {
          D.skipped(ik, true);
          block[i] = nullptr;
          break;
        }
      }
    }
  }
  size_t sky = 0;
  for (auto &&x : block) {
    if (!x) {
      continue;
    }
    auto xk = x->key;
    if (!D.skyline(xk)) {
      if (I.dominate(d, xk)) {
        D.skipped(xk, true);
      } else {
        D.skyline(xk, true);
        I.skyline(d, xk);
        S.push_back(xk);
        ++db::SKY;
        ++sky;
#ifndef WITHOUT_STOPLINE
        auto best = stopline_ ? better_(stop_, xk) : xk;
        if (best != stop_) {
          I.stop();
          stop_ = best;
          stopline_ = I.offsets(stop_);
          ++db::STOP;
        }
#endif
      }
    } else {
      I.skyline(d, xk);
    }
  }
  return sky;
}

}
