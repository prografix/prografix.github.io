
#pragma once

template <class T> class AVL_Tree
{
public:
    struct Node
    {
        T key;
        Node * left, * right;
        int bal;
    };

    class NodeStor
    {
    public:
        virtual Node * get() = 0;
        virtual void put ( Node * ) = 0;
    };
private:
    Node * root;
    NodeStor * stor;

    static const T & add ( const T & x, Node * & p, bool & h, NodeStor * stor )
    {
        if ( ! p )
        {
            p = stor ? stor->get() : new Node;
            p->key = x;
            p->left = p->right = 0;
            p->bal = 0;
            h = true;
            return p->key;
        }
        if ( p->key > x )
        {
            const T & res = add ( x, p->left, h, stor );
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
            const T & res = add ( x, p->right, h, stor );
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
        return p->key;
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

    static bool delet ( const T & x, Node * & p, NodeStor * stor )
    {
        if ( ! p )
        {
            return false;
        }
        bool h;
        if ( p->key > x )
        {
            h = delet ( x, p->left, stor );
            if ( h ) h = balanceL ( p );
        }
        else
        if ( p->key < x )
        {
            h = delet ( x, p->right, stor );
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
            if ( stor )
                stor->put ( q );
            else
                delete q;
        }
        return h;
    }

    static void delAll ( Node * p, NodeStor * stor )
    {
        if ( ! p ) return;
        delAll ( p->left, stor );
        delAll ( p->right, stor );
        if ( stor )
            stor->put ( p );
        else
            delete p;
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
    AVL_Tree ( NodeStor * p = 0 ) : root ( 0 ), stor ( p ) {}

    ~AVL_Tree ()
    {
        delAll ( root, stor );
    }

    const T & add ( const T & x )
    {
        bool h;
        return add ( x, root, h, stor );
    }

    void del ( const T & x )
    {
        delet ( x, root, stor );
    }

    const T * find ( const T & x ) const
    {
        const Node * p = root;
        while ( p )
        {
            if ( p->key > x )
                p = p->left;
            else
            if ( p->key < x )
                p = p->right;
            else
                return & p->key;
        }
        return 0;
    }

    unsigned countItems() const
    {
        return countItems ( root );
    }

    bool test() const
    {
        return test ( root );
    }
};

template <class T> class AVL_TreeNodeStor : public AVL_Tree<T>::NodeStor
{
    typename AVL_Tree<T>::Node * data;
public:
    AVL_TreeNodeStor() : data(0) {}

    ~AVL_TreeNodeStor()
    {
        while ( data )
        {
            AVL_Tree<T>::Node * p = data->left;
            delete data;
            data = p;
        }
    }

    virtual typename AVL_Tree<T>::Node * get()
    {
        if ( data )
        {
            AVL_Tree<T>::Node * p = data;
            data = p->left;
            return p;
        }
        else
            return new AVL_Tree<T>::Node;
    }

    virtual void put ( typename AVL_Tree<T>::Node * p )
    {
        p->left = data;
        data = p;
    }

    const T * find ( const T & x ) const
    {
        const AVL_Tree<T>::Node * p = data;
        while ( p )
        {
            if ( p->key == x ) return & p->key;
            p = p->left;
        }
        return 0;
    }
};
