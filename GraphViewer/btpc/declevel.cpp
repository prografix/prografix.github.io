//
// Binary tree predictive coding
// Helper routines for decoding, plus main decoding function
//
// Copyright (C) John Robinson, 1994, 1995.
//
#define INCEDGE 1
#include "compact.hpp"
// Following table is used to clip
// Larger than 512 now, because color clipping can require more
static unsigned char * clip;

unsigned char * initclip ( const int maxval )
{
    int i;
    static unsigned char Aclip[512+256];
    clip = &Aclip[256];
    for (i = -256; i < 0; i++) clip[i] = 0;
    for (i = 0; i < maxval; i++) clip[i] = i;
    for (; i < 512; i++) clip[i] = maxval;
    return clip;
}

static int Asq_decode_table[513] = { 0};
static int *sq_decode_table;
static int sq_range;
static int sq_out_of_range[256];
static int Adi_decode_table[513] = { 0};
static int *di_decode_table;
static int di_range;
static int di_out_of_range[256];

int prepare_decoder(const int& sq_quant_spacing, const int& di_quant_spacing)
// Sets up tables of representative levels to use in the current bands.
{
    int i;
    // First do the square band
    sq_decode_table = &Asq_decode_table[256];
    int rep_level = sq_quant_spacing + (sq_quant_spacing-1)/2;
    if (rep_level != sq_decode_table[1])	// Already loaded
    {
	    sq_range = 255/sq_quant_spacing + 1;
	    sq_decode_table[0] = 0;
	    for (i = 1; i <= sq_range; i++, rep_level += sq_quant_spacing)
        {
	        sq_decode_table[i] = rep_level;
	        sq_decode_table[-i] = -rep_level;
	    }
	    for (i = 0; i < 256; i++)
	    {
	        sq_out_of_range[i] = (255-i)/sq_quant_spacing;
	    }
    }
    // Then the diamond band
    di_decode_table = &Adi_decode_table[256];
    rep_level = di_quant_spacing + (di_quant_spacing-1)/2;
    if (rep_level == di_decode_table[1])	// Already loaded
    	return 0;
    di_range = 255/di_quant_spacing + 1;
    di_decode_table[0] = 0;
    for (i = 1; i <= di_range; i++, rep_level += di_quant_spacing) {
	di_decode_table[i] = rep_level;
	di_decode_table[-i] = -rep_level;
    }
    for (i = 0; i < 256; i++)
	{
	di_out_of_range[i] = (255-i)/di_quant_spacing;
	}
    return 0;
}


void btpc_dec_level(unsigned char **out, const int& XMAX, const int& YMAX, 
const int& level, const int& sq_spacing, const int& di_spacing,
HuffmanDecoder *codein)
{
    // printf("level %d\n", level); fflush(stdout);
    for (int ii = 0; ii < numcoders; ii++)
	codein[ii].readtree();
    register int error;
    int X, Y;
    register int p,q,r,s, t, u, v;	
    // Surround pels
    register int estimate;
    register int elow, ehigh;
    int scratch;

    prepare_decoder(sq_spacing, di_spacing);
    const int sq_ridgethresh = 2*sq_spacing + 7;
    const int di_ridgethresh = 2*di_spacing + 7;
    const int XINTERVAL = 1 << level;
    const int YINTERVAL = 1 << level;
    const int XHINT = XINTERVAL >> 1;
    const int YHINT = YINTERVAL >> 1;
    const int XQINT = XHINT >> 1;
    const int YQINT = YHINT >> 1;
    const int sq_neg_correct = sq_range - 8;
    const int di_neg_correct = di_range - 8;
    for (Y = YHINT; Y < YMAX; Y += YINTERVAL)
    {
	// printf("Y = %d\n",Y); fflush(stdout);
	unsigned char *py = out[Y];
	int yplus = Y+YHINT;
	int yqplus = Y+YQINT;
	int yqminus = Y-YQINT;
	int yminus = Y-YHINT;
	int y2minus = Y-YINTERVAL;
	if (y2minus < 0)
		y2minus = 0;
	int y3minus = y2minus-YHINT;
	if (y3minus < 0)
		y3minus = 0;
	unsigned char *pyplus;
	if (yplus == YMAX)
		pyplus = out[yminus]; // So at bottom of picture, take from
				      // previous line
	else
		pyplus = out[yplus];
	unsigned char *pyqplus = out[yqplus];
	unsigned char *pyqminus = out[yqminus];
	unsigned char *pyminus = out[yminus];
	unsigned char *py2minus = out[y2minus];
	unsigned char *py3minus = out[y3minus];
	register int signtemp;
	register int st2;
	register unsigned char mixed;
	register unsigned char temp;
        int nextscratch = -1;
	int xplus = XINTERVAL;
	int xminus = 0;
	register int sq_next_error;
	register int di_next_error;
	// Set up q and r and v for first row
	v = q = pyminus[0];
	r = pyplus[0];
	for (X = XHINT; X < XMAX; X+=XINTERVAL, xplus+=XINTERVAL, xminus+=XINTERVAL) 
	    {
	    p = q;
	    s = r;
	    u = v;
	    if (xplus != XMAX) {
		q = pyminus[xplus];
		r = pyplus[xplus];
	    }
	    // Otherwise q and r are already correct
	register int fastpred = 3;
#define SQUARE 1
#include "macro.cpp"
#undef SQUARE
	if (nextscratch < 0) { // Left sibling 
		nextscratch = scratch = py[X];
		if (!scratch) {
			mixed = codein[0].get();
			if (mixed & 0x80)
				scratch = 1;
			if (mixed & 0x08)
				nextscratch = 1;
			temp = (mixed>>4) & 0x7;
			if (temp == 4)
				error = codein[1].get();
			else if (temp > 4)
				error = temp + sq_neg_correct;
			else
				error = temp;
			temp = mixed & 0x07;
			if (temp == 4)
				sq_next_error = codein[1].get();
			else if (temp > 4)
				sq_next_error = temp + sq_neg_correct;
			else
				sq_next_error = temp;
			}
		else {
			error = 0;
			sq_next_error = 0;
			}
		}
	else {
		scratch = nextscratch;
		nextscratch = -1;
		error = sq_next_error;
		}

#define SQUARE_PROP 1
#include "macro.cpp"
#undef SQUARE_PROP
	    if (!yminus)
		t = p;
	    else
		t = py2minus[X];
#define DIAMOND_LEFT 1
#include "macro.cpp"
#undef DIAMOND_LEFT
	if (!scratch) {
		mixed = codein[2].get();
		if (mixed & 0x80)	// Leaf to be propagated
			pyqminus[X-XQINT] = 1;	//PROPEL
		if (mixed & 0x08)	// Other leaf
			pyqplus[X-XQINT] = 1;	
		temp = (mixed>>4) & 0x7;
		if (temp == 4)
			error = codein[3].get();
		else if (temp > 4)
			error = temp + di_neg_correct;
		else
			error = temp;
		temp = mixed & 0x07;
		if (temp == 4)
			di_next_error = codein[3].get();
		else if (temp > 4)
			di_next_error = temp + di_neg_correct;
		else
			di_next_error = temp;
		}
	else {
		error = 0;
		di_next_error = 0;
		if (level > 1) {
			pyqminus[X-XQINT] = 1;
			pyqplus[X-XQINT] = 1;	
			}
		}
#define DIAMOND_PROP_LEFT 1
#include "macro.cpp"
#undef DIAMOND_PROP_LEFT
#define DIAMOND_RIGHT 1
#include "macro.cpp"
#undef DIAMOND_RIGHT
	error = di_next_error;
#define DIAMOND_PROP_RIGHT 1
#include "macro.cpp"
#undef DIAMOND_PROP_RIGHT
	}
    }
}

void btpc_dec_plane(unsigned char **out,const int& xmax, const int& ymax,
    const int& fromlevel, const int& tolevel, const int *spacing,
    HuffmanDecoder *codein, const int& clipval)
{
	// printf("decplane\n");
	int X, Y;
	// First get dpcm lowpass
	int predpel;
	int inc = 1 << fromlevel;
	codein[0].readtree();
	for (Y = 0; Y < ymax; Y += inc)
	for (X = 0; X < xmax; X += inc)
    {
		predpel =  (X ? out[Y][X-inc] : (Y ? out[Y-inc][X] : 0));
		int temp = codein[0].get() + predpel;
		if (temp < 0) temp += 256;
		if (temp > 255) temp -= 256;
		out[Y][X] = temp;
	}
	// Now do bands in pairs
	initclip(clipval);
	int band = 1;
	for (int level = fromlevel; level > tolevel; level--, band += 2)
    {
		btpc_dec_level(out,xmax,ymax,level,spacing[band],
			spacing[band+1], codein);
	}
}
