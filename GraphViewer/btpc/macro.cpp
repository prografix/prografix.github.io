//
// Binary tree predictive coding
// Macros used by enclevel.C and declevel.C
//
// Copyright (C) John Robinson, 1994, 1995.
//
#if defined(SQUARE) || defined(DIAMOND_LEFT) || defined(DIAMOND_RIGHT)
#undef P
#undef Q
#undef R
#undef S
#undef RIDGETHRESH
#undef EDGETHRESH
#undef HOK
#undef VOK
#undef HTEST
#undef VTEST
#undef SHAPE_H_EST
#undef SHAPE_V_EST
#undef SPACING
#ifdef ENCODER
#undef RANGE
#undef ENCODE_TABLE
#undef DECODE_TABLE
#endif
#endif

#ifdef SQUARE
#define P (p)
#define Q (q)
#define R (r)
#define S (s)
#define RIDGETHRESH (sq_ridgethresh)
#define EDGETHRESH 15
#define HOK ((X!=XHINT)&&(yplus!=YMAX))
#define VOK ((Y!=YHINT)&&(xplus!=XMAX))
#define HTEST ((X!=XHINT)&&(yplus!=YMAX)&& ((p==pyminus[xminus-XINTERVAL])&&\
		    (s==pyplus[xminus-XINTERVAL])))
#define VTEST ((Y!=YHINT)&&(xplus!=XMAX)&&((p==py3minus[xminus])&&\
		    (q==py3minus[xplus])))
#define SHAPE_H_EST (py[X-XINTERVAL])
#define SHAPE_V_EST (py2minus[X])
#define SPACING (sq_spacing)
#ifdef ENCODER
#define RANGE (sq_range);
#define ENCODE_TABLE (sq_encode_table)
#define DECODE_TABLE (sq_decode_table)
#endif
#endif

#ifdef DIAMOND_LEFT
#define P (p)
#define Q (t)
#define R (q)
#define S (v)
#define RIDGETHRESH (di_ridgethresh)
#define EDGETHRESH 15
#define HOK ((xplus!=XMAX)&&(yminus))
#define VOK ((xminus) &&(yminus))
#define HTEST ((xplus!=XMAX)&&(yminus)&& ((p==py3minus[xplus])&&\
                            (v==py2minus[X+XINTERVAL])))
#define VTEST ((xminus) &&(yminus)&& ((p==py2minus[X-XINTERVAL])&&\
                            (q==py3minus[xminus])))
#define SHAPE_H_EST (py2minus[xplus])
#define SHAPE_V_EST (py2minus[xminus])
#define SPACING (di_spacing)
#ifdef ENCODER
#define RANGE (di_range);
#define ENCODE_TABLE (di_encode_table)
#define DECODE_TABLE (di_decode_table)
#endif
#endif

#ifdef DIAMOND_RIGHT
#define P (u)
#define Q (p)
#define R (v)
#define S (s)
#define RIDGETHRESH (di_ridgethresh)
#define EDGETHRESH 15
#define HOK ((xplus!=XMAX)&&(yminus))
#define VOK ((xminus) &&(yminus))
#define HTEST ((xplus!=XMAX)&&(yminus)&& ((u==t)&&(s==q)))
#define VTEST ((xminus) &&(yminus)&& ((u==pyminus[xminus-XINTERVAL])&&\
                            (p==py2minus[X-XINTERVAL])))
#define SHAPE_H_EST (pyminus[X])
#define SHAPE_V_EST (pyminus[X-XINTERVAL])
#define SPACING (di_spacing)
#ifdef ENCODER
#define RANGE (di_range);
#define ENCODE_TABLE (di_encode_table)
#define DECODE_TABLE (di_decode_table)
#endif
#endif

//
// Here is the predictor. For each pel, it's a set of condition tests on its
// neighbors, to decide which combination of them to use as the prediction.
// The neighbors form a square around the current pel, labelled clockwise from
// the top left, P, Q, R, S. The comments in the predictor code refer to the
// signs of differences around the square of neighbor. For example -0++ means
// P > Q, Q = R, R < S, S < P.
//
#if defined(SQUARE) || defined(DIAMOND_LEFT) || defined(DIAMOND_RIGHT)
#ifdef DIAMOND_LEFT
    if (fastpred & 1)
	estimate = (p+q)>>1;
    else
	{
#endif
#ifdef DIAMOND_RIGHT
    if (fastpred & 2)
	estimate = (p+s)>>1;
    else
	{
#endif
	if ((signtemp = Q-P) < 0) {				// -???
		if ((signtemp = R-Q) <= 0) {			// --?? or -0??
			if ((st2 = S-R) > 0)			// --+? or -0+?
				{
				if ((st2=P-S) > 0)		// --++ or -0++
					estimate = (Q+S)>>1;
				else if (!(signtemp + st2))	// -0+0
				// CASE 7
					{
					if (VTEST)
						{
						estimate = SHAPE_V_EST;
						fastpred = 2;
						}
#ifdef INCEDGE
					else if (P-Q > EDGETHRESH && VOK && 
					    ((SHAPE_V_EST==P)||(SHAPE_V_EST==Q))){
						elow = Q;
						ehigh = P;
						estimate = -1;
						fastpred = 2;
						}
#endif
					else
						estimate = (P+Q)>>1;
					}
				else				// --+0 -0+-
					estimate = (P+Q)>>1;
				}
			else if (!st2)				// --0+,-00+
				{
				estimate = (Q+S)>>1;
				} 
			else					// ---+,-0-+, 
				{ 
					estimate = (Q+R)>>1;
				}
			}
		else						// -+??
			{
			if ((signtemp = S-R) > 0) {		// -++?
				if (P-S <= 0)			// -++-,-++0
					estimate = (P+R)>>1;
				else				// -+++
					{
						estimate = (R+S)>>1;
					}
				}
			else  {					// -+-? or -+0?
				if (P-S <= 0)			// -+--, -+-0,
								// -+0-, -+00
					{
						{
						estimate = (P+S)>>1;
						// Allow fastpred on this point
						// because P is not an extreme
						}
					}
				else if (signtemp) {		// -+-+ 
				// CASE 3
					if ((ehigh=P+R)-(elow=Q+S)<=RIDGETHRESH)
						estimate = (ehigh+elow+1)>>2;
					else
						{
						elow >>= 1;
						ehigh >>= 1;
						estimate = -1;
						}
		    			}
				else				// -+0+
					{
						estimate = R;
					}
				}
			}
		}
	else if (signtemp) {
		if ((signtemp = R-Q) >= 0) {
			if ((st2 = S-R) < 0)
				{
				if ((st2 = P-S) < 0)
					estimate = (Q+S)>>1;
				else if (!(signtemp + st2))
				// CASE 7
					{
					if (VTEST)
						{
						estimate = SHAPE_V_EST;
						fastpred = 2;
						}
#ifdef INCEDGE
					else if (Q-P > EDGETHRESH && VOK && 
					    ((SHAPE_V_EST==P)||(SHAPE_V_EST==Q))){
						elow = P;
						ehigh = Q;
						estimate = -1;
						fastpred = 2;
						}
#endif
					else
						estimate = (P+Q)>>1;
					}
				else			
					estimate = (P+Q)>>1;
				}
			else if (!st2)	
				{
				estimate = (Q+S)>>1;
				}
			else		
				{
					estimate = (Q+R)>>1;
				}
			}
		else		
			{
			if ((signtemp = S-R) < 0) {	
				if (P-S >= 0)	
					estimate = (P+R)>>1;
				else	
					{
						estimate = (R+S)>>1;
					}
				}
			else  {	
				if (P-S >= 0)
					{
						{
						estimate = (P+S)>>1;
						// Allow fastpred on this point
						// because P is not an extreme
						}
					}
				else if (signtemp) {
					// CASE 3
					if ((ehigh=Q+S)-(elow=P+R)<=RIDGETHRESH)
						estimate = (ehigh+elow+1)>>2;
					else
						{
						ehigh >>= 1;
						elow >>= 1;
						estimate = -1;
						}
		    			}
				else		
					{
						estimate = R;
					}
				}
			}
		}
	else {
		if ((signtemp = R-Q) < 0) {
			if ((signtemp = S-R) < 0)
				estimate = (P+R)>>1;
			else if (signtemp) {
				if (P-S < 0)
					{
						estimate = P;
					}
				else
					{
					estimate = (P+S)>>1;
					}
				}
			else
				{
				// CASE 6
				if (HTEST)
					{
					estimate = SHAPE_H_EST;
					fastpred = 1;
					}
#ifdef INCEDGE
				else if (P-R > EDGETHRESH && HOK && 
				    ((SHAPE_H_EST==P)||(SHAPE_H_EST==R))){
					elow = R;
					ehigh = P;
					estimate = -1;
					fastpred = 1;
					}
#endif
				else
					estimate = (P+R)>>1;
				}
			}
		else if (signtemp) {
			if ((signtemp = S-R) > 0)
				estimate = (P+R)>>1;
			else if (signtemp) {
				if (P-S > 0)
					{
						estimate = P;
					}
				else
					{
					estimate = (P+S)>>1;
					}
				}
			else
				{
				// CASE 6
				if (HTEST)
					{
					estimate = SHAPE_H_EST;
					fastpred = 1;
					}
#ifdef INCEDGE
				else if (R-P > EDGETHRESH && HOK && 
				    ((SHAPE_H_EST==P)||(SHAPE_H_EST==R))){
					elow = P;
					ehigh = R;
					estimate = -1;
					fastpred = 1;
					}
#endif
				else
					estimate = (P+R)>>1;
				}
			}
		else {
			if (S == R)
				{
				// CASE 5
				if (HTEST)
					{
					estimate = SHAPE_H_EST;
					}
				else if (VTEST)
					{
					estimate = SHAPE_V_EST;
					}
				else
					estimate = P;
				}
			else
				{
				estimate = P;
				}
			}
		}
#if defined(DIAMOND_LEFT) || defined(DIAMOND_RIGHT)
    }
#endif
//
// In the encoder, go straight on to find the prediction error
//
#ifdef ENCODER
	if (estimate < 0) {	// I.e. if the shape is a ridge.
		fastpred = 0;
		if (prepel-elow < ehigh-prepel)
			{
			estimate = elow;
			ridgepel = 2;
			}
		else
			{
			estimate = ehigh;
			ridgepel = 3;
			}
		}
        // At this point have estimate
	error = prepel - estimate;
        // Now quantize the error
        code = error = ENCODE_TABLE[error];
        if (code < 0)
                code += RANGE;
		// code += 256;	// New for new coders
        // May cause wrap around for SPACING = 1

#ifdef SQUARE
	if (error)
		fastpred = 0;
#endif
        // Finally change the value of the input pel to reflect what will
        // be recovered.
        prepel = DECODE_TABLE[error] + estimate;
        if (prepel < 0) prepel = 0;
        else if (prepel > maxval) prepel = maxval;
#endif

#endif


#if defined(DIAMOND_PROP_LEFT)||defined(DIAMOND_PROP_RIGHT)||defined(SQUARE_PROP)
#undef OUTPEL
#undef SPACING
#undef ONESTEP
#undef OUT_OF_RANGE
#undef DECODE_TABLE
#undef RANGE
#endif

#ifdef SQUARE_PROP
#define OUTPEL (v)
#define SPACING (sq_spacing)
#define ONESTEP (sq_onestep)
#define OUT_OF_RANGE (sq_out_of_range)
#define DECODE_TABLE (sq_decode_table)
#define RANGE (sq_range)
#endif

#ifdef DIAMOND_PROP_LEFT
#define OUTPEL (pyminus[X])
#define SPACING (di_spacing)
#define ONESTEP (di_onestep)
#define OUT_OF_RANGE (di_out_of_range)
#define DECODE_TABLE (di_decode_table)
#define RANGE (di_range)
#endif

#ifdef DIAMOND_PROP_RIGHT
#define OUTPEL (py[xminus])
#define SPACING (di_spacing)
#define ONESTEP (di_onestep)
#define OUT_OF_RANGE (di_out_of_range)
#define DECODE_TABLE (di_decode_table)
#define RANGE (di_range)
#endif

//
// Here is the code for the decoder which recovers the pel value
//
#if defined(DIAMOND_PROP_LEFT)||defined(DIAMOND_PROP_RIGHT)||defined(SQUARE_PROP)
	    if (estimate < 0)
		{
		fastpred = 0;
		if (codein[0].getbit())
			estimate = ehigh;
		else
			estimate = elow;
		}
	    if (!error) {
		OUTPEL = estimate;
#ifdef SQUARE_PROP
		if (scratch && (fastpred == 3)) // Do fastest
			{
	    		py[X] = v;
			pyminus[X] = (p+q)>>1;
			py[xminus] = (p+s)>>1;
			if (level > 1) {
				pyqminus[X-XQINT] = 1;
				pyqplus[X-XQINT] = 1;
				}
			continue;	// Straight to next sq point
			}
#endif
		}
            else
		{
#ifdef SQUARE_PROP
		fastpred = 0;
#endif
               	OUTPEL =((error>OUT_OF_RANGE[estimate]) ?
	      	    clip[DECODE_TABLE[error-RANGE]+estimate] :
	      	    // clip[DECODE_TABLE[error-256]+estimate] :
               	    clip[DECODE_TABLE[error]+estimate]);
		}
#ifdef SQUARE_PROP
	    py[X] = v;
#endif

#endif
