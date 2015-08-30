#pragma once

#include <string>
#include <vector>

template <class T> struct MapEntry
{
    unsigned int Key;
    T Value;
};

template <class T> class HashMap
{
    private:
        std::vector<MapEntry<T>> m_TableEntries;

    public:
        HashMap(void)
        {
             m_TableEntries.reserve(128);
        }

        ~HashMap(void)
        {
            m_TableEntries.clear();
        }

        std::vector<MapEntry<T>> &GetHahTable(void)
        {
            return m_TableEntries;
        }

        unsigned int GetKeyHash(const std::string &l_Key)
        {
            int l_HashKey = 0;

            for(unsigned int i = 0; i < l_Key.size(); ++i)
                l_HashKey += l_Key[i];

            return l_HashKey;
        }

        MapEntry<T> *find(const std::string &l_Key)
        {
            unsigned int l_HashKey = GetKeyHash(l_Key);

            for(unsigned int i = 0; i < m_TableEntries.size(); ++i)
            {
                if(l_HashKey == m_TableEntries[i].Key) return &m_TableEntries[i];
            }

            return NULL;
        }

        MapEntry<T> *find(unsigned int l_HashKey)
        {
            for(unsigned int i = 0; i < m_TableEntries.size(); ++i)
            {
                if(l_HashKey == m_TableEntries[i].Key) return &m_TableEntries[i];
            }

            return NULL;
        }

        bool add(const std::string &l_Key, const T &l_Value, bool l_Override = false)
        {
            unsigned int l_KeyHash = GetKeyHash(l_Key);
            MapEntry<T> *l_EntryFound = find(l_KeyHash);

            if(!l_Override && l_EntryFound != NULL)
            {
                return false;
            }

            if(l_EntryFound != NULL)
            {
                l_EntryFound->Value = l_Value;
                return true;
            }

            MapEntry<T> l_Entry = { l_KeyHash, l_Value };
            m_TableEntries.push_back(l_Entry);

            return true;
        }
};

