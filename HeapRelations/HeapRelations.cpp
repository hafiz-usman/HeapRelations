// HeapRelations.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>

using namespace std;

class Heap
{
public:
    Heap(int capacity) :
        _capacity(capacity),
        _size(0)
    {
        assert(_capacity > _size);
        _heap.resize(_capacity + 1);  // we don't use 0-th index hence the +1 in actual underlying capacity
    }

    virtual bool top(int& retVal)
    {
        if (empty() != false)
        {
            return false;
        }
        retVal = _heap[1];
        return true;
    }

    virtual bool empty()
    {
        return (_size == 0);
    }

    virtual int size()
    {
        return _size;
    }

    virtual bool push(int val) = 0;
    virtual bool pop() = 0;

protected:
    int _capacity;
    int _size;
    vector<int> _heap;

    void _swap(int& a, int& b)
    {
        int temp = a;
        a = b;
        b = temp;
    }

    int _getParentIndex(int iCurrent)
    {
        // since index 0 is unused
        return (iCurrent / 2);
    }

    int _getLeftChildIndex(int iParent)
    {
        // since index 0 is unused
        return (iParent * 2);
    }

    int _getRightChildIndex(int iParent)
    {
        // since index 0 is unused
        return (iParent * 2 + 1);
    }
};

class MaxHeap : public Heap
{
public:
    MaxHeap(int capacity) :
        Heap(capacity)
    {}

    bool push(int val) override
    {
        if (_size >= _capacity)
        {
            return false;
        }

        // we don't use 0-th index hence the +1
        // start by pushing to last index
        int iPush = _size + 1;
        _heap[iPush] = val;

        // sift-up: move the element up to the right place
        int iCurrent = iPush;
        while ((iCurrent > 1) && (_heap[_getParentIndex(iCurrent)] < _heap[iCurrent]))
        {
            swap(_heap[_getParentIndex(iCurrent)], _heap[iCurrent]);
            iCurrent = _getParentIndex(iCurrent);
        }

        _size++;

        return true;
    }

    bool pop() override
    {
        if (empty() != false)
        {
            return false;
        }

        // move last element in heap to parent, update size
        // we don't use 0-th index hence the +1
        int iCurrent = 1;
        _heap[iCurrent] = _heap[_size];
        _size--;

        // sift-down
        while (_getLeftChildIndex(iCurrent) <= _size) // if there's no left child that means there's no right child hence no more children to check!
        {
            // let's assume the left child is greater than right child until proven otherwise!
            int iLargerChild = _getLeftChildIndex(iCurrent);
            if ((_getRightChildIndex(iCurrent) <= _size) &&
                (_heap[_getRightChildIndex(iCurrent)] > _heap[_getLeftChildIndex(iCurrent)]))
            {
                iLargerChild = _getRightChildIndex(iCurrent);
            }

            if (_heap[iCurrent] > _heap[iLargerChild])
            {
                // done!
                break;
            }
            _swap(_heap[iCurrent], _heap[iLargerChild]);
            iCurrent = iLargerChild;
        }

        return true;
    }
};


void testMaxHeap()
{
    vector<int>input{2,3,0,9,8,8,};
    vector<int> inputSortedAsc = input;
    sort(inputSortedAsc.begin(), inputSortedAsc.end());
    vector<int> inputSortedDesc = input;
    sort(inputSortedDesc.begin(), inputSortedDesc.end(), greater<int>());
    int capacity = input.size();
    bool flag = false;
    int val = -1;

    MaxHeap maxH(capacity);

    assert(maxH.empty() != false);
    assert(maxH.size() == 0);
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.top(val) == false);

    // add one element
    val = input[0];
    assert(maxH.push(val) == true);
    assert(maxH.empty() == false);
    assert(maxH.size() == 1);
    assert(maxH.top(val) != false);
    assert(val == input[0]);
    assert(maxH.pop() != false);
    // now the heap should be empty again
    assert(maxH.pop() == false);
    assert(maxH.empty() != false);
    assert(maxH.size() == 0);
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.top(val) == false);

    // TWO ELEMENT TEST (ascending)
    // add an element
    val = input[0];
    assert(maxH.push(val) == true);
    assert(maxH.empty() == false);
    assert(maxH.size() == 1);
    assert(maxH.top(val) != false);
    assert(val == input[0]);
    // add another smaller element
    val = input[0] - 1;
    assert(maxH.push(val) == true);
    assert(maxH.empty() == false);
    assert(maxH.size() == 2);
    assert(maxH.top(val) != false);
    assert(val == input[0]); // since input[0] is bigger than input[0]-1
    assert(maxH.pop() != false);
    assert(maxH.empty() == false);
    assert(maxH.size() == 1);
    assert(maxH.top(val) != false);
    assert(val == (input[0] - 1)); // since input[0] was bigger than input[0]-1
    assert(maxH.pop() != false);
    // now the heap should be empty again
    assert(maxH.pop() == false);
    assert(maxH.empty() != false);
    assert(maxH.size() == 0);
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.top(val) == false);

    // TWO ELEMENT TEST (descending)
    // add an element
    val = input[0];
    assert(maxH.push(val) == true);
    assert(maxH.empty() == false);
    assert(maxH.size() == 1);
    assert(maxH.top(val) != false);
    assert(val == input[0]);
    // add another bigger element
    val = input[0] + 1;
    assert(maxH.push(val) == true);
    assert(maxH.empty() == false);
    assert(maxH.size() == 2);
    assert(maxH.top(val) != false);
    assert(val == (input[0] + 1)); // since input[0] was smaller than input[0]-1
    assert(maxH.pop() != false);
    assert(maxH.empty() == false);
    assert(maxH.size() == 1);
    assert(maxH.top(val) != false);
    assert(val == input[0]); // since input[0] is smaller than input[0]-1
    assert(maxH.pop() != false);
    // now the heap should be empty again
    assert(maxH.pop() == false);
    assert(maxH.empty() != false);
    assert(maxH.size() == 0);
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.top(val) == false);


    // RANDOM ELEMENT TEST
    for (int i = 0; i < input.size(); i++)
    {
        assert(maxH.push(input[i]) == true);
    }
    assert(maxH.empty() == false);
    assert(maxH.size() == input.size());
    // maxHeap is full, one more insert should fail
    assert(maxH.push(100) == false);
    for (int i = 0; i < input.size(); i++)
    {
        assert(maxH.empty() == false);
        assert(maxH.top(val) == true);
        assert(val == inputSortedDesc[i]);
        assert(maxH.pop() == true);
    }
    // maxHeap is empty, no more to retrieve
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.empty() == true);
    assert(maxH.size() == 0);

    // ASCENDING ELEMENT TEST
    for (int i = 0; i < inputSortedAsc.size(); i++)
    {
        assert(maxH.push(inputSortedAsc[i]) == true);
    }
    assert(maxH.empty() == false);
    assert(maxH.size() == inputSortedAsc.size());
    // maxHeap is full, one more insert should fail
    assert(maxH.push(100) == false);
    for (int i = 0; i < inputSortedAsc.size(); i++)
    {
        assert(maxH.empty() == false);
        assert(maxH.top(val) == true);
        assert(val == inputSortedDesc[i]);
        assert(maxH.pop() == true);
    }
    // maxHeap is empty, no more to retrieve
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.empty() == true);
    assert(maxH.size() == 0);

    // DESCENDING ELEMENT TEST
    for (int i = 0; i < inputSortedDesc.size(); i++)
    {
        assert(maxH.push(inputSortedDesc[i]) == true);
    }
    assert(maxH.empty() == false);
    assert(maxH.size() == inputSortedDesc.size());
    // maxHeap is full, one more insert should fail
    assert(maxH.push(100) == false);
    for (int i = 0; i < inputSortedDesc.size(); i++)
    {
        assert(maxH.empty() == false);
        assert(maxH.top(val) == true);
        assert(val == inputSortedDesc[i]);
        assert(maxH.pop() == true);
    }
    // maxHeap is empty, no more to retrieve
    assert(maxH.top(val) == false);
    assert(maxH.pop() == false);
    assert(maxH.empty() == true);
    assert(maxH.size() == 0);
}

int main()
{
    testMaxHeap();
    return 0;
}

