
#include <string.h>
#include "File.h"

/********** LZSS compression **********/

#define N		4096	/* buffer size */
#define F		60	/* lookahead buffer size */
#define THRESHOLD	2
#define NIL		N	/* leaf of tree */

class LZ_Tree
{
public:
    int16	match_position, match_length, lson[N + 1], rson[N + 257], dad[N + 1];
    
    LZ_Tree();
    void InsertNode(int16 r, nat8 * text_buf);
    void DeleteNode(int16 p);
};

LZ_Tree::LZ_Tree()  /* initialize trees */
{
	int16  i;

	for (i = N + 1; i <= N + 256; i++)
		rson[i] = NIL;			/* root */
	for (i = 0; i < N; i++)
		dad[i] = NIL;			/* node */
}

void LZ_Tree::InsertNode(int16 r, nat8 * text_buf)  /* insert to tree */
{
	int16  i, p, cmp;
	nat8 * key;
	nat16 c;

	cmp = 1;
	key = &text_buf[r];
	p = N + 1 + key[0];
	rson[r] = lson[r] = NIL;
	match_length = 0;
	for ( ; ; )
    {
		if (cmp >= 0)
        {
			if (rson[p] != NIL)
				p = rson[p];
			else
            {
				rson[p] = r;
				dad[r] = p;
				return;
			}
		}
        else
        {
			if (lson[p] != NIL)
				p = lson[p];
			else
            {
				lson[p] = r;
				dad[r] = p;
				return;
			}
		}
		for (i = 1; i < F; i++)
			if ((cmp = key[i] - text_buf[p + i]) != 0)
				break;
		if (i > THRESHOLD)
        {
			if (i > match_length)
            {
				match_position = ((r - p) & (N - 1)) - 1;
				if ((match_length = i) >= F)
					break;
			}
			if (i == match_length)
            {
				if ((c = ((r - p) & (N - 1)) - 1) < match_position)
                {
					match_position = c;
				}
			}
		}
	}
	dad[r] = dad[p];
	lson[r] = lson[p];
	rson[r] = rson[p];
	dad[lson[p]] = r;
	dad[rson[p]] = r;
	if (rson[dad[p]] == p)
		rson[dad[p]] = r;
	else
		lson[dad[p]] = r;
	dad[p] = NIL;  /* remove p */
}

void LZ_Tree::DeleteNode(int16 p)  /* remove from tree */
{
	int16  q;

	if (dad[p] == NIL)
		return;			/* not registered */
	if (rson[p] == NIL)
		q = lson[p];
	else
	if (lson[p] == NIL)
		q = rson[p];
	else
    {
		q = lson[p];
		if (rson[q] != NIL)
        {
			do
            {
				q = rson[q];
			}
            while (rson[q] != NIL);
			rson[dad[q]] = lson[q];
			dad[lson[q]] = dad[q];
			lson[q] = lson[p];
			dad[lson[p]] = q;
		}
		rson[q] = rson[p];
		dad[rson[p]] = q;
	}
	dad[q] = dad[p];
	if (rson[dad[p]] == p)
		rson[dad[p]] = q;
	else
		lson[dad[p]] = q;
	dad[p] = NIL;
}

////////////////////////////////// Huffman coding /////////////////////////////

#define N_CHAR  	(256 - THRESHOLD + F)
				/* kinds of characters (character code = 0..N_CHAR-1) */
#define T 		(N_CHAR * 2 - 1)	/* size of table */
#define R 		(T - 1)			/* position of root */
#define MAX_FREQ	0x8000		/* updates tree when the root frequency comes to this value. */

/* table for encoding and decoding the upper 6 bits of position */

/* for encoding */
const nat8 p_len[64] = {
	0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08
};

const nat8 p_code[64] = {
	0x00, 0x20, 0x30, 0x40, 0x50, 0x58, 0x60, 0x68,
	0x70, 0x78, 0x80, 0x88, 0x90, 0x94, 0x98, 0x9C,
	0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
	0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
	0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
	0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

/* for decoding */
const nat8 d_code[256] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
	0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
	0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
	0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
	0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
	0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
	0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
	0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
	0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
	0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
	0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
	0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
	0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
	0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
	0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

const nat8 d_len[256] = {
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
};

class HuffmanTree
{
    nat16 freq[T + 1];	/* frequency table */
public:
    int16 prnt[T + N_CHAR];	/* pointers to parent nodes, except for the */
			    /* elements [T..T + N_CHAR - 1] which are used to get */
			    /* the positions of leaves corresponding to the codes. */

    int16 son[T];		/* pointers to child nodes (son[], son[] + 1) */
private:
    void reconst();
public:
    HuffmanTree();
    void update(int16 c);
};

/* initialization of tree */

HuffmanTree::HuffmanTree()
{
	int16 i, j;

	for (i = 0; i < N_CHAR; i++)
    {
		freq[i] = 1;
		son[i] = i + T;
		prnt[i + T] = i;
	}
	i = 0; j = N_CHAR;
	while (j <= R)
    {
		freq[j] = freq[i] + freq[i + 1];
		son[j] = i;
		prnt[i] = prnt[i + 1] = j;
		i += 2; j++;
	}
	freq[T] = 0xffff;
	prnt[R] = 0;
}

/* reconstruction of tree */

void HuffmanTree::reconst()
{
	int16 i, j, k;
	nat16 f, l;

	/* collect leaf nodes in the first half of the table */
	/* and replace the freq by (freq + 1) / 2. */
	j = 0;
	for (i = 0; i < T; i++)
    {
		if (son[i] >= T)
        {
			freq[j] = (freq[i] + 1) / 2;
			son[j] = son[i];
			j++;
		}
	}
	/* begin constructing tree by connecting sons */
	for (i = 0, j = N_CHAR; j < T; i += 2, j++)
    {
		k = i + 1;
		f = freq[j] = freq[i] + freq[k];
		for (k = j - 1; f < freq[k]; k--);
		k++;
		l = (j - k) * 2;
		memmove(&freq[k + 1], &freq[k], l);
		freq[k] = f;
		memmove(&son[k + 1], &son[k], l);
		son[k] = i;
	}
	/* connect prnt */
	for (i = 0; i < T; i++)
    {
		if ((k = son[i]) >= T) prnt[k] = i;
		else
			prnt[k] = prnt[k + 1] = i;
	}
}

/* increment frequency of given code by one, and update tree */

void HuffmanTree::update(int16 c)
{
	int16 i, j, k, l;

	if (freq[R] == MAX_FREQ) reconst();
	c = prnt[c + T];
	do
    {
		k = ++freq[c];
		/* if the order is disturbed, exchange nodes */
		if (k > freq[l = c + 1])
        {
			while (k > freq[++l]);
			l--;
			freq[c] = freq[l];
			freq[l] = k;

			i = son[c];
			prnt[i] = l;
			if (i < T) prnt[i + 1] = l;

			j = son[l];
			son[l] = i;

			prnt[j] = c;
			if (j < T) prnt[j + 1] = c;
			son[c] = j;

			c = l;
		}
	}
    while ((c = prnt[c]) != 0);	/* repeat up to root */
}

////////////////////////////////// Encode /////////////////////////////

class LZH_Encoder
{
    nat16 putbuf;
    nat8 putlen;
    IWriteFile & outfile;
	HuffmanTree huffmanTree;
    
    void putCode(nat16 l, nat16 c);
public:
    LZH_Encoder(IWriteFile & file) : outfile(file)
    {
        putbuf = 0;
        putlen = 0;
    }
    void putChar(nat16 c);
    void putPosition(nat16 c);
    void end();
};

void LZH_Encoder::putCode(nat16 l, nat16 c)		/* output c bits of code */
{
	putbuf |= c >> putlen;
	if ((putlen += l) >= 8)
    {
        nat16 i = putbuf >> 8;
		outfile.putc(&i);
		if ((putlen -= 8) >= 8)
        {
		    outfile.putc(&putbuf);
			putlen -= 8;
			putbuf = c << (l - putlen);
		}
        else
        {
			putbuf <<= 8;
		}
	}
}

void LZH_Encoder::putChar(nat16 c)
{
	nat16 i = 0;
	nat16 j = 0;
	int16 k = huffmanTree.prnt[c + T];

	/* travel from leaf to root */
	do
    {
		i >>= 1;
		/* if node's address is odd-numbered, choose bigger brother node */
		if (k & 1) i += 0x8000;
		j++;
	}
    while ((k = huffmanTree.prnt[k]) != R);
	putCode(j, i);
	huffmanTree.update(c);
}

void LZH_Encoder::putPosition(nat16 c)
{
	nat16 i;

	/* output upper 6 bits by table lookup */
	i = c >> 6;
	putCode(p_len[i], (nat16)p_code[i] << 8);

	/* output lower 6 bits verbatim */
	putCode(6, (c & 0x3f) << 10);
}

void LZH_Encoder::end()
{
	if (putlen)
    {
        nat16 i = putbuf >> 8;
		outfile.putc(&i);
	}
}

/* compression */

void lzh_pack(IReadSeekFile & infile, IWriteFile & outfile)  /* compression */
{
    nat8 c = 0;
	int16 i, len, r, s, last_match_length;
    nat8 text_buf[N + F - 1];

	infile.seek_end(0);
	unsigned long textsize = infile.tell();
	outfile.write(&textsize, 4, 1);
	if (textsize == 0) return;
    LZH_Encoder encoder(outfile);
	infile.rewind();
	textsize = 0;
	LZ_Tree lz_tree;
	s = 0;
	r = N - F;
	for (i = s; i < r; i++)
		text_buf[i] = ' ';
	for (len = 0; len < F && infile.getc(&c); len++)
		text_buf[r + len] = c;
	textsize = len;
	for (i = 1; i <= F; i++)
		lz_tree.InsertNode(r - i, text_buf);
	lz_tree.InsertNode(r, text_buf);
	do
    {
		if (lz_tree.match_length > len)
			lz_tree.match_length = len;
		if (lz_tree.match_length <= THRESHOLD)
        {
			lz_tree.match_length = 1;
			encoder.putChar(text_buf[r]);
		}
        else
        {
			encoder.putChar(255 - THRESHOLD + lz_tree.match_length);
			encoder.putPosition(lz_tree.match_position);
		}
		last_match_length = lz_tree.match_length;
		for (i = 0; i < last_match_length && infile.getc(&c); i++)
        {
			lz_tree.DeleteNode(s);
			text_buf[s] = c;
			if (s < F - 1)
				text_buf[s + N] = c;
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
			lz_tree.InsertNode(r, text_buf);
		}
		while (i++ < last_match_length)
        {
			lz_tree.DeleteNode(s);
			s = (s + 1) & (N - 1);
			r = (r + 1) & (N - 1);
			if (--len) lz_tree.InsertNode(r, text_buf);
		}
	}
    while (len > 0);
	encoder.end();
}

////////////////////////////////// Decode /////////////////////////////

class LZH_Decoder
{
    nat16 getbuf;
    nat8 getlen;
    IReadFile & infile;
	HuffmanTree huffmanTree;

    nat16 GetBit ();
    nat16 GetByte();
public:
    LZH_Decoder ( IReadFile & file ) : infile(file)
    {
        getbuf = 0;
        getlen = 0;
    }
    nat16 getChar();
    nat16 getPosition();
};

nat16 LZH_Decoder::GetBit()	/* get one bit */
{
	int16 i = 0;

	while (getlen <= 8)
    {
		if (infile.getc(&i) == false) i = 0;
		getbuf |= i << (8 - getlen);
		getlen += 8;
	}
	i = getbuf;
	getbuf <<= 1;
	getlen--;
	return (i < 0);
}

nat16 LZH_Decoder::GetByte()	/* get one byte */
{
	nat16 i = 0;

	while (getlen <= 8)
    {
		if (infile.getc(&i) == false) i = 0;
		getbuf |= i << (8 - getlen);
		getlen += 8;
	}
	i = getbuf;
	getbuf <<= 8;
	getlen -= 8;
	return i >> 8;
}

nat16 LZH_Decoder::getChar()
{
	nat16 c;

	c = huffmanTree.son[R];

	/* travel from root to leaf, */
	/* choosing the smaller child node (son[]) if the read bit is 0, */
	/* the bigger (son[]+1} if 1 */
	while (c < T)
    {
		c += GetBit();
		c = huffmanTree.son[c];
	}
	c -= T;
	huffmanTree.update(c);
	return c;
}

nat16 LZH_Decoder::getPosition()
{
	nat16 i, j, c;

	/* recover upper 6 bits from table */
	i = GetByte();
	c = (nat16)d_code[i] << 6;
	j = d_len[i];

	/* read lower 6 bits verbatim */
	j -= 2;
	while (j--)
    {
		i = (i << 1) + GetBit();
	}
	return c | (i & 0x3f);
}

void lzh_unpack(IReadFile & infile, IWriteFile & outfile)  /* recover */
{
	int16  i, j, k, r, c;
	nat32  count, textsize = 0;
    nat8 text_buf[N + F - 1];

	infile.read(&textsize, 4, 1);
	if (textsize == 0) return;
	for ( i = 0; i < N - F; i++ ) text_buf[i] = ' ';
	r = N - F;
    LZH_Decoder decoder(infile);
	for ( count = 0; count < textsize; )
    {
		c = decoder.getChar();
		if (c < 256)
        {
			outfile.putc(&c);
			text_buf[r++] = nat8 ( c );
			r &= (N - 1);
			count++;
		}
        else
        {
			i = (r - decoder.getPosition() - 1) & (N - 1);
			j = c - 255 + THRESHOLD;
			for (k = 0; k < j; k++)
            {
				c = text_buf[(i + k) & (N - 1)];
			    outfile.putc(&c);
				text_buf[r++] = nat8 ( c );
				r &= (N - 1);
				count++;
			}
		}
	}
}
