#pragma once

class CCrashDump
{
private:
	static long _DumpCount;

private:
	CCrashDump() {}
	~CCrashDump() {}

public:
	static void Init();
	static void Crash();

	static LONG WINAPI MyExceptionFilter(__in PEXCEPTION_POINTERS pExceptionPointer);

	static void SetHandlerDump();

	static void myInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
	static int _custom_Report_hook(int ireposttype, char* message, int* returnvalue);
	static void myPurecallHandler(void);

};
