//------------------------------------------------------
//
// Heap Sort and Max PQ Heap
// The working Max PQ heap is mostly based off of the code provided by professor McCarthy.
// My attempts can be found after that, commented out.
// I was finding a bug where my integers were changing going into the less function and I could not find out where. 
// Most likely this is due to not allocating the array.
//
//-------------------------------------------------------

/*
#include <iostream>
#include <cassert>
#include "Utils.h"
#include "Array.h"

template <typename T>
class heap_sort {
public:
	static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>()) {// heapify phase
		for (int k = n / 2; k >= 1; k--) {
			sink(arr, k, n, comp);
		}

		// sortdown phase
		int k = n;
		while (k > 1) {
			exch(arr, 1, k--);
			sink(arr, 1, k, comp);
			for (int i = 0; i < n; i++)
			{
				std::cout << arr[i] << " ";
			}
			std::cout << std::endl;
		}

		assert(is_sorted(arr, n));
	}&*/

	/***************************************************************************
	 * Helper functions to restore the heap invariant.
	 ***************************************************************************/

/*private:
	static void sink(T* arr, int k, int n, const comparator<T>& comp = fwd_comparator<T>()) {
		while ((2 * k) <= n) 
		{
			int j = (2 * k);

			if ((j < n) && less(arr[j-1], arr[j], comp))
			{
				j++;
			}

			if (!less(arr[k-1], arr[j-1], comp))
			{
				break;
			}
			
			exch(arr, k, j);

			k = j;
		}
	}*/

	/***************************************************************************
	 * Helper functions for comparisons and swaps.
	 * Indices are "off-by-one" to support 1-based indexing.
	 ***************************************************************************/
	/*static void exch(T* arr, int i, int j) {
		T swap = arr[i - 1];
		arr[i - 1] = arr[j - 1];
		arr[j - 1] = swap;
	}
};


#define MIN_CAPACITY 3

//------------------------------------------------------
template <typename T>
class max_pq_heap {
public:
	max_pq_heap(const comparator<T>& compar = fwd_comparator<T>) {};
	max_pq_heap(const comparator<T>& compar) : max_pq_heap(MIN_CAPACITY, compar) { }

	max_pq_heap(size_t capacity, const comparator<T>& compar)
		: pq_(capacity + 1), comp(compar) {
		pq_.push_back(T());
	}  // make space for skipped-over 0 entry

	max_pq_heap(const std::initializer_list<T>& li, const comparator<T>& compar)
		: max_pq_heap(li.size(), compar) {
		for (const T& el : li) {
			if (el != "-") {
				insert(el);
			}
			else { std::cout << delMax() << " "; }
		}
		std::cout << "\t(" << size() << " left on the heap)\n\n"; \
	}

	bool empty() const { return pq_.size() == 1; }   // leaves 0 index empty
	size_t size() const { return pq_.size() - 1; }   // ditto

	void insert(const T& value) {
		pq_.push_back(value);
		swim(size());
	}
	T delMax() {
		T maximum = pq_[1];
		size_t n = size();

		exchange(1, n);
		pq_.pop_back();
		sink(1);
		pq_[n] = T();
		return maximum;
	}
	void swim(size_t k) {
		while (k > 1 && less(k / 2, k)) {
			exchange(k, k / 2);
			k /= 2;
		}
	}
	void sink(size_t k) {
		size_t n = size();

		while (2 * k <= n) {
			size_t j = 2 * k;
			if (j < n && less(j, j + 1)) { ++j; }
			if (!less(k, j)) { break; }

			exchange(k, j);
			k = j;
		}
	}
	friend std::ostream& operator<<(std::ostream& os, const max_pq_heap& maxpq) {
		if (maxpq.empty()) { return os << "max_pq_heap is empty\n"; }
		return os << maxpq.pq_ << "\n";
	}

	void printPQ() {
		size_t n = size();
		for (size_t i = 0; i < n; ++i) {
			std::cout << pq_[i] << " ";
		}
		std::cout << '\n';
	}

	

private:
	static const size_t MIN_CAPACITY_;

	void exchange(size_t i, size_t j) {
		T temp = pq_[i];
		pq_[i] = pq_[j];
		pq_[j] = temp;
	}

	bool less(size_t i, size_t j) {
		return ::less(pq_[i], pq_[j], comp);
	}

	array_<T> pq_;                // uses expandable array from array.h, not std::array<T>
	const comparator<T>& comp;
};*/



//-----------------------------------------------------
//MY ATTEMPT AT MAX_PQ_HEAP
//-----------------------------------------------------

/*template <typename T>
class max_pq_heap {
private:


public:


	static bool isEmpty(array_<T>& myPQ) {
		return(myPQ.size() == 1);
	}


	static int size(array_<T>& myPQ) {
		return(myPQ.size());
	}


	static T max(array_<T>& myPQ) {
		if (isEmpty())
		{
			assert(false);
		}
		return (myPQ[1]);
	}


	static void insert(T x, array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		// add x, and percolate it up to maintain heap invariant
		myPQ.push_back(x);

		for (int i = 1; i < myPQ.size(); i++)
		{
			std::cout << myPQ[i] << " ";
		}
		std::cout << std::endl;
		swim(myPQ.size()-1, myPQ, comp);

		assert(isMaxHeap(myPQ, comp));
	}


	static T delMax(array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		if (isEmpty(myPQ))
		{
			assert(false);
		}
		exch(0, myPQ.size()-1, myPQ);
		sink(0, myPQ, comp);
		T max = myPQ.pop_back();    // to avoid loitering and help with garbage collection
		//assert(isMaxHeap(myPQ, comp));
		return max;
	}

	// helper function to double the size of the heap array
private:



	static void swim(int k, array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		while (k > 1) {
			for (int i = 1; i < myPQ.size(); i++)
			{
				std::cout << myPQ[i] << " ";
			}
			std::cout << std::endl;
			if (less((k / 2), k, myPQ, comp))
			{
				exch(k, (k / 2), myPQ);
				k = k / 2;
			}
			else {
				break;
			}
		}
	}

	static void sink(int k, array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		while (2 * k <= myPQ.size()) {
			int j = 2 * k;

			if (j < myPQ.size() && less(j, j+1, myPQ, comp))
			{
				j++;
			}
			if (!less(k, j, myPQ, comp)) {
				break;
			}
			exch(k, j, myPQ);
			k = j;
		}
	}


	static void exch(int i, int j, array_<T>& myPQ) {
		T swap = myPQ[i];
		myPQ[i] = myPQ[j];
		myPQ[j] = swap;
	}

	// is myPQ[1..n] a max heap?
	static bool isMaxHeap(array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		for (int i = 1; i < size(); ++i) {
			if (myPQ[i] == null) {
				return false;
			}
		}
		if (isEmpty(myPQ))
		{
			return false;
		}
		return isMaxHeapOrdered(1, myPQ, comp);
	}

	// is subtree of myPQ[1..n] rooted at k a max heap?
	static bool isMaxHeapOrdered(int k, array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		if (k >= myPQ.size())
		{
			return true;
		}
		int left = 2 * k;
		int right = 2 * k + 1;

		if (left < myPQ.size()) {
			if(less(k, left, myPQ, comp))
			{
				return false;
			}
		}
		if (right < myPQ.size()) {
			if (less(k, right, myPQ, comp))
			{
				return false;
			}
		}
		return (isMaxHeapOrdered(left, myPQ, comp) && isMaxHeapOrdered(right, myPQ, comp));
	}

	static bool less(size_t i, size_t j, array_<T>& myPQ, const comparator<T>& comp = fwd_comparator<T>()) {
		return ::less(myPQ[i], myPQ[j], comp);
	}

public:
	static void sort(T* arr, size_t n, const comparator<T>& comp = fwd_comparator<T>()) {
		array_<T> myPQ;
		myPQ.push_back(T());
		for (int i = 1; i < n; i++)
		{
			insert(arr[i], myPQ, comp);
		}
		for (int k = myPQ.size() / 2; k >= 1; k--)
		{
			sink(k, myPQ, comp);
		}

		myPQ.size();
		for (int i = 0; i < n; i++)
		{
			arr[i] = delMax(myPQ, comp);
		}
	}
};*/
