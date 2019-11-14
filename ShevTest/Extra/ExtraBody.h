
#pragma once

#include "Vert.h"
//#include "octMath/octFigure3.h"

class ExtraBone;
class ExtraBonePtrList;

class ExtraBonePtr : public ShevItem
{
    ExtraBonePtr ( ExtraBonePtr & );
    void operator = ( ExtraBonePtr & );
public:
    ExtraBone * bone;
    ExtraBonePtr ( ExtraBone * b = 0 ) : bone ( b ) {}

    Vert * vert0 ();
    Vert * vert1 ();
    ExtraBonePtrList * another ();

    const Vert * vert0 () const;
    const Vert * vert1 () const;
    const ExtraBonePtrList * another () const;
};

class ExtraBone : public ShevItem
{
    ExtraBone ( ExtraBone & );
    void operator = ( ExtraBone & );
public:
    Vert * v0, * v1;
    ExtraBonePtrList * c0, * c1;
    ExtraBonePtr b0,  b1;
    ExtraBone () : v0(0), v1(0), c0(0), c1(0)
    {
        b0.bone = b1.bone = this;
    }
    ExtraBone ( Vert * p1, Vert * p2, ExtraBonePtrList * p3, ExtraBonePtrList * p4, int i = -123456789 ) : v0(p1), v1(p2), c0(p3), c1(p4), ShevItem(i)
    {
        b0.bone = b1.bone = this;
    }
    /*double qmod () const
    {
        return ::qmod ( v0->point - v1->point );
    }*/
};

inline Vert * ExtraBonePtr:: vert0  () { return & bone->b0 == this ? bone->v0 : bone->v1; }
inline Vert * ExtraBonePtr:: vert1  () { return & bone->b1 == this ? bone->v0 : bone->v1; }
inline ExtraBonePtrList * ExtraBonePtr::another () { return & bone->b1 == this ? bone->c0 : bone->c1; }

inline const Vert * ExtraBonePtr:: vert0  () const { return & bone->b0 == this ? bone->v0 : bone->v1; }
inline const Vert * ExtraBonePtr:: vert1  () const { return & bone->b1 == this ? bone->v0 : bone->v1; }
inline const ExtraBonePtrList * ExtraBonePtr::another () const { return & bone->b1 == this ? bone->c0 : bone->c1; }

typedef List<ExtraBone> ExtraBoneList;

class ExtraBonePtrList : public List<ExtraBonePtr>
{
public:
    int flag;
    class ExtraSide * side;

    ExtraBonePtrList () : side(0) {}

    ExtraBonePtrList & setSelf ()
    {
        ExtraBone * b = cur()->bone;
        if ( & b->b0 == cur() ) b->c0 = this;
        else                    b->c1 = this;
        return *this;
    }
    ExtraBonePtrList & setAllSelf ();
    ExtraBonePtrList & jump ( ExtraBonePtr * i ) { return (ExtraBonePtrList&) ShevList::jump (i); }
    ExtraBonePtrList & del  ( ExtraBonePtr * m ) { jump ( m ).outCur(); return *this; }
    void delFir () { outFir (); }
    void delLas () { outLas (); }
    void delCur () { outCur (); }
    void delAll () { outAll (); }
    ~ExtraBonePtrList () { outAll(); }
};

class ExtraSide : public ShevItem
{
public:
    Plane3d plane;
    ExtraBonePtrList blist;
    List<ListItem<ExtraBonePtrList> > holes;

/*    ExtraSide & jump ( ExtraBonePtr * i ) { return (ExtraSide&) ShevList::jump (i); }

    Vert * vert0 () { return blist.cur()->vert0(); }
    Vert * vert1 () { return blist.cur()->vert1(); }
    ExtraSide * another () { return blist.cur()->another(); }

    const Vert * vert0 () const { return blist.cur()->vert0(); }
    const Vert * vert1 () const { return blist.cur()->vert1(); }
    const ExtraSide * another () const { return blist.cur()->another(); }
    bool index () const
    {
        const ExtraBone * b = blist.cur()->bone;
        return & b->b1 == blist.cur();
    }*/
    ExtraSide () {}
    ExtraSide ( const Plane3d & p ) : plane ( p ) {}
//    Def<Vector3d> centerOfMass () const;   // Центр масс
    double area () const;                  // Площадь
    double perimeter () const;             // Периметр
    void initPlane ();                     // Инициализация плоскости по вершинам

    template <typename T> ExtraSide & operator *= ( const T & t )
    {
        plane *= t;
        return *this;
    }
};

typedef List<ExtraSide> ExtraSideList;

class ExtraBodyStor
{
public:
    VertList verts;
    ExtraBoneList bones;
    ExtraSideList sides;
    List<ListItem<ExtraBonePtrList> > bplist;
};

class ExtraBody
{
    ExtraBody ( ExtraBody & );
public:
    VertList verts;
    ExtraBoneList bones;
    ExtraSideList sides;

    ExtraBody () {}
    ExtraBody & operator = ( const ExtraBody & );
// Свойства многогранника
    double volume() const;                // Объём
//    Def<Vector3d> centerOfMass() const;   // Центр масс
// Построение геометрических фигур
    ExtraBody & makeVoid ();                      // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )
    ExtraBody & makeVoid ( ExtraBodyStor & stor );  // Пустой многогранник ( 0 вершин, 0 граней, 0 рёбер )

    ExtraBody & makeCuboid ( double x, double y, double z );
    ExtraBody & makeCube ( double r )
    {
        return makeCuboid ( r, r, r );
    }

    ExtraBody & cut ( const Plane3d & plane, ExtraBodyStor & stor, int mark = 0 );
    ExtraBody & cut ( const Plane3d & plane, int mark = 0 )
    {
        return cut ( plane, ExtraBodyStor(), mark );
    }

    template <typename T> ExtraBody & operator *= ( const T & t )
    {
        verts *= t;
        sides *= t;
        return *this;
    }
    /*
    ExtraBody & operator *= ( const Conform3d & conf )
    {
        return *this *= Similar3d ( conf );
    }

    ExtraBody & operator *= ( const Spin3d & spin )
    {
        return *this *= Ortho3d ( spin );
    }
    */
    ExtraBody & operator += ( const Vector3d & v );
    ExtraBody & operator -= ( const Vector3d & v );
};

void swap ( ExtraBody & body1, ExtraBody & body2 );
