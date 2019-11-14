
#pragma once

enum PolyhedronDoctorEnum0
{
    PHD0_FatalError,
};

enum PolyhedronDoctorEnum1
{
    PHD1_ModelHasHoles,             // hole number (unsigned)
    PHD1_FacetNumberLess2,          // facet number (unsigned)
    PHD1_VertexNumberLess3,         // vertex number (unsigned)
    PHD1_FacetHasEqualVertexIndex,  // facet index (unsigned)
    PHD1_FacetHasBadPlane,          // facet index (unsigned)
};

enum PolyhedronDoctorEnum2
{
    PHD2_VertexNumberInFacetLess3,  // facet index (unsigned), vertex number (unsigned)
    PHD2_VertexNumberInFacetMore3,  // facet index (unsigned), vertex number (unsigned)
    PHD2_VertexIndexOutOfRange,     // facet index (unsigned), vertex index (unsigned)
};

class IPolyhedronDoctorReport
{
public:
    virtual void operator() ( PolyhedronDoctorEnum0 tag ) = 0;
    virtual void operator() ( PolyhedronDoctorEnum1 tag, unsigned p1 ) = 0;
    virtual void operator() ( PolyhedronDoctorEnum2 tag, unsigned p1, unsigned p2 ) = 0;
};

class PolyhedronDoctorReportDummy: public IPolyhedronDoctorReport
{
public:
    virtual void operator() ( PolyhedronDoctorEnum0 tag ) {}
    virtual void operator() ( PolyhedronDoctorEnum1 tag, unsigned p1 ) {}
    virtual void operator() ( PolyhedronDoctorEnum2 tag, unsigned p1, unsigned p2 ) {}
};

enum PolyhedronDoctorReturn
{
    PDR_FatalError  = 0,
    PDR_ModelIsGood = 1,
    PDR_ModelChange = 2,
};

PolyhedronDoctorReturn doctorPolyhedron ( Suite<Vector3d> & vertex, CArrRef<Set2<CmbArray<nat, 3>, Plane3d> > facet, 
                             bool isTrian, Suite<Set2<CmbArray<nat, 3>, Plane3d> > & facet2, IPolyhedronDoctorReport & rep );

template <class T>
PolyhedronDoctorReturn doctorPolyhedronShell ( const T & poly1, T & poly2, bool isTrian, IPolyhedronDoctorReport & rep )
{
    try
    {
        // «аписываем данные о вершинах и гран€х в массивы vertex и facet1
        Suite<Vector3d> vertex;
        Suite<Set2<CmbArray<nat, 3>, Plane3d> > facet1, facet2;
        if ( ! fillData ( poly1, vertex, facet1, rep ) ) return PDR_FatalError;
        // «апускаем доктор и, если не было исправлений, выходим
        PolyhedronDoctorReturn res = doctorPolyhedron ( vertex, facet1, isTrian, facet2, rep );
        //  опируем данные во второй многогранник
        if ( res == PDR_ModelChange ) copyData ( vertex, facet2, poly2 );
        return res;
    }
    catch(...)
    {
        rep ( PHD0_FatalError );
    }
    return PDR_FatalError;
}

#include "../test/Display.h"

class PolyhedronDoctorReportDisplay: public IPolyhedronDoctorReport
{
public:
    virtual void operator() ( PolyhedronDoctorEnum0 tag )
    {
        switch ( tag )
        {
        case PHD0_FatalError: display << "Doctor: Fatal Error" << NL; break;
        }
    }
    virtual void operator() ( PolyhedronDoctorEnum1 tag, unsigned p1 )
    {
        switch ( tag )
        {
        case PHD1_ModelHasHoles: display << "Doctor: Model has" << p1 << "holes" << NL; break;
        case PHD1_FacetNumberLess2: display << "Doctor: Facet number =" << p1 << NL; break;
        case PHD1_VertexNumberLess3: display << "Doctor: Vertex number =" << p1 << NL; break;
        case PHD1_FacetHasEqualVertexIndex: display << "Doctor: Facet Has Equal Vertex Index" << p1 << NL; break;
        case PHD1_FacetHasBadPlane: display << "Doctor: Facet Has Bad Plane" << p1 << NL; break;
        }
    }
    virtual void operator() ( PolyhedronDoctorEnum2 tag, unsigned p1, unsigned p2 )
    {
        switch ( tag )
        {
        case PHD2_VertexNumberInFacetLess3: display << "Doctor: Vertex Number In Facet" << p1 << "=" << p2 << NL; break;
        case PHD2_VertexNumberInFacetMore3: display << "Doctor: Vertex Number In Facet" << p1 << "=" << p2 << NL; break;
        case PHD2_VertexIndexOutOfRange:  display << "Doctor: Vertex Index Out Of Range" << p1 << "=" << p2 << NL; break;
        }
    }
};

