
#pragma once

#include "template.h"

template <class T>
class BaseHeap // Очередь с приоритетом
{
protected:
    nat _size, max_size;
    T * heap;
// Запрет конструктора копии и оператора присваивания
    BaseHeap ( const BaseHeap & );
    void operator = ( const BaseHeap & );
public:
    explicit BaseHeap ( nat n ) : _size(0), max_size(n), heap ( new T[n+1] ) {}
    ~BaseHeap () { delete[] heap; }
// Указатель на i-тый элемент ( 0, если i выходит за предел )
    const T * operator[] ( nat i ) const { return _size > i ? heap + i + 1 : 0; }
          T * operator[] ( nat i )       { return _size > i ? heap + i + 1 : 0; }
// Количество элементов в очереди
    nat size() const { return _size; }
// Очистить очередь
    void clear() { _size = 0; }
};

// Очередь с максимальным приоритетом
template <class T, void (swap_func) ( T & i1, T & i2 ) = _swap<T>> 
class MaxHeap : public BaseHeap<T>
{
// Запрет конструктора копии и оператора присваивания
    MaxHeap ( const MaxHeap & );
    void operator = ( const MaxHeap & );
public:
    explicit MaxHeap ( nat n ) : BaseHeap<T> ( n ) {}
// Поднять i-тый элемент
    void raise ( nat i )
    {
        if ( i >= _size ) return;
        for ( ++i; i > 1; )
        {
            const nat j = i >> 1;
            if ( heap[j] >= heap[i] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Опустить i-тый элемент
    void down ( nat i )
    {
        for (++i;;)
        {
            const nat i1 = i + i;
            if ( i1 > _size ) break;
            const nat i2 = i1 + 1;
            const nat j = i2 > _size ? i1 : heap[i1] >= heap[i2] ? i1 : i2;
            if ( heap[i] >= heap[j] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Удалить i-тый элемент
    bool del ( nat i )
    {
        if ( i >= _size ) return false;
        const nat i1 = i + 1;
        if ( i1 == _size ) { _size--; return true; }
        swap_func ( heap[i1], heap[_size] );
        if ( heap[i1] < heap[_size--] )
            down ( i );
        else
            raise ( i );
        return true;
    }
// Добавить элемент в очередь
    bool operator << ( const T & o )
    {
        if ( _size == max_size ) return false;
        heap[++_size] = o;
        raise ( _size - 1 );
        return true;
    }
// Убрать максимальный элемент из очереди
    bool operator >> ( T & o )
    {
        if ( _size == 0 ) return false;
        o = heap[1];
        swap_func ( heap[1], heap[_size--] );
        down ( 0 );
        return true;
    }
};

template<class T> bool testMaxHeap ( const MaxHeap<T> & heap )
{
    const nat n = heap.size() / 2;
    for ( nat j = 0; j < n; ++j )
    {
        const T * t1 = heap[j];
        const T * t2 = heap[j+j+1];
        if ( t2 && *t2 > *t1 )
            return false;
        const T * t3 = heap[j+j+2];
        if ( t3 && *t3 > *t1 )
            return false;
    }
    return true;
}

// Очередь с минимальным приоритетом
template <class T, void (swap_func) ( T & i1, T & i2 ) = _swap<T>> 
class MinHeap : public BaseHeap<T>
{
// Запрет конструктора копии и оператора присваивания
    MinHeap ( const MinHeap & );
    void operator = ( const MinHeap & );
public:
    explicit MinHeap ( nat n ) : BaseHeap<T> ( n ) {}
// Поднять i-тый элемент
    void raise ( nat i )
    {
        if ( i >= _size ) return;
        for ( ++i; i > 1; )
        {
            const nat j = i >> 1;
            if ( heap[j] <= heap[i] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Опустить i-тый элемент
    void down ( nat i )
    {
        for (++i;;)
        {
            const nat i1 = i + i;
            if ( i1 > _size ) break;
            const nat i2 = i1 + 1;
            const nat j = i2 > _size ? i1 : heap[i1] <= heap[i2] ? i1 : i2;
            if ( heap[i] <= heap[j] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Удалить i-тый элемент
    bool del ( nat i )
    {
        if ( i >= _size ) return false;
        const nat i1 = i + 1;
        if ( i1 == _size ) { _size--; return true; }
        swap_func ( heap[i1], heap[_size] );
        if ( heap[i1] > heap[_size--] )
            down ( i );
        else
            raise ( i );
        return true;
    }
// Добавить элемент в очередь
    bool operator << ( const T & o )
    {
        if ( _size == max_size ) return false;
        heap[++_size] = o;
        raise ( _size - 1 );
        return true;
    }
// Убрать минимальный элемент из очереди
    bool operator >> ( T & o )
    {
        if ( _size == 0 ) return false;
        o = heap[1];
        swap_func ( heap[1], heap[_size--] );
        down ( 0 );
        return true;
    }
};

template<class T> bool testMinHeap ( const MinHeap<T> & heap )
{
    const nat n = heap.size() / 2;
    for ( nat j = 0; j < n; ++j )
    {
        const T * t1 = heap[j];
        const T * t2 = heap[j+j+1];
        if ( t2 && *t2 < *t1 )
            return false;
        const T * t3 = heap[j+j+2];
        if ( t3 && *t3 < *t1 )
            return false;
    }
    return true;
}

#include "ShevArray.h"

// Очередь с максимальным приоритетом
template <class T, void (swap_func) ( T & i1, T & i2 ) = _swap<T>>
class MaxDynHeap
{
    Suite<T> heap;
// Запрет конструктора копии и оператора присваивания
    MaxDynHeap ( const MaxDynHeap & );
    void operator = ( const MaxDynHeap & );
public:
    explicit MaxDynHeap ( nat n = 7 ) : heap ( n + 1, 1 ) {}
// Указатель на i-тый элемент ( 0, если i выходит за предел )
    const T * operator[] ( nat i ) const { return heap ( i + 1 ); }
          T * operator[] ( nat i )       { return heap ( i + 1 ); }
// Количество элементов в очереди
    nat size() const { return heap.size() - 1; }
// Очистить очередь
    void clear() { heap.resize(1); }
// Поднять i-тый элемент
    void raise ( nat i )
    {
        if ( ++i >= heap.size() ) return;
        while ( i > 1 )
        {
            const nat j = i >> 1;
            if ( heap[j] >= heap[i] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Опустить i-тый элемент
    void down ( nat i )
    {
        for (++i;;)
        {
            const nat i1 = i + i;
            if ( i1 >= heap.size() ) break;
            const nat i2 = i1 + 1;
            const nat j = i2 >= heap.size() ? i1 : heap[i1] >= heap[i2] ? i1 : i2;
            if ( heap[i] >= heap[j] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Удалить i-тый элемент
    bool del ( nat i )
    {
        const nat i1 = i + 1;
        if ( i1 >= heap.size() ) return false;
        if ( i1 + 1 == heap.size() )
        {
            heap.dec();
        }
        else
        {
            T & t = heap.las();
            swap_func ( heap[i1], t );
            heap.dec();
            if ( heap[i1] < t )
                down ( i );
            else
                raise ( i );
        }
        return true;
    }
// Добавить элемент в очередь
    void operator << ( const T & t )
    {
        heap.inc() = t;
        raise ( heap.size() - 2 );
    }
// Убрать максимальный элемент из очереди
    bool operator >> ( T & t )
    {
        if ( heap.size() == 1 ) return false;
        t = heap[1];
        swap_func ( heap[1], heap.las() );
        heap.dec();
        down ( 0 );
        return true;
    }
};

// Очередь с минимальным приоритетом
template <class T, void (swap_func) ( T & i1, T & i2 ) = _swap<T>>
class MinDynHeap
{
    Suite<T> heap;
// Запрет конструктора копии и оператора присваивания
    MinDynHeap ( const MinDynHeap & );
    void operator = ( const MinDynHeap & );
public:
    explicit MinDynHeap ( nat n = 7 ) : heap ( n + 1, 1 ) {}
// Указатель на i-тый элемент ( 0, если i выходит за предел )
    const T * operator[] ( nat i ) const { return heap ( i + 1 ); }
          T * operator[] ( nat i )       { return heap ( i + 1 ); }
// Количество элементов в очереди
    nat size() const { return heap.size() - 1; }
// Очистить очередь
    void clear() { heap.resize(1); }
// Поднять i-тый элемент
    void raise ( nat i )
    {
        if ( ++i >= heap.size() ) return;
        while ( i > 1 )
        {
            const nat j = i >> 1;
            if ( heap[j] <= heap[i] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Опустить i-тый элемент
    void down ( nat i )
    {
        for (++i;;)
        {
            const nat i1 = i + i;
            if ( i1 >= heap.size() ) break;
            const nat i2 = i1 + 1;
            const nat j = i2 >= heap.size() ? i1 : heap[i1] <= heap[i2] ? i1 : i2;
            if ( heap[i] <= heap[j] ) break;
            swap_func ( heap[i], heap[j] );
            i = j;
        }
    }
// Удалить i-тый элемент
    bool del ( nat i )
    {
        const nat i1 = i + 1;
        if ( i1 >= heap.size() ) return false;
        if ( i1 + 1 == heap.size() )
        {
            heap.dec();
        }
        else
        {
            T & t = heap.las();
            swap_func ( heap[i1], t );
            heap.dec();
            if ( heap[i1] > t )
                down ( i );
            else
                raise ( i );
        }
        return true;
    }
// Добавить элемент в очередь
    void operator << ( const T & t )
    {
        heap.inc() = t;
        raise ( heap.size() - 2 );
    }
// Убрать максимальный элемент из очереди
    bool operator >> ( T & t )
    {
        if ( heap.size() == 1 ) return false;
        t = heap[1];
        swap_func ( heap[1], heap.las() );
        heap.dec();
        down ( 0 );
        return true;
    }
};