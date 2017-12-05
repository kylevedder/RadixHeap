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

#ifndef RADIXQUEUE_H
#define RADIXQUEUE_H

#include <functional>
#include <unordered_set>
#include <vector>
#include "radixheap.h"

namespace datastructures {
namespace radix {
namespace queue {
template <class Key>
class RadixQueue {
 private:
  static constexpr bool kDebug = false;

  struct FrontierListKey {
    const int data_index_;
    const std::vector<Key>& key_store_;

    FrontierListKey() = delete;
    FrontierListKey(const int& data_index, const std::vector<Key>& key_store)
        : data_index_(data_index), key_store_(key_store) {}

    bool operator==(const FrontierListKey& other) const {
      return key_store_[data_index_] == key_store_[other.data_index_];
    }

    size_t operator()() const { return (key_store_[data_index_])(); }
  };

  struct FrontierListKeyHasher {
    size_t operator()(const FrontierListKey& data) const { return data(); }
  };

  const int max_queue_elements_;
  const int64_t max_key_;
  heap::RadixHeap radix_heap_;
  std::vector<Key> key_store_;
  std::unordered_set<FrontierListKey, FrontierListKeyHasher> frontier_set_;

 public:
  RadixQueue() = delete;
  RadixQueue(const int& max_queue_elements, const int64_t& max_key)
      : max_queue_elements_(max_queue_elements),
        max_key_(max_key),
        radix_heap_(max_queue_elements, max_key) {}

  // Either adds a new element or takes the min of the current weight and the
  // new weight.
  void Push(const Key& key, const int64_t& value) {
    key_store_.push_back(key);
    const int insertion_index = (key_store_.size() - 1);
    const auto frontier_set_find =
        frontier_set_.find({insertion_index, key_store_});
    if (frontier_set_find == frontier_set_.end()) {
      frontier_set_.insert({insertion_index, key_store_});
      radix_heap_.insert(insertion_index, value);
    } else {
      radix_heap_.decreaseKey(frontier_set_find->data_index_, value);
      key_store_.erase(key_store_.begin() + insertion_index);
    }
  }

  Key Pop() {
    const int index = radix_heap_.deleteMin();
    if (kDebug) {
      if (index >= key_store_.size()) {
        std::cerr << "Out of bounds!\n";
      }
    }
    const Key key = key_store_[index];
    frontier_set_.erase({index, key_store_});
    return key;
  }

  bool Empty() {
    if (kDebug) {
      if ((key_store_.size() != frontier_set_.size()) ||
          (key_store_.size() != radix_heap_.nItems())) {
        std::cerr << "Improper sizing!!!";
        exit(-1);
      }
    }

    return radix_heap_.nItems() <= 0;
  }
};
}  // namespace queue
}  // namespace radix
}  // namespace datastructures

#endif  // RADIXQUEUE_H
