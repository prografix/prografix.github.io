
#pragma once

class ExtraBody;

class IWriteFile;

bool savePYH ( IWriteFile & file, const ExtraBody & body, bool writeBones, const char * text = 0 );

class IReadFile;

// ������� loadPYH2 ����������:
// 0 - ������
// 1 - �������� ���� 1-�� ������
// 2 - �������� ���� 2-�� ������

nat loadPYH ( IReadFile & file, ExtraBody & body, char * text = 0 ); // text[80]
