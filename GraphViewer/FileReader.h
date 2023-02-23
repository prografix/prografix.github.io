
class FileReader : public GraphicInterface
{
    FileReader ( const FileReader & );
    void operator = ( const FileReader & );
// Data
    int s;
    Node * data, * freeNode;
//
    virtual const Node * getFirstNode() const;
    virtual const Node * getFreeNode () const;
public:
    FileReader ( const char * name );
    ~FileReader ();
    int size() { return s; }
    bool operator >> ( GraphicInterface & gi ) const
    {
        return GraphicInterface::operator >> ( gi );
    }
};