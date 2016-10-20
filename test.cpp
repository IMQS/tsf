#include <assert.h>
#include "tsf.h"

using namespace tsf;

template<char EscapeA, char EscapeB>
size_t my_escape_gen(char* outBuf, size_t outBufSize, const fmtarg& val)
{
	if (val.Type != fmtarg::TCStr)
		return 0;

	size_t len = strlen(val.CStr);
	if (outBufSize < (size_t) len + 2)
		return -1;

	outBuf[0] = EscapeA;
	strcpy(outBuf + 1, val.CStr);
	outBuf[len + 1] = EscapeB;

	return len + 2;
}

static size_t my_escape_q(char* outBuf, size_t outBufSize, const fmtarg& val)
{
	return my_escape_gen<'\'', '\''>(outBuf, outBufSize, val);
}

static size_t my_escape_Q(char* outBuf, size_t outBufSize, const fmtarg& val)
{
	return my_escape_gen<'[', ']'>(outBuf, outBufSize, val);
}

int main(int argc, char** argv)
{
	assert(fmt("%*s", "abc") == "abc"); // ignore asterisk
	assert(fmt("xyz") == "xyz");
	assert(fmt("%5s", "abc") == "  abc");
	assert(fmt("%-5v", "abc") == "abc  ");
	assert(fmt("%5v", "abc") == "  abc");
	assert(fmt("%-5s", "abc") == "abc  ");
	assert(fmt("%%%") == "%");
	assert(fmt("%s %d", "abc", 123) == "abc 123");
	assert(fmt("%v %v", "abc", 123) == "abc 123");
	assert(fmt("%v %v %5v", "abc", 123) == "abc 123 %5v");
	assert(fmt("%v %v %%", "abc", 123) == "abc 123 %");
	assert(fmt("%% %v %v", "abc", 123) == "% abc 123");
	assert(fmt("%v %v", 1.2, 1.3) == "1.2 1.3");
	assert(fmt("%s %v", L"abc", L"xyz") == "abc xyz");
	assert(fmt("%s", L"abc") == "abc");
	assert(fmt("%S", L"abc") == "abc");
	assert(fmt("%s", "abc") == "abc");
	assert(fmt("%S", "abc") == "abc");
	assert(fmt("%ls", "abc") == "abc");
	assert(fmt("%lS", "abc") == "abc");
	assert(fmt("%ls", L"abc") == "abc");
	assert(fmt("%lS", L"abc") == "abc");
	assert(fmt("%6g", -5.5) == "  -5.5");
	assert(fmt("%-6g", -5.5) == "-5.5  ");
	assert(fmt("%20v", 5.5) == "                 5.5");
	assert(fmt("%20f", 5.5) == "            5.500000");
	assert(fmt("%v", (int64_t) 55) == "55");
	assert(fmt("%d", (int64_t) 55) == "55");
	assert(fmt("%u", (int64_t) 55) == "55");
	assert(fmt("%f", (int64_t) 55) == "55");
	assert(fmt("%f", 55) == "55");
	assert(fmt("%x", 0xdeadbeef) == "deadbeef");
	assert(fmt("%X", 0xdeadbeef) == "DEADBEEF");
	assert(fmt("%x", 0xdeadbeefd00df00d) == "deadbeefd00df00d");
	assert(fmt("%d", (int64_t) 9223372036854775807) == "9223372036854775807");
	assert(fmt("%d", (uint64_t) 9223372036854775807) == "9223372036854775807");
	assert(fmt("%v", INT32_MIN) == "-2147483648");
	assert(fmt("%v", UINT32_MAX) == "4294967295");
	assert(fmt("%v", INT64_MIN) == "-9223372036854775808");
	assert(fmt("%v", UINT64_MAX) == "18446744073709551615");
	assert(fmt("%s", std::string("")) == "");
	assert(fmt("%s", std::wstring(L"")) == "");
	assert(fmt("%s", std::string("abc")) == "abc");
	assert(fmt("%S", std::string("abc")) == "abc");
	assert(fmt("%s", std::wstring(L"abc")) == "abc");
	assert(fmt("%S", std::wstring(L"abc")) == "abc");
	fmt_print("Hello!\n", 1);
	fmt_print("Hello! %v\n", "abc");
	fmt_write(stderr, "Hello! %v\n", 123);

	{
		assert(fmt("%q %d", "Hello", 1) == "%q Hello");     // I'm unsure about whether we should consume the "Hello" on the %q or not.
		assert(fmt("%Q %d", "Hello", 2) == "%Q Hello");

		fmt_context cx;
		cx.Escape_q = my_escape_q;
		fmtarg args[2] = {"Hello", 5};
		fmt_core(cx, "%q %v", 2, args);
		assert(fmt_core(cx, "%q %v", 2, args) == "'Hello' 5");

		cx.Escape_Q = my_escape_Q;
		fmtarg argsQ[3] = {"Hello", "Hello", 5};
		assert(fmt_core(cx, "%q %Q %v", 3, argsQ) == "'Hello' [Hello] 5");
	}
	{
		char buf[10];
		CharLenPair res = fmt_static_buf(buf, 10, "%d", 123);
		assert(strcmp(res.Str, "123") == 0 && buf == res.Str && res.Len == 3);

		res = fmt_static_buf(buf, 10, "too long to fit %d", 123);
		assert(strcmp(res.Str, "too long to fit 123") == 0 && buf != res.Str && res.Len == 19);
		delete[] res.Str;
	}
	{
		assert(fmt("%0000000000000d", 5) == "5"); // valid
		assert(fmt("%00000000000000d", 5) == "%00000000000000d"); // invalid - too long
	}
	{
		for (int len = 10; len < 1000; len++)
		{
			char* longstr = new char[len];
			memset(longstr, 'a', len);
			longstr[len - 1] = 0;
			longstr[0] = '%';
			longstr[1] = 'd';
			auto res = fmt(longstr, 5);
			assert(res.length() == len - 2);
			assert(res[0] == '5');
			assert(res[1] == 'a');
			assert(res[len - 3] == 'a');
			delete[] longstr;
		}
	}
	return 0;
}