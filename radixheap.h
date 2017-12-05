// Copyright 2017 Shane Saunders (???), and Kyle Vedder (kvedder@umass.edu)
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

#ifndef RADIXHEAP_H
#define RADIXHEAP_H

//#define RADIXHEAP_DEBUG
namespace datastructures {
namespace radix {

namespace heap {
class RadixHeapNode {
 public:
  int item;
  int64_t key;
  int bucket;
  RadixHeapNode *next, *prev;
};

class RadixHeap {
 public:
  RadixHeap(int n, int MaxKey);
  ~RadixHeap();

  int deleteMin();
  void insert(int item, int64_t k);
  void decreaseKey(int item, int64_t newValue);
  int nItems() const { return itemCount; }

  int64_t nComps() const { return compCount; }
  void dump() const;

 private:
  void placeNode(int startBucket, RadixHeapNode *node);
  void insertNode(int i, RadixHeapNode *node);
  void removeNode(RadixHeapNode *node);

  RadixHeapNode **nodes;
  RadixHeapNode *bucketHeaders;
  int *u;

  int nBuckets;
  int dMin;

  int itemCount;
  int compCount;
};
}  // namespace heap
}  // namespace radix
}  // namespace datastructures
#endif  // RADIXHEAP_H
