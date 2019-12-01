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
 * $Id: sdi-db.h 497 2019-12-01 08:36:53Z li $
 */

#ifndef SDI_DB_H
#define SDI_DB_H

#include <iostream>
#include "sdi-types.h"

namespace sdibench {

class db {
  friend std::istream &operator>>(std::istream &, db &);
  friend std::ostream &operator<<(std::ostream &, const db &);
public:
  static size_t DT; // Dominance Test count
  static size_t IO; // IO count
  static size_t SKY; // Skyline size
  static size_t STOP; // Stop line count
  static size_t TT; // Tested tuple count
  db() = default;
  explicit db(size_t, size_t);
  virtual ~db();
  bool dominate(V *, V *);
  bool dominate(V *, size_t);
  bool dominate(size_t, size_t);
  bool empty();
  size_t height() const;
  size_t length() const;
  size_t size() const;
  bool skipped(size_t) const;
  void skipped(size_t, bool);
  bool skyline(size_t) const;
  void skyline(size_t, bool);
  bool tested(size_t) const;
  void tested(size_t, bool);
  size_t width() const;
  V *operator()(size_t);
  V *operator()(size_t) const;
  V &operator[](size_t);
  V &operator[](size_t) const;
private:
  void put_(V);
  V *data_ = nullptr;
  size_t height_ = 0;
  size_t length_ = 0;
  size_t width_ = 0;
};

}

#endif //SDI_DB_H
