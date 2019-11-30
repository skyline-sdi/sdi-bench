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

#ifndef SDI_INDEX_H
#define SDI_INDEX_H

#define INDEX_BUFFER 4096

#include <map>
#include <vector>
#include "sdi-block.h"
#include "sdi-db.h"
#include "sdi-entry.h"

namespace sdibench {

class index {
public:
  typedef typename std::vector<entry>::iterator iterator;
  explicit index(db &);
  virtual ~index();
  size_t best();
  void build();
  bool dominate(size_t, K);
  void dump(std::ostream &);
  size_t height() const;
  size_t *offsets(K) const;
  void skyline(size_t, K);
  void stop();
  size_t stop(size_t);
  size_t width() const;
  entry *operator()(size_t);
  entry *operator()(size_t) const;
private:
  db &D_; // The database D.
  block<entry> *I_; // The dimension index I.
  block<size_t> *O_; // The offset list O.
  block<V *> *S_; // The dimensional skyline S.
  size_t cardinality_ = 0;
  size_t dimensionality_ = 0;
  size_t *skyline_ = nullptr;
  bool *stop_ = nullptr;
};

}

#endif //SDI_INDEX_H
