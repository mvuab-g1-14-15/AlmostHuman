#ifndef DEBUG_HELPER_H
#define DEBUG_HELPER_H

class CDebugHelper
{
public:
    CDebugHelper()
    {
    }

    ~CDebugHelper()
    {
        mMap.clear();
    }

    void Set(std::string aKey, std::string aValue)
    {
        mMap[aKey] = aValue;
    }

    void Update()
    {
        // Breakpoint here to check the map
        mMap.empty();
    }

private:
    std::map<std::string, std::string> mMap;
};

#endif // DEBUG_HELPER_H
