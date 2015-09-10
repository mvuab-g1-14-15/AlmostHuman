#ifndef __MSG_HANDLER_H__
#define __MSG_HANDLER_H__

class CMessageHandler
{
public:
  static void Assert( bool& aIgnoreAssert, const char* file, long line, const char* aAssertMsg, ... );
  static void FatalError( const char* file, long line, const char* aAssertMsg, ... );
};

#endif //__MSG_HANDLER_H__
