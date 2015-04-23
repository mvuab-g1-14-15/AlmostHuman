///////////////////////////////////////////////////////////////////////////////
// 
// ObjectFactory
// 
// The ObjectFactory class is a object factory implementation.  It allows users
// to register and unregister classes during run-time by specifying a
// user-defined unique identifier per class.  Instances of any registered class
// can then be instantiated simply by calling the create method and passing the
// proper unique identifier.
// 
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2004 Robert Geiman.
//
// Permission to copy, modify, and use this code for personal and commercial
// software is granted provided this copyright notice appears in all copies.
// This software is provided "as is" without any expressed or implied warranty.
//
// Any comments or questions can be sent to: rgeiman@buckeye-express.com
//
///////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H


///////////////////////////////////////////////////////////////////////////////
// This is a pragma ignores the many warnings Visual C++ 6.0 gives when using
// the STL map container.
///////////////////////////////////////////////////////////////////////////////
#pragma warning (disable: 4786)


#include <map>
#include "MacroRepeat.h"


///////////////////////////////////////////////////////////////////////////////
// This is a work-around for those non-compliant compilers that do not support
// 'explicit template argument specification' for member functions, such as
// Visual C++ 6.0.  It's not pretty syntax for the user, but at least it works.
///////////////////////////////////////////////////////////////////////////////
template<typename Type>
struct Type2Type
{
   typedef Type OriginalType;
};


///////////////////////////////////////////////////////////////////////////////
// Because some compilers, such as Visual C++ 6.0, do not support partial
// template specialization we must create seperate ObjectFactory classes, one
// for each constructor parameter we allow the user to use.
///////////////////////////////////////////////////////////////////////////////
#define OBJECT_FACTORY(num)                                                                                 \
   template<typename BaseClassType MACRO_LIST_APPEND(num, MACRO_TEMPLATE_PARAMETER), typename UniqueIdType> \
   class ObjectFactory##num                                                                                 \
   {                                                                                                        \
   protected:                                                                                               \
      class CreateObjectBase                                                                                \
      {                                                                                                     \
      public:                                                                                               \
         virtual ~CreateObjectBase() {}                                                                     \
         virtual BaseClassType *operator()(MACRO_LIST(num, MACRO_FUNCTION_PARAMETER)) = 0;                  \
      };                                                                                                    \
                                                                                                            \
      template<typename ClassType>                                                                          \
      class CreateObject : public CreateObjectBase                                                          \
      {                                                                                                     \
      public:                                                                                               \
         BaseClassType *operator()(MACRO_LIST(num, MACRO_FUNCTION_PARAMETER))                               \
         {                                                                                                  \
			return new ClassType MACRO_BEGIN_PAREN(num, MACRO_EMPTY_MACRO) MACRO_LIST(num, MACRO_FUNCTION_ARGUMENT) MACRO_END_PAREN(num, MACRO_EMPTY_MACRO); \
         }                                                                                                  \
      };                                                                                                    \
                                                                                                            \
   public:                                                                                                  \
      typedef typename std::map<UniqueIdType, CreateObjectBase*>::const_iterator ConstIterator;             \
      typedef typename std::map<UniqueIdType, CreateObjectBase*>::iterator Iterator;                        \
                                                                                                            \
      ~ObjectFactory##num()                                                                                 \
      {                                                                                                     \
         Iterator iter = m_object_creator.begin();                                                          \
                                                                                                            \
         while (iter != m_object_creator.end())                                                             \
         {                                                                                                  \
            delete (*iter).second;                                                                          \
            ++iter;                                                                                         \
         }                                                                                                  \
      }                                                                                                     \
                                                                                                            \
      template<typename ClassType>                                                                          \
      bool Register(UniqueIdType unique_id, Type2Type<ClassType>)                                           \
      {                                                                                                     \
         if (m_object_creator.find(unique_id) != m_object_creator.end())                                    \
            return false;                                                                                   \
                                                                                                            \
         m_object_creator[unique_id] = new CreateObject<ClassType>;                                         \
                                                                                                            \
         return true;                                                                                       \
      }                                                                                                     \
                                                                                                            \
      bool Unregister(UniqueIdType unique_id)                                                               \
      {                                                                                                     \
         Iterator iter = m_object_creator.find(unique_id);                                                  \
                                                                                                            \
         if (iter != m_object_creator.end())                                                                \
         {                                                                                                  \
            delete (*iter).second;                                                                          \
            m_object_creator.erase(iter);                                                                   \
                                                                                                            \
            return true;                                                                                    \
         }                                                                                                  \
                                                                                                            \
         return false;                                                                                      \
      }                                                                                                     \
                                                                                                            \
      BaseClassType *Create(UniqueIdType unique_id MACRO_LIST_APPEND(num, MACRO_FUNCTION_PARAMETER))        \
      {                                                                                                     \
         Iterator iter = m_object_creator.find(unique_id);                                                  \
                                                                                                            \
         if (iter == m_object_creator.end())                                                                \
            return NULL;                                                                                    \
                                                                                                            \
         return ((*iter).second)->operator()(MACRO_LIST(num, MACRO_FUNCTION_ARGUMENT));                     \
      }                                                                                                     \
                                                                                                            \
      ConstIterator GetBegin() const                                                                        \
      {                                                                                                     \
         return m_object_creator.begin();                                                                   \
      }                                                                                                     \
                                                                                                            \
      Iterator GetBegin()                                                                                   \
      {                                                                                                     \
         return m_object_creator.begin();                                                                   \
      }                                                                                                     \
                                                                                                            \
      ConstIterator GetEnd() const                                                                          \
      {                                                                                                     \
         return m_object_creator.end();                                                                     \
      }                                                                                                     \
                                                                                                            \
      Iterator GetEnd()                                                                                     \
      {                                                                                                     \
         return m_object_creator.end();                                                                     \
      }                                                                                                     \
                                                                                                            \
   protected:                                                                                               \
      std::map<UniqueIdType, CreateObjectBase*> m_object_creator;                                           \
   };                                                                                                       

MACRO_REPEAT(16, OBJECT_FACTORY)
#undef OBJECT_FACTORY

#endif