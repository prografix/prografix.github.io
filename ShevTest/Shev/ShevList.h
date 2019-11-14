
#ifndef SHEV_LIST_H
#define SHEV_LIST_H

#include "typedef.h"

namespace SL
{

class ShevItem
{
    ShevItem * prevPtr;
    ShevItem * nextPtr;
// Запрет конструктора копии и оператора присваивания:
    ShevItem ( ShevItem & );
    void operator = ( ShevItem & );
protected:
    virtual ~ShevItem ();
public:
    int info; // дополнительная информация
// Конструктор
    explicit ShevItem ( int i = -123456789 ) : prevPtr(0), nextPtr(0), info(i) {}

friend class ShevList;
friend class ShowList;
};

//***************************************************************************//

class Predicate1
{
public:
    virtual bool operator () ( const ShevItem * ) = 0;
};

class InfoEQ : public Predicate1
{
public:
    const int i;
    explicit InfoEQ ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class InfoNE : public Predicate1
{
public:
    const int i;
    explicit InfoNE ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class InfoLT : public Predicate1
{
public:
    const int i;
    explicit InfoLT ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class InfoGT : public Predicate1
{
public:
    const int i;
    explicit InfoGT ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class InfoLE : public Predicate1
{
public:
    const int i;
    explicit InfoLE ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class InfoGE : public Predicate1
{
public:
    const int i;
    explicit InfoGE ( int p ) : i(p) {}
    virtual bool operator () ( const ShevItem * p );
};

class NotPredicate1 : public Predicate1
{
public:
    Predicate1 & pre;
    explicit NotPredicate1 ( Predicate1 & p ) : pre(p) {}
    virtual bool operator () ( const ShevItem * p );
};

inline NotPredicate1 operator ! ( Predicate1 & p ) { return NotPredicate1(p); }

class Or_Predicate1 : public Predicate1
{
public:
    Predicate1 & pre1;
    Predicate1 & pre2;
    Or_Predicate1 ( Predicate1 & p1, Predicate1 & p2 ) : pre1(p1), pre2(p2) {}
    virtual bool operator () ( const ShevItem * p );
};

inline Or_Predicate1 operator || ( Predicate1 & p1, Predicate1 & p2 ) { return Or_Predicate1(p1,p2); }

class AndPredicate1 : public Predicate1
{
public:
    Predicate1 & pre1;
    Predicate1 & pre2;
    AndPredicate1 ( Predicate1 & p1, Predicate1 & p2 ) : pre1(p1), pre2(p2) {}
    virtual bool operator () ( const ShevItem * p );
};

inline AndPredicate1 operator && ( Predicate1 & p1, Predicate1 & p2 ) { return AndPredicate1(p1,p2); }

//***************************************************************************//

class Predicate2
{
public:
    virtual bool operator () ( const ShevItem *, const ShevItem * ) = 0;
};

class Double123Sorter : public Predicate2
{
    const double * array;
public:
    explicit Double123Sorter ( const double * p ) : array ( p ) {} 
    bool operator () ( const ShevItem *, const ShevItem * );
};

class Double321Sorter : public Predicate2
{
    const double * array;
public:
    explicit Double321Sorter ( const double * p ) : array ( p ) {} 
    bool operator () ( const ShevItem *, const ShevItem * );
};

//***************************************************************************//

class Alter
{
public:
    virtual void operator () ( ShevItem * );
};

extern Alter none;

class InfoAsn : public Alter
{
public:
    const int i;
    explicit InfoAsn ( int p ) : i(p) {}
    virtual void operator () ( ShevItem * p );
};

class InfoAdd : public Alter
{
public:
    const int i;
    explicit InfoAdd ( int p ) : i(p) {}
    virtual void operator () ( ShevItem * p );
};

class InfoSub : public Alter
{
public:
    const int i;
    explicit InfoSub ( int p ) : i(p) {}
    virtual void operator () ( ShevItem * p );
};

class InfoMul : public Alter
{
public:
    const int i;
    explicit InfoMul ( int p ) : i(p) {}
    virtual void operator () ( ShevItem * p );
};

class InfoDiv : public Alter
{
public:
    const int i;
    explicit InfoDiv ( int p ) : i(p) {}
    virtual void operator () ( ShevItem * p );
};

class InfoAP : public Alter
{
public:
    int start, step;
    InfoAP ( int p1, int p2 ) : start(p1), step(p2) {}
    virtual void operator () ( ShevItem * p );
};

class If : public Alter
{
public:
    Predicate1 & pre;
    Alter & alt1, & alt2;
    If ( Predicate1 & p, Alter & a1, Alter & a2 ) : pre(p), alt1(a1), alt2(a2) {}
    virtual void operator () ( ShevItem * p );
};

//***************************************************************************//

class InfoActGen
{
public:
    InfoEQ operator == ( int i ) { return InfoEQ ( i ); }
    InfoNE operator != ( int i ) { return InfoNE ( i ); }
    InfoLT operator <  ( int i ) { return InfoLT ( i ); }
    InfoGT operator >  ( int i ) { return InfoGT ( i ); }
    InfoLE operator <= ( int i ) { return InfoLE ( i ); }
    InfoGE operator >= ( int i ) { return InfoGE ( i ); }

    InfoAsn operator  = ( int i ) { return InfoAsn ( i ); }
    InfoAdd operator += ( int i ) { return InfoAdd ( i ); }
    InfoSub operator -= ( int i ) { return InfoSub ( i ); }
    InfoMul operator *= ( int i ) { return InfoMul ( i ); }
    InfoDiv operator /= ( int i ) { return InfoDiv ( i ); }
};

extern InfoActGen Info;

//***************************************************************************//

class ShevList
{
    ShevItem * cPtr; // указатель на текущий элемент списка
    ShevItem * fPtr; // указатель на первый элемент списка
    ShevItem * lPtr; // указатель на последний элемент списка
    nat listSize; // количество элементов в списке
// Запрет конструктора копии и оператора присваивания:
    ShevList ( ShevList & );
    void operator = ( ShevList & );
public:
// Конструктор и деструктор:
    ShevList () { cPtr = fPtr = lPtr = 0; listSize = 0; }
    ~ShevList ();
// Количество элементов в списке:
    nat size () const { return listSize; }
// Количество элементов с заданным свойством:
    nat count ( Predicate1 & ) const;
// Первый элемент с заданным свойством:
          ShevItem * fir ( Predicate1 & );
    const ShevItem * fir ( Predicate1 & ) const;
// Последний элемент с заданным свойством:
          ShevItem * las ( Predicate1 & );
    const ShevItem * las ( Predicate1 & ) const;
// Первый элемент с минимальным info:
    const ShevItem * firMinInfo () const;
// Первый элемент с максимальным info:
    const ShevItem * firMaxInfo () const;
// Текущий элемент первый?
    bool curIsFir () const { return cPtr == fPtr; }
// Текущий элемент последний?
    bool curIsLas () const { return cPtr == lPtr; }
// Имеется ли в списке данный элемент?
    bool has ( const ShevItem * ) const;
// Доступ к отдельному элементу списка:
    const ShevItem * fir () const { return fPtr; }
    const ShevItem * cur () const { return cPtr; }
    const ShevItem * las () const { return lPtr; }
    ShevItem * fir () { return fPtr; }
    ShevItem * cur () { return cPtr; }
    ShevItem * las () { return lPtr; }
// Сделать текущим первый элемент:
    ShevItem * top () { return cPtr = fPtr; }
// Сделать текущим последний элемент:
    ShevItem * end () { return cPtr = lPtr; }
// Сделать текущим предыдущий элемент:
    ShevItem * prev () { return cPtr != fPtr ? cPtr = cPtr->prevPtr : 0; }
// Сделать текущим следующий элемент:
    ShevItem * next () { return cPtr != lPtr ? cPtr = cPtr->nextPtr : 0; }
// Сделать текущим предыдущий элемент в цикле:
    ShevItem * cprev () { return cPtr = (cPtr==fPtr) ? lPtr : cPtr->prevPtr; }
// Сделать текущим следующий элемент в цикле:
    ShevItem * cnext () { return cPtr = (cPtr==lPtr) ? fPtr : cPtr->nextPtr; }
// Сделать текущим данный элемент ( небезопасная функция ):
    ShevList & jump ( ShevItem * m ) { cPtr = m; return *this; }
// Сделать текущим первый элемент с заданным свойством:
    bool findFir ( Predicate1 & );
// Сделать текущим последний элемент с заданным свойством:
    bool findLas ( Predicate1 & );
// Добавить один элемент
    ShevList & addBefFir ( ShevItem * ); // перед первым
    ShevList & addBefCur ( ShevItem * ); // перед текущим и сделать его текущим
    ShevList & addAftCur ( ShevItem * ); // после текущего и сделать его текущим
    ShevList & addAftLas ( ShevItem * ); // после последнего элемента
// Добавить из другого списка
    ShevList & addAllBefFir ( ShevList & ); // все элементы в начало
    ShevList & addAllBefCur ( ShevList & ); // все элементы перед текущим
    ShevList & addAllAftCur ( ShevList & ); // все элементы после текущего
    ShevList & addAllAftLas ( ShevList & ); // все элементы в конец
// Изменение порядка следования элементов в списке:
    ShevList & invert ();
    ShevList & makeFir ( ShevItem * );
    ShevList & makeLas ( ShevItem * );
    ShevList & makeCurFir () { return makeFir ( cPtr ); }
    ShevList & makeCurLas () { return makeLas ( cPtr ); }
    ShevList & sort123 ();
    ShevList & sort321 ();
    ShevList & sort ( Predicate2 & );
// Перемещение одного элемента из списка в список ( Эти действия двухактовые -
// вначале элемент выносится из первого списка, затем добавляется во второй.
// Это принципиально когда списки совпадают. ):
    ShevList & movFirBefFir ( ShevList & list ) { list.addBefFir ( outFir() ); return *this; }
    ShevList & movFirBefCur ( ShevList & list ) { list.addBefCur ( outFir() ); return *this; }
    ShevList & movFirAftCur ( ShevList & list ) { list.addAftCur ( outFir() ); return *this; }
    ShevList & movFirAftLas ( ShevList & list ) { list.addAftLas ( outFir() ); return *this; }
    ShevList & movCurBefFir ( ShevList & list );
    ShevList & movCurBefCur ( ShevList & list );
    ShevList & movCurAftCur ( ShevList & list );
    ShevList & movCurAftLas ( ShevList & list );
    ShevList & movLasBefFir ( ShevList & list ) { list.addBefFir ( outLas() ); return *this; }
    ShevList & movLasBefCur ( ShevList & list ) { list.addBefCur ( outLas() ); return *this; }
    ShevList & movLasAftCur ( ShevList & list ) { list.addAftCur ( outLas() ); return *this; }
    ShevList & movLasAftLas ( ShevList & list ) { list.addAftLas ( outLas() ); return *this; }
// Перемещение группы элементов из списка в список:
    ShevList & movBefCurBefCur ( ShevList & ); // Перенос всех элементов перед текущим из одного списка в другой список перед текущим
    ShevList & movAftCurAftCur ( ShevList & ); // Перенос всех элементов после текущего из одного списка в другой список после текущего
    ShevList & movFirBefFir ( nat, ShevList & ); // Перенос п первых элементов из одного списка в начало другого списка
    ShevList & movLasAftLas ( nat, ShevList & ); // Перенос п последних элементов из одного списка в конец другого списка
    ShevList & movAllBefFir ( ShevList & ); // Перенос всех элементов из одного списка в начало другого списка
    ShevList & movAllBefCur ( ShevList & ); // Перенос всех элементов из одного списка перед текущим другого списка
    ShevList & movAllAftCur ( ShevList & ); // Перенос всех элементов из одного списка после текущего другого списка
    ShevList & movAllAftLas ( ShevList & ); // Перенос всех элементов из одного списка в конец другого списка
    ShevList & mov123BefFir ( Predicate1 &, ShevList & );
    ShevList & mov123AftLas ( Predicate1 &, ShevList & );
    ShevList & mov321BefFir ( Predicate1 &, ShevList & );
    ShevList & mov321AftLas ( Predicate1 &, ShevList & );
    ShevList & movAftCurAftLas ( ShevList & ); // Перенос всех элементов после текущего из одного списка в конец другого списка
// Обмен элементами внутри списка:
    ShevList & swapCurAndFir ();
    ShevList & swapCurAndLas ();
    ShevList & swapFirAndLas ();
// Обмен элементами между двумя списками:
    ShevList & swapCurAndCur ( ShevList & ); // Обмен текущими элементами
    ShevList & swapAllAndAll ( ShevList & ); // Обмен всеми элементами
// Для всех элементов присвоить члену info значение i:
    ShevList & setAllInfo ( int i );
// Обработать все элементы списка:
    ShevList & for_each123 ( Alter & );
    ShevList & for_each321 ( Alter & );
// Обработать некоторые элементы списка:
    ShevList & for_some123 ( Predicate1 &, Alter & );
    ShevList & for_some321 ( Predicate1 &, Alter & );
protected:
// Вынос элементов из списка:
    ShevItem * outFir ();
    ShevItem * outCur ();
    ShevItem * outLas ();
    ShevList & outAll () { cPtr = fPtr = lPtr = 0; listSize = 0; return *this; }
public:
// Удаление данного элемента из списка ( небезопасная функция ):
    ShevList & del ( ShevItem * m ) { cPtr = m; delete outCur (); return *this; }
// Удаление элементов из списка:
    ShevList & delFir () { delete outFir (); return *this; }
    ShevList & delCur () { delete outCur (); return *this; }
    ShevList & delLas () { delete outLas (); return *this; }
    ShevList & delAll ();
// Операции с сигналом о последнем элементе:
    bool delCur_ () { bool res = cPtr == lPtr; delete outCur(); return res; }
    bool movCurAftLas_ ( ShevList & list ) { bool res = cPtr == lPtr; movCurAftLas ( list ); return res; }
// Проверка на наличие ошибок ( возвращает указатель на строку с описанием ошибки или 0, если ошибок не обнаружено )
    const char * test () const;
// Класс просмотра константного списка
    friend class ShowList;
};

inline void swap ( ShevList & a, ShevList & b ) { a.swapAllAndAll ( b ); }

//***************************************************************************//

class ShowList
{
    const ShevItem *       cPtr; // указатель на текущий элемент списка
    const ShevItem * const fPtr; // указатель на первый элемент списка
    const ShevItem * const lPtr; // указатель на последний элемент списка
    const nat listSize; // количество элементов в списке
public:
    ShowList ( const ShevList & p ) : cPtr(p.cPtr), fPtr(p.fPtr), lPtr(p.lPtr), listSize(p.listSize) {}
// Количество элементов в списке:
    nat size () const { return listSize; }
// Количество элементов с заданным свойством
    nat count ( Predicate1 & ) const;
// Первый элемент с заданным свойством:
    const ShevItem * fir ( Predicate1 & ) const;
// Последний элемент с заданным свойством:
    const ShevItem * las ( Predicate1 & ) const;
// Первый элемент с минимальным info:
    const ShevItem * firMinInfo () const;
// Первый элемент с максимальным info:
    const ShevItem * firMaxInfo () const;
// Текущий элемент первый?
    bool curIsFir () const { return cPtr == fPtr; }
// Текущий элемент последний?
    bool curIsLas () const { return cPtr == lPtr; }
// Имеется ли в списке данный элемент?
    bool has ( const ShevItem * ) const;
// Доступ к отдельному элементу списка
    const ShevItem * fir () const { return fPtr; }
    const ShevItem * cur () const { return cPtr; }
    const ShevItem * las () const { return lPtr; }
// Сделать текущим первый элемент
    const ShevItem * top () { return cPtr = fPtr; }
// Сделать текущим последний элемент
    const ShevItem * end () { return cPtr = lPtr; }
// Сделать текущим предыдущий элемент
    const ShevItem * prev () { return cPtr != fPtr ? cPtr = cPtr->prevPtr : 0; }
// Сделать текущим следующий элемент
    const ShevItem * next () { return cPtr != lPtr ? cPtr = cPtr->nextPtr : 0; }
// Сделать текущим предыдущий элемент в цикле
    const ShevItem * cprev () { return cPtr = (cPtr==fPtr) ? lPtr : cPtr->prevPtr; }
// Сделать текущим следующий элемент в цикле
    const ShevItem * cnext () { return cPtr = (cPtr==lPtr) ? fPtr : cPtr->nextPtr; }
// Сделать текущим данный элемент ( небезопасная функция ):
    ShowList & jump ( const ShevItem * m ) { cPtr = m; return *this; }
// Сделать текущим первый элемент с заданным свойством:
    bool findFir ( Predicate1 & );
// Сделать текущим последний элемент с заданным свойством:
    bool findLas ( Predicate1 & );
};

//***************************************************************************//

template <class T> class ListItem : public T, public ShevItem
{
public:
    ListItem () {}
    explicit ListItem ( const T & t, int i = -123456789 ) : T(t), ShevItem(i) {}
    T & base () { return *this; }
};

template <class T> class ListItemA : public ShevItem
{
public:
    T a;
    ListItemA () {}
    explicit ListItemA ( const T & t, int i = -123456789 ) : a(t), ShevItem(i) {}
    operator const T & () const { return a; }
};

template <class T> class List : public ShevList
{
// Запрет конструктора копии и оператора присваивания:
    List ( List & );
    void operator = ( List & );
public:
    typedef T Par;
    List() {}

    const T * fir() const { return (const T *) ShevList::fir(); }
    const T * cur() const { return (const T *) ShevList::cur(); }
    const T * las() const { return (const T *) ShevList::las(); }

    T * fir ( Predicate1 & pre ) { return (T *) ShevList::fir(pre); }
    T * las ( Predicate1 & pre ) { return (T *) ShevList::las(pre); }

    const T * fir ( Predicate1 & pre ) const { return (const T *) ShevList::fir(pre); }
    const T * las ( Predicate1 & pre ) const { return (const T *) ShevList::las(pre); }

    T * fir ()  { return (T *) ShevList::fir (); }
    T * cur ()  { return (T *) ShevList::cur (); }
    T * las ()  { return (T *) ShevList::las (); }
    T * top ()  { return (T *) ShevList::top (); }
    T * end ()  { return (T *) ShevList::end (); }
    T * prev()  { return (T *) ShevList::prev(); }
    T * next()  { return (T *) ShevList::next(); }
    T * cprev() { return (T *) ShevList::cprev(); }
    T * cnext() { return (T *) ShevList::cnext(); }
// Сделать текущим данный элемент ( небезопасная функция ):
    List & jump ( T * i ) { return (List&) ShevList::jump(i); }
// Добавить в список один элемент
    List & addBefFir ( T * i ) { return (List&) ShevList::addBefFir(i); } // перед первым
    List & addBefCur ( T * i ) { return (List&) ShevList::addBefCur(i); } // перед текущим и сделать его текущим
    List & addAftCur ( T * i ) { return (List&) ShevList::addAftCur(i); } // после текущего и сделать его текущим
    List & addAftLas ( T * i ) { return (List&) ShevList::addAftLas(i); } // после последнего элемента
// Добавить из другого списка
    List & addAllBefFir ( List & list ) { return (List&) ShevList::addAllBefFir(list); } // все элементы в начало
    List & addAllBefCur ( List & list ) { return (List&) ShevList::addAllBefCur(list); } // все элементы перед текущим
    List & addAllAftCur ( List & list ) { return (List&) ShevList::addAllAftCur(list); } // все элементы после текущего
    List & addAllAftLas ( List & list ) { return (List&) ShevList::addAllAftLas(list); } // все элементы в конец

    T * addNewBefFir ( List & list )
    {
        if ( list.las() )
            list.ShevList::movLasBefFir ( *this );
        else
            ShevList::addBefFir ( new T );
        return fir();
    }

    T * addNewAftLas ( List & list )
    {
        if ( list.las() )
            list.ShevList::movLasAftLas ( *this );
        else
            ShevList::addAftLas ( new T );
        return las();
    }

    List & invert     () { return (List&) ShevList::invert (); }
    List & makeCurFir () { return (List&) ShevList::makeCurFir (); }
    List & makeCurLas () { return (List&) ShevList::makeCurLas (); }
    List & makeFir ( T * p ) { return (List&) ShevList::makeFir ( p ); }
    List & makeLas ( T * p ) { return (List&) ShevList::makeLas ( p ); }
    List & sort123    () { return (List&) ShevList::sort123 (); }
    List & sort321    () { return (List&) ShevList::sort321 (); }
    List & sort ( Predicate2 & p ) { return (List&) ShevList::sort ( p ); }

    List & movFirBefFir ( List & list ) { return (List&) ShevList::movFirBefFir ( list ); }
    List & movFirBefCur ( List & list ) { return (List&) ShevList::movFirBefCur ( list ); }
    List & movFirAftCur ( List & list ) { return (List&) ShevList::movFirAftCur ( list ); }
    List & movFirAftLas ( List & list ) { return (List&) ShevList::movFirAftLas ( list ); }
    List & movCurBefFir ( List & list ) { return (List&) ShevList::movCurBefFir ( list ); }
    List & movCurBefCur ( List & list ) { return (List&) ShevList::movCurBefCur ( list ); }
    List & movCurAftCur ( List & list ) { return (List&) ShevList::movCurAftCur ( list ); }
    List & movCurAftLas ( List & list ) { return (List&) ShevList::movCurAftLas ( list ); }
    List & movLasBefFir ( List & list ) { return (List&) ShevList::movLasBefFir ( list ); }
    List & movLasBefCur ( List & list ) { return (List&) ShevList::movLasBefCur ( list ); }
    List & movLasAftCur ( List & list ) { return (List&) ShevList::movLasAftCur ( list ); }
    List & movLasAftLas ( List & list ) { return (List&) ShevList::movLasAftLas ( list ); }

    List & movFirBefFir ( nat n, List & list ) { return (List&) ShevList::movFirBefFir ( n, list ); }
    List & movLasAftLas ( nat n, List & list ) { return (List&) ShevList::movLasAftLas ( n, list ); }

    List & movAllBefFir ( List & list ) { return (List&) ShevList::movAllBefFir ( list ); }
    List & movAllBefCur ( List & list ) { return (List&) ShevList::movAllBefCur ( list ); }
    List & movAllAftCur ( List & list ) { return (List&) ShevList::movAllAftCur ( list ); }
    List & movAllAftLas ( List & list ) { return (List&) ShevList::movAllAftLas ( list ); }
// Обмен элементами внутри списка:
    ShevList & swapCurAndFir () { return (List&) ShevList::swapCurAndFir (); }
    ShevList & swapCurAndLas () { return (List&) ShevList::swapCurAndLas (); }
    ShevList & swapFirAndLas () { return (List&) ShevList::swapFirAndLas (); }
// Обмен элементами между двумя списками:
    List & swapCurAndCur ( List & list ) { return (List&) ShevList::swapCurAndCur ( list ); } // Обмен текущими элементами
    List & swapAllAndAll ( List & list ) { return (List&) ShevList::swapAllAndAll ( list ); } // Обмен всеми элементами
// Для всех элементов присвоить члену info значение i:
    List & setAllInfo ( int i ) { return (List&) ShevList::setAllInfo(i); }
// Удаление элементов из списка
    List & delFir () { return (List&) ShevList::delFir (); }
    List & delCur () { return (List&) ShevList::delCur (); }
    List & delLas () { return (List&) ShevList::delLas (); }
    List & delAll () { return (List&) ShevList::delAll (); }

    template <class F> List & delSel ( F & filter )
    {
        if ( top() ) for(;;)
        { 
            if ( filter ( *cur() ) )
            {
                if ( delCur_() ) break;
                continue;
            }
            if ( ! next() ) break; 
        }
        return *this;
    }

    template <class T2> List & operator += ( const T2 & p )
    {
        if ( top() ) do { *cur() += p; } while ( next() );  return *this;
    }

    template <class T2> List & operator -= ( const T2 & p )
    {
        if ( top() ) do { *cur() -= p; } while ( next() );  return *this;
    }

    template <class T2> List & operator *= ( const T2 & p )
    {
        if ( top() ) do { *cur() *= p; } while ( next() );  return *this;
    }

    template <class T2> List & operator /= ( const T2 & p )
    {
        if ( top() ) do { *cur() /= p; } while ( next() );  return *this;
    }

    List & resize ( nat n )
    {
        while ( size() > n ) ShevList::delLas();
        while ( size() < n ) ShevList::addAftLas ( new T );
        return *this;
    }

    List & resize ( nat n, List & stor )
    {
        stor.addAllBefFir(*this).movFirBefFir ( n, *this );
        while ( size() < n ) ShevList::addAftLas ( new T );
        return *this;
    }
};

template <class T> class Show : public ShowList
{
public:
    Show ( const List<T> & p ) : ShowList ( p ) {}
// Доступ к отдельному элементу списка:
    const T * fir () const { return (const T *) ShowList::fir(); }
    const T * cur () const { return (const T *) ShowList::cur(); }
    const T * las () const { return (const T *) ShowList::las(); }
// Первый элемент с заданным свойством:
    const T * fir ( Predicate1 & p ) const { return (const T *) ShowList::fir(); }
// Последний элемент с заданным свойством:
    const T * las ( Predicate1 & p ) const { return (const T *) ShowList::las(); }
// Сделать текущим первый элемент
    const T * top () { return (const T *) ShowList::top(); }
// Сделать текущим последний элемент
    const T * end () { return (const T *) ShowList::end(); }
// Сделать текущим предыдущий элемент
    const T * prev () { return (const T *) ShowList::prev(); }
// Сделать текущим следующий элемент
    const T * next () { return (const T *) ShowList::next(); }
// Сделать текущим предыдущий элемент в цикле
    const T * cprev () { return (const T *) ShowList::cprev(); }
// Сделать текущим следующий элемент в цикле
    const T * cnext () { return (const T *) ShowList::cnext(); }
// Сделать текущим данный элемент ( небезопасная функция ):
    Show & jump ( const T * m ) { return *(Show*)& ShowList::jump ( m ); }
};

} // namespace SL

using namespace SL;

#endif
