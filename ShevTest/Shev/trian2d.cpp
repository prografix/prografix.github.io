
#include "math.h"

#include "Rand.h"
#include "func1t.h"
#include "atrian.h"
#include "trian2t.h"
#include "trian2d.h"
#include "Vector2d.h"
#include "ShevList.h"
#include "ShevArray.h"
#include "lists.h"

//**************************** 18.07.2007 *********************************//
//
//          Триангуляция многоугольников ( локальная оптимизация )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianNat1L1MaxMinArea ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_Area<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxMinAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxSumAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxMinTan ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

SuiteRef<Set3<nat> > & trianNat1L1MaxSumMinAngle ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_MinAngle<double, Vector2d> quality ( vert );
    return tempTrianNat1L1<double, Vector2d, double> ( quality, merge, vert, res );
}

//**************************** 18.07.2007 *********************************//
//
//          Триангуляция многоугольников ( глобальная оптимизация )
//
//**************************** 27.08.2011 *********************************//

SuiteRef<Set3<nat> > & trianG1MaxMinArea ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_Area<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxMinAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxSumAdivR2 ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxMinTan ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

SuiteRef<Set3<nat> > & trianG1MaxSumMinAngle ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    const PosSum2a<double> merge;
    const TQ_MinAngle<double, Vector2d> quality ( vert );
    return maxG1<double> ( quality, merge, vert.size(), res );
}

//**************************** 31.12.2014 *********************************//
//
//            Триангуляция многоугольников ( алгоритм Зейделя )
//
//**************************** 31.12.2014 *********************************//

/* Segment attributes */

typedef struct {	
  Vector2d v0, v1;		/* two endpoints */
  bool is_inserted;		/* inserted in trapezoidation yet ? */
  int root0, root1;		/* root nodes in Q */
  int next;			/* Next logical segment */
  int prev;			/* Previous segment */
} segment_t;


/* Trapezoid attributes */

typedef struct {
  int lseg, rseg;		/* two adjoining segments */
  Vector2d hi, lo;		/* max/min y-values */
  int u0, u1;
  int d0, d1;
  int sink;			/* pointer to corresponding in Q */
  int usave, uside;		/* I forgot what this means */
  int state;
  bool visited;
} trap_t;


/* Node attributes for every node in the query structure */

typedef struct {
  int nodetype;			/* Y-node or S-node */
  int segnum;
  Vector2d yval;
  int trnum;
  int parent;			/* doubly linked DAG */
  int left, right;		/* children */
} node_t;


typedef struct {
  nat vnum;
  nat next;			/* Circularly linked list  */
  nat prev;			/* describing the monotone */
  bool marked;			/* polygon */
} monchain_t;			


typedef struct {
  Vector2d pt;
  int vnext[4];			/* next vertices for the 4 chains */
  int vpos[4];			/* position of v in the 4 chains */
  int nextfree;
} vertexchain_t;


/* Node types */

#define T_X     1
#define T_Y     2
#define T_SINK  3


#define FIRSTPT 1		/* checking whether pt. is inserted */ 
#define LASTPT  2


#define S_LEFT 1		/* for merge-direction */
#define S_RIGHT 2


#define ST_VALID 1		/* for trapezium state */
#define ST_INVALID 2	


#define SP_SIMPLE_LRUP 1	/* for splitting trapezoids */
#define SP_SIMPLE_LRDN 2
#define SP_2UP_2DN     3
#define SP_2UP_LEFT    4
#define SP_2UP_RIGHT   5
#define SP_2DN_LEFT    6
#define SP_2DN_RIGHT   7
#define SP_NOSPLIT    -1

#define TR_FROM_UP 1		/* for traverse-direction */
#define TR_FROM_DN 2

#define TRI_LHS 1
#define TRI_RHS 2

inline bool cross ( const Vector2d & v0, const Vector2d & v1, const Vector2d & v2 )
{
    return ( v1 - v0 ) % ( v2 - v0 ) > 0;
}

/* Get log*n for given n */
inline nat math_logstar_n ( nat n )
{
    nat i;
    double v = n;
    for ( i = 0; v >= 1; ++i ) v = log(v) / log(2.);
    return i - 1;
}

inline nat math_N ( nat n, nat h )
{
    double v = n;
    for ( nat i = 0; i < h; ++i ) v = log(v) / log(2.);
    return (nat) ceil ( n / v );
}

struct TrapezoidData
{
    Suite<node_t> qs;		/* Query structure */
    Suite<trap_t> tr;		/* Trapezoid structure */
    DynArray<segment_t> seg;		/* Segment table */
    // Return a new node to be added into the query tree
    nat newnode()
    {
        const nat i = qs.size();
        qs.inc(); 
        return i;
    }
    // Return a free trapezoid
    int newtrap()
    {
        const nat i = tr.size();
        trap_t & t = tr.inc();
        t.d0 = t.d1 = t.u0 = t.u1 = t.sink = t.usave = t.uside = 0; 	
        t.visited = false;
        t.lseg = -1;
        t.rseg = -1;
        t.state = ST_VALID;
        return i;
    }
    void init_query_structure ( int segnum );
    void merge_trapezoids ( int segnum, int tfirst, int tlast, int side );
    void add_segment ( int segnum );
    nat locate_endpoint ( const Vector2d & v, const Vector2d & vo, nat r );

    Suite<monchain_t> mchain; /* Table to hold all the monotone */
				  /* polygons . Each monotone polygon */
				  /* is a circularly linked list */

    DynArray<vertexchain_t> vert; /* chain init. information. This */
				    /* is used to decide which */
				    /* monotone polygon to split if */
				    /* there are several other */
				    /* polygons touching at the same */
				    /* vertex  */
    DynArray<nat> mon;	/* contains position of any vertex in */
				/* the monotone chain for the polygon */
    int mon_idx;
    /* return a new chain element from the table */
    int new_chain_element()
    {
        const nat i = mchain.size();
        mchain.inc().marked = false;
        return i;
    }
    int make_new_monotone_poly(int mcur, int v0, int v1);
    void traverse_polygon ( int mcur, int trnum, int from, int dir );
    void monotonate_trapezoids ();
    void triangulate_polygon ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vertices );
};

/* Return the maximum of the two points */
inline const Vector2d & max ( const Vector2d & v0, const Vector2d & v1)
{
    if ( v0.y > v1.y )
        return v0;
    if ( v0.y < v1.y )
        return v1;
    return v0.x > v1.x ? v0 : v1;
}


/* Return the minimum of the two points */
inline const Vector2d & min ( const Vector2d & v0, const Vector2d & v1 )
{
    if ( v0.y < v1.y )
        return v0;
    if ( v0.y > v1.y )
        return v1;
    return v0.x < v1.x ? v0 : v1;
}

inline bool _greater_than ( const Vector2d & v0, const Vector2d & v1 )
{
    if ( v0.y > v1.y )
        return true;
    else 
    if ( v0.y < v1.y )
        return false;
    else
        return v0.x > v1.x;
}

inline bool _less_than ( const Vector2d & v0, const Vector2d & v1 )
{
    if ( v0.y < v1.y )
        return true;
    else if ( v0.y > v1.y )
        return false;
    else
        return v0.x < v1.x;
}


/* Initilialise the query structure (Q) and the trapezoid table (T) 
 * when the first segment is added to start the trapezoidation. The
 * query-tree starts out with 4 trapezoids, one S-node and 2 Y-nodes
 */

void TrapezoidData::init_query_structure ( int segnum )
{
    segment_t & s = seg[segnum];

    const int i1 = newnode();
    qs[i1].nodetype = T_Y;
    qs[i1].yval = max ( s.v0, s.v1 ); /* root */

    const int i2 = qs[i1].right = newnode();
    qs[i2].nodetype = T_SINK;
    qs[i2].parent = i1;

    const int i3 = qs[i1].left = newnode();
    qs[i3].nodetype = T_Y;
    qs[i3].yval = min ( s.v0, s.v1 ); /* root */
    qs[i3].parent = i1;
  
    const int i4 = qs[i3].left = newnode();
    qs[i4].nodetype = T_SINK;
    qs[i4].parent = i3;
  
    const int i5 = qs[i3].right = newnode();
    qs[i5].nodetype = T_X;
    qs[i5].segnum = segnum;
    qs[i5].parent = i3;
  
    const int i6 = qs[i5].left = newnode();
    qs[i6].nodetype = T_SINK;
    qs[i6].parent = i5;

    const int i7 = qs[i5].right = newnode();
    qs[i7].nodetype = T_SINK;
    qs[i7].parent = i5;

/*    
 *                4
 *   -----------------------------------
 *  		  \
 *  	1	   \        2
 *  		    \
 *   -----------------------------------
 *                3
 */
  
    newtrap(); //
    const int t1 = newtrap();		/* middle left */
    const int t2 = newtrap();		/* middle right */
    const int t3 = newtrap();		/* bottom-most */
    const int t4 = newtrap();		/* topmost */

    tr[t1].hi = tr[t2].hi = tr[t4].lo = qs[i1].yval;
    tr[t1].lo = tr[t2].lo = tr[t3].hi = qs[i3].yval;
    tr[t4].hi = Vector2d ( +1e300, +1e300 );
    tr[t3].lo = Vector2d ( -1e300, -1e300 );
    tr[t1].rseg = tr[t2].lseg = segnum;
    tr[t1].u0 = tr[t2].u0 = t4;
    tr[t1].d0 = tr[t2].d0 = t3;
    tr[t4].d0 = tr[t3].u0 = t1;
    tr[t4].d1 = tr[t3].u1 = t2;
  
    tr[t1].sink = i6;
    tr[t2].sink = i7;
    tr[t3].sink = i4;
    tr[t4].sink = i2;

    qs[i2].trnum = t4;
    qs[i4].trnum = t3;
    qs[i6].trnum = t1;
    qs[i7].trnum = t2;

    s.is_inserted = true;
}


/* Retun TRUE if the vertex v is to the left of line segment no.
 * segnum. Takes care of the degenerate cases when both the vertices
 * have the same y--cood, etc.
 */

bool is_left_of ( const segment_t & s, const Vector2d & v )
{
    if ( _greater_than ( s.v1, s.v0 ) ) /* seg. going upwards */
    {
        if ( s.v1.y == v.y ) return v.x < s.v1.x;
        if ( s.v0.y == v.y ) return v.x < s.v0.x;
	    return cross ( s.v0, s.v1, v );
    }
    if ( s.v1.y == v.y ) return v.x < s.v1.x;
    if ( s.v0.y == v.y ) return v.x < s.v0.x;
    return cross ( s.v1, s.v0, v );
}

/* This is query routine which determines which trapezoid does the 
 * point v lie in. The return value is the trapezoid number. 
 */

nat TrapezoidData::locate_endpoint ( const Vector2d & v, const Vector2d & vo, nat r )
{
    const node_t & rptr = qs[r];
  
    switch (rptr.nodetype)
    {
    case T_SINK:
      return rptr.trnum;
      
    case T_Y:
      if ( _greater_than ( v, rptr.yval ) ) /* above */
	return locate_endpoint(v, vo, rptr.right);
      else if ( v == rptr.yval ) /* the point is already */
	{			          /* inserted. */
	  if ( _greater_than ( vo, rptr.yval ) ) /* above */
	    return locate_endpoint(v, vo, rptr.right);
	  else 
	    return locate_endpoint(v, vo, rptr.left); /* below */	    
	}
      else
	return locate_endpoint(v, vo, rptr.left); /* below */

    case T_X:
      if ( v == seg[rptr.segnum].v0 || v == seg[rptr.segnum].v1 )
	{
	  if ( v.y == vo.y ) /* horizontal segment */
	    {
	      if (vo.x < v.x)
		return locate_endpoint(v, vo, rptr.left); /* left */
	      else
		return locate_endpoint(v, vo, rptr.right); /* right */
	    }

	  else if (is_left_of(seg[rptr.segnum], vo))
	    return locate_endpoint(v, vo, rptr.left); /* left */
	  else
	    return locate_endpoint(v, vo, rptr.right); /* right */
	}
      else if (is_left_of(seg[rptr.segnum], v))
	return locate_endpoint(v, vo, rptr.left); /* left */
      else
	return locate_endpoint(v, vo, rptr.right); /* right */	

    default:
//      fprintf(stderr, "Haggu !!!!!\n");
      break;
    }
    return 0;
}


/* Thread in the segment into the existing trapezoidation. The 
 * limiting trapezoids are given by tfirst and tlast (which are the
 * trapezoids containing the two endpoints of the segment. Merges all
 * possible trapezoids which flank this segment and have been recently
 * divided because of its insertion
 */

void TrapezoidData::merge_trapezoids ( int segnum, int tfirst, int tlast, int side )
{
    int t, tnext, cond;
    int ptnext;

    /* First merge polys on the LHS */
    t = tfirst;
    while ( t > 0 && ! _less_than ( tr[t].lo, tr[tlast].lo ) )
    {
        if (side == S_LEFT)
	        cond = ((((tnext = tr[t].d0) > 0) && (tr[tnext].rseg == segnum)) ||
		        (((tnext = tr[t].d1) > 0) && (tr[tnext].rseg == segnum)));
        else
	        cond = ((((tnext = tr[t].d0) > 0) && (tr[tnext].lseg == segnum)) ||
		        (((tnext = tr[t].d1) > 0) && (tr[tnext].lseg == segnum)));
      
        if ( cond )
	    {
	      if ((tr[t].lseg == tr[tnext].lseg) &&
	          (tr[t].rseg == tr[tnext].rseg)) /* good neighbours */
	        {			              /* merge them */
	          /* Use the upper node as the new node i.e. t */
	          
	          ptnext = qs[tr[tnext].sink].parent;
	          
	          if (qs[ptnext].left == tr[tnext].sink)
		    qs[ptnext].left = tr[t].sink;
	          else
		    qs[ptnext].right = tr[t].sink;	/* redirect parent */
	          
	          
	          /* Change the upper neighbours of the lower trapezoids */
	          
	          if ((tr[t].d0 = tr[tnext].d0) > 0)
		    if (tr[tr[t].d0].u0 == tnext)
		      tr[tr[t].d0].u0 = t;
		    else if (tr[tr[t].d0].u1 == tnext)
		      tr[tr[t].d0].u1 = t;
	          
	          if ((tr[t].d1 = tr[tnext].d1) > 0)
		    if (tr[tr[t].d1].u0 == tnext)
		      tr[tr[t].d1].u0 = t;
		    else if (tr[tr[t].d1].u1 == tnext)
		      tr[tr[t].d1].u1 = t;
	          
	          tr[t].lo = tr[tnext].lo;
	          tr[tnext].state = ST_INVALID; /* invalidate the lower */
				                /* trapezium */
	        }
	      else		    /* not good neighbours */
	        t = tnext;
	    }
          else		    /* do not satisfy the outer if */
	    t = tnext;
      
    } /* end-while */
}


/* Add in the new segment into the trapezoidation and update Q and T
 * structures. First locate the two endpoints of the segment in the
 * Q-structure. Then start from the topmost trapezoid and go down to
 * the  lower trapezoid dividing all the trapezoids in between .
 */

void TrapezoidData::add_segment ( int segnum )
{
    int tu, tl, sk, tfirst, tlast;
    int tfirstr, tlastr, tfirstl, tlastl;
    int i1, i2, t, tn, tmptriseg;
    bool tribot = false;
    bool is_swapped = false;

    segment_t s = seg[segnum];
    if ( _greater_than ( s.v1, s.v0 ) ) /* Get higher vertex in v0 */
    {
        _swap ( s.v0, s.v1 );
        _swap ( s.root0, s.root1 );
        is_swapped = true;
    }

    if ( ! seg[is_swapped ? seg[segnum].next : seg[segnum].prev].is_inserted ) /* insert v0 in the tree */
    {
      int tmp_d;

      tu = locate_endpoint(s.v0, s.v1, s.root0);
      tl = newtrap();		/* tl is the new lower trapezoid */
      tr[tl] = tr[tu];
      tr[tu].lo.y = tr[tl].hi.y = s.v0.y;
      tr[tu].lo.x = tr[tl].hi.x = s.v0.x;
      tr[tu].d0 = tl;      
      tr[tu].d1 = 0;
      tr[tl].u0 = tu;
      tr[tl].u1 = 0;

      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;

      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;

      /* Now update the query structure and obtain the sinks for the */
      /* two trapezoids */ 
      
      i1 = newnode();		/* Upper trapezoid sink */
      i2 = newnode();		/* Lower trapezoid sink */
      sk = tr[tu].sink;
      
      qs[sk].nodetype = T_Y;
      qs[sk].yval = s.v0;
      qs[sk].segnum = segnum;	/* not really reqd ... maybe later */
      qs[sk].left = i2;
      qs[sk].right = i1;

      qs[i1].nodetype = T_SINK;
      qs[i1].trnum = tu;
      qs[i1].parent = sk;

      qs[i2].nodetype = T_SINK;
      qs[i2].trnum = tl;
      qs[i2].parent = sk;

      tr[tu].sink = i1;
      tr[tl].sink = i2;
      tfirst = tl;
    }
    else				/* v0 already present */
    {       /* Get the topmost intersecting trapezoid */
        tfirst = locate_endpoint(s.v0, s.v1, s.root0);
    }

    if ( ! seg[is_swapped ? seg[segnum].prev : seg[segnum].next].is_inserted ) /* insert v1 in the tree */
    {
      int tmp_d;

      tu = locate_endpoint(s.v1, s.v0, s.root1);

      tl = newtrap();		/* tl is the new lower trapezoid */
      tr[tl] = tr[tu];
      tr[tu].lo.y = tr[tl].hi.y = s.v1.y;
      tr[tu].lo.x = tr[tl].hi.x = s.v1.x;
      tr[tu].d0 = tl;      
      tr[tu].d1 = 0;
      tr[tl].u0 = tu;
      tr[tl].u1 = 0;

      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d0) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;

      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u0 == tu))
	tr[tmp_d].u0 = tl;
      if (((tmp_d = tr[tl].d1) > 0) && (tr[tmp_d].u1 == tu))
	tr[tmp_d].u1 = tl;
      
      /* Now update the query structure and obtain the sinks for the */
      /* two trapezoids */ 
      
      i1 = newnode();		/* Upper trapezoid sink */
      i2 = newnode();		/* Lower trapezoid sink */
      sk = tr[tu].sink;
      
      qs[sk].nodetype = T_Y;
      qs[sk].yval = s.v1;
      qs[sk].segnum = segnum;	/* not really reqd ... maybe later */
      qs[sk].left = i2;
      qs[sk].right = i1;

      qs[i1].nodetype = T_SINK;
      qs[i1].trnum = tu;
      qs[i1].parent = sk;

      qs[i2].nodetype = T_SINK;
      qs[i2].trnum = tl;
      qs[i2].parent = sk;

      tr[tu].sink = i1;
      tr[tl].sink = i2;
      tlast = tu;
    }
    else				/* v1 already present */
    {       /* Get the lowermost intersecting trapezoid */
        tlast = locate_endpoint(s.v1, s.v0, s.root1);
        tribot = true;
    }
  
  /* Thread the segment into the query tree creating a new X-node */
  /* First, split all the trapezoids which are intersected by s into */
  /* two */

    t = tfirst;			/* topmost trapezoid */
  
    while ( t > 0 && ! _less_than ( tr[t].lo, tr[tlast].lo ) ) /* traverse from top to bot */
    {
        int t_sav, tn_sav;
        sk = tr[t].sink;
        i1 = newnode();		/* left trapezoid sink */
        i2 = newnode();		/* right trapezoid sink */
      
        qs[sk].nodetype = T_X;
        qs[sk].segnum = segnum;
        qs[sk].left = i1;
        qs[sk].right = i2;

        qs[i1].nodetype = T_SINK;	/* left trapezoid (use existing one) */
        qs[i1].trnum = t;
        qs[i1].parent = sk;

        qs[i2].nodetype = T_SINK;	/* right trapezoid (allocate new) */
        qs[i2].trnum = tn = newtrap();
        qs[i2].parent = sk;

        if ( t == tfirst ) tfirstr = tn;
        if ( tr[t].lo == tr[tlast].lo ) tlastr = tn;

        tr[tn] = tr[t];
        tr[t].sink = i1;
        tr[tn].sink = i2;
        t_sav = t;
        tn_sav = tn;

        if ((tr[t].d0 <= 0) && (tr[t].d1 <= 0)) /* case cannot arise */
	    {
            //fprintf(stderr, "add_segment: error\n");
	        break;
	    }
      
      /* only one trapezoid below. partition t into two and make the */
      /* two resulting trapezoids t and tn as the upper neighbours of */
      /* the sole lower trapezoid */
      
        if ( tr[t].d0 > 0 && tr[t].d1 <= 0 )
	    {			/* Only one trapezoid below */
	        if ( tr[t].u0 > 0 && tr[t].u1 > 0 )
	        {			/* continuation of a chain from abv. */
	            if ( tr[t].usave > 0 ) /* three upper neighbours */
		        {
		            if ( tr[t].uside == S_LEFT )
		            {
		                tr[tn].u0 = tr[t].u1;
		                tr[t].u1 = -1;
		                tr[tn].u1 = tr[t].usave;
		              
		                tr[tr[t].u0].d0 = t;
		                tr[tr[tn].u0].d0 = tn;
		                tr[tr[tn].u1].d0 = tn;
		            }
		            else		/* intersects in the right */
		            {
		                tr[tn].u1 = -1;
		                tr[tn].u0 = tr[t].u1;
		                tr[t].u1 = tr[t].u0;
		                tr[t].u0 = tr[t].usave;

		                tr[tr[t].u0].d0 = t;
		                tr[tr[t].u1].d0 = t;
		                tr[tr[tn].u0].d0 = tn;		      
		            }
		  
		            tr[t].usave = tr[tn].usave = 0;
		        }
	            else		/* No usave.... simple case */
		        {
		            tr[tn].u0 = tr[t].u1;
		            tr[t].u1 = tr[tn].u1 = -1;
		            tr[tr[tn].u0].d0 = tn;
		        }
	        }
	        else 
	        {			/* fresh seg. or upward cusp */
	            int tmp_u = tr[t].u0;
	            int td0, td1;
	            if (((td0 = tr[tmp_u].d0) > 0) && ((td1 = tr[tmp_u].d1) > 0))
		        {		/* upward cusp */
		            if ((tr[td0].rseg > 0) && !is_left_of(seg[tr[td0].rseg], s.v1))
		            {
		                tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		                tr[tr[tn].u0].d1 = tn;
		            }
		            else		/* cusp going leftwards */
		            { 
		                tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		                tr[tr[t].u0].d0 = t;
		            }
		        }
	            else		/* fresh segment */
		        {
		            tr[tr[t].u0].d0 = t;
		            tr[tr[t].u0].d1 = tn;
		        }   
	        }

	        if ( tribot && tr[t].lo == tr[tlast].lo )
	        {		/* bottom forms a triangle */

	            if (is_swapped)	
		            tmptriseg = seg[segnum].prev;
	            else
		            tmptriseg = seg[segnum].next;
	      
	            if ((tmptriseg > 0) && is_left_of(seg[tmptriseg], s.v0))
		        {
				    /* L-R downward cusp */
		            tr[tr[t].d0].u0 = t;
		            tr[tn].d0 = tr[tn].d1 = -1;
		        }
	            else
		        {
				    /* R-L downward cusp */
		            tr[tr[tn].d0].u1 = tn;
		            tr[t].d0 = tr[t].d1 = -1;
		        }
	        }
	        else
	        {
	            if ((tr[tr[t].d0].u0 > 0) && (tr[tr[t].d0].u1 > 0))
		        {
		            if (tr[tr[t].d0].u0 == t) /* passes thru LHS */
		            {
		                tr[tr[t].d0].usave = tr[tr[t].d0].u1;
		                tr[tr[t].d0].uside = S_LEFT;
		            }
		            else
		            {
		                tr[tr[t].d0].usave = tr[tr[t].d0].u0;
		                tr[tr[t].d0].uside = S_RIGHT;
		            }		    
		        }
	            tr[tr[t].d0].u0 = t;
	            tr[tr[t].d0].u1 = tn;
	        }
	  
	        t = tr[t].d0;
	    }


      else if ((tr[t].d0 <= 0) && (tr[t].d1 > 0))
	{			/* Only one trapezoid below */
	  if ((tr[t].u0 > 0) && (tr[t].u1 > 0))
	    {			/* continuation of a chain from abv. */
	      if (tr[t].usave > 0) /* three upper neighbours */
		{
		  if (tr[t].uside == S_LEFT)
		    {
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = -1;
		      tr[tn].u1 = tr[t].usave;
		      
		      tr[tr[t].u0].d0 = t;
		      tr[tr[tn].u0].d0 = tn;
		      tr[tr[tn].u1].d0 = tn;
		    }
		  else		/* intersects in the right */
		    {
		      tr[tn].u1 = -1;
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = tr[t].u0;
		      tr[t].u0 = tr[t].usave;

		      tr[tr[t].u0].d0 = t;
		      tr[tr[t].u1].d0 = t;
		      tr[tr[tn].u0].d0 = tn;		      
		    }
		  
		  tr[t].usave = tr[tn].usave = 0;
		}
	      else		/* No usave.... simple case */
		{
		  tr[tn].u0 = tr[t].u1;
		  tr[t].u1 = tr[tn].u1 = -1;
		  tr[tr[tn].u0].d0 = tn;
		}
	    }
	  else 
	    {			/* fresh seg. or upward cusp */
	      int tmp_u = tr[t].u0;
	      int td0, td1;
	      if (((td0 = tr[tmp_u].d0) > 0) && 
		  ((td1 = tr[tmp_u].d1) > 0))
		{		/* upward cusp */
		  if ((tr[td0].rseg > 0) &&
		      !is_left_of(seg[tr[td0].rseg], s.v1))
		    {
		      tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		      tr[tr[tn].u0].d1 = tn;
		    }
		  else 
		    {
		      tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		      tr[tr[t].u0].d0 = t;
		    }
		}
	      else		/* fresh segment */
		{
		  tr[tr[t].u0].d0 = t;
		  tr[tr[t].u0].d1 = tn;
		}
	    }
	  
	  if ( tribot && tr[t].lo == tr[tlast].lo )
	    {		/* bottom forms a triangle */
	      int tmpseg;

	      if (is_swapped)	
		tmpseg = seg[segnum].prev;
	      else
		tmpseg = seg[segnum].next;

	      if ((tmpseg > 0) && is_left_of(seg[tmpseg], s.v0))
		{
		  /* L-R downward cusp */
		  tr[tr[t].d1].u0 = t;
		  tr[tn].d0 = tr[tn].d1 = -1;
		}
	      else
		{
		  /* R-L downward cusp */
		  tr[tr[tn].d1].u1 = tn;
		  tr[t].d0 = tr[t].d1 = -1;
		}
	    }		
	  else
	    {
	      if ((tr[tr[t].d1].u0 > 0) && (tr[tr[t].d1].u1 > 0))
		{
		  if (tr[tr[t].d1].u0 == t) /* passes thru LHS */
		    {
		      tr[tr[t].d1].usave = tr[tr[t].d1].u1;
		      tr[tr[t].d1].uside = S_LEFT;
		    }
		  else
		    {
		      tr[tr[t].d1].usave = tr[tr[t].d1].u0;
		      tr[tr[t].d1].uside = S_RIGHT;
		    }		    
		}
	      tr[tr[t].d1].u0 = t;
	      tr[tr[t].d1].u1 = tn;
	    }
	  
	  t = tr[t].d1;
	}

      /* two trapezoids below. Find out which one is intersected by */
      /* this segment and proceed down that one */
      
      else
	{
	  int tmpseg = tr[tr[t].d0].rseg;
	  double y0, yt;
	  Vector2d tmppt;
	  int tnext;
      bool i_d0 = false;
	  if ( tr[t].lo.y == s.v0.y )
	  {
	      i_d0 = tr[t].lo.x > s.v0.x;
	  }
	  else
	  {
	      tmppt.y = y0 = tr[t].lo.y;
	      yt = (y0 - s.v0.y)/(s.v1.y - s.v0.y);
	      tmppt.x = s.v0.x + yt * (s.v1.x - s.v0.x);
	      
	      if ( _less_than ( tmppt, tr[t].lo ) ) i_d0 = true;
	  }
	  
	  /* check continuity from the top so that the lower-neighbour */
	  /* values are properly filled for the upper trapezoid */

	  if ((tr[t].u0 > 0) && (tr[t].u1 > 0))
	    {			/* continuation of a chain from abv. */
	      if (tr[t].usave > 0) /* three upper neighbours */
		{
		  if (tr[t].uside == S_LEFT)
		    {
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = -1;
		      tr[tn].u1 = tr[t].usave;
		      
		      tr[tr[t].u0].d0 = t;
		      tr[tr[tn].u0].d0 = tn;
		      tr[tr[tn].u1].d0 = tn;
		    }
		  else		/* intersects in the right */
		    {
		      tr[tn].u1 = -1;
		      tr[tn].u0 = tr[t].u1;
		      tr[t].u1 = tr[t].u0;
		      tr[t].u0 = tr[t].usave;

		      tr[tr[t].u0].d0 = t;
		      tr[tr[t].u1].d0 = t;
		      tr[tr[tn].u0].d0 = tn;		      
		    }
		  
		  tr[t].usave = tr[tn].usave = 0;
		}
	      else		/* No usave.... simple case */
		{
		  tr[tn].u0 = tr[t].u1;
		  tr[tn].u1 = -1;
		  tr[t].u1 = -1;
		  tr[tr[tn].u0].d0 = tn;
		}
	    }
	  else 
	    {			/* fresh seg. or upward cusp */
	      int tmp_u = tr[t].u0;
	      int td0, td1;
	      if (((td0 = tr[tmp_u].d0) > 0) && 
		  ((td1 = tr[tmp_u].d1) > 0))
		{		/* upward cusp */
		  if ((tr[td0].rseg > 0) &&
		      !is_left_of(seg[tr[td0].rseg], s.v1))
		    {
		      tr[t].u0 = tr[t].u1 = tr[tn].u1 = -1;
		      tr[tr[tn].u0].d1 = tn;
		    }
		  else 
		    {
		      tr[tn].u0 = tr[tn].u1 = tr[t].u1 = -1;
		      tr[tr[t].u0].d0 = t;
		    }
		}
	      else		/* fresh segment */
		{
		  tr[tr[t].u0].d0 = t;
		  tr[tr[t].u0].d1 = tn;
		}
	    }
	  
	  if ( tribot && tr[t].lo == tr[tlast].lo )
	    {
	      /* this case arises only at the lowest trapezoid.. i.e.
		 tlast, if the lower endpoint of the segment is
		 already inserted in the structure */
	      
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = -1;
	      tr[tr[t].d1].u0 = tn;
	      tr[tr[t].d1].u1 = -1;

	      tr[tn].d0 = tr[t].d1;
	      tr[t].d1 = tr[tn].d1 = -1;
	      
	      tnext = tr[t].d1;	      
	    }
	  else if (i_d0)
				/* intersecting d0 */
	    {
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = tn;
	      tr[tr[t].d1].u0 = tn;
	      tr[tr[t].d1].u1 = -1;
	      
	      /* new code to determine the bottom neighbours of the */
	      /* newly partitioned trapezoid */
	      
	      tr[t].d1 = -1;

	      tnext = tr[t].d0;
	    }
	  else			/* intersecting d1 */
	    {
	      tr[tr[t].d0].u0 = t;
	      tr[tr[t].d0].u1 = -1;
	      tr[tr[t].d1].u0 = t;
	      tr[tr[t].d1].u1 = tn;

	      /* new code to determine the bottom neighbours of the */
	      /* newly partitioned trapezoid */
	      
	      tr[tn].d0 = tr[t].d1;
	      tr[tn].d1 = -1;
	      
	      tnext = tr[t].d1;
	    }	    
	  
	  t = tnext;
	}
      
      tr[t_sav].rseg = tr[tn_sav].lseg  = segnum;
    } /* end-while */
  
  /* Now combine those trapezoids which share common segments. We can */
  /* use the pointers to the parent to connect these together. This */
  /* works only because all these new trapezoids have been formed */
  /* due to splitting by the segment, and hence have only one parent */

  tfirstl = tfirst; 
  tlastl = tlast;
  merge_trapezoids(segnum, tfirstl, tlastl, S_LEFT);
  merge_trapezoids(segnum, tfirstr, tlastr, S_RIGHT);

  seg[segnum].is_inserted = true;
}

inline double get_angle ( const Vector2d & vp0, const Vector2d & vpnext, const Vector2d & vp1 )
{
    const Vector2d v0 ( vpnext - vp0 );
    const Vector2d v1 (  vp1   - vp0 );
    const double t = ( v0 * v1) / sqrt ( (v0*v0) * (v1*v1) );
    return v0 % v1 >= 0 ? t : -t - 2;
}

/* (v0, v1) is the new diagonal to be added to the polygon. Find which */
/* chain to use and return the positions of v0 and v1 in p and q */ 
void get_vertex_positions ( const vertexchain_t & vp0, const vertexchain_t & vp1, 
                            CCArrRef<vertexchain_t> vert, nat & ip, nat & iq )
{
    double angle, temp;
    nat i, tp, tq;

  /* p is identified as follows. Scan from (v0, v1) rightwards till */
  /* you hit the first segment starting from v0. That chain is the */
  /* chain of our interest */
  
    angle = -4.0;
    for ( i = 0; i < 4; ++i )
    {
        if ( vp0.vnext[i] <= 0 )
	        continue;
        if ( ( temp = get_angle ( vp0.pt, vert[vp0.vnext[i]].pt, vp1.pt ) ) > angle )
	    {
	        angle = temp;
	        tp = i;
	    }
    }
    ip = tp;

  /* Do similar actions for q */

    angle = -4.0;
    for (i = 0; i < 4; i++)
    {
        if ( vp1.vnext[i] <= 0 )
	        continue;      
        if ( ( temp = get_angle ( vp1.pt,  vert[vp1.vnext[i]].pt, vp0.pt ) ) > angle )
	    {
	        angle = temp;
	        tq = i;
	    }
    }
    iq = tq;
}

/* v0 and v1 are specified in anti-clockwise order with respect to 
 * the current monotone polygon mcur. Split the current polygon into 
 * two polygons using the diagonal (v0, v1) 
 */
int TrapezoidData::make_new_monotone_poly ( int mcur, int v0, int v1 )
{
    nat ip, iq;
    vertexchain_t & vp0 = vert[v0];
    vertexchain_t & vp1 = vert[v1];

    get_vertex_positions ( vp0, vp1, vert, ip, iq );

    nat p = vp0.vpos[ip];
    nat q = vp1.vpos[iq];

  /* At this stage, we have got the positions of v0 and v1 in the */
  /* desired chain. Now modify the linked lists */

    nat i = new_chain_element();	/* for the new list */
    nat j = new_chain_element();

    mchain[i].vnum = v0;
    mchain[j].vnum = v1;

    mchain[i].next = mchain[p].next;
    mchain[mchain[p].next].prev = i;
    mchain[i].prev = j;
    mchain[j].next = i;
    mchain[j].prev = mchain[q].prev;
    mchain[mchain[q].prev].next = j;

    mchain[p].next = q;
    mchain[q].prev = p;

    nat nf0 = vp0.nextfree;
    nat nf1 = vp1.nextfree;

    vp0.vnext[ip] = v1;

    vp0.vpos[nf0] = i;
    vp0.vnext[nf0] = mchain[mchain[i].next].vnum;
    vp1.vpos[nf1] = j;
    vp1.vnext[nf1] = v0;

    vp0.nextfree++;
    vp1.nextfree++;

  int mnew = ++mon_idx;
  mon[mcur] = p;
  mon[mnew] = i;
  return mnew;
}

/* Main routine to get monotone polygons from the trapezoidation of 
 * the polygon.
 */

void TrapezoidData::monotonate_trapezoids ()
{
    nat i;
  /* First locate a trapezoid which lies inside the polygon */
  /* and which is triangular */
    for ( i = 0; i < tr.size(); ++i )
    {
        const trap_t & t = tr[i];
        if ( t.state == ST_INVALID ) continue;
        if ( t.lseg <= 0 || t.rseg <= 0 ) continue;
        if ( ( t.u0 <= 0 && t.u1 <= 0 ) || 
             ( t.d0 <= 0 && t.d1 <= 0 ) ) /* triangle */
            if ( _greater_than ( seg[t.rseg].v1, seg[t.rseg].v0 ) ) break;
    }
    /* traverse the polygon */
    if ( tr[i].u0 > 0 )
        traverse_polygon ( 0, i, tr[i].u0, TR_FROM_UP );
    else if ( tr[i].d0 > 0 )
        traverse_polygon ( 0, i, tr[i].d0, TR_FROM_DN );
}


/* recursively visit all the trapezoids */
void TrapezoidData::traverse_polygon ( int mcur, int trnum, int from, int dir )
{
    if ( trnum <= 0 || tr[trnum].visited ) return;
  trap_t *t = &tr[trnum];
  t->visited = true;
  int mnew;
  int v0, v1;
  int retval;
  bool do_switch = false;
  
  /* We have much more information available here. */
  /* rseg: goes upwards   */
  /* lseg: goes downwards */

  /* Initially assume that dir = TR_FROM_DN (from the left) */
  /* Switch v0 and v1 if necessary afterwards */


  /* special cases for triangles with cusps at the opposite ends. */
  /* take care of this first */
  if ((t->u0 <= 0) && (t->u1 <= 0))
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* downward opening triangle */
	{
	  v0 = tr[t->d1].lseg;
	  v1 = t->lseg;
	  if (from == t->d1)
	    {
	      do_switch = true;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);	    
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
	    }
	}
      else
	{
	  retval = SP_NOSPLIT;	/* Just traverse all neighbours */
	  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	}
    }
  
  else if ((t->d0 <= 0) && (t->d1 <= 0))
    {
      if ((t->u0 > 0) && (t->u1 > 0)) /* upward opening triangle */
	{
	  v0 = t->rseg;
	  v1 = tr[t->u0].rseg;
	  if (from == t->u1)
	    {
	      do_switch = true;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);	    
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
	    }
	}
      else
	{
	  retval = SP_NOSPLIT;	/* Just traverse all neighbours */
	  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	}
    }
  
  else if ((t->u0 > 0) && (t->u1 > 0)) 
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* downward + upward cusps */
	{
	  v0 = tr[t->d1].lseg;
	  v1 = tr[t->u0].rseg;
	  retval = SP_2UP_2DN;
	  if (((dir == TR_FROM_DN) && (t->d1 == from)) ||
	      ((dir == TR_FROM_UP) && (t->u1 == from)))
	    {
	      do_switch = true;
	      mnew = make_new_monotone_poly(mcur, v1, v0);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
	    }
	  else
	    {
	      mnew = make_new_monotone_poly(mcur, v0, v1);
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);	      
	    }
	}
      else			/* only downward cusp */
	{
	  if ( t->lo == seg[t->lseg].v1 )
	    {
	      v0 = tr[t->u0].rseg;
	      v1 = seg[t->lseg].next;

	      retval = SP_2UP_LEFT;
	      if ((dir == TR_FROM_UP) && (t->u0 == from))
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		}
	    }
	  else
	    {
	      v0 = t->rseg;
	      v1 = tr[t->u0].rseg;	
	      retval = SP_2UP_RIGHT;
	      if ((dir == TR_FROM_UP) && (t->u1 == from))
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	}
    }
  else if ((t->u0 > 0) || (t->u1 > 0)) /* no downward cusp */
    {
      if ((t->d0 > 0) && (t->d1 > 0)) /* only upward cusp */
	{
	  if ( t->hi == seg[t->lseg].v0 )
	    {
	      v0 = tr[t->d1].lseg;
	      v1 = t->lseg;
	      retval = SP_2DN_LEFT;
	      if (!((dir == TR_FROM_DN) && (t->d0 == from)))
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);	      
		}
	    }
	  else
	    {
	      v0 = tr[t->d1].lseg;
	      v1 = seg[t->rseg].next;

	      retval = SP_2DN_RIGHT;	    
	      if ((dir == TR_FROM_DN) && (t->d1 == from))
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		}
	    }
	}
      else			/* no cusp */
	{
	  if ( t->hi == seg[t->lseg].v0 && t->lo == seg[t->rseg].v0 )
	    {
	      v0 = t->rseg;
	      v1 = t->lseg;
	      retval = SP_SIMPLE_LRDN;
	      if (dir == TR_FROM_UP)
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	  else if ( t->hi == seg[t->rseg].v1 && t->lo == seg[t->lseg].v1 )
	    {
	      v0 = seg[t->rseg].next;
	      v1 = seg[t->lseg].next;

	      retval = SP_SIMPLE_LRUP;
	      if (dir == TR_FROM_UP)
		{
		  do_switch = true;
		  mnew = make_new_monotone_poly(mcur, v1, v0);
		  traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->d0, trnum, TR_FROM_UP);
		}
	      else
		{
		  mnew = make_new_monotone_poly(mcur, v0, v1);
		  traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);
		  traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
		  traverse_polygon(mnew, t->u0, trnum, TR_FROM_DN);
		  traverse_polygon(mnew, t->u1, trnum, TR_FROM_DN);
		}
	    }
	  else			/* no split possible */
	    {
	      retval = SP_NOSPLIT;
	      traverse_polygon(mcur, t->u0, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d0, trnum, TR_FROM_UP);
	      traverse_polygon(mcur, t->u1, trnum, TR_FROM_DN);
	      traverse_polygon(mcur, t->d1, trnum, TR_FROM_UP);	      	      
	    }
	}
    }
}

void triangulate_monotone_polygons ( ArrRef<monchain_t> & mchain, CCArrRef<nat> & mon, CCArrRef<vertexchain_t> & vert, nat nmonpoly, SuiteRef<Set3<nat> > & res )
{
    Suite<nat> buf;
    res.resize();
    for ( nat i = 0; i < nmonpoly; ++i )
    {
        nat vcount = 1;
        nat posmax = mon[i];
        nat vfirst = mchain[posmax].vnum;
        double ymax = vert[vfirst].pt.y;
        nat p = mchain[posmax].next;
        nat v;
        while ( (v = mchain[p].vnum) != vfirst )
	    {
	        if ( ymax < vert[v].pt.y )
	        {
	            ymax = vert[v].pt.y;
	            posmax = p;
	        }
	        p = mchain[p].next;
	        vcount++;
        }
      
        if ( vcount == 3 )		/* already a triangle */
	    {
            Set3<nat> & s = res.inc();
	        s.a = mchain[p].vnum - 1;
	        s.b = mchain[mchain[p].next].vnum - 1;
	        s.c = mchain[mchain[p].prev].vnum - 1;
	    }
        else			/* triangulate the polygon */
	    {
            buf.resize();
            buf.inc() = posmax;
	        nat prev = mchain[posmax].prev;
	        nat next = mchain[posmax].next;
            if ( vert[mchain[prev].vnum].pt.y < vert[mchain[next].vnum].pt.y )
            {
                buf.inc() = next;
                next = mchain[next].next;
            }
            else
            {
                buf.inc() = prev;
                prev = mchain[prev].prev;
            }
            nat v;
            for ( nat i = 3; i < vcount; ++i )
            {
                if ( vert[mchain[prev].vnum].pt.y < vert[mchain[next].vnum].pt.y )
                {
                    v = next;
                    next = mchain[next].next;
                }
                else
                {
                    v = prev;
                    prev = mchain[prev].prev;
                }
                if ( mchain[v].prev == buf[0] || mchain[v].next == buf[0] )
                {
                    const nat a = buf.las();
                    while ( buf.size() > 1 )
                    {
                        const nat i0 = buf.las(0);
                        const nat i1 = buf.las(1);
                        Set3<nat> & s = res.inc();
                        if ( mchain[v].prev == buf[0] )
                        {
	                        s.a = mchain[i0].vnum - 1;
	                        s.b = mchain[i1].vnum - 1;
                        }
                        else
                        {
	                        s.a = mchain[i1].vnum - 1;
	                        s.b = mchain[i0].vnum - 1;
                        }
	                    s.c = mchain[v].vnum - 1;
                        buf.dec();
                    }
                    buf[0] = a;
                }
                else
                {
                    const Vector2d & v2 = vert[mchain[v].vnum].pt;
                    const bool order = mchain[v].next == buf.las();
                    while ( buf.size() > 1 )
                    {
                        const nat i0 = buf.las(0);
                        const nat i1 = buf.las(1);
                        const Vector2d & v0 = vert[mchain[i0].vnum].pt;
                        const Vector2d & v1 = vert[mchain[i1].vnum].pt;
                        const double t = ( v1 - v0 ) % ( v2 - v1 );
                        if ( t != 0 && ( t < 0 == order ) ) break;
                        Set3<nat> & s = res.inc();
                        if ( order )
                        {
	                        s.a = mchain[i0].vnum - 1;
	                        s.b = mchain[i1].vnum - 1;
                        }
                        else
                        {
	                        s.a = mchain[i1].vnum - 1;
	                        s.b = mchain[i0].vnum - 1;
                        }
	                    s.c = mchain[v].vnum - 1;
                        buf.dec();
                    }
                }
                buf.inc() = v;
            }
            if ( vert[mchain[prev].vnum].pt.y < vert[mchain[next].vnum].pt.y )
            {
                v = next;
                next = mchain[next].next;
            }
            else
            {
                v = prev;
                prev = mchain[prev].prev;
            }
            while ( buf.size() > 1 )
            {
                const nat i0 = buf.las(0);
                const nat i1 = buf.las(1);
                Set3<nat> & s = res.inc();
                if ( mchain[v].prev == buf[0] )
                {
	                s.a = mchain[i0].vnum - 1;
	                s.b = mchain[i1].vnum - 1;
                }
                else
                {
	                s.a = mchain[i1].vnum - 1;
	                s.b = mchain[i0].vnum - 1;
                }
	            s.c = mchain[v].vnum - 1;
                buf.dec();
            }
        }
    }
}

void TrapezoidData::triangulate_polygon ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vertices )
{
// Инициализация
    nat i, n = 0;
    for ( i = 0; i < cntr.size(); ++i ) n += cntr[i];
    seg .resize ( n + 1 );
    vert.resize ( n + 1 );
    mon .resize ( n + 1 );
    mchain.resize ( n + 1 );
    i = 1;
    for ( nat k = 0; k < cntr.size(); ++k )
    {
        const nat npoints = cntr[k];
        const nat first = i;
        const nat last = first + npoints - 1;
        for ( nat j = 0; j < npoints; j++, i++ )
	    {
	        seg[i].v0 = vertices[i-1];
	        seg[i].is_inserted = false;
            seg[i].root0 = seg[i].root1 = 0;
	        if (i == last)
	        {
	            seg[i].next = first;
	            seg[i].prev = i-1;
	            seg[i-1].v1 = seg[i].v0;
	        }
	        else if (i == first)
	        {
	            seg[i].next = i+1;
	            seg[i].prev = last;
	            seg[last].v1 = seg[i].v0;
	        }
	        else
	        {
	            seg[i].prev = i-1;
	            seg[i].next = i+1;
	            seg[i-1].v1 = seg[i].v0;
	        }

            mchain[i].marked = false;
            mchain[i].prev = seg[i].prev;
            mchain[i].next = seg[i].next;
            mchain[i].vnum = i;

            vert[i].pt = seg[i].v0;
            vert[i].vnext[0] = seg[i].next; /* next vertex */
            vert[i].vpos[0] = i;	/* locn. of next vertex */
            vert[i].nextfree = 1;
            vert[i].vnext[1] = vert[i].vnext[2] = vert[i].vnext[3] =
            vert[i].vpos[1] = vert[i].vpos[2] = vert[i].vpos[3] = 0;
	    }
    }
// Перемешивание
    DynArray<nat> permute ( n );
    for ( i = 0; i < n; ++i ) permute[i] = i + 1;
    shuffle ( permute, PRand() );
    nat choose_idx = 0;
// Add the first segment and get the query structure and trapezoid list initialised
    tr.resize(4*n).resize(0);
    qs.resize(8*n).resize(0);
    init_query_structure ( permute[choose_idx++] );
    for ( i = 1; i <= n; ++i ) seg[i].root0 = seg[i].root1 = 0;
    const int m = math_logstar_n ( n );
    for ( int h = 0; h < m; ++h )
    {
        for ( i = math_N ( n, h ) + 1; i <= math_N ( n, h + 1 ); ++i ) add_segment ( permute[choose_idx++] );
        /* Find a new root for each of the segment endpoints */
        /* Update the roots stored for each of the endpoints of the segment.
         * This is done to speed up the location-query for the endpoint when
         * the segment is inserted into the trapezoidation subsequently
         */
        for ( i = 1; i <= n; ++i )
        {
            segment_t & s = seg[i];
            if ( s.is_inserted ) continue;
            s.root0 = locate_endpoint ( s.v0, s.v1, s.root0 );
            s.root0 = tr[s.root0].sink;
            s.root1 = locate_endpoint ( s.v1, s.v0, s.root1 );
            s.root1 = tr[s.root1].sink;  
        }
    }
    for ( i = math_N ( n, m ); i < n; ++i ) add_segment ( permute[choose_idx++] );

    mon_idx = 0;
    monotonate_trapezoids();
}

SuiteRef<Set3<nat> > & trianSeidel ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    TrapezoidData data;
    data.triangulate_polygon ( cntr, vert );
    triangulate_monotone_polygons ( data.mchain, data.mon, data.vert, data.mon_idx+1, res );
    return res;
}

SuiteRef<Set3<nat> > & trianSeidel ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    FixArray<nat, 1> cntr;
    cntr[0] = vert.size();
    return trianSeidel ( cntr, vert, res );
}

//**************************** 25.11.2015 *********************************//
//
//            Триангуляция многоугольников ( заметающая прямая )
//
//**************************** 19.08.2018 *********************************//

class ChainNode
{
public:
    nat index, prev, next;
    Item3n * segm;
};

class Polygon
{
public:
    nat size, vert;
};
void triangulate_monotone_polygons ( CCArrRef<Polygon> & monpolys, CCArrRef<ChainNode> & node, CCArrRef<Vector2d> & vertex, SuiteRef<Set3<nat> > & res )
{
    Suite<nat> buf;
    for ( nat i = 0; i < monpolys.size(); ++i )
    {
        const Polygon & poly = monpolys[i];
        if ( poly.size == 3 )
	    {
            const ChainNode & v = node[poly.vert];
            Set3<nat> & s = res.inc();
	        s.a = v.index;
	        s.b = node[v.next].index;
	        s.c = node[v.prev].index;
	    }
        else
	    {
            bool isPrev = true;
            buf.resize();
            buf.inc() = poly.vert;
            nat prev = node[poly.vert].prev;
            nat next = node[poly.vert].next;
            if ( vertex[node[prev].index].x > vertex[node[next].index].x )
            {
                buf.inc() = next;
                next = node[next].next;
                isPrev = false;
            }
            else
            {
                buf.inc() = prev;
                prev = node[prev].prev;
            }
            nat v;
            for ( nat i = 3; i < poly.size; ++i )
            {
                if ( isPrev )
                {
                    if ( vertex[node[next].index].x <= vertex[node[prev].index].x )
                    {
                        v = next;
                        next = node[next].next;
                        const nat a = buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            Set3<nat> & s = res.inc();
	                        s.a = node[i0].index;
	                        s.b = node[i1].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                        buf[0] = a;
                        isPrev = false;
                    }
                    else
                    {
                        v = prev;
                        prev = node[prev].prev;
                        const Vector2d & v2 = vertex[node[v].index];
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            const Vector2d & v0 = vertex[node[i0].index];
                            const Vector2d & v1 = vertex[node[i1].index];
                            const double t = ( v1 - v0 ) % ( v2 - v1 );
                            if ( t < 0 ) break;
                            Set3<nat> & s = res.inc();
	                        s.a = node[i0].index;
	                        s.b = node[i1].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                    }
                }
                else
                {
                    if ( vertex[node[prev].index].x <= vertex[node[next].index].x )
                    {
                        v = prev;
                        prev = node[prev].prev;
                        const nat a = buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            Set3<nat> & s = res.inc();
	                        s.a = node[i1].index;
	                        s.b = node[i0].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                        buf[0] = a;
                        isPrev = true;
                    }
                    else
                    {
                        v = next;
                        next = node[next].next;
                        const Vector2d & v2 = vertex[node[v].index];
                        const bool order = node[v].next == buf.las();
                        while ( buf.size() > 1 )
                        {
                            const nat i0 = buf.las(0);
                            const nat i1 = buf.las(1);
                            const Vector2d & v0 = vertex[node[i0].index];
                            const Vector2d & v1 = vertex[node[i1].index];
                            const double t = ( v1 - v0 ) % ( v0 - v2 );
                            if ( t < 0 ) break;
                            Set3<nat> & s = res.inc();
	                        s.a = node[i1].index;
	                        s.b = node[i0].index;
	                        s.c = node[v].index;
                            buf.dec();
                        }
                    }
                }
                buf.inc() = v;
            }
            if ( isPrev )
            {
                while ( buf.size() > 1 )
                {
                    const nat i0 = buf.las(0);
                    const nat i1 = buf.las(1);
                    Set3<nat> & s = res.inc();
	                s.a = node[i0].index;
	                s.b = node[i1].index;
	                s.c = node[next].index;
                    buf.dec();
                }
            }
            else
            {
                while ( buf.size() > 1 )
                {
                    const nat i0 = buf.las(0);
                    const nat i1 = buf.las(1);
                    Set3<nat> & s = res.inc();
	                s.a = node[i1].index;
	                s.b = node[i0].index;
	                s.c = node[prev].index;
                    buf.dec();
                }
            }
        }
    }
}

enum TypeEven { NONE_BEFORE, PREV_BEFORE, NEXT_BEFORE, BOTH_BEFORE };

inline TypeEven typeEven ( CCArrRef<ChainNode> & node, nat v )
{
    const ChainNode & b = node[v];
    if ( node[b.prev].segm )
        return b.segm ? BOTH_BEFORE : PREV_BEFORE;
    else
        return b.segm ? NEXT_BEFORE : NONE_BEFORE;
}

void link ( Suite<ChainNode> & node, nat a, nat b )
{
    const nat a1 = node.size();
    const nat b1 = a1 + 1;
    node.inc(2);
    ChainNode & na = node[a];
    ChainNode & nb = node[b];
    ChainNode & na1 = node[a1];
    ChainNode & nb1 = node[b1];
    na1.segm = nb1.segm = 0;
    na1.index = na.index;
    nb1.index = nb.index;
    na1.prev = na.prev;
    nb1.prev = nb.prev;
    na1.next = b;
    nb1.next = a;
    node[na1.prev].next = nb.prev = a1;
    node[nb1.prev].next = na.prev = b1;
}

bool isConvex ( CCArrRef<ChainNode> & node, nat v, CCArrRef<Vector2d> & vertex )
{
    const ChainNode & n = node[v];
    const Vector2d & b = vertex[n.index];
    const Vector2d & a = vertex[node[n.prev].index];
    const Vector2d & c = vertex[node[n.next].index];
    if ( a.y >= b.y && b.y >= c.y ) return true;
    if ( a.y <= b.y && b.y <= c.y ) return false;
    return ( b - a ) % ( c - b ) >= 0;
}

bool isInto ( CCArrRef<ChainNode> & node, nat v, CCArrRef<Vector2d> & vertex, const Item3n * p )
{
    const Vector2d & c = vertex[node[v].index];
    const ChainNode & a = node[p->a];
    const Vector2d & a1 = vertex[a.index];
    const Vector2d & b1 = vertex[node[a.next].index];
    if ( a1.x == b1.x )
    {
        if ( c.y < b1.y ) return false;
    }
    else
    {
        if ( ( b1 - a1 ) % ( c - b1 ) < 0 ) return false;
    }
    const ChainNode & b = node[p->b];
    const Vector2d & a2 = vertex[b.index];
    const Vector2d & b2 = vertex[node[b.next].index];
    if ( a2.x == b2.x )
    {
        if ( c.y > a2.y ) return false;
    }
    else
    {
        if ( ( b2 - a2 ) % ( c - b2 ) < 0 ) return false;
    }
    return true;
}

void noneBefore ( Suite<ChainNode> & node, nat v, List3n & sweepLine, List3n & stor3n, CCArrRef<Vector2d> & vertex )
{
    if ( ! isConvex ( node, v, vertex ) )
    {
        if ( sweepLine.top() )
        do
        {
            Item3n * t = sweepLine.cur();
            if ( ! isInto ( node, v, vertex, t ) ) continue;
            link ( node, t->c, v );
            Item3n * p = sweepLine.addNewAftLas ( stor3n );
            p->c = node.size() - 1;
            p->b = node[p->c].prev;
            p->a = t->a;
            node[p->a].segm = node[p->b].segm = p;
            t->a = t->c = v;
            node[t->a].segm = t;
            return;
        }
        while ( sweepLine.next() );
    }
    Item3n * p = sweepLine.addNewAftLas ( stor3n );
    p->a = p->c = v;
    p->b = node[v].prev;
    node[p->a].segm = node[p->b].segm = p;
}

void prevBefore ( Suite<ChainNode> & node, nat v )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    const nat pre = a.segm->c;
    ( b.segm = a.segm )->a = v;
    b.segm->c = v;
    a.segm = 0;
    if ( pre != b.prev ) link ( node, pre, v );
}

void nextBefore ( Suite<ChainNode> & node, nat v )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    const nat pre = b.segm->c;
    ( a.segm = b.segm )->b = b.prev;
    a.segm->c = v;
    b.segm = 0;
    if ( pre != b.next )
    {
        a.segm->c = node.size() + 1;
        link ( node, pre, v );
    }
}

void bothBefore ( Suite<ChainNode> & node, nat v, List3n & sweepLine, List3n & stor3n )
{
    ChainNode & b = node[v];
    ChainNode & a = node[b.prev];
    if ( a.segm == b.segm )
    {
        if  ( ! a.segm ) return;
        const nat pre = a.segm->c;
        sweepLine.jump ( a.segm ).movCurAftLas ( stor3n );
        a.segm = b.segm = 0;
        if ( pre != b.prev && pre != b.next ) link ( node, pre, v );
    }
    else
    {
        const nat pa = a.segm->c;
        const nat pb = b.segm->c;
        b.segm->b = a.segm->b;
        Item3n * segm = node[b.segm->b].segm = b.segm;
        b.segm->c = v;
        sweepLine.jump ( a.segm ).movCurAftLas ( stor3n );
        a.segm = b.segm = 0;
        const bool linka = pa != node[v].prev;
        const bool linkb = pb != node[v].next;
        if ( linkb )
        {
            segm->c = node.size() + 1;
            link ( node, pb, v );
        }
        if ( linka ) link ( node, pa, segm->c );
    }
}

SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<nat> & cntr, CCArrRef<Vector2d> & vertex, SuiteRef<Set3<nat> > & res )
{
    nat i, nv = 0;
    res.resize();
    if ( vertex.size() < 3 ) return res;
    for ( i = 0; i < cntr.size(); ++i )
    {
        if ( cntr[i] < 3 ) return res;
        nv += cntr[i];
    }
    if ( nv < 3 || nv > vertex.size() ) return res;
// Подготовка данных
    Suite<ChainNode> node ( nv );
    for ( i = 0; i < cntr.size(); ++i )
    {
        const nat n = cntr[i];
        const nat m = node.size();
        node.inc ( n );
        for ( nat j = 0; j < n; ++j )
        {
            const nat k = m + j;
            ChainNode & vi = node[k];
            vi.next = k + 1;
            vi.prev = k - 1;
            vi.index = k;
            vi.segm = 0;
        }
        node[m].prev = node.size() - 1;
        node.las().next = m;
    }
// Построение монотонных многоугольников
    {
        DynArray<SortItem<double, nat> > schedule ( node.size() );
        for ( i = 0; i < node.size(); ++i )
        {
            SortItem<double, nat> & si = schedule[i];
            si.head = vertex[node[i].index].x;
            si.tail = i;
        }
        quickSort123 ( schedule );
        List3n sweepLine, stor3n;
        for ( i = 0; i < schedule.size(); ++i )
        {
            const nat v = schedule[i].tail;
            const TypeEven e = typeEven ( node, v );
            switch ( e )
            {
            case NONE_BEFORE: noneBefore ( node, v, sweepLine, stor3n, vertex ); break;
            case PREV_BEFORE: prevBefore ( node, v ); break;
            case NEXT_BEFORE: nextBefore ( node, v ); break;
            case BOTH_BEFORE: bothBefore ( node, v, sweepLine, stor3n ); break;
            }
//test ( vertex, node, sweepLine );
        }
    }
// Выделение монотонных многоугольников
    Item3n dummy;
    Suite<Polygon> temp;
    for ( i = 0; i < node.size(); ++i )
    {
        ChainNode & ni = node[i];
        if ( ni.segm ) continue;
        ni.segm = & dummy;
        Polygon & poly = temp.inc();
        poly.vert = i;
        poly.size = 1;
        nat j = node[i].next;
        while ( j != i )
        {
            ChainNode & nj = node[j];
            nj.segm = & dummy;
            if ( vertex[node[poly.vert].index].x > vertex[nj.index].x ) poly.vert = j;
            j = nj.next;
            ++poly.size;
        }
    }
// Триангуляция монотонных многоугольников
    triangulate_monotone_polygons ( temp, node, vertex, res );
    return res;
}

SuiteRef<Set3<nat> > & trianSweepLine ( CCArrRef<Vector2d> & vert, SuiteRef<Set3<nat> > & res )
{
    FixArray<nat, 1> cntr;
    cntr[0] = vert.size();
    return trianSweepLine ( cntr, vert, res );
}

//**************************** 24.08.2018 *********************************//
//
//      Перестройка триангуляции на триангуляцию Делоне
//
//**************************** 24.08.2018 *********************************//

ArrRef<Set3<nat> > & rebuildDelauney ( CCArrRef<Vector2d> & vert, ArrRef<Set3<nat> > & res )
{
    const nat nt = res.size();
    const nat nv = vert.size();
    if ( nt < 2 || nv < 4 ) return res;
    // Запишем массив полурёбер SemiRib.
    // Причём рёбра принадлежащие к одному треугольнику должны находится последовательно, 
    // а поле twin должно быть меньше количества рёбер только у одного ребра из пары.
    nat i, k;
    const nat nr = 3 * nt;
    DynArray<SemiRib> rib ( nr );
    DynArray<SortItem<Set2<nat>, nat> > sar ( nr );
    for ( k = 0; k < nt; ++k )
    {
        const Set3<nat> & t = res[k];
        const nat na = 3 * k;
        const nat nb = na + 1;
        const nat nc = nb + 1;
        SemiRib & ra = rib[na];
        ra.next = nb;
        ra.twin = nr;
        ra.vert = t.a;
        SemiRib & rb = rib[nb];
        rb.next = nc;
        rb.twin = nr;
        rb.vert = t.b;
        SemiRib & rc = rib[nc];
        rc.next = na;
        rc.twin = nr;
        rc.vert = t.c;
        SortItem<Set2<nat>, nat> & sa = sar[na];
        sa.head = t.a < t.b ? Set2<nat> ( t.a, t.b ) : Set2<nat> ( t.b, t.a );
        sa.tail = na;
        SortItem<Set2<nat>, nat> & sb = sar[nb];
        sb.head = t.b < t.c ? Set2<nat> ( t.b, t.c ) : Set2<nat> ( t.c, t.b );
        sb.tail = nb;
        SortItem<Set2<nat>, nat> & sc = sar[nc];
        sc.head = t.c < t.a ? Set2<nat> ( t.c, t.a ) : Set2<nat> ( t.a, t.c );
        sc.tail = nc;
    }
    quickSort123 ( sar );
    for ( i = 1; i < nr; ++i )
    {
        SortItem<Set2<nat>, nat> & sa = sar[i];
        SortItem<Set2<nat>, nat> & sb = sar[i-1];
        if ( sa == sb )
        {
            rib[sa.tail].twin = sb.tail;
            ++i;
        }
    }
    const Min2a<double> merge;
    const TQ_MinTan<double, Vector2d> quality ( vert );
    maxL1<double> ( quality, merge, rib );
    for ( i = 0, k = 0; i < nr; ++i )
    {
        SemiRib & a = rib[i];
        if ( a.vert >= nv ) continue;
        SemiRib & b = rib[a.next];
        SemiRib & c = rib[b.next];
        Set3<nat> & t = res[k++];
        t.a = a.vert;
        t.b = b.vert;
        t.c = c.vert;
        a.vert = b.vert = c.vert = nv;
    }
    return res;
}

//**************************** 01.08.2007 *********************************//
//
//      Проверка на наличие триангуляции с положительными треугольниками
//
//**************************** 27.08.2011 *********************************//

bool trianTestNat1L1Area ( CCArrRef<Vector2d> & vert )
{
    const TQ_Area<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

bool trianTestNat1L1AdivR2 ( CCArrRef<Vector2d> & vert )
{
    const TQ_AdivR2<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

bool trianTestNat1L1MinTan ( CCArrRef<Vector2d> & vert )
{
    const TQ_MinTan<double, Vector2d> quality ( vert );
    return tempTrianTestNat1L1<double, Vector2d, double> ( quality, vert );
}

//**************************** 07.08.2008 *********************************//
//
//              Разбиение многоугольника на выпуклые части
//
//**************************** 27.08.2018 *********************************//

struct GEdge
{
    nat next, prev, vert, facet;
};

void convexParts ( CCArrRef<Vector2d> & poly, List< ListItem<ShevList> > & res )
{
    nat i;
    const nat n = poly.size();
    if ( n < 4 )
    {
        ListItem<ShevList> * list = res.resize(1).cur();
        list->delAll();
        for ( i = 0; i < n; ++i ) list->addAftCur ( new ShevItem ( i ) );
        return;
    }
    res.delAll();
// Делаем триангуляцию
    const nat nf = n - 2;
    Suite< Set3<nat> > trip ( nf );
    trianSweepLine ( poly, trip );
    rebuildDelauney ( poly, trip );
// Находим диагонали
    const nat ne = 3 * trip.size();
    DynArray<GEdge> edge ( ne );
    const nat nd = n - 3;
    DynArray< SortItem<nat, nat> > buf ( nd + nd );
    LtdSuiteRef< SortItem<nat, nat> > one ( buf, 0, nd ), two ( buf, nd, nd );
    DynArray<nat> facet ( nf );
    SortItem<nat, nat> item;
    for ( i = 0; i < nf; ++i )
    {
        const Set3<nat> & t = trip[i];
// Заполнение рёбер графа
        const nat j0 = 3 * i;
        const nat j1 = j0 + 1;
        const nat j2 = j0 + 2;
        GEdge & e0 = edge[j0];
        e0.vert = t.a;
        e0.next = j1;
        e0.prev = j2;
        GEdge & e1 = edge[j1];
        e1.vert = t.b;
        e1.next = j2;
        e1.prev = j0;
        GEdge & e2 = edge[j2];
        e2.vert = t.c;
        e2.next = j0;
        e2.prev = j1;
        e0.facet = e1.facet = e2.facet = i;
        facet[i] = j0;
// Заполнение диагоналей
        if ( t.b != t.a + 1 && t.b + n != t.a + 1 )
        {
            item.tail = j0;
            if ( t.b > t.a )
            {
                item.head = n * t.a + t.b;
                one.inc() = item;
            }
            else
            {
                item.head = n * t.b + t.a;
                two.inc() = item;
            }
        }
        if ( t.c != t.b + 1 && t.c + n != t.b + 1 )
        {
            item.tail = j1;
            if ( t.c > t.b )
            {
                item.head = n * t.b + t.c;
                one.inc() = item;
            }
            else
            {
                item.head = n * t.c + t.b;
                two.inc() = item;
            }
        }
        if ( t.a != t.c + 1 && t.a + n != t.c + 1 )
        {
            item.tail = j2;
            if ( t.a > t.c )
            {
                item.head = n * t.c + t.a;
                one.inc() = item;
            }
            else
            {
                item.head = n * t.a + t.c;
                two.inc() = item;
            }
        }
    }
    quickSort123 ( one );
    quickSort123 ( two );
// Объединие многоугольников, если оно будет выпуклым
    for ( i = 0; i < nd; ++i )
    {
        const nat i1 = one[i].tail;
        const nat i2 = two[i].tail;
        GEdge & e1 = edge[i1];
        GEdge & e2 = edge[i2];
        const Vector2d & v1 = poly[edge[e1.prev].vert];
        const Vector2d & v2 = poly[e1.vert];
        const Vector2d & v3 = poly[edge[edge[e2.next].next].vert];
        if ( ( v2 - v1 ) % ( v3 - v2 ) < 0 ) continue;
        const Vector2d & u1 = poly[edge[e2.prev].vert];
        const Vector2d & u2 = poly[e2.vert];
        const Vector2d & u3 = poly[edge[edge[e1.next].next].vert];
        if ( ( u2 - u1 ) % ( u3 - u2 ) < 0 ) continue;
        edge[e1.next].prev = e2.prev;
        edge[e1.prev].next = e2.next;
        edge[e2.next].prev = e1.prev;
        edge[e2.prev].next = e1.next;
        nat j = e2.next;
        do
        {
            edge[j].facet = e1.facet;
            j = edge[j].next;
        }
        while ( j != e1.next );
        facet[e1.facet] = e1.next;
        facet[e2.facet] = ne;
    }
// Заполняем ответ
    for ( i = 0; i < nf; ++i )
    {
        if ( facet[i] == ne ) continue;
        res.addAftCur ( new ListItem<ShevList> );
        ShevList * list = res.cur();
        nat j = facet[i];
        do
        {
            list->addAftLas ( new ShevItem ( edge[j].vert ) );
            j = edge[j].next;
        }
        while ( j != facet[i] );
    }
}
