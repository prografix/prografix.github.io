
#include "../Shev/HeapAndTree.h"

#include "display.h"

/*

    unsigned height ( const T & x ) const
    {
        const Node * p = root;
        nat i = 0;
        while ( p )
        {
            ++i;
            if ( p->key > x )
                p = p->left;
            else
            if ( p->key < x )
                p = p->right;
            else
                return i;
        }
        return 0;
    }

    unsigned height2 ( const T & x ) const
    {
        const Node * p = root;
        nat i = 0;
        while ( p )
        {
            ++i;
            if ( p->key > x )
                p = p->left;
            else
            if ( p->key < x )
                p = p->right;
            else
                return !p->right && !p->left ? i : 0;
        }
        return 0;
    }
*/

inline 
void _swap ( SortItem<double, SortItemPtr<nat> > & p1, SortItem<double, SortItemPtr<nat> > & p2 )
{
    _swap ( p1.head, p2.head );
    _swap ( p1.tail, p2.tail );
    _swap ( p1.tail.ptr->tail, p2.tail.ptr->tail );
}

namespace
{

void tree_test1()
{
    const nat nn = 27;
    AVL_TreeNodeStor<nat> stor;
    for ( nat n = 1; n <= nn; ++n )
    {
        for ( nat i = 1; i <= n; ++i )
        {
            AVL_Tree<nat> tree ( & stor );
            for ( nat j = 1; j <= n; ++j )
            {
                tree.add ( j );
            }
            tree.del ( i );
            if ( ! tree.test() ) display << "error" << NL;
        }
    }
    display << "end" << NL;
}

void heap_test1()
{
    const nat nn = 17;
    for ( nat n = nn; n <= nn; ++n )
    {
		nat j;
        //for ( nat i = 1; i <= n; ++i )
        {
            MaxHeap<nat> heap ( nn );
            for ( j = 1; j <= n; ++j )
            {
                heap << j;
                if ( ! testMaxHeap ( heap ) )
                    display << "error" << NL;
            }
            for ( j = 1; j <= n; j += 2 )
            {
                nat * p = heap[j];
                if ( ! p ) continue;
                nat k = *p;
                if ( heap.del ( j ) )
                    display << heap.size() << k << NL;
                if ( ! testMaxHeap ( heap ) )
                    display << "error" << NL;
            }
            for ( j = 1; j <= n; ++j )
            {
                nat k = 0;
                if ( heap >> k )
                    display << k << NL;
                if ( ! testMaxHeap ( heap ) )
                    display << "error" << NL;
            }
        }
    }
    display << "end" << NL;
}

template <class T>  void MaxHeapAndTree_test()
{
    const nat nn = 17;
    for ( nat n = nn; n <= nn; ++n )
    {
        //for ( nat i = 1; i <= n; ++i )
        {
            MaxHeapAndTree<T> heap;
            for ( nat j = 1; j <= n; ++j )
            {
                if ( ! heap.add ( j, j ) )
                   display << "error" << NL;
            }
            for ( j = 1; j <= n; j += 2 )
            {
                if ( ! heap.del ( j ) )
                    display << "error" << NL;
            }
            for ( j = 1; j <= n; ++j )
            {
                const T * k = heap.maxItem();
                if ( k )
                {
                   display << *k << NL;
                   heap.del ( *k );
                }
            }
        }
    }
    display << "end" << NL;
}

} // end of namespace

void tree_test()
{
//    MaxHeapAndTree_test<nat>();
    tree_test1();
//    heap_test1();
}
