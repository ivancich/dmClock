// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*- 
// vim: ts=8 sw=2 smarttab
/*
 * Copyright (C) 2015 Red Hat Inc.
 */

#include <vector>
#include <ostream>

namespace rh {
  template<typename T, typename C>
  class Heap {

  public:
    
    class iterator {

      friend Heap<T,C>;

      Heap<T,C>& heap;
      int        index;

      iterator(Heap<T,C>& _heap, int _index) :
	heap(_heap),
	index(_index)
      {
	// empty
      }

    public:

      iterator(iterator&& other) :
	heap(other.heap),
	index(other.index)
      {
	// empty
      }

      iterator& operator++() {
	++index;
	return *this;
      }

      bool operator==(const iterator& other) const {
	return index == other.index;
      }

      bool operator!=(const iterator& other) const {
	return !(*this == other);
      }

      T& operator*() {
	return heap.data[index];
      }
    }; // class iterator

    friend iterator;

  protected:

    std::vector<T> data;
    int count;
    C comparator;

    int parent(int i) { return (i - 1) / 2; }
    int lhs(int i) { return 2*i + 1; }
    int rhs(int i) { return 2*i + 2; }

    void siftDown(int i) {
      while (i < count) {
	int li = lhs(i);
	int ri = rhs(i);

	if (li < count) {
	  if (comparator(data[li], data[i])) {
	    if (ri < count && comparator(data[ri], data[li])) {
	      T temp = data[i];
	      data[i] = data[ri];
	      data[ri] = temp;
	      i = ri;
	    } else {
	      T temp = data[i];
	      data[i] = data[li];
	      data[li] = temp;
	      i = li;
	    }
	  } else if (ri < count && comparator(data[ri], data[i])) {
	    T temp = data[i];
	    data[i] = data[ri];
	    data[ri] = temp;
	    i = ri;
	  } else {
	    break;
	  }
	} else {
	  break;
	}
      }
    }


  public:

    Heap() :
      count(0)
    {
      // empty
    }

    bool empty() const { return 0 == count; }

    T& top() { return data[0]; }

    void push(T item) {
      int i = count++;
      data.push_back(item);
      while (i > 0) {
	int pi = parent(i);
	if (comparator(data[pi], data[i])) {
	  break;
	}

	T temp = data[pi];
	data[pi] = data[i];
	data[i] = temp;
	i = pi;
      }
    }

    void pop() {
      data[0] = data[--count];
      data.resize(count);
      siftDown(0);
    }

    void updateTop() {
      siftDown(0);
    }

    void clear() {
      count = 0;
      data.resize(0);
    }

    iterator begin() {
      return iterator(*this, 0);
    }

    iterator end() {
      return iterator(*this, count);
    }

    template<typename T1, typename T2>
    friend std::ostream& operator<<(std::ostream&, const Heap<T1,T2>&);
  }; // class Heap

  
  template<typename T1, typename T2>
  std::ostream& operator<<(std::ostream& out, const Heap<T1,T2>& h) {
    out << "{";
    if (h.count) {
      out << h.data[0];
    }
    for (int i = 1; i < h.count; i++) {
      out << ", " << h.data[i];
    }
    out << "}";
    return out;
  }
} // namespace