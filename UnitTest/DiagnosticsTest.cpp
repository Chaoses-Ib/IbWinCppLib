#include "pch.h"
#include "CppUnitTest.h"
#include "IbWinCppLib/WinCppLib.hpp"
#include <iostream>
#include <csignal>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ib;

namespace DiagnosticsTest
{
	TEST_CLASS(DebugOStreamTest)
	{
	public:
		TEST_METHOD(TestDebugOStream_CHECK)
		{
			{
				DebugOStream dout;
				dout << L"Hello Bug! " << 1 << std::endl;
			}
			{
				DebugStreamBuf buf;
				std::wostream dout(&buf);
				dout << L"Hello Bug! " << 2 << std::endl;
			}
			{
				using std::wcout;
				std::wstreambuf* wcout_buf = wcout.rdbuf();

				DebugStreamBuf buf;
				wcout.rdbuf(&buf);
				wcout << L"Hello Bug! " << 3 << std::endl;

				wcout.rdbuf(wcout_buf);
			}
			Logger::WriteMessage(L"#CHECK");
		}
	};
}