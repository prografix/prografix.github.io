//
// Binary tree predictive coding
// Helper routines for encoding, plus main encoding function
//
// Note: I have not optimized the encoding. The place to start
// would be the Huffman statistics gathering phase, which could be integrated
// with the formation of the binary tree.
//
// Copyright (C) John Robinson, 1994, 1995, 1997.
//
#define INCEDGE 1
#define ENCODER 1
#include <string.h>
#include "compact.hpp"


static int Asq_encode_table[513] = { 0};
static int *sq_encode_table;
static int Asq_decode_table[513] = { 0};
static int *sq_decode_table;
static int sq_range;
static int sq_arr[5];
static int Adi_encode_table[513] = { 0};
static int *di_encode_table;
static int Adi_decode_table[513] = { 0};
static int *di_decode_table;
static int di_range;
static int di_arr[5];

void prepare_encoder(const int& sq_quant_spacing, const int& di_quant_spacing)
// Sets up tables of representative levels to use in the current bands.
{
    int i;
    // First do the square band
    sq_range = 255/sq_quant_spacing + 1;
    sq_encode_table = &Asq_encode_table[256];
    for (i = 1; i < 256; i++) {
	sq_encode_table[i] = i/sq_quant_spacing;
	sq_encode_table[-i] = -i/sq_quant_spacing;
	}
    sq_decode_table = &Asq_decode_table[256];
    int rep_level = sq_quant_spacing + (sq_quant_spacing-1)/2;
    sq_decode_table[0] = 0;
    for (i = 1; i <= sq_range; i++, rep_level += sq_quant_spacing) {
	sq_decode_table[i] = rep_level;
	sq_decode_table[-i] = -rep_level;
    	}
    // Then the diamond band
    di_range = 255/di_quant_spacing + 1;
    di_encode_table = &Adi_encode_table[256];
    for (i = 1; i < 256; i++) {
	di_encode_table[i] = i/di_quant_spacing;
	di_encode_table[-i] = -i/di_quant_spacing;
	}
    di_decode_table = &Adi_decode_table[256];
    rep_level = di_quant_spacing + (di_quant_spacing-1)/2;
    di_decode_table[0] = 0;
    for (i = 1; i <= di_range; i++, rep_level += di_quant_spacing) {
	di_decode_table[i] = rep_level;
	di_decode_table[-i] = -rep_level;
    }
}

static unsigned char **pyr;
static unsigned char **ridge;

void makescratch ( const int xmax, const int ymax )
// Make space for scratch images. Assume we don't run out of space.
{
	pyr = new unsigned char *[ymax];
	ridge = new unsigned char *[ymax];
	for ( int i = 0; i < ymax; i++ )
	{
		pyr[i] = new unsigned char[xmax];
		ridge[i] = new unsigned char[xmax];
	}
}

void deletescratch ( const int ymax )
{
	for (int i = ymax; --i >= 0; )
    {
	    delete[] ridge[i];
	    delete[] pyr[i];
	}
	delete[] ridge;
	delete[] pyr;
}

void btpc_enc_level(unsigned char **in,unsigned char **pyr,
unsigned char **ridge, const int& XMAX, const int& YMAX, const int& level, 
const int& sq_spacing, const int& di_spacing, const int& maxval)
{
    register int error;
    int code;
    register int prepel;
    int X, Y;
    register int ridgepel;
    register int p,q,r,s, t, u, v;	
    // Surround pels
    register int estimate;
    register int elow, ehigh;
    prepare_encoder(sq_spacing, di_spacing);
    sq_arr[level] = sq_range;
    di_arr[level] = di_range;

    const int sq_ridgethresh = 2*sq_spacing + 7;
    const int di_ridgethresh = 2*di_spacing + 7;
    const int XINTERVAL = 1 << level;
    const int YINTERVAL = 1 << level;
    const int XHINT = XINTERVAL >> 1;
    const int YHINT = YINTERVAL >> 1;
    const int XQINT = XHINT >> 1;
    const int YQINT = YHINT >> 1;
    //printf("level = %d\n",level);
    for (Y = YHINT; Y < YMAX; Y += YINTERVAL)
    {
	//printf("Y = %d\n",Y);
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
	unsigned char *py = in[Y];
	unsigned char *pyplus;
	if (yplus == YMAX)
		pyplus = in[yminus]; // So at bottom of picture, take from
				     // previous line
	else
		pyplus = in[yplus];
	unsigned char *pyqplus = in[yqplus];
	unsigned char *pyqminus = in[yqminus];
	unsigned char *pyminus = in[yminus];
	unsigned char *py2minus = in[y2minus];
	unsigned char *py3minus = in[y3minus];
	register int signtemp;
	register int st2;
	// Set up q and r and v for first row
	v = q = pyminus[0];
	r = pyplus[0];
	for (X = XHINT; X < XMAX; ) {
	    int xplus = X+XHINT;
	    int xminus = X-XHINT;
	    p = q;
	    s = r;
	    u = v;
	    if (xplus != XMAX) {
		q = pyminus[xplus];
		r = pyplus[xplus];
	    }
	    // Otherwise q and r are already correct
	    if (!yminus)
		t = p;
	    else
		t = py2minus[X];
	    prepel = py[X];
	    ridgepel = 0;
	    register int fastpred = 3;
#define SQUARE 1
#include "macro.cpp"
#undef SQUARE
	    py[X] = prepel;
	    pyr[Y][X] = code;
	    ridge[Y][X] = ridgepel;
	    v = prepel;
	    prepel = pyminus[X];
	    ridgepel = 0;
#define DIAMOND_LEFT 1
#include "macro.cpp"
#undef DIAMOND_LEFT
	    pyminus[X] = prepel;
	    pyr[yminus][X] = code;
	    ridge[yminus][X] = ridgepel;
	    prepel = py[xminus];
	    ridgepel = 0;
#define DIAMOND_RIGHT 1
#include "macro.cpp"
#undef DIAMOND_RIGHT
	    py[xminus] = prepel;
	    pyr[Y][xminus] = code;
	    ridge[Y][xminus] = ridgepel;
	    X += XINTERVAL;
	    }
	}
    }

inline void count_next_char(unsigned char& loutval, unsigned char& lscratchval, 
	unsigned char& routval, unsigned char& rscratchval,
	HuffmanEncoder *codeout, int& range, const int& firstbig)
	{
	if (!lscratchval)
		return;
	unsigned char mixed = 0;
	int sendl = 0;
	int sendr = 0;
	if (lscratchval == 1) mixed = 0x80;
	if (rscratchval == 1) mixed |= 0x08;
	if (loutval < firstbig)
		mixed |= (loutval<<4);
	else if (loutval > range-firstbig) 
		mixed |= (((loutval+(256-range)) & 0x07)<<4);
	else {
		mixed |= 0x40;
		sendl = 1;
		}
	if (routval < firstbig)
		mixed |= routval;
	else if (routval > range-firstbig)
		mixed |= (routval+(256-range)) & 0x07;
	else {
		mixed |= 0x04;
		sendr = 1;
		}
	(codeout)->inc(mixed);
	rscratchval = mixed;	// Store the first codeword in rscratch
	if (sendl)
		(codeout+1)->inc(loutval);
	if (sendr)
		(codeout+1)->inc(routval);
	}
inline void put_next_char(unsigned char& loutval, unsigned char& lscratchval, 
	unsigned char& routval, unsigned char& rscratchval, HuffmanEncoder *codeout)
	{
	if (!lscratchval)
		return;
	(codeout)->put(rscratchval);	// I.e. value generated as mixed in
					// count_next_char
	if ((rscratchval & 0x70) == 0x40)
		(codeout+1)->put(loutval);
	if ((rscratchval & 0x07) == 0x04)
		(codeout+1)->put(routval);
	}

//
// bincode works from the finest band upwards, chunking zeros up the binary
// tree until a non-zero entry is reached, which is marked as a leaf. It
// then works down the tree, Huffman coding each band.
//
int bincode(unsigned char **pyr, unsigned char **ridge, 
	const int& inrows, const int& incols, 
	HuffmanEncoder *codeout, unsigned char **scratch, 
	int fromlevel, int tolevel)
{
	int x, y;
	int i,j;
	int pel;
	int rpel;
	// Flags on scratch image interpreted as follows:
	// 0: Beyond end of a tree - don't code
	// 1: Tree terminator
	// 2: Ordinary
	const int rows = inrows;
	const int cols = incols;
	for (y = 0; y < rows; y++)
		memset(scratch[y],2,cols);
	// First blank out highest pass
	int yodd = 0;
	for (y = 0; y < rows; y++, yodd ^= 1)
	    {
	    for (x = (yodd? 0:1); x < cols; x += 2)
		{
		if (tolevel) {
			scratch[y][x] = 0;
			continue;
			}
		pel = pyr[y][x];
		if (pel == 0)
			scratch[y][x] = 1;
		}
	    }
	// Now first mpass
	for (y = 1; y < rows; y += 2)
	for (x = 1; x < cols; x += 2)
		{
		if (tolevel){
			scratch[y][x] = 1;
			pyr[y][x] = 0;
			continue;
			}
		if (scratch[y-1][x] == 1)
		    {
		    if (scratch[y][x-1] == 1)
			{
			scratch[y][x] = 1;
			scratch[y-1][x] = 0;
			scratch[y][x-1] = 0;
			}
		    }
		    
		}
	// Now remainder of bands
	for (i = 2; i <= 2*fromlevel; i *= 2)
		{
		int twoi = i<<1;
		int halfi = i>>1;
		yodd = 0;
		for (y = 0; y < rows; y += i, yodd ^= 1)
		    {
		    int lchild = yodd? halfi : -halfi;
		    for (x = (yodd? 0: i); x < cols; x += twoi)
			  {
			  if ((scratch[y+halfi][x+lchild] == 1)&&
				(pyr[y+halfi][x+lchild] == 0))
			    {
			    if ((scratch[y+halfi][x+lchild+i] == 1)&&
				(pyr[y+halfi][x+lchild+i] == 0))
				{
				scratch[y][x]  = 1;
			  	scratch[y+halfi][x+lchild] = 0;
			   	scratch[y+halfi][x+lchild+i] = 0;
				}
			    }
			  }
		    }
		for (y = i; y < rows; y += twoi)
		for (x = i; x < cols; x += twoi)
			{
			  if((scratch[y-i][x] == 1)&&(pyr[y-i][x] == 0))
			    {
			    if((scratch[y][x-i] == 1)&&(pyr[y][x-i] == 0))
				{
				scratch[y][x] = 1;
			  	scratch[y-i][x] = 0;
			    	scratch[y][x-i] = 0;
				}
			    }
			}
		}
	// Finally, change all the terminators in the highest band back
	// to zeros
	yodd = 0;
	for (y = 0; y < rows; y++, yodd ^= 1)
	{
	    for (x = (yodd? 0:1); x < cols; x += 2)
		{
		    if (scratch[y][x] == 1) scratch[y][x] = 2;
		}
	 }

// For now, just going to do everything twice to form Huffman trees
	// DPCM code the low band
	int predpel;
	int lpspacing = 1<<fromlevel;
	for (y = 0; y < rows; y += lpspacing)
	for (x = 0; x < cols; x += lpspacing)
	{
		predpel =  (x ? pyr[y][x-lpspacing] : (y ? pyr[y-lpspacing][x] : 0));
		int temp = pyr[y][x] - predpel;
		// Ensure wrapping round
		if (temp < -128) temp += 256;
		if (temp > 127) temp -= 256;
		codeout[0].inc(scratch[y][x] = (unsigned char) temp);
	}
	codeout[0].buildtree();
	codeout[0].writetree();
	for (y = 0; y < rows; y += lpspacing)
	for (x = 0; x < cols; x += lpspacing)
		codeout[0].put((unsigned char) scratch[y][x]);
	codeout[0].reset();
	// Now rest of levels
	const int firstbig = 2;
	// const for now. May vary in research versions
	for (i = fromlevel; i > tolevel; i--)
	{
		// printf("Level = %d\n", i);
		int hoffset = 1<<(i-1);
		int offset = 1<<i;
		for (y = hoffset; y < rows; y += offset)
		    {
		    for (x = hoffset; x < cols; x += 2*offset)
			{
			if (!scratch[y][x])
				continue;
				// If one is scratch = 0, both will be
			count_next_char(pyr[y][x], scratch[y][x], 
				pyr[y][x+offset], scratch[y][x+offset],
				&codeout[0],sq_arr[i],firstbig);
			count_next_char(
				pyr[y-hoffset][x], scratch[y-hoffset][x], 
				pyr[y][x-hoffset],scratch[y][x-hoffset],
				&codeout[2],di_arr[i],firstbig);
			if (x+offset >= cols)
		    	// To avoid problems here when pic size is not
		    	// a multiple of 32
				continue;
			count_next_char(
				pyr[y-hoffset][x+offset], 
				scratch[y-hoffset][x+offset], 
				pyr[y][x+hoffset],scratch[y][x+hoffset],
				&codeout[2],di_arr[i],firstbig);
			}
		    }
		for (j = 0; j < numcoders; j++)
        {
			codeout[j].buildtree();
			codeout[j].writetree();
		}
		for (y = hoffset; y < rows; y += offset)
		{
		    // printf("y = %d\n",y);
		    for (x = hoffset; x < cols; x += 2*offset)
			{
			put_next_char(pyr[y][x], scratch[y][x], 
				pyr[y][x+offset], scratch[y][x+offset],
				&codeout[0]);
			if ((rpel = ridge[y][x]) != 0)
				codeout[0].putbit(rpel - 2);
			put_next_char(
				pyr[y-hoffset][x], scratch[y-hoffset][x], 
				pyr[y][x-hoffset],scratch[y][x-hoffset],
				&codeout[2]);
			if ((rpel = ridge[y-hoffset][x]) != 0)
				codeout[0].putbit(rpel - 2);
			if ((rpel = ridge[y][x-hoffset]) != 0)
				codeout[0].putbit(rpel - 2);
			if (x+offset >= cols)
				continue;
// Note next ridge pel is for second of square pels... because decoder
// doesn't get to it until now
			if ((rpel = ridge[y][x+offset]) != 0)
				codeout[0].putbit(rpel - 2);
			put_next_char(
				pyr[y-hoffset][x+offset], 
				scratch[y-hoffset][x+offset], 
				pyr[y][x+hoffset],scratch[y][x+hoffset],
				&codeout[2]);
			if ((rpel = ridge[y-hoffset][x+offset]) != 0)
				codeout[0].putbit(rpel - 2);
			if ((rpel = ridge[y][x+hoffset]) != 0)
				codeout[0].putbit(rpel - 2);
			}
		}
		for (j = 0; j < numcoders; j++) codeout[j].reset();
	}
	return 0;
}

void btpc_enc_plane ( unsigned char ** in, const int xmax, const int ymax,
    const int fromlevel, const int tolevel, const int * const spacing, 
    HuffmanEncoder *codeout, const int clipval )
 {
	// First put lowband into pyr
	int lpspacing = 1 << fromlevel;
	for (int y = 0; y < ymax; y += lpspacing)
	for (int x = 0; x < xmax; x += lpspacing)
		pyr[y][x] = in[y][x];
	// Do bands in pairs
	int band = 1;
	for ( int level = fromlevel; level > tolevel; level--, band += 2 )
	{
		btpc_enc_level ( in, pyr, ridge, xmax, ymax, level, spacing[band], spacing[band+1], clipval );
	}
	bincode ( pyr, ridge, ymax, xmax, codeout, in, fromlevel, tolevel );
}
