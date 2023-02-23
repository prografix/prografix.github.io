//
// Colour space conversion stuff, plus smoothing for downsampling and
// pel replication for upsampling.
//
// These routines are based on jccolor.c, jdcolor.c, jcsample.c, jdsample.c
// from the Independent JPEG Group's software, copyright (C) 1991-1994
// Thomas G. Lane.
//
// The relevant part of the IJG copyright notice is:
/*
The authors make NO WARRANTY or representation, either express or implied,
with respect to this software, its quality, accuracy, merchantability, or
fitness for a particular purpose.  This software is provided "AS IS", and you,
its user, assume the entire risk as to its quality and accuracy.

This software is copyright (C) 1991, 1992, 1993, 1994, Thomas G. Lane.
All Rights Reserved except as specified below.

Permission is hereby granted to use, copy, modify, and distribute this
software (or portions thereof) for any purpose, without fee, subject to these
conditions:
(1) If any part of the source code for this software is distributed, then this
README file must be included, with this copyright and no-warranty notice
unaltered; and any additions, deletions, or changes to the original files
must be clearly indicated in accompanying documentation.
(2) If only executable code is distributed, then the accompanying
documentation must state that "this software is based in part on the work of
the Independent JPEG Group".
(3) Permission for use of this software is granted only if the user accepts
full responsibility for any undesirable consequences; the authors accept
NO LIABILITY for damages of any kind.

These conditions apply to any software derived from or based on the IJG code,
not just to the unmodified library.  If you use our work, you ought to
acknowledge us.

Permission is NOT granted for the use of any IJG author's name or company name
in advertising or publicity relating to this software or products derived from
it.  This software may be referred to only as "the Independent JPEG Group's
software".

We specifically permit and encourage the use of this software as the basis of
commercial products, provided that all warranty or liability claims are
assumed by the product vendor.
*/
//
// I have extracted the relevant routines from the IJG software, converted
// them to work with the rest of the BTPC code (made them much less general),
// and combined some operations. The IJG original is of excellent quality.
// The modifications merely make it more compact for inclusion with my code.
//
// The C-style comments are taken directly from the IJG's files; the C++
// comments are mine. (John Robinson, June 1995)
//

/**************** RGB -> YCbCr conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *	Y  =  0.29900 * R + 0.58700 * G + 0.11400 * B
 *	Cb = -0.16874 * R - 0.33126 * G + 0.50000 * B  + MAXJSAMPLE/2
 *	Cr =  0.50000 * R - 0.41869 * G - 0.08131 * B  + MAXJSAMPLE/2
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times R,G,B for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The MAXJSAMPLE/2 offsets and the rounding fudge-factor of 0.5 are included
 * in the tables to save adding them separately in the inner loop.
 */

#define INT32		long
#define SCALEBITS	16	/* speediest right-shift on some machines */
#define ONE_HALF	((INT32) 1 << (SCALEBITS-1))
#define FIX(x)		((INT32) ((x) * (1L<<SCALEBITS) + 0.5))

/* We allocate one big table and divide it up into eight parts, instead of
 * doing eight alloc_small requests.  This lets us use a single table base
 * address, which can be held in a register in the inner loops on many
 * machines (more than can hold all eight addresses, anyway).
 */

#define R_Y_OFF		0			/* offset to R => Y section */
#define G_Y_OFF		(1*(256))	/* offset to G => Y section */
#define B_Y_OFF		(2*(256))	/* etc. */
#define R_CB_OFF	(3*(256))
#define G_CB_OFF	(4*(256))
#define B_CB_OFF	(5*(256))
#define R_CR_OFF	B_CB_OFF		/* B=>Cb, R=>Cr are the same */
#define G_CR_OFF	(6*(256))
#define B_CR_OFF	(7*(256))
#define TABLE_SIZE	(8*(256))


/*
 * Initialize for RGB->YCC colorspace conversion.
 */

INT32 *rgb_ycc_start()
{
  INT32 i;
  INT32 *rgb_ycc_tab = new INT32[TABLE_SIZE];
  for (i = 0; i <= 255; i++) {
    rgb_ycc_tab[i+R_Y_OFF] = FIX(0.29900) * i;
    rgb_ycc_tab[i+G_Y_OFF] = FIX(0.58700) * i;
    rgb_ycc_tab[i+B_Y_OFF] = FIX(0.11400) * i     + ONE_HALF;
    rgb_ycc_tab[i+R_CB_OFF] = (-FIX(0.16874)) * i;
    rgb_ycc_tab[i+G_CB_OFF] = (-FIX(0.33126)) * i;
    rgb_ycc_tab[i+B_CB_OFF] = FIX(0.50000) * i    + ONE_HALF*(256);
/*  B=>Cb and R=>Cr tables are the same
    rgb_ycc_tab[i+R_CR_OFF] = FIX(0.50000) * i    + ONE_HALF*(256);
*/
    rgb_ycc_tab[i+G_CR_OFF] = (-FIX(0.41869)) * i;
    rgb_ycc_tab[i+B_CR_OFF] = (-FIX(0.08131)) * i;
  }
  return rgb_ycc_tab;
}


/*
 * Convert some rows of samples to the JPEG colorspace.
 *
 */

void rgb_ycc(unsigned char ***components, int num_rows, int num_cols, int sm)
{
  register int r, g, b;
  register unsigned char *pg;
  register unsigned char *pr;
  register unsigned char *pb;
  register INT32 * ctab = rgb_ycc_start();

  int row;
  for (row = 0; row < num_rows; row++) {
    pg = components[0][row];
    pr = components[1][row];
    pb = components[2][row];
    for (int col = 0; col < num_cols; col++) {
      g = *pg;
      r = *pr;
      b = *pb;
      /* If the inputs are 0..MAXJSAMPLE, the outputs of these equations
       * must be too; we do not need an explicit range-limiting operation.
       * Hence the value being shifted is never negative.
       */
      /* Y */
      *pg++ = (unsigned char)
		((ctab[r+R_Y_OFF] + ctab[g+G_Y_OFF] + ctab[b+B_Y_OFF])
		 >> SCALEBITS);
      /* Cb */
      *pb++ = (unsigned char)
		((ctab[r+R_CB_OFF] + ctab[g+G_CB_OFF] + ctab[b+B_CB_OFF])
		 >> SCALEBITS);
      /* Cr */
      *pr++ = (unsigned char)
		((ctab[r+R_CR_OFF] + ctab[g+G_CR_OFF] + ctab[b+B_CR_OFF])
		 >> SCALEBITS);
    }
  }
  delete [] ctab;
  if (!sm) // No smoothing
	return;
  for (row = 0; row < num_rows; row += 2) {
    pr = components[1][row];
    register unsigned char *prr = components[1][row+1];
    pb = components[2][row];
    register unsigned char *pbb = components[2][row+1];
    for (int col = 0; col < num_cols; col += 2) {
      *pr = (*pr + *(pr+1) + *prr + *(prr+1) + 2)>>2;
      *pb = (*pb + *(pb+1) + *pbb + *(pbb+1) + 2)>>2;
      // Note, only put in top left corner of block, because only this
      // one is used.
      pr += 2;
      prr += 2;
      pb += 2;
      pbb += 2;
    }
  }
}


/**************** YCbCr -> RGB conversion: most common case **************/

/*
 * YCbCr is defined per CCIR 601-1, except that Cb and Cr are
 * normalized to the range 0..MAXJSAMPLE rather than -0.5 .. 0.5.
 * The conversion equations to be implemented are therefore
 *	R = Y                + 1.40200 * Cr
 *	G = Y - 0.34414 * Cb - 0.71414 * Cr
 *	B = Y + 1.77200 * Cb
 * where Cb and Cr represent the incoming values less MAXJSAMPLE/2.
 * (These numbers are derived from TIFF 6.0 section 21, dated 3-June-92.)
 *
 * To avoid floating-point arithmetic, we represent the fractional constants
 * as integers scaled up by 2^16 (about 4 digits precision); we have to divide
 * the products by 2^16, with appropriate rounding, to get the correct answer.
 * Notice that Y, being an integral input, does not contribute any fraction
 * so it need not participate in the rounding.
 *
 * For even more speed, we avoid doing any multiplications in the inner loop
 * by precalculating the constants times Cb and Cr for all possible values.
 * For 8-bit JSAMPLEs this is very reasonable (only 256 entries per table);
 * for 12-bit samples it is still acceptable.  It's not very reasonable for
 * 16-bit samples, but if you want lossless storage you shouldn't be changing
 * colorspace anyway.
 * The Cr=>R and Cb=>B values can be rounded to integers in advance; the
 * values for the G calculation are left scaled up, since we must add them
 * together before rounding.
 */

#define RIGHT_SHIFT(x,shft)	((x) >> (shft))

/*
 * Convert some rows of samples to the output colorspace.
 */

void ycc_rgb(unsigned char ***components, const int& num_rows, 
	const int& num_cols, const unsigned char *clip, const char& expand)
{
  INT32 i, x2;

  int *Cr_r_tab = new int[256];
  int *Cb_b_tab = new int[256];
  INT32 *Cr_g_tab = new INT32[256];
  INT32 *Cb_g_tab = new INT32[256];
  for (i = 0; i <= 255; i++) {
    /* i is the actual input pixel value, in the range 0..MAXJSAMPLE */
    /* The Cb or Cr value we are thinking of is x = i - MAXJSAMPLE/2 */
    x2 = 2*i - 255;	/* twice x */
    /* Cr=>R value is nearest int to 1.40200 * x */
    Cr_r_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.40200/2) * x2 + ONE_HALF, SCALEBITS);
    /* Cb=>B value is nearest int to 1.77200 * x */
    Cb_b_tab[i] = (int)
		    RIGHT_SHIFT(FIX(1.77200/2) * x2 + ONE_HALF, SCALEBITS);
    /* Cr=>G value is scaled-up -0.71414 * x */
    Cr_g_tab[i] = (- FIX(0.71414/2)) * x2;
    /* Cb=>G value is scaled-up -0.34414 * x */
    /* We also add in ONE_HALF so that need not do it in inner loop */
    Cb_g_tab[i] = (- FIX(0.34414/2)) * x2 + ONE_HALF;
  }
  register int cb, cr;
  register unsigned char *pcr;
  register unsigned char *pcb;
  register unsigned char *pcrplus;
  register unsigned char *pcbplus;
  if (expand != '0') {	// Only box filter implemented so far
    for (int row = 0; row < num_rows; row += 2) {
      pcr = components[1][row];
      pcb = components[2][row];
      pcrplus = components[1][row+1];
      pcbplus = components[2][row+1];
      for (register int col = 0; col < num_cols; col += 2) {
	cr = *pcr++;
	*pcr++ = cr;
	*pcrplus++ = cr; *pcrplus++ = cr;
	cb = *pcb++;
	*pcb++ = cb;
	*pcbplus++ = cb; *pcbplus++ = cb;
	}
    }
  }
  register int y;
  register unsigned char *py;
  for (int row = 0; row < num_rows; row++) {
    py = components[0][row];
    pcr = components[1][row];
    pcb = components[2][row];
    for (register int col = 0; col < num_cols; col++) {
      y = *py;
      cr = *pcr;
      cb = *pcb;
      *pcr++ = clip[y + Cr_r_tab[cr]];
      *py++ = clip[y + ((int) RIGHT_SHIFT(Cb_g_tab[cb] + Cr_g_tab[cr],
					SCALEBITS))];
      *pcb++ = clip[y + Cb_b_tab[cb]];
    }
  }
  delete[] Cb_g_tab;
  delete[] Cr_g_tab;
  delete[] Cb_b_tab;
  delete[] Cr_r_tab;
}

