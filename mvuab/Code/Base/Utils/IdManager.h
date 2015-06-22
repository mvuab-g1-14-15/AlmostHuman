#ifndef ID_MANAGER_H_
#define ID_MANAGER_H_

#include <climits>

class CIdManager : public CManager
{
public:
    CIdManager() {};
    CIdManager( const CXMLTreeNode& atts ) : CManager( atts ) {};
    ~CIdManager()
    {
        mIds.clear();
    }

    void Init() {};
    void Update() {};
    void Render() {};

    unsigned int GetId( std::string type )
    {
        if ( mIds.find( type ) != mIds.end() )
        {
            unsigned int lId = ++mIds[type];

            if ( lId == UINT_MAX )
                mIds[type] = 0;

            return lId;
        }
        else
        {
            mIds[type] = 0;
            return 0;
        }
    }

private:
    std::map<std::string, unsigned int> mIds;
};

#endif //ID_MANAGER_H_