#ifndef RADIXHEAP_H
#define RADIXHEAP_H

//#define RADIXHEAP_DEBUG
namespace datastructures {
class RadixHeapNode {
 public:
  int item, key;
  int bucket;
  RadixHeapNode *next, *prev;
};

class RadixHeap {
 public:
  RadixHeap(int n, int MaxKey);
  ~RadixHeap();

  int deleteMin();
  void insert(int item, long k);
  void decreaseKey(int item, long newValue);
  int nItems() const { return itemCount; }

  long nComps() const { return compCount; }
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
}  // namespace datastructures
#endif
