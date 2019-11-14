

void lzh_pack  (class IReadSeekFile & infile, class IWriteFile & outfile);  /* compression */
void lzh_unpack(class IReadFile     & infile, class IWriteFile & outfile);  /* recover */

void asc_pack  (class IReadFile & infile, class IWriteFile & outfile);
void asc_unpack(class IReadFile & infile, class IWriteFile & outfile);