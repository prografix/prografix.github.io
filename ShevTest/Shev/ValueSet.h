
#ifndef VALUE_SET_H
#define VALUE_SET_H

#include "typedef.h"
#include "template.h"

#pragma pack (push, 4)

class ValueSet
{
public:
    enum Type
    {
        out_of_range, 
        void_type, void_ptr_type, cvoid_ptr_type,
        bool_type, bool_ptr_type, cbool_ptr_type,
        char_type, char_ptr_type, cchar_ptr_type,
        int_type, int_ptr_type, cint_ptr_type,
        nat_type, nat_ptr_type, cnat_ptr_type,
        dbl_type, dbl_ptr_type, cdbl_ptr_type,
    };
private:
    union Value
    {
              void   * vp;
        const void   * cvp;
              bool     b;
              bool   * bp;
        const bool   * cbp;
              char     c;
              char   * cp;
        const char   * ccp;
              int      i;
              int    * ip;
        const int    * cip;
              nat      n;
              nat    * np;
        const nat    * cnp;
              double   d;
              double * dp;
        const double * cdp;
    };
    class Node
    {
    public:
        Value value;
        Type  type;
    };
// Data
    Node * data;
    nat nn, max_size;
// Functions
    void resize ( nat index );
    ValueSet ( const ValueSet & );
public:
    ValueSet ( nat ms = 1024 );
    ValueSet & operator = ( const ValueSet & );
    ValueSet & swap ( ValueSet & vs )
    {
        _swap ( max_size, vs.max_size );
        _swap ( data, vs.data );
        _swap ( nn, vs.nn );
        return *this;
    }
   ~ValueSet () { delete[] data; }

    nat size () const { return max_size; }
    Type type ( nat index ) const;
    void clear ();

    bool putVoid    ( nat index );
    bool putVoidPtr ( nat index, void * value );
    bool putCVoidPtr( nat index, const void * value );

    bool putBool    ( nat index, bool value );
    bool putBoolPtr ( nat index, bool * value );
    bool putCBoolPtr( nat index, const bool * value );

    bool putChar    ( nat index, char value );
    bool putCharPtr ( nat index, char * value );
    bool putCCharPtr( nat index, const char * value );

    bool putInt    ( nat index, int value );
    bool putIntPtr ( nat index, int * value );
    bool putCIntPtr( nat index, const int * value );

    bool putNat    ( nat index, nat value );
    bool putNatPtr ( nat index, nat * value );
    bool putCNatPtr( nat index, const nat * value );

    bool putDbl    ( nat index, double value );
    bool putDblPtr ( nat index, double * value );
    bool putCDblPtr( nat index, const double * value );
    
    bool getVoidPtr ( nat index, void * & value ) const;
    bool getCVoidPtr( nat index, const void * & value ) const;

    bool getBool    ( nat index, bool & value ) const;
    bool getBoolPtr ( nat index, bool * & value ) const;
    bool getCBoolPtr( nat index, const bool * & value ) const;

    bool getChar    ( nat index, char & value ) const;
    bool getCharPtr ( nat index, char * & value ) const;
    bool getCCharPtr( nat index, const char * & value ) const;

    bool getInt    ( nat index, int & value ) const;
    bool getIntPtr ( nat index, int * & value ) const;
    bool getCIntPtr( nat index, const int * & value ) const;

    bool getNat    ( nat index, nat & value ) const;
    bool getNatPtr ( nat index, nat * & value ) const;
    bool getCNatPtr( nat index, const nat * & value ) const;

    bool getDbl    ( nat index, double & value ) const;
    bool getDblPtr ( nat index, double * & value ) const;
    bool getCDblPtr( nat index, const double * & value ) const;
};

inline bool getValue ( const ValueSet & ui, int & v, int tag )
{
    return ui.getInt ( tag, v );
}

inline bool getValue ( const ValueSet & ui, double & v, int tag )
{
    return ui.getDbl ( tag, v );
}

inline bool setValue ( ValueSet & ui, int v, int tag )
{
    return ui.putInt ( tag, v );
}

inline bool setValue ( ValueSet & ui, double v, int tag )
{
    return ui.putDbl ( tag, v );
}

#pragma pack (pop)

#endif