
#include "stdafx.h"
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "GraphicInterface.h"
#include "FileReader.h"

FileReader::FileReader ( const char * name ) : data(0), freeNode(0), s(0)
{
    int h = _open ( name, _O_RDONLY | _O_BINARY );
    if ( h == -1 )
    {
        AfxMessageBox("Can't to open file");
        return;
    }
    unsigned int flags;
    _read ( h, &flags, 4 );
    if ( flags > 1 && AfxMessageBox("Unknown flags. Continue?", MB_YESNO) != IDYES )
    {
        _close ( h );
        return;
    }
    unsigned int comment_size;
    _read ( h, &comment_size, 4 );
    _lseek ( h, comment_size, SEEK_CUR );
    unsigned int vers;
    _read ( h, &vers, 4 );
    if ( vers > 4 && AfxMessageBox("Unknown version. Continue?", MB_YESNO) != IDYES )
    {
        _close ( h );
        return;
    }
    unsigned int n = _filelength(h) - _tell(h);
    s = n >> 2;
    data = new Node[s];
    _read ( h, data, 4*s );
    freeNode = data + s;
    _close ( h );
}

FileReader::~FileReader ()
{
    delete[] data;
}

const GraphicInterface::Node * FileReader::getFirstNode() const { return data; }
const GraphicInterface::Node * FileReader::getFreeNode () const { return freeNode; }
