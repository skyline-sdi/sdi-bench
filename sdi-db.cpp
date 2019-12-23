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
 * $Id: sdi-db.cpp 567 2019-12-23 19:21:14Z li $
 */

#include <array>
#include <cstring>
#include <iomanip>
#include "sdi-db.h"

#define FLAGS 5
#define TEST 0
#define SKYLINE 1
#define SKIP 2
#define MIN 3
#define SUM 4

namespace sdibench {

size_t db::DT = 0;
size_t db::DTE = 0;
size_t db::IO = 0;
size_t db::SKY = 0;
size_t db::STOP = 0;
size_t db::TT = 0;

auto operator>>(std::istream &in, db &db) -> std::istream & {
  auto delim = " ,";
  std::array<char, 4096> line{};
  auto data = line.data();
  while (in.good()) {
    in.getline(data, 4096);
    char *ptr = strtok(data, delim);
    size_t n = 0;
    V min = 1;
    V sum = 0;
    V value = 0;
    if (ptr != nullptr) {
      while (n++ < db.width_) {
        value = strtod(ptr, nullptr);
        if (value < min) {
          min = value;
        }
        sum += value;
        db.put_(value);
        ptr = strtok(nullptr, delim);
      }
      db.put_(0); // Testing flag.
      db.put_(0); // Skyline flag.
      db.put_(0); // Skip flag.
      db.put_(min); // Min value.
      db.put_(sum); // Sum value.
      min = 1;
      sum = 0;
    }
  }
  return in;
}

auto operator<<(std::ostream &out, const db &db) -> std::ostream & {
  size_t n = 0;
  while (n < db.length_) {
    out << std::setprecision(8);
    out << db[n++];
    for (size_t i = 1; i < db.width_; ++i) {
      out << " " << db[n++];
    }
    for (size_t i = 0; i < FLAGS; ++i) {
      n++; // Skip flags.
    }
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

auto db::dominate(V *p1, V *p2) -> bool {
  ++DTE;
  if (incomparable(p1, p2)) {
    return false;
  }
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

auto db::dominate(V *p1, size_t row2) -> bool {
  return dominate(p1, &data_[row2 * (width_ + FLAGS)]);
}

auto db::dominate(size_t row1, size_t row2) -> bool {
  return dominate(&data_[row1 * (width_ + FLAGS)], &data_[row2 * (width_ + FLAGS)]);
}

auto db::empty() -> bool {
  return length_ == 0;
}

auto db::height() const -> size_t {
  return height_;
}

auto db::incomparable(const V *s, const V *t) -> bool {
  // Returns true of a skyline s is incomparable with a testing tuple t.
  return !(*(s + width_ + MIN) <= *(t + width_ + MIN) && *(s + width_ + SUM) <= *(t + width_ + SUM));
}

auto db::length() const -> size_t {
  return length_;
}

auto db::size() const -> size_t {
  return length_ / (width_ + FLAGS);
}

auto db::skipped(size_t row) const -> bool {
  return data_[row * (width_ + FLAGS) + width_ + SKIP] > 0;
}
void db::skipped(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + SKIP] = flag;
}

auto db::skyline(size_t row) const -> bool {
  return data_[row * (width_ + FLAGS) + width_ + SKYLINE] > 0;
}

void db::skyline(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + SKYLINE] = flag;
}

auto db::tested(size_t row) const -> bool {
  return data_[row * (width_ + FLAGS) + width_ + TEST] > 0;
}
void db::tested(size_t row, bool flag) {
  data_[row * (width_ + FLAGS) + width_ + TEST] = flag;
}

auto db::width() const -> size_t {
  return width_;
}

auto db::operator()(size_t row) -> V * {
  ++IO;
  return &data_[row * (width_ + FLAGS)];
}

auto db::operator()(size_t row) const -> V * {
  ++IO;
  return &data_[row * (width_ + FLAGS)];
}

auto db::operator[](size_t n) -> V & {
  return data_[n];
}

auto db::operator[](size_t n) const -> V & {
  return data_[n];
}

void db::put_(V x) {
  data_[length_++] = x;
}

}
