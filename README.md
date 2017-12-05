# RadixHeap
Fast, simple RadixHeap with update-able weights.

Provides test providing performance benchmarks versus the `std::priority_queue`.

Benchmarks on `Intel Core i7-7820HQ CPU @ 3.9GHz`:

```
Custom push time delta: 0.00218115
STL push time delta: 0.00189612
Custom read time delta: 0.0133945
STL read time delta: 0.00855569
```
