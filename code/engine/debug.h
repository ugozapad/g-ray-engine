#ifndef DEBUG_H
#define DEBUG_H

class Debug
{
public:
	static void Initialize();
	static void AssertionFailed(const char* expression, const char* file, int line, const char* function);
	static void AssertionFailed(const char* expression, const char* file, int line, const char* function, const char* message);
	static void AssertionFailedBackend(const char* buf);
};

#define ASSERT(EXPR) \
	if ( !(EXPR) ) \
		Debug::AssertionFailed(#EXPR, __FILE__, __LINE__, __FUNCTION__)

#define ASSERT_EX(EXPR, ...) \
	if ( !(EXPR) ) \
		Debug::AssertionFailed(#EXPR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#ifdef assert
#undef assert
#endif // assert

#define assert ASSERT

#endif // !DEBUG_H
