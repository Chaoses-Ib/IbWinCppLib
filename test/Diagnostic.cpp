#include <catch.hpp>
#include <IbWinCpp/Diagnostic.hpp>
#include <iostream>

TEST_CASE("DebugOStream") {
	std::cout << "DebugOStream: need to be checked manually\n";

	{
		ib::DebugOStream dout;
		dout << L"** TEST MESSAGE ** " << 1 << std::endl;
	}
	{
		ib::DebugStreamBuf buf;
		std::wostream dout(&buf);
		dout << L"** TEST MESSAGE ** " << 2 << std::endl;
	}
	{
		using std::wcout;
		std::wstreambuf* wcout_buf = wcout.rdbuf();

		ib::DebugStreamBuf buf;
		wcout.rdbuf(&buf);
		wcout << L"** TEST MESSAGE ** " << 3 << std::endl;

		wcout.rdbuf(wcout_buf);
	}

	SECTION("test line prefix") {
		{
			ib::DebugOStream dout(L"Line prefix: ");
			dout << L"** TEST MESSAGE ** " << 1 << std::endl;
			dout << L"** TEST MESSAGE ** " << 2 << std::endl;
			dout << L"** TEST MESSAGE ** " << 3 << std::endl;
		}
		{
			ib::DebugOStream dout(std::wstring_view(L"Line prefix: "));
			dout << L"** TEST MESSAGE ** " << 4 << std::endl;
		}
		{
			ib::DebugOStream dout(std::wstring(L"Line prefix: "));
			dout << L"** TEST MESSAGE ** " << 5 << std::endl;
		}

		{
			ib::DebugStreamBuf buf(L"Line prefix: ");
			std::wostream dout(&buf);
			dout << L"** TEST MESSAGE ** " << 6 << std::endl;
		}
		{
			using std::wcout;
			std::wstreambuf* wcout_buf = wcout.rdbuf();

			ib::DebugStreamBuf buf(L"Line prefix: ");
			wcout.rdbuf(&buf);
			wcout << L"** TEST MESSAGE ** " << 7 << std::endl;

			wcout.rdbuf(wcout_buf);
		}
	}

	SECTION("test line prefix with a wrapper function") {
		auto DebugOStream = []() -> ib::DebugOStream<> {
			return { L"Line prefix: " };
		};
		DebugOStream() << L"** TEST MESSAGE ** " << 1 << std::endl;
	}
}