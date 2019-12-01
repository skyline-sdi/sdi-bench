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
 * $Id: sdi-db.cpp 497 2019-12-01 08:36:53Z li $
 */

#include <array>
#include <cstring>
#include <iomanip>
#include "sdi-db.h"

#define FLAGS 3
#define SKIP 2
#define SKYLINE 1
#define TEST 0

namespace sdibench {

size_t db::DT = 0;
size_t db::IO = 0;
size_t db::SKY = 0;
size_t db::STOP = 0;
size_t db::TT = 0;

std::istream &operator>>(std::istream &in, db &db) {
  auto delim = " ,";
  std::array<char, 4096> line{};
  auto data = line.data();
  while (in.good()) {
    in.getline(data, 4096);
    char *ptr = strtok(data, delim);
    size_t n = 0;
    if (ptr != nullptr) {
      while (n++ < db.width_) {
        db.put_(strtod(ptr, nullptr));
        ptr = strtok(nullptr, delim);
      }
      db.put_(0); // Testing flag.
      db.put_(0); // Skyline flag.
      db.put_(0); // Skip flag.
    }
  }
  return in;
}

std::ostream &operator<<(std::ostream &out, const db &db) {
  size_t n = 0;
  while (n < db.length_) {
    out << std::setprecision(8);
    out << db[n++];
    for (size_t i = 1; i < db.width_; ++i) {
      out << " " << db[n++];
    }
    n++; // Skip testing flag.
    n++; // Skip skyline flag.
    n++; // Skip skipping flag.
    out << std::endl;
  }
  return out;
}

db::db(size_t height, size_t width) : height_(height), width_(width) {
  data_ = new V[height_ * (width_ + FLAGS)];
}

db::~db() {
  delete[] data_;
}

bool db::dominate(V *p1, V *p2) {
  ++DT;
  bool dominating = false;
  for (size_t i = 0; i < width_; ++i, ++p1, ++p2) {
    if (*p1 > *p2) {
      return false;
    } else if (*p1 < *p2 && !dominating) {
      dominating = true;
    }
  }
  return dominating;
}

bool db::dominate(V *p1, size_t row2) {
  return dominate(p1, &data_[row2 * (width_ + FLAGS)]);
}

bool db::dominate(size_t row1, size_t row2) {
  return dominate(&data_[row1 * (width_ + FLAGS)], &data_[row2 * (width_ + FLAGS)]);
}

bool db::empty() {
  return length_ == 0;
}

size_t db::height() const {
  return height_;
}

size_t db::length() const {
  return length_;
}

size_t db::size() const {
  return length_ / (width_ + FLAGS);
}

bool db::skipped(size_t row) const {
  return data_[row * (width_ + FLAGS) + width_ + SKIP] > 0;
}
void db::skipped(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + SKIP] = flag;
}

bool db::skyline(size_t row) const {
  return data_[row * (width_ + FLAGS) + width_ + SKYLINE] > 0;
}

void db::skyline(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + SKYLINE] = flag;
}

bool db::tested(size_t row) const {
  return data_[row * (width_ + FLAGS) + width_ + TEST] > 0;
}
void db::tested(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + TEST] = flag;
}

size_t db::width() const {
  return width_;
}

V *db::operator()(size_t row) {
  ++IO;
  return &data_[row * (width_ + FLAGS)];
}

V *db::operator()(size_t row) const {
  ++IO;
  return &data_[row * (width_ + FLAGS)];
}

V &db::operator[](size_t n) {
  return data_[n];
}

V &db::operator[](size_t n) const {
  return data_[n];
}

void db::put_(V x) {
  data_[length_++] = x;
}

}
