
#pragma once

#include "AVL_Tree.h"
#include "Heap.h"

template <class T> struct HeapAndTreeStor
{
    AVL_TreeNodeStor<SortItem<T, nat> > stor;
};

template <class T> class MaxHeapAndTree : public HeapAndTreeStor<T>
{
public:
    AVL_Tree<SortItem<T, nat>> tree;
    MaxDynHeap<SortItem<double, SwapPtr<nat>>> heap;
public:
    MaxHeapAndTree() : tree ( & stor ) {}
    bool add ( double d, const T & t )
    {
        SortItem<T, nat> si;
        si.head = t;
        si.tail = heap.size();
        const SortItem<T, nat> & r = tree.add ( si );
        if ( r.tail != si.tail ) return false;
        heap << SortItem<double, SwapPtr<nat>> ( d, (nat*) &r.tail );
        return true;
    }
    bool del ( const T & t )
    {
        const SortItem<T, nat> * p = tree.find ( t );
        if ( ! p ) return false;
        if ( ! heap.del ( p->tail ) ) return false;
        tree.del ( t );
        return true;
    }
    const T * maxItem() const
    {
        const SortItem<double, SwapPtr<nat>> * p = heap[0];
        if ( ! p ) return 0;
        typedef SortItem<T, nat> TN;
        const TN * t = container_of ( p->tail.ptr, TN, tail );
        return & t->head;
    }
    bool has ( const T & t ) const
    {
        return tree.find ( t ) != 0;
    }
};

template <class T> void checkMHAT ( const MaxHeapAndTree<T> & mhat )
{
    nat nt = mhat.tree.countItems();
    if ( mhat.heap.size() != nt )
    {
        nt = nt;
    }
    for ( nat i = 0; i < mhat.heap.size(); ++i )
    {
        typedef SortItem<T, nat> tn;
        const SortItem<T, nat> * ptr1 = container_of ( mhat.heap[i]->tail.ptr, tn, tail );
        for ( nat j = i+1; j < mhat.heap.size(); ++j )
        {
            if ( ptr1->head == container_of ( mhat.heap[j]->tail.ptr, tn, tail )->head )
                j=j;
        }
        const SortItem<T, nat> * ptr2 = mhat.tree.find ( *ptr1 );
        if ( ptr1 != ptr2 )
        {
            const SortItem<T, nat> * ptr3 = mhat.stor.find ( *ptr1 );
            i = i;
        }
        if ( i != ptr1->tail )
            i=i;
    }
}
