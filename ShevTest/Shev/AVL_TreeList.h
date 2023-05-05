
#pragma once

#include "typedef.h"


template <typename K, typename D = Void> class AVL_TreeList
{
public:
    struct Node
    {
        K key;
        D data;
        Node * left, * right, * prev, * next;
        int bal;
    };

    class NodeStor
    {
    public:
        virtual Node * get() = 0;
        virtual void put ( Node * ) = 0;
    };
private:
    Node * root, * fir, * cur;
    NodeStor * stor;
    unsigned count;
    bool isDel;

    D & add ( const K & x, const D & d, Node * & p, bool & h )
    {
        if ( ! p )
        {
            p = stor ? stor->get() : new Node;
            ++count;
            p->key = x;
            p->data = d;
            p->left = p->right = p->prev = p->next =0;
            p->bal = 0;
            h = true;
            return p->data;
        }
        if ( p->key > x )
        {
            Node * pp = p->left;
            D & res = add ( x, d, p->left, h );
            if ( ! pp )
            {
                pp = p->left;
                pp->prev = p->prev;
                pp->next = p;
                p->prev = pp;
            }
            if ( h )
            {
                switch ( p->bal )
                {
                case 0: p->bal = -1; break;
                case -1:
                    {
                        Node * p1 = p->left;
                        if ( p1->bal == -1 )
                        {
                            p->left = p1->right;
                            p1->right = p;
                            p->bal = 0;
                            p = p1;
                        }
                        else
                        {
                            Node * p2 = p1->right;
                            p1->right = p2->left;
                            p2->left = p1;
                            p->left = p2->right;
                            p2->right = p;
                            p ->bal = p2->bal == -1 ? 1 : 0;
                            p1->bal = p2->bal == 1 ? -1 : 0;
                            p = p2;
                        }
                    }
                case 1:
                    p->bal = 0;
                    h = false;
                }
            }
            return res;
        }
        if ( p->key < x )
        {
            Node * pp = p->right;
            D & res = add ( x, d, p->right, h );
            if ( ! pp )
            {
                pp = p->right;
                pp->next = p->next;
                pp->prev = p;
                p->next = pp;
            }
            if ( h )
            {
                switch ( p->bal )
                {
                case 0: p->bal = 1; break;
                case 1:
                    {
                        Node * p1 = p->right;
                        if ( p1->bal == 1 )
                        {
                            p->right = p1->left;
                            p1->left = p;
                            p->bal = 0;
                            p = p1;
                        }
                        else
                        {
                            Node * p2 = p1->left;
                            p1->left = p2->right;
                            p2->right = p1;
                            p->right = p2->left;
                            p2->left = p;
                            p ->bal = p2->bal == 1 ? -1 : 0;
                            p1->bal = p2->bal == -1 ? 1 : 0;
                            p = p2;
                        }
                    }
                case -1:
                    p->bal = 0;
                    h = false;
                }
            }
            return res;
        }
        h = false;
        p->data = d;
        return p->data;
    }

    static bool balanceL ( Node * & p ) // Левая ветвь стала короче
    {
        bool h = true;
        switch ( p->bal )
        {
        case -1:
            {
                p->bal = 0;
                break;
            }
        case 0: 
            {
                p->bal = 1;
                h = false;
                break;
            }
        case 1: // Балансировка
            {
                Node * p1 = p->right;
                const int b1 = p1->bal;
                if ( b1 >= 0 ) // Однократный RR-поворот
                {
                    p->right = p1->left;
                    p1->left = p;
                    if ( b1 == 0 )
                    {
                        p->bal = 1;
                        p1->bal = -1;
                        h = false;
                    }
                    else
                    {
                        p->bal = p1->bal = 0;
                    }
                    p = p1;
                }
                else // Двойной RL-поворот
                {
                    Node * p2 = p1->left;
                    const int b2 = p2->bal;
                    p1->left = p2->right;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left = p;
                    p->bal = b2 == 1 ? -1 : 0;
                    p1->bal = b2 == -1 ? 1 : 0;
                    p = p2;
                    p->bal = 0;
                }
                break;
            }
        }
        return h;
    }

    static bool balanceR ( Node * & p ) // Правая ветвь стала короче
    {
        bool h = true;
        switch ( p->bal )
        {
        case 1:
            {
                p->bal = 0;
                break;
            }
        case 0: 
            {
                p->bal = -1;
                h = false;
                break;
            }
        case -1: // Балансировка
            {
                Node * p1 = p->left;
                const int b1 = p1->bal;
                if ( b1 <= 0 ) // Однократный LL-поворот
                {
                    p->left = p1->right;
                    p1->right = p;
                    if ( b1 == 0 )
                    {
                        p->bal = -1;
                        p1->bal = 1;
                        h = false;
                    }
                    else
                    {
                        p->bal = p1->bal = 0;
                    }
                    p = p1;
                }
                else // Двойной LR-поворот
                {
                    Node * p2 = p1->right;
                    const int b2 = p2->bal;
                    p1->right = p2->left;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right = p;
                    p->bal = b2 == -1 ? 1 : 0;
                    p1->bal = b2 == 1 ? -1 : 0;
                    p = p2;
                    p->bal = 0;
                }
                break;
            }
        }
        return h;
    }

    static bool del ( Node * & q, Node * & r )
    {
        if ( r->right )
        {
            if ( ! del ( q, r->right ) ) return false;
            return balanceR ( r );
        }
        q = r;
        r = r->left;
        return true;
    }

    bool delet ( const K & x, Node * & p )
    {
        if ( ! p )
        {
            return false;
        }
        bool h;
        if ( p->key > x )
        {
            h = delet ( x, p->left );
            if ( h ) h = balanceL ( p );
        }
        else
        if ( p->key < x )
        {
            h = delet ( x, p->right );
            if ( h ) h = balanceR ( p );
        }
        else
        {
            Node * q = p;
            if ( ! q->right )
            {
                p = q->left;
                h = true;
            }
            else
            if ( ! q->left )
            {
                p = q->right;
                h = true;
            }
            else
            {
                h = del ( q, q->left );
                Node * t = q;
                q = p;
                p = t;
                p->bal = q->bal;
                p->left = q->left;
                p->right = q->right;
                if ( h ) h = balanceL ( p );
            }
            if ( q == fir ) fir = fir->next;
            if ( q == cur )
            {
                cur = cur->next ? cur->next : cur->prev;
            }
            if ( q->prev ) q->prev->next = q->next;
            if ( q->next ) q->next->prev = q->prev;
            if ( stor ) stor->put ( q ); else delete q;
            --count;
            isDel = true;
        }
        return h;
    }

    void delAll ( Node * p )
    {
        if ( ! p ) return;
        delAll ( p->left );
        delAll ( p->right );
        if ( stor ) stor->put ( p ); else delete p;
    }

    static int height ( const Node * p )
    {
        if ( ! p ) return 0;
        int n1 = height ( p->left );
        int n2 = height ( p->right );
        return 1 + ( n1 > n2 ? n1 : n2 );
    }

    static unsigned countItems ( const Node * p )
    {
        if ( !p ) return 0;
        return 1 + countItems ( p->left ) + countItems ( p->right );
    }

    unsigned countItems() const
    {
        return countItems ( root );
    }

    static bool test ( const Node * p )
    {
        if ( ! p ) return true;
        int n1 = height ( p->left );
        int n2 = height ( p->right );
        if ( n1 + 1 < n2 || n2 + 1 < n1 ) return false;
        if ( n2 != n1 + p->bal ) return false;
        return test ( p->left ) && test ( p->right );
    }
public:
    explicit AVL_TreeList ( NodeStor * s = 0 ) : root ( 0 ), fir(0), cur(0), stor ( s ), count ( 0 ) {}

    ~AVL_TreeList ()
    {
        delAll ( root );
    }

    D & add ( const K & x, const D & d )
    {
        bool h;
        D & r = add ( x, d, root, h );
        if ( ! fir )
        {
            fir = cur = root;
            fir->prev = fir->next = 0;
        }
        return r;
    }

    D & add ( const K & x )
    {
        return add ( x, D() );
    }

    bool del ( const K & x )
    {
        isDel = false;
        delet ( x, root );
        return isDel;
    }

    D * find ( const K & x )
    {
        Node * p = root;
        while ( p )
        {
            if ( p->key > x )
                p = p->left;
            else
            if ( p->key < x )
                p = p->right;
            else
                return & p->data;
        }
        return 0;
    }

    Node * first()
    {
        return cur = fir;
    }

    Node * next()
    {
        return cur ? cur = cur->next : 0;
    }

    unsigned size() const
    {
        return count;
    }

    bool test() const
    {
        if ( count != countItems() )
            return false;
        Node * p = fir;
        if ( p )
            while ( p->next )
            {
                if ( p->key > p->next->key )
                    return false;
                p = p->next;
            }
        return test ( root );
    }
};