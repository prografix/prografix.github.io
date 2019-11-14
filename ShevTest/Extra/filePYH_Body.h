
#pragma once

class Body;

class IWriteFile;

bool savePYH1 ( IWriteFile & file, const Body & body, const char * text = 0 );
bool savePYH2 ( IWriteFile & file, const Body & body, const char * text = 0 );

class IReadFile;

bool loadPYH1 ( IReadFile & file, Body & body, char * text = 0 ); // text[80]

// ������� loadPYH2 ����������:
// 0 - ������
// 1 - �������� ���� 1-�� ������
// 2 - �������� ���� 2-�� ������

nat loadPYH2 ( IReadFile & file, Body & body, char * text = 0 ); // text[80]

nat loadPYH2 ( const char * filename, Body & body, char * text = 0 ); // text[80]
