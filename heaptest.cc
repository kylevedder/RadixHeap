// Copyright 2017 Kyle Vedder (kvedder@umass.edu)
// College of Information and Computer Sciences,
// University of Massachusetts Amherst
//
// This software is free: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License Version 3,
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// Version 3 in the file COPYING that came with this distribution.
// If not, see <http://www.gnu.org/licenses/>.
// ========================================================================
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "radixheap.h"
#include "radixqueue.h"

static constexpr float kIntMaxF = 2147483647;

double GetMonotonicTime() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  const double time = static_cast<double>(ts.tv_sec) +
                      static_cast<double>(ts.tv_nsec) * (1.0E-9);
  return time;
}

struct NonTrivialType {
  float data;
  NonTrivialType() = default;
  NonTrivialType(const float data) : data(data) {}
  bool operator<(const NonTrivialType& other) const {
    return data < other.data;
  }

  bool operator==(const NonTrivialType& other) const {
    return data == other.data;
  }

  size_t operator()() const { return static_cast<size_t>(data); }
};

struct STLQueueDataHolder {
  float key;
  int index;

  STLQueueDataHolder() = delete;
  STLQueueDataHolder(const float key, const int index)
      : key(key), index(index) {}

  bool operator<(const STLQueueDataHolder& other) const {
    return key > other.key;
  }
};

int main() {
  srand(time(nullptr));
  constexpr int n = 100000;
  constexpr int kMaxKey = 500000;
  // Must be a power of two to prevent floating point error.
  constexpr float kKeyScaler = 128.0f;
  datastructures::radix::queue::RadixQueue<NonTrivialType> queue(n, kMaxKey);
  queue.Push({0.0f}, 200);
  queue.Push({0.0f}, 100);
  queue.Push({1.0f}, 150);
  const NonTrivialType first = queue.Pop();
  std::cout << "First: " << first.data << '\n';
  const NonTrivialType second = queue.Pop();
  std::cout << "Second: " << second.data << '\n';
  if (!queue.Empty()) {
    std::cerr << "Not correct count!\n";
  }

  datastructures::radix::heap::RadixHeap heap(n, kMaxKey);
  std::priority_queue<STLQueueDataHolder, std::vector<STLQueueDataHolder>>
      stl_queue;

  // Generate random numbers.
  std::vector<NonTrivialType> non_trivial_types;
  for (size_t i = 0; i < n / 2; ++i) {
    const float rand_f =
        floor(static_cast<float>(rand()) / kIntMaxF * kMaxKey) / kKeyScaler;
    // Ensure that duplicates can be properly handled.
    non_trivial_types.push_back({rand_f});
    non_trivial_types.push_back({rand_f});
  }

  // Add data to the custom heap.
  const auto custom_push_start = GetMonotonicTime();
  for (size_t i = 0; i < non_trivial_types.size(); ++i) {
    // Key is the index in the unsorted vector, value is the float converted to
    // a int64_t.
    heap.insert(i,
                static_cast<int64_t>(non_trivial_types[i].data * kKeyScaler));
  }
  const auto custom_push_end = GetMonotonicTime();
  std::cout << "Custom push time delta: "
            << (custom_push_end - custom_push_start) << '\n';

  // Add data to the STL queue.
  const auto stl_push_start = GetMonotonicTime();
  for (size_t i = 0; i < non_trivial_types.size(); ++i) {
    // Key is the index in the unsorted vector, value is the float itself.
    stl_queue.push({non_trivial_types[i].data, static_cast<int>(i)});
  }
  const auto stl_push_end = GetMonotonicTime();
  std::cout << "STL push time delta: " << (stl_push_end - stl_push_start)
            << '\n';

  std::vector<NonTrivialType> sorted_non_trivial_types = non_trivial_types;
  std::sort(sorted_non_trivial_types.begin(), sorted_non_trivial_types.end());

  std::vector<NonTrivialType> custom_non_trivial_types(
      non_trivial_types.size());
  const auto custom_read_start = GetMonotonicTime();
  for (size_t i = 0; heap.nItems() > 0; ++i) {
    const int index = heap.deleteMin();
    custom_non_trivial_types[i] = non_trivial_types[index];
  }
  const auto custom_read_end = GetMonotonicTime();
  std::cout << "Custom read time delta: "
            << (custom_read_end - custom_read_start) << '\n';

  std::vector<NonTrivialType> stl_queue_non_trivial_types(
      non_trivial_types.size());
  const auto stl_read_start = GetMonotonicTime();
  for (size_t i = 0; !stl_queue.empty(); ++i) {
    const int index = stl_queue.top().index;
    stl_queue.pop();
    stl_queue_non_trivial_types[i] = non_trivial_types[index];
  }
  const auto stl_read_end = GetMonotonicTime();
  std::cout << "STL read time delta: " << (stl_read_end - stl_read_start)
            << '\n';

  if ((sorted_non_trivial_types.size() != custom_non_trivial_types.size()) ||
      (sorted_non_trivial_types.size() != stl_queue_non_trivial_types.size())) {
    std::cerr << "Sizes are not equal!\n";
    return 0;
  }

  size_t error_count = 0;
  for (size_t i = 0; i < sorted_non_trivial_types.size(); ++i) {
    const auto& stl_queue = stl_queue_non_trivial_types[i];
    const auto& queue = custom_non_trivial_types[i];
    const auto& sorted = sorted_non_trivial_types[i];
    if (queue.data != sorted.data) {
      std::cerr << "Not equal: STL: " << stl_queue.data
                << " vs Custom: " << queue.data
                << " vs Reference: " << sorted.data << " \n";
      if (++error_count >= 20) {
        std::cerr << "Too many errors, exiting!\n";
        return 0;
      }
    }
  }
}
