
// ����������� ����������� � ����������� ����

bool cbtpc ( int nx, int ny, const unsigned char * data,
             int bpp, int maxval, int quality, class IWriteFile & file );

// �������������� ����������� �� ������������ �����

bool dbtpc ( class IReadSeekFile & file, int & nx, int & ny, unsigned char * & data, int & bpp );