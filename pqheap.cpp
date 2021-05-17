// Creates a heap usign bubbling up and bubbling down.
#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/*
 * The destructor is responsible for cleaning up any resources
 * used by this instance of the class. The array
 * memory used for elements is deallocated here.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/*
 * Enqueues in bubbling up order.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (_numFilled == _numAllocated - 1) {
        // Create array of double the size of _elements
        DataPoint* newElements = new DataPoint[_numAllocated * 2];
        for (int i = 0; i < _numFilled; i++) {
            // Copy _elements into newElements
            newElements[i] = _elements[i];
        }
        delete [] _elements;
        _elements = newElements;
        _numAllocated *= 2;
    }
    int childSpot = _numFilled;
    int parentSpot = 1;
    _elements[childSpot] = elem;
    while (parentSpot != 0 && elem.priority < _elements[getParentIndex(childSpot)].priority) {
        parentSpot = getParentIndex(childSpot);
        _elements[childSpot] = _elements[parentSpot];
        _elements[parentSpot] = elem;
        childSpot = parentSpot;
    }
    _numFilled ++;
}

/*
 * Peeks at the first element.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek empty pqueue");
    }
    return _elements[0];
}

/*
 * Dequeues in bubbling down order.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty())
        error("Cannot dequeue an empty pqueue");
    DataPoint dequeuingValue = peek();
    int parentSpot = 0;
    int childSpot = 0;
    int bubbler = _numFilled - 1;
    _elements[parentSpot] = _elements[bubbler];
    while (getLeftChildIndex(childSpot) != -1) {
        if (_elements[parentSpot].priority > _elements[getLeftChildIndex(parentSpot)].priority
            || _elements[parentSpot].priority > _elements[getRightChildIndex(parentSpot)].priority) {
            childSpot = getSmallerChildIndex(parentSpot);
            _elements[parentSpot] = _elements[childSpot];
            _elements[childSpot] = _elements[bubbler];
            parentSpot = childSpot;
        }
    }
    _numFilled --;
//    DataPoint* newElements = new DataPoint[_numAllocated];
//    for (int i = 0; i < _numFilled; i++) {
//        // Copy _elements into newElements
//        newElements[i] = _elements[i];
//    }
//    delete [] _elements;
//    _elements = newElements;
    return dequeuingValue;
}

/*
 * Returns that the size of the heap is 0 to indicate an empty heap.
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}

/*
 * Returns the size of the heap, or the amount of items filled.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * _numFilled is made equal to 0 to clear the items in the heap.
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/*
 * Prints debugger information.
 */
//void PQHeap::printDebugInfo(string label) {
//    cout << label << endl;
//    for (int i = 0; i < size(); i++) {
//        cout << "[" << i << "] = " << _elements[i] << endl;
//    }
//}

/* Traverses the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /* Loop over the elements in the array and compare priority of pair of
     * parent/child elements. If current element has larger priority
     * than the previous this indicates array elements are not in
     * in expected decreasing sorted order. Use error to report this problem.
     */
    for (int i = 0; i < size(); i++) {
        if (_elements[i].priority > _elements[getLeftChildIndex(i)].priority ||
                _elements[i].priority > _elements[getRightChildIndex(i)].priority)
            error("Array elements out of order at index " + integerToString(i));
    }
}

/* Calculates the index of the smaller child of the parent with the
 * provided parent index.
 */
int PQHeap::getSmallerChildIndex(int parentIndex) {
    if (_elements[getLeftChildIndex(parentIndex)].priority < _elements[getRightChildIndex(parentIndex)].priority)
        return getLeftChildIndex(parentIndex);
    else {
        return getRightChildIndex(parentIndex);
    }
}

/* Calculates the index of the parent of the element with the
 * provided index.
 */
int PQHeap::getParentIndex(int curIndex) {
    if (((curIndex - 1) / 2)  < 0)
        return -1;
    return (curIndex - 1) / 2;
}

/* Calculates the index of the left child of the element with the
 * provided index.
 */
int PQHeap::getLeftChildIndex(int curIndex) {
    if ((2 * curIndex + 1) >= _numFilled)
        return -1;
    return 2 * curIndex + 1;
}

/* Calculates the index of the right child of the element with the
 * provided index.
 */
int PQHeap::getRightChildIndex(int curIndex) {
    if ((2 * curIndex + 2) >= _numFilled)
        return -1;
    return 2 * curIndex + 2;
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("enqueue tests") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    EXPECT_EQUAL(pq.size(), 9);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

static void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;

    TIME_OPERATION(40000, fillQueue(pq, 40000));
    TIME_OPERATION(40000, emptyQueue(pq, 40000));
}

