
#include "ValueSet.h"

ValueSet::ValueSet ( nat ms ) : nn(16), max_size(ms)
{
    data = new Node[nn];
    for ( nat i = 0; i < nn; ++i ) data[i].type = void_type;
}

ValueSet & ValueSet::operator = ( const ValueSet & set )
{
    if ( this == &set ) return *this;
    max_size = set.max_size;
    if ( nn != set.nn )
    {
        nn = set.nn;
        delete[] data;
        data = new Node[nn];
    }
    for ( nat i = 0; i < nn; ++i ) data[i] = set.data[i];
    return *this;
}
    
ValueSet::Type ValueSet::type ( nat index ) const
{
    if ( index >= max_size ) return out_of_range;
    return index >= nn ? void_type : data[index].type;
}

void ValueSet::resize ( nat index )
{
    nat n = nn;
    while ( index >= n ) n <<= 1;
    Node * node = new Node[n];
    nat i;
    for ( i = 0; i < nn; ++i ) node[i] = data[i];
    for ( i = nn; i < n; ++i ) node[i].type = void_type;
    delete[] data;
    data = node;
    nn = n;
}

void ValueSet::clear ()
{
    for ( nat i = 0; i < nn; ++i ) data[i].type = void_type;
}

bool ValueSet::putVoid ( nat index )
{
    if ( index >= max_size ) return false;
    if ( index < nn ) data[index].type = void_type;
    return true;
}

bool ValueSet::putVoidPtr ( nat index, void * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.vp = value;
    data[index].type = void_ptr_type;
    return true;
}

bool ValueSet::putCVoidPtr ( nat index, const void * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cvp = value;
    data[index].type = cvoid_ptr_type;
    return true;
}

bool ValueSet::putBool ( nat index, bool value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.b = value;
    data[index].type = bool_type;
    return true;
}

bool ValueSet::putBoolPtr ( nat index, bool * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.bp = value;
    data[index].type = bool_ptr_type;
    return true;
}

bool ValueSet::putCBoolPtr ( nat index, const bool * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cbp = value;
    data[index].type = cbool_ptr_type;
    return true;
}

bool ValueSet::putChar ( nat index, char value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.c = value;
    data[index].type = char_type;
    return true;
}

bool ValueSet::putCharPtr ( nat index, char * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cp = value;
    data[index].type = char_ptr_type;
    return true;
}

bool ValueSet::putCCharPtr ( nat index, const char * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.ccp = value;
    data[index].type = cchar_ptr_type;
    return true;
}

bool ValueSet::putInt ( nat index, int value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.i = value;
    data[index].type = int_type;
    return true;
}

bool ValueSet::putIntPtr ( nat index, int * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.ip = value;
    data[index].type = int_ptr_type;
    return true;
}

bool ValueSet::putCIntPtr ( nat index, const int * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cip = value;
    data[index].type = cint_ptr_type;
    return true;
}

bool ValueSet::putNat ( nat index, nat value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.n = value;
    data[index].type = nat_type;
    return true;
}

bool ValueSet::putNatPtr ( nat index, nat * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.np = value;
    data[index].type = nat_ptr_type;
    return true;
}

bool ValueSet::putCNatPtr ( nat index, const nat * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cnp = value;
    data[index].type = cnat_ptr_type;
    return true;
}

bool ValueSet::putDbl ( nat index, double value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.d = value;
    data[index].type = dbl_type;
    return true;
}

bool ValueSet::putDblPtr ( nat index, double * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.dp = value;
    data[index].type = dbl_ptr_type;
    return true;
}

bool ValueSet::putCDblPtr ( nat index, const double * value )
{
    if ( index >= max_size ) return false;
    if ( index >= nn ) resize ( index );
    data[index].value.cdp = value;
    data[index].type = cdbl_ptr_type;
    return true;
}

bool ValueSet::getVoidPtr ( nat index, void * & value ) const
{
    if ( index >= nn || data[index].type != void_ptr_type || index >= max_size ) return false;
    value = data[index].value.vp;
    return true;
}

bool ValueSet::getCVoidPtr ( nat index, const void * & value ) const
{
    if ( index >= nn || index >= max_size ) return false;
    const Node & node = data[index];
    if ( node.type != cvoid_ptr_type && node.type != void_ptr_type ) return false;
    value = node.value.cvp;
    return true;
}

bool ValueSet::getBool ( nat index, bool & value ) const
{
    if ( index >= nn || data[index].type != bool_type || index >= max_size ) return false;
    value = data[index].value.b;
    return true;
}

bool ValueSet::getBoolPtr ( nat index, bool * & value ) const
{
    if ( index >= nn || data[index].type != bool_ptr_type || index >= max_size ) return false;
    value = data[index].value.bp;
    return true;
}

bool ValueSet::getCBoolPtr ( nat index, const bool * & value ) const
{
    if ( index >= nn || index >= max_size ) return false;
    const Node & node = data[index];
    if ( node.type != cbool_ptr_type && node.type != bool_ptr_type ) return false;
    value = data[index].value.cbp;
    return true;
}

bool ValueSet::getInt ( nat index, int & value ) const
{
    if ( index >= nn || data[index].type != int_type || index >= max_size ) return false;
    value = data[index].value.i;
    return true;
}

bool ValueSet::getIntPtr ( nat index, int * & value ) const
{
    if ( index >= nn || data[index].type != int_ptr_type || index >= max_size ) return false;
    value = data[index].value.ip;
    return true;
}

bool ValueSet::getCIntPtr ( nat index, const int * & value ) const
{
    if ( index >= nn || index >= max_size ) return false;
    const Node & node = data[index];
    if ( node.type != cint_ptr_type && node.type != int_ptr_type ) return false;
    value = node.value.cip;
    return true;
}

bool ValueSet::getNat ( nat index, nat & value ) const
{
    if ( index >= nn || data[index].type != nat_type || index >= max_size ) return false;
    value = data[index].value.n;
    return true;
}

bool ValueSet::getNatPtr ( nat index, nat * & value ) const
{
    if ( index >= nn || data[index].type != nat_ptr_type || index >= max_size ) return false;
    value = data[index].value.np;
    return true;
}

bool ValueSet::getCNatPtr ( nat index, const nat * & value ) const
{
    if ( index >= nn || index >= max_size ) return false;
    const Node & node = data[index];
    if ( node.type != cnat_ptr_type && node.type != nat_ptr_type ) return false;
    value = node.value.cnp;
    return true;
}

bool ValueSet::getDbl ( nat index, double & value ) const
{
    if ( index >= nn || data[index].type != dbl_type || index >= max_size ) return false;
    value = data[index].value.d;
    return true;
}

bool ValueSet::getDblPtr ( nat index, double * & value ) const
{
    if ( index >= nn || data[index].type != dbl_ptr_type || index >= max_size ) return false;
    value = data[index].value.dp;
    return true;
}

bool ValueSet::getCDblPtr ( nat index, const double * & value ) const
{
    if ( index >= nn || index >= max_size ) return false;
    const Node & node = data[index];
    if ( node.type != cdbl_ptr_type && node.type != dbl_ptr_type ) return false;
    value = node.value.cdp;
    return true;
}