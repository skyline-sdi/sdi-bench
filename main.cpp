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
 * $Id: main.cpp 567 2019-12-23 19:21:14Z li $
 */

#include <array>
#include <fstream>
#include "sdi.h"
#include "timer.h"
using namespace sdibench;

auto run_skyline(const char *name, size_t cardinality, size_t dimensionality, const char *filename) -> bool {
  timer build;
  timer query;
  sdi method(cardinality, dimensionality);
  std::cerr << "Building... ";
  if (!filename) {
    std::cerr << "(STDIN) ";
    build.start();
    method.build(std::cin);
    build.stop();
  } else {
    std::cerr << "(" << filename << ") ";
    std::ifstream fin(filename);
    if (!fin.good()) {
      std::cerr << "- cannot open file. " << filename << std::endl;
      return false;
    }
    build.start();
    method.build(fin);
    build.stop();
  }
  double bt = build.runtime() * 1000;
  std::cerr << "done in " << bt << " ms." << std::endl;
  std::cerr << "Querying... ";
  query.start();
  method.query();
  query.stop();
  double qt = query.runtime() * 1000;
  std::cerr << "done in " << qt << " ms." << std::endl;
  double tt = bt + qt;
  std::cout << "# Method: " << name << std::endl;
  std::cout << "# Size: " << cardinality << std::endl;
  std::cout << "# Dimensions: " << dimensionality << std::endl;
  std::cout << "# Skyline: " << db::SKY << std::endl;
  std::cout << "# Dominance Test Count: " << db::DT << std::endl;
  std::cout << "# Dominance Test Extended Count: " << db::DTE << std::endl;
  std::cout << "# Stop Line Count: " << db::STOP << std::endl;
  std::cout << "# Tested Tuple Count: " << db::TT << std::endl;
  std::cout << "# IO Count: " << db::IO << std::endl;
  std::cout << "# Build Time: " << bt << " ms" << std::endl;
  std::cout << "# Query Time: " << qt << " ms" << std::endl;
  std::cout << "# Total Time: " << tt << " ms" << std::endl;
  std::cout << "#= " << name << " | " << cardinality << " | " << dimensionality << " | ";
  std::cout << db::SKY << " | " << db::DT << " | " << db::IO << " | ";
  std::cout << bt << " | " << qt << " | " << tt << std::endl;
  return true;
}

auto main(int argc, char **argv) -> int {
  if (argc < 3) {
    std::cout << "Usage: bench-sdi [FILE] DIMENSIONALITY CARDINALITY" << std::endl;
    return 0;
  }
  const char *filename = argc > 3 ? argv[1] : nullptr;
  size_t dimensionality = argc > 3 ? strtoul(argv[2], nullptr, 10): strtoul(argv[1], nullptr, 10);
  size_t cardinality = argc > 3 ? strtoul(argv[3], nullptr, 10) : strtoul(argv[2], nullptr, 10);
  run_skyline("SDI", cardinality, dimensionality, filename);
  return 0;
}
