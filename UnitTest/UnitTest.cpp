#include "pch.h"
#include "CppUnitTest.h"
#include "../WinCppLib/include/WinCppLib.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ib;

namespace MemoryTest
{
	TEST_CLASS(AddrTest)
	{
	public:
		TEST_METHOD(TestArithmetic)
		{
			Addr a = (int*)0x10000000;
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)(a + 3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset(3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset<Byte>(3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset<Byte[3]>());

			a += 6;
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)(a - 3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset(-3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset<Byte>(-3));
			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)a.offset<Byte[3]>(-1));
			a -= 6;
			Assert::AreEqual<uintptr_t>(0x10000000, (uintptr_t)a);

			Addr b = 0x10000003;
			Assert::AreEqual<Offset>(3, b - a);

			Assert::AreEqual<uintptr_t>(0x10000003, (uintptr_t)b.align_up(1));
			Assert::AreEqual<uintptr_t>(0x10000004, (uintptr_t)b.align_up(4));
			Assert::AreEqual<uintptr_t>(0x10000010, (uintptr_t)b.align_up(16));
			Assert::AreEqual<uintptr_t>(0x10000000, (uintptr_t)b.align_down(4));
			b = 0x10000004;
			Assert::AreEqual<uintptr_t>(0x10000004, (uintptr_t)b.align_up(4));
			Assert::AreEqual<uintptr_t>(0x10000004, (uintptr_t)b.align_down(4));
		}

		TEST_METHOD(TestConversion)
		{
			Addr a = (int*)0x10000001;
			Assert::AreEqual((Byte*)0x10000001, a.p);
			Assert::AreEqual((float*)0x10000001, a.ptr<float>());

			Assert::AreEqual<bool>(true, a);
			a = nullptr;
			Assert::AreEqual<bool>(false, a);
			a = 0x10000001;

			Assert::AreEqual(true, a == (int*)0x10000001);
			Assert::AreEqual(true, a < (int*)0x10000002);
			Assert::AreEqual(true, a <= (int*)0x10000001);
			Assert::AreEqual(true, a > (int*)0x10000000);
			Assert::AreEqual(true, a >= (int*)0x10000001);

			Assert::AreEqual(false, a != (int*)0x10000001);
			Assert::AreEqual(false, a >= (int*)0x10000002);
			Assert::AreEqual(false, a > (int*)0x10000001);
			Assert::AreEqual(false, a <= (int*)0x10000000);
			Assert::AreEqual(false, a < (int*)0x10000001);
		}

		TEST_METHOD(TestReadWrite)
		{
			int a = 123;
			Addr p = &a;

			Assert::AreEqual(123, p.read<int>());
			Assert::AreEqual(123, (int)p.read());

			p.write<int>(456);
			Assert::AreEqual(456, p.read<int>());

			uintptr_t b[2] = { 0, (uintptr_t)&a };
			p = &b;
			Assert::AreEqual<void*>(p[0], nullptr);
			Assert::AreEqual<void*>(p[sizeof uintptr_t], &a);
		}
	};
}
