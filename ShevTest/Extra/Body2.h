
#pragma once

#include "Vert.h"
#include "../shev/template.h"

class Bone;
class Side;

class BonePtr : public ShevItem
{
    BonePtr ( BonePtr & );
    void operator = ( BonePtr & );
public:
    Bone * bone;
    BonePtr ( Bone * b = 0 ) : bone ( b ) {}

    Vert * vert0 ();
    Vert * vert1 ();
    Side * another ();

    const Vert * vert0 () const;
    const Vert * vert1 () const;
    const Side * another () const;
};

class Bone : public ShevItem
{
    Bone ( Bone & );
    void operator = ( Bone & );
public:
    Vert * v0, * v1;
    Side * s0, * s1;
    BonePtr b0,  b1;
    Bone () : v0(0), v1(0), s0(0), s1(0)
    {
        b0.bone = b1.bone = this;
    }
    Bone ( Vert * p1, Vert * p2, Side * p3, Side * p4, int i = -123456789 ) : v0(p1), v1(p2), s0(p3), s1(p4), ShevItem(i)
    {
        b0.bone = b1.bone = this;
    }
    double qmod () const
    {
        return ::qmod ( v0->point - v1->point );
    }
    double getDistance ( const Vector3d & p ) const; // Расстояние до точки
};

inline Vert * BonePtr:: vert0  () { return & bone->b0 == this ? bone->v0 : bone->v1; }
inline Vert * BonePtr:: vert1  () { return & bone->b1 == this ? bone->v0 : bone->v1; }
inline Side * BonePtr::another () { return & bone->b1 == this ? bone->s0 : bone->s1; }

inline const Vert * BonePtr:: vert0  () const { return & bone->b0 == this ? bone->v0 : bone->v1; }
inline const Vert * BonePtr:: vert1  () const { return & bone->b1 == this ? bone->v0 : bone->v1; }
inline const Side * BonePtr::another () const { return & bone->b1 == this ? bone->s0 : bone->s1; }

typedef List<Bone> BoneList;

class BonePtrList : public List<BonePtr>
{
public:
    BonePtrList & jump ( BonePtr * i ) { return (BonePtrList&) ShevList::jump (i); }
    BonePtrList & del  ( BonePtr * m ) { jump ( m ).outCur(); return *this; }
    bool delCur_ () { bool res = curIsLas(); outCur(); return res; }
    void delFir () { outFir (); }
    void delLas () { outLas (); }
    void delCur () { outCur (); }
    void delAll () { outAll (); }
    ~BonePtrList ();
};

class Side : public ListItem<BonePtrList>
{
friend class Body;
friend Body & copy ( const Body & source, class BodyStor & stor, Body & target );
    int flag;
public:
    Plane3d plane;

    Side & jump ( BonePtr * i ) { return (Side&) ShevList::jump (i); }

    Vert * vert0   () { return cur()->vert0  (); }
    Vert * vert1   () { return cur()->vert1  (); }
    Side * another () { return cur()->another(); }

    const Vert * vert0   () const { return cur()->vert0  (); }
    const Vert * vert1   () const { return cur()->vert1  (); }
    const Side * another () const { return cur()->another(); }

    Side & setCurSelf ()
    {
        Bone * b = cur()->bone;
        if ( & b->b0 == cur() ) b->s0 = this;
        else                    b->s1 = this;
        return *this;
    }

    Side & setAllSelf ()
    {
        if ( top() )
        do
        {
            Bone * b = cur()->bone;
            if ( & b->b0 == cur() ) b->s0 = this;
            else                    b->s1 = this;
        }
        while ( next() );
        return *this;
    }

    bool index () const
    {
        const Bone * b = cur()->bone;
        return & b->b1 == cur();
    }

    Side () {}
    Side ( const Plane3d & p ) : plane ( p ) {}
    Def<Vector3d> centerOfMass () const;   // Центр масс
    double area () const;                  // Площадь
    double perimeter () const;             // Периметр
    double getDistance ( const Vector3d & p ) const;  // Расстояние до точки
    void initPlane ();                     // Инициализация плоскости по вершинам

    template <typename T> Side & operator *= ( const T & t )
    {
        plane *= t;
        return *this;
    }
};

typedef List<Side> SideList;

class BodyStor
{
public:
    VertList verts;
    BoneList bones;
    SideList sides;
};

class Body
{
    Body ( Body & );
public:
    VertList verts;
    BoneList bones;
    SideList sides;

    Body () {}
    Body & operator = ( const Body & );
// Свойства многогранника
    double volume() const;                // Объём
    double getArea() const;               // Площадь поверхности
    Def<Vector3d> centerOfMass() const;   // Центр масс
// Построение геометрических фигур
    Body & makeVoid ();                   // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
    Body & makeVoid ( BodyStor & stor );  // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )

    Body & makeParallelepiped ( double x, double y, double z );
    Body & makeCube ( double r )
    {
        return makeParallelepiped ( r, r, r );
    }

    Body & cut ( const Plane3d & plane, BodyStor & stor, int mark = 0 );
    Body & cut ( const Plane3d & plane, int mark = 0 )
    {
        return cut ( plane, BodyStor(), mark );
    }

    template <typename T> Body & operator *= ( const T & t )
    {
        verts *= t;
        sides *= t;
        return *this;
    }

    Body & operator *= ( const Conform3d & conf )
    {
        return *this *= Similar3d ( conf );
    }

    Body & operator *= ( const Spin3d & spin )
    {
        return *this *= Ortho3d ( spin );
    }

    Body & operator += ( const Vector3d & v );
    Body & operator -= ( const Vector3d & v );
};

#include "Body2Func.h"
