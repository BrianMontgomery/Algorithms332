/******************************************************************************
 *  Compilation:  javac RedBlackBST.java
 *  Execution:    java RedBlackBST < input.txt
 *  Dependencies: StdIn.java StdOut.java
 *  Data files:   https://algs4.cs.princeton.edu/33balanced/tinyST.txt
 *
 *  A symbol table implemented using a left-leaning red-black BST.
 *  This is the 2-3 version.
 *
 *  Note: commented out assertions because DrJava now enables assertions
 *        by default.
 *
 *  % more tinyST.txt
 *  S E A R C H E X A M P L E
 *
 *  % java RedBlackBST < tinyST.txt
 *  A 8
 *  C 4
 *  E 12
 *  H 5
 *  L 11
 *  M 9
 *  P 10
 *  R 3
 *  S 0
 *  X 7
 *
 ******************************************************************************/

/**
 *  The {@code BST} class represents an ordered symbol table of generic
 *  key-value pairs.
 *  It supports the usual <em>put</em>, <em>get</em>, <em>contains</em>,
 *  <em>delete</em>, <em>size</em>, and <em>is-empty</em> methods.
 *  It also provides ordered methods for finding the <em>minimum</em>,
 *  <em>maximum</em>, <em>floor</em>, and <em>ceiling</em>.
 *  It also provides a <em>keys</em> method for iterating over all of the keys.
 *  A symbol table implements the <em>associative array</em> abstraction:
 *  when associating a value with a key that is already in the symbol table,
 *  the convention is to replace the old value with the new value.
 *  Unlike {@link java.util.Map}, this class uses the convention that
 *  values cannot be {@code null}—setting the
 *  value associated with a key to {@code null} is equivalent to deleting the key
 *  from the symbol table.
 *  <p>
 *  It requires that
 *  the key type implements the {@code Comparable} interface and calls the
 *  {@code compareTo()} and method to compare two keys. It does not call either
 *  {@code equals()} or {@code hashCode()}.
 *  <p>
 *  This implementation uses a <em>left-leaning red-black BST</em>.
 *  The <em>put</em>, <em>get</em>, <em>contains</em>, <em>remove</em>,
 *  <em>minimum</em>, <em>maximum</em>, <em>ceiling</em>, <em>floor</em>,
 *  <em>rank</em>, and <em>select</em> operations each take
 *  &Theta;(log <em>n</em>) time in the worst case, where <em>n</em> is the
 *  number of key-value pairs in the symbol table.
 *  The <em>size</em>, and <em>is-empty</em> operations take &Theta;(1) time.
 *  The <em>keys</em> methods take
 *  <em>O</em>(log <em>n</em> + <em>m</em>) time, where <em>m</em> is
 *  the number of keys returned by the iterator.
 *  Construction takes &Theta;(1) time.
 *  <p>
 *  For alternative implementations of the symbol table API, see {@link ST},
 *  {@link BinarySearchST}, {@link SequentialSearchST}, {@link BST},
 *  {@link SeparateChainingHashST}, {@link LinearProbingHashST}, and
 *  {@link AVLTreeST}.
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/33balanced">Section 3.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef bst_red_black_h
#define bst_red_black_h

#include <iostream>
#include <cstring>
#include <stdexcept>

#include "Queue.h"
#include "Utils.h"

template <typename S, typename T>
class bst_red_black {

private:
	const static bool RED = true;
	const static bool BLACK = false;

	// BST helper node data type
	template <typename S, typename T>
	 class Node{
	 public:
		 S key;				// key
		 T val;				// associated data
		 Node* left;
		 Node* right;  // links to left and right subtrees
		 bool color;     // color of parent link
		 int size;          // subtree count

	 public:
		Node(S key, T val, bool color, int size) 
		: key(key), val(val), color(color), size(size)
		{
			left = nullptr;
			right = nullptr;
		 }
	 };

	 int compare(S first, S second)
	 {
		 if (first < second)
		 {
			 return -1;
		 }
		 else if (first > second)
		 {
			 return 1;
		 }
		 else
		 {
			 return 0;
		 }
	 }

	 Node<S, T>* root;     // root of the BST
	/**
	 * Initializes an empty symbol table.
	 */
public:
	bst_red_black() {
	}

	/***************************************************************************
	 *  Node helper methods.
	 ***************************************************************************/
	 // is node x red; false if x is null ?
private:
	bool isRed(Node<S, T>* x) {
		if (x == nullptr) { return false; }
		return x->color == RED;
	}

	// number of node in subtree rooted at x; 0 if x is null
	 int size(Node<S, T>* x) {
		 if (x == nullptr) { return 0; }
		return x->size;
	}


	/**
	 * Returns the number of key-value pairs in this symbol table.
	 * @return the number of key-value pairs in this symbol table
	 */
public:
	int size() {
		return size(root);
	}

	/**
	  * Is this symbol table empty?
	  * @return {@code true} if this symbol table is empty and {@code false} otherwise
	  */
	 bool isEmpty() {
		return root == nullptr;
	}


	/***************************************************************************
	 *  Standard BST search.
	 ***************************************************************************/

	 /**
	  * Returns the value associated with the given key.
	  * @param key the key
	  * @return the value associated with the given key if the key is in the symbol table
	  *     and {@code null} if the key is not in the symbol table
	  * @throws IllegalArgumentException if {@code key} is {@code null}
	  */
	 T get(S key) {
		 if (key == S()) { throw std::invalid_argument("argument to get() is null"); }
		 return get(root, key);
	 }

	// value associated with the given key in subtree rooted at x; null if no such key
private:
	T get(Node<S, T>* x, S key) {
		while (x != nullptr) {
			int cmp = compare(key, x->key);
			if (cmp < 0) { x = x->left; }
			else if (cmp > 0) { x = x->right; }
			else { return x->val; }
		}
		return nullptr;
	}

	/**
	 * Does this symbol table contain the given key?
	 * @param key the key
	 * @return {@code true} if this symbol table contains {@code key} and
	 *     {@code false} otherwise
	 * @throws IllegalArgumentException if {@code key} is {@code null}
	 */
	public:
		bool contains(S key) {
			return get(key) != S();
		}

	/***************************************************************************
	 *  Red-black tree insertion.
	 ***************************************************************************/

	 /**
	  * Inserts the specified key-value pair into the symbol table, overwriting the old
	  * value with the new value if the symbol table already contains the specified key.
	  * Deletes the specified key (and its associated value) from this symbol table
	  * if the specified value is {@code null}.
	  *
	  * @param key the key
	  * @param val the value
	  * @throws IllegalArgumentException if {@code key} is {@code null}
	  */
	 void put(S key, T val) {
		 if (key == S()) { throw std::invalid_argument("first argument to put() is null"); }
		if (val == T()) {
			deleteNode(key);
			return;
		}

		root = put(root, key, val);
		root->color = BLACK;
		// assert check();
	}

	// insert the key-value pair in the subtree rooted at h
	private:
		Node<S, T>* put(Node<S, T>* h, S key, T val) {
			if (h == nullptr) { return new Node<S, T>(key, val, RED, 1); }

			int cmp = compare(key, h->key);
			if (cmp < 0) { h->left = put(h->left, key, val); }
			else if (cmp > 0) { h->right = put(h->right, key, val); }
			else { h->val = val; }

			// fix-up any right-leaning links
			if (isRed(h->right) && !isRed(h->left)) { h = rotateLeft(h); }
			if (isRed(h->left) && isRed(h->left->left)) { h = rotateRight(h); }
			if (isRed(h->left) && isRed(h->right)) { flipColors(h); }
			h->size = size(h->left) + size(h->right) + 1;

			return h;
		}

	/***************************************************************************
	 *  Red-black tree deletion.
	 ***************************************************************************/

	 /**
	  * Removes the smallest key and associated value from the symbol table.
	  * @throws NoSuchElementException if the symbol table is empty
	  */
	public:
		void deleteMin() {
			if (isEmpty()) { throw std::invalid_argument("BST underflow"); }

			// if both children of root are black, set root to red
			if (!isRed(root->left) && !isRed(root->right)) {
				root->color = RED;
			}

			root = deleteMin(root);
			if (!isEmpty()) { root->color = BLACK; }
			// assert check();
		}

	// delete the key-value pair with the minimum key rooted at h
	private:
		Node<S, T>* deleteMin(Node<S, T>* h) {
			if (h->left == nullptr) {
				return nullptr;
			}

			if (!isRed(h->left) && !isRed(h->left->left)) {
				h = moveRedLeft(h);
			}

			h->left = deleteMin(h->left);
			return balance(h);
		}


	/**
	 * Removes the largest key and associated value from the symbol table.
	 * @throws NoSuchElementException if the symbol table is empty
	 */
	public:
		void deleteMax() {
			if (isEmpty()) { throw std::invalid_argument("BST underflow"); }

			// if both children of root are black, set root to red
			if (!isRed(root->left) && !isRed(root->right)) {
				root->color = RED;
			}

			root = deleteMax(root);
			if (!isEmpty()) { root->color = BLACK; }
			// assert check();
		}

	// delete the key-value pair with the maximum key rooted at h
	private:
		Node<S, T>* deleteMax(Node<S, T>* h) {
			if (isRed(h->left)) {
				h = rotateRight(h);
			}

			if (h->right == nullptr) {
				return nullptr;
			}

			if (!isRed(h->right) && !isRed(h->right->left)) {
				h = moveRedRight(h);
			}

			h->right = deleteMax(h->right);

			return balance(h);
		}

	/**
	 * Removes the specified key and its associated value from this symbol table
	 * (if the key is in this symbol table).
	 *
	 * @param  key the key
	 * @throws IllegalArgumentException if {@code key} is {@code null}
	 */
	public:
		void deleteNode(S key) {
			if (key == S()) { throw std::invalid_argument("argument to delete() is null"); }
			if (!contains(key)) { return; }

			// if both children of root are black, set root to red
			if (!isRed(root->left) && !isRed(root->right)) {
				root->color = RED;
			}

			root = deleteNode(root, key);
			if (!isEmpty()) { root->color = BLACK; }
			// assert check();
		}

	// delete the key-value pair with the given key rooted at h
	private:
		Node<S, T>* deleteNode(Node<S, T>* h, S key) {
			// assert get(h, key) != null;
	
			if (compare(key, h->key) < 0) {
				if (!isRed(h->left) && !isRed(h->left->left)) {
					h = moveRedLeft(h);
				}
				h->left = deleteNode(h->left, key);
			}
			else {
				if (isRed(h->left))
				{
					h = rotateRight(h);
				}
				if (compare(key, h->key) == 0 && (h->right == nullptr))
				{
					return nullptr;
				}
				if (!isRed(h->right) && !isRed(h->right->left))
				{
					h = moveRedRight(h);
				}
				if (compare(key, h->key) == 0) {
					Node<S, T>* x = min(h->right);
					h->key = x->key;
					h->val = x->val;
					// h.val = get(h->right, min(h->right).key);
					// h.key = min(h->right).key;
					h->right = deleteMin(h->right);
				} else {
					h->right = deleteNode(h->right, key);
				}
			}
			return balance(h);
		}

	/***************************************************************************
	 *  Red-black tree helper functions.
	 ***************************************************************************/

	 // make a left-leaning link lean to the right
	private:
		Node<S, T>* rotateRight(Node<S, T>* h) {
			// assert (h != null) && isRed(h->left);
			Node<S, T>* x = h->left;
			h->left = x->right;
			x->right = h;
			x->color = x->right->color;
			x->right->color = RED;
			x->size = h->size;
			h->size = size(h->left) + size(h->right) + 1;
			return x;
		}

	// make a right-leaning link lean to the left
		Node<S, T>* rotateLeft(Node<S, T>* h) {
		// assert (h != null) && isRed(h->right);
			Node<S, T>* x = h->right;
		h->right = x->left;
		x->left = h;
		x->color = x->left->color;
		x->left->color = RED;
		x->size = h->size;
		h->size = size(h->left) + size(h->right) + 1;
		return x;
	}

	// flip the colors of a node and its two children
	 void flipColors(Node<S, T>* h) {
		// h must have opposite color of its two children
		// assert (h != null) && (h->left != null) && (h->right != null);
		// assert (!isRed(h) &&  isRed(h->left) &&  isRed(h->right))
		//    || (isRed(h)  && !isRed(h->left) && !isRed(h->right));
		h->color = !h->color;
		h->left->color = !h->left->color;
		h->right->color = !h->right->color;
	}

	// Assuming that h is red and both h->left and h->left->left
	// are black, make h->left or one of its children red.
	 Node<S, T>* moveRedLeft(Node<S, T>* h) {
		// assert (h != null);
		// assert isRed(h) && !isRed(h->left) && !isRed(h->left->left);

		flipColors(h);
		if (isRed(h->right->left)) {
			h->right = rotateRight(h->right);
			h = rotateLeft(h);
			flipColors(h);
		}
		return h;
	}

	// Assuming that h is red and both h->right and h->right->left
	// are black, make h->right or one of its children red.
	 Node<S, T>* moveRedRight(Node<S, T>* h) {
		// assert (h != null);
		// assert isRed(h) && !isRed(h->right) && !isRed(h->right->left);
		flipColors(h);
		if (isRed(h->left->left)) {
			h = rotateRight(h);
			flipColors(h);
		}
		return h;
	}

	// restore red-black tree invariant
	 Node<S, T>* balance(Node<S, T>* h) {
		// assert (h != null);

		 if (isRed(h->right)) { h = rotateLeft(h); }
		 if (isRed(h->left) && isRed(h->left->left)) { h = rotateRight(h); }
		 if (isRed(h->left) && isRed(h->right)) { flipColors(h); }

		 h->size = size(h->left) + size(h->right) + 1;
	 	 return h;
	 }


	/***************************************************************************
	 *  Utility functions.
	 ***************************************************************************/

	 /**
	  * Returns the height of the BST (for debugging).
	  * @return the height of the BST (a 1-Node tree has height 0)
	  */
	public:
		int height() {
			return height(root);
		}
	private:
		int height(Node<S, T>* x) {
			if (x == nullptr) { return -1; }
			return 1 + std::max(height(x->left), height(x->right));
		}

	/***************************************************************************
	 *  Ordered symbol table methods.
	 ***************************************************************************/

	 /**
	  * Returns the smallest key in the symbol table.
	  * @return the smallest key in the symbol table
	  * @throws NoSuchElementException if the symbol table is empty
	  */
	public:
		S min() {
			if (isEmpty()) { throw std::invalid_argument("calls min() with empty symbol table"); }
			return min(root)->key;
		}

	// the smallest key in subtree rooted at x; null if no such key
	private:
		Node<S, T>* min(Node<S, T>* x) {
			// assert x != null;
			if (x->left == nullptr) { return x; }
			else { return min(x->left); }
		}

	/**
	 * Returns the largest key in the symbol table.
	 * @return the largest key in the symbol table
	 * @throws NoSuchElementException if the symbol table is empty
	 */
	public:
		S max() {
			if (isEmpty()) { throw std::invalid_argument("calls max() with empty symbol table"); }
			return max(root)->key;
		}

	// the largest key in the subtree rooted at x; null if no such key
	private:
		Node<S, T>* max(Node<S, T>* x) {
			// assert x != null;
			if (x->right == nullptr) { return x; }
			else { return max(x->right); }
		}


	/**
	 * Returns the largest key in the symbol table less than or equal to {@code key}.
	 * @param key the key
	 * @return the largest key in the symbol table less than or equal to {@code key}
	 * @throws NoSuchElementException if there is no such key
	 * @throws IllegalArgumentException if {@code key} is {@code null}
	 */
	public:
		S floor(S key) {
			if (key == nullptr) { throw std::invalid_argument("argument to floor() is null"); }
			if (isEmpty()) { throw std::invalid_argument("calls floor() with empty symbol table"); }
			Node<S, T>* x = floor(root, key);
			if (x == nullptr) { throw std::invalid_argument("argument to floor() is too small"); }
			else { return x->key; }
		}

	// the largest key in the subtree rooted at x less than or equal to the given key
	private:
		Node<S, T>* floor(Node<S, T>* x, S key) {
			if (x == nullptr) { return nullptr; }
			int cmp = compare(key, x->key);
			if (cmp == 0) { return x; }
			if (cmp < 0) { return floor(x->left, key); }
			Node<S, T>* t = floor(x->right, key);
			if (t != nullptr) { return t; }
			else { return x; }
		}

	/**
	 * Returns the smallest key in the symbol table greater than or equal to {@code key}.
	 * @param key the key
	 * @return the smallest key in the symbol table greater than or equal to {@code key}
	 * @throws NoSuchElementException if there is no such key
	 * @throws IllegalArgumentException if {@code key} is {@code null}
	 */
	public:
		S ceiling(S key) {
			if (key == nullptr) { throw std::invalid_argument("argument to ceiling() is null"); }
			if (isEmpty()) { throw std::invalid_argument("calls ceiling() with empty symbol table"); }
			Node<S, T>* x = ceiling(root, key);
			if (x == nullptr) { throw std::invalid_argument("argument to ceiling() is too small"); }
			else { return x->key; }
		}

	// the smallest key in the subtree rooted at x greater than or equal to the given key
	private:
		Node<S, T>* ceiling(Node<S, T>* x, S key) {
			if (x == nullptr) { return nullptr; }
			int cmp = compare(key, x->key);
			if (cmp == 0) { return x; }
			if (cmp > 0) { return ceiling(x->right, key); }
			Node<S, T>* t = ceiling(x->left, key);
			if (t != nullptr) { return t; }
			else { return x; }
		}

	/**
	 * Return the key in the symbol table of a given {@code rank}.
	 * This key has the property that there are {@code rank} keys in
	 * the symbol table that are smaller. In other words, this key is the
	 * ({@code rank}+1)st smallest key in the symbol table.
	 *
	 * @param  rank the order statistic
	 * @return the key in the symbol table of given {@code rank}
	 * @throws IllegalArgumentException unless {@code rank} is between 0 and
	 *        <em>n</em>–1
	 */
	public:
		S select(int rank) {
			if (rank < 0 || rank >= size()) {
				throw std::invalid_argument("argument to select() is invalid: " + rank);
			}
			return select(root, rank);
		}

	// Return key in BST rooted at x of given rank.
	// Precondition: rank is in legal range.
	private:
		S select(Node<S, T>* x, int rank) {
			if (x == nullptr) { return nullptr; }
			int leftSize = size(x->left);
			if (leftSize > rank) { return select(x->left, rank); }
			else if (leftSize < rank) { return select(x->right, rank - leftSize - 1); }
			else { return x->key; }
		}

	/**
	 * Return the number of keys in the symbol table strictly less than {@code key}.
	 * @param key the key
	 * @return the number of keys in the symbol table strictly less than {@code key}
	 * @throws IllegalArgumentException if {@code key} is {@code null}
	 */
	public:
		int rank(S key) {
			if (key == nullptr) { throw std::invalid_argument("argument to rank() is null"); }
			return rank(key, root);
		}

	// number of keys less than key in the subtree rooted at x
	private:
		int rank(S key, Node<S, T>* x) {
			if (x == nullptr) { return 0; }
			int cmp = compare(key, x->key);
			if (cmp < 0) { return rank(key, x->left); }
			else if (cmp > 0) { return 1 + size(x->left) + rank(key, x->right); }
			else { return size(x->left); }
		}

	/***************************************************************************
	 *  Range count and range search.
	 ***************************************************************************/

	 /**
	  * Returns all keys in the symbol table as an {@code Iterable}.
	  * To iterate over all of the keys in the symbol table named {@code st},
	  * use the foreach notation: {@code for (Key key : st.keys())}.
	  * @return all keys in the symbol table as an {@code Iterable}
	  */
	public:
		array_queue<S> keys() {
			if (isEmpty()) { return *(new array_queue<S>()); }
			return *keys(min(), max());
		}

	/**
	 * Returns all keys in the symbol table in the given range,
	 * as an {@code Iterable}.
	 *
	 * @param  lo minimum endpoint
	 * @param  hi maximum endpoint
	 * @return all keys in the symbol table between {@code lo}
	 *    (inclusive) and {@code hi} (inclusive) as an {@code Iterable}
	 * @throws IllegalArgumentException if either {@code lo} or {@code hi}
	 *    is {@code null}
	 */
	public:
		array_queue<S>* keys(S lo, S hi) {
			if (lo == S()) { throw std::invalid_argument("first argument to keys() is null"); }
			if (hi == S()) { throw std::invalid_argument("second argument to keys() is null"); }

			array_queue<S>* queue = new array_queue<S>();
			// if (isEmpty() || lo.compareTo(hi) > 0) return queue;
			keys(root, queue, lo, hi);
			return queue;
		}

	// add the keys between lo and hi in the subtree rooted at x
	// to the queue
	private:
		void keys(Node<S, T>* x, array_queue<S>* queue, S lo, S hi) {
			if (x == nullptr) { return; }
			int cmplo = compare(lo, x->key);
			int cmphi = compare(hi, x->key);
			if (cmplo < 0) { keys(x->left, queue, lo, hi); }
			if (cmplo <= 0 && cmphi >= 0) { queue->enqueue(x->key); }
			if (cmphi > 0) { keys(x->right, queue, lo, hi); }
		}

	/**
	 * Returns the number of keys in the symbol table in the given range.
	 *
	 * @param  lo minimum endpoint
	 * @param  hi maximum endpoint
	 * @return the number of keys in the symbol table between {@code lo}
	 *    (inclusive) and {@code hi} (inclusive)
	 * @throws IllegalArgumentException if either {@code lo} or {@code hi}
	 *    is {@code null}
	 */
	public:
		int size(S lo, S hi) {
			if (lo == nullptr) { throw std::invalid_argument("first argument to size() is null"); }
			if (hi == nullptr) { throw std::invalid_argument("second argument to size() is null"); }

			if (compare(lo, hi) > 0) { return 0; }
			if (contains(hi)) { return rank(hi) - rank(lo) + 1; }
			else { return rank(hi) - rank(lo); }
		}


	/***************************************************************************
	 *  Check integrity of red-black tree data structure.
	 ***************************************************************************/
	private:
		bool check() {
			if (!isBST()) { std::cout << "Not in symmetric order" << std::endl; }
			if (!isSizeConsistent()) { std::cout << ("Subtree counts not consistent") << std::endl; }
			if (!isRankConsistent()) { std::cout << ("Ranks not consistent") << std::endl; }
			if (!is23()) { std::cout << ("Not a 2-3 tree") << std::endl; }
			if (!isBalanced()) { std::cout << ("Not balanced") << std::endl; }
			return isBST() && isSizeConsistent() && isRankConsistent() && is23() && isBalanced();
		}

	// does this binary tree satisfy symmetric order?
	// Note: this test also ensures that data structure is a binary tree since order is strict
		bool isBST() {
			return isBST(root, nullptr, nullptr);
		}

	// is the tree rooted at x a BST with all keys strictly between min and max
	// (if min or max is null, treat as empty constraint)
	// Credit: Bob Dondero's elegant solution
	 bool isBST(Node<S, T>* x, S min, S max) {
		 if (x == nullptr) { return true; }
		 if (min != nullptr && compare(x->key, min) <= 0) { return false; }
		 if (max != nullptr && compare(x->key, max) >= 0) { return false; }
		return isBST(x->left, min, x->key) && isBST(x->right, x->key, max);
	 }

	// are the size fields correct?
	 bool isSizeConsistent() { return isSizeConsistent(root); }
	 bool isSizeConsistent(Node<S, T>* x) {
		 if (x == nullptr) { return true; }
		 if (x->size != size(x->left) + size(x->right) + 1) { return false; }
		return isSizeConsistent(x->left) && isSizeConsistent(x->right);
	 }

	// check that ranks are consistent
	 bool isRankConsistent() {
		 for (int i = 0; i < size(); i++){
			 if (i != rank(select(i))) { return false; }
		 }
		for (S key : keys()){
			if (compare(key, select(rank(key))) != 0) {
				return false;
			}
			return true;
		}
	}

	// Does the tree have no red right links, and at most one (left)
	// red links in a row on any path?
	 bool is23() { return is23(root); }
	 bool is23(Node<S, T>* x) {
		 if (x == nullptr) { return true; }
			if (isRed(x->right)) { return false; }
			if (x != root && isRed(x) && isRed(x->left)) {
				return false;
			}
		return is23(x->left) && is23(x->right);
	}

	// do all paths from root to leaf have same number of black edges?
	 bool isBalanced() {
		int black = 0;     // number of black links on path from root to min
		Node<S, T>* x = root;
		while (x != nullptr) {
			if (!isRed(x)) black++;
			x = x->left;
		}
		return isBalanced(root, black);
	}

	// does every path from the root to a leaf have the given number of black links?
	 bool isBalanced(Node<S, T>* x, int black) {
		 if (x == nullptr) { return black == 0; }
		 if (!isRed(x)) { black--; }
		return isBalanced(x->left, black) && isBalanced(x->right, black);
	}

	 public:
		 static void run_tests() {
			 run_test("tinyST.txt");
			 run_test("gettysburgST.txt");
		 }

	private:    // helper functions

		array_queue<S> level_order() {
			array_queue<S> keys;
			array_queue<Node<S,T>*> q;

			q.enqueue(root);
			while (!q.empty()) {
				Node<S, T>* x = q.dequeue();
				if (x == nullptr) { continue; }

				keys.enqueue(x->key);
				q.enqueue(x->left);
				q.enqueue(x->right);
			}
			return keys;
		}

		static void run_test(const std::string filename) {
			char buf[BUFSIZ];
			bst_red_black<std::string, std::string> st;

			std::ifstream ifs(filename);
			if (!ifs.is_open()) {
				std::cerr << "Could not open file: '" << filename << "'\n";  exit(2);
			}

			std::cout << "Building symbol table for file: '" << filename << "'. \n";
			std::cout << "(punctuation automatically stripped)... \n";
			int i = 0;
			std::string s;
			while (ifs >> s) {
				strcpy(buf, s.c_str());
				strconvert(buf, tolower);
				strstrip(buf);
				std::string key = std::string(buf);
				if (key != "") { st.put(key, std::to_string(i++)); }
				//      st.print_inorder();
			}

			std::cout << "\n";

			std::cout << "\nin level order (root to leaves)...\n";
			array_queue<S> keys = st.level_order();
			for (std::string& key : keys) {
				std::cout << std::setw(12) << key << "  " << std::setw(2) << st.get(key) << "\n";
			}

			std::cout << "\nin alphabetical order...\n";
			keys = st.keys();
			for (std::string& key : keys) {
				std::cout << std::setw(14) << key << "  " << std::setw(2) << st.get(key) << "\n";
			}
		}
};

#endif /*bst_red_black_h */