
#pragma once

class ExtraBody;

class IWriteFile;

bool savePYH ( IWriteFile & file, const ExtraBody & body, bool writeBones, const char * text = 0 );

class IReadFile;

// Функция loadPYH2 возвращает:
// 0 - ошибка
// 1 - прочитан файл 1-ой версии
// 2 - прочитан файл 2-ой версии

nat loadPYH ( IReadFile & file, ExtraBody & body, char * text = 0 ); // text[80]
