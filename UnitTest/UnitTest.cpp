#include "pch.h"
#include "CppUnitTest.h"
#include "IbWinCppLib/WinCppLib.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ib;
using namespace std::literals;

namespace CommonTest
{
	TEST_CLASS(HolderTest)
	{
	public:
		struct RaiiClass {
			static inline int count = 0;

			RaiiClass() { count++; }
			RaiiClass(int count) { this->count = count; }
			void inc() { count++; }
			void dec() { count--; }
			~RaiiClass() { count--; }
		};

		TEST_METHOD(TestHold)
		{
			{
				Holder<RaiiClass> obj;
				Assert::AreEqual(0, obj->count);

				obj.create();
				Assert::AreEqual(1, obj->count);

				obj->inc();
				Assert::AreEqual(2, obj->count);

				obj.recreate();
				Assert::AreEqual(2, obj->count);

				obj.destroy();
				Assert::AreEqual(1, obj->count);
			}
			Assert::AreEqual(1, RaiiClass::count);
			RaiiClass::count = 0;
		}

		TEST_METHOD(TestCreate)
		{
			{
				Holder<RaiiClass> obj(123);
				Assert::AreEqual(123, obj->count);

				obj.recreate(456);
				Assert::AreEqual(456, obj->count);
			}
			RaiiClass::count = 0;
			{
				Holder<RaiiClass> obj;
				Assert::AreEqual(0, obj->count);

				obj.create(789);
				Assert::AreEqual(789, obj->count);
			}
			RaiiClass::count = 0;
			{
				Holder<RaiiClass> obj(Holder<void>::Default);
				Assert::AreEqual(1, obj->count);
			}
			RaiiClass::count = 0;
		}
	};

	TEST_CLASS(HolderBTest)
	{
	public:
		struct RaiiClass {
			static inline int count = 0;

			RaiiClass() { count++; }
			RaiiClass(int count) { this->count = count; }
			void inc() { count++; }
			void dec() { count--; }
			~RaiiClass() { count--; }
		};

		TEST_METHOD(TestHoldB)
		{
			{
				HolderB<RaiiClass> obj;
				Assert::AreEqual(0, obj->count);
				Assert::AreEqual(false, obj.has_created());

				obj.create();
				Assert::AreEqual(1, obj->count);
				Assert::AreEqual(true, obj.has_created());

				obj->inc();
				Assert::AreEqual(2, obj->count);

				obj.recreate();
				Assert::AreEqual(2, obj->count);
				Assert::AreEqual(true, obj.has_created());

				obj.destroy();
				Assert::AreEqual(1, obj->count);
				Assert::AreEqual(false, obj.has_created());
			}
			Assert::AreEqual(1, RaiiClass::count);
			RaiiClass::count = 0;
		}

		TEST_METHOD(TestCreate)
		{
			{
				HolderB<RaiiClass> obj(123);
				Assert::AreEqual(123, obj->count);
				Assert::AreEqual(true, obj.has_created());

				obj.recreate(456);
				Assert::AreEqual(456, obj->count);
			}
			RaiiClass::count = 0;
			{
				HolderB<RaiiClass> obj;
				Assert::AreEqual(0, obj->count);

				obj.create(789);
				Assert::AreEqual(789, obj->count);
			}
			RaiiClass::count = 0;
			{
				HolderB<RaiiClass> obj(Holder<void>::Default);
				Assert::AreEqual(1, obj->count);
				Assert::AreEqual(true, obj.has_created());
			}
			RaiiClass::count = 0;
		}
	};
}

namespace FileSystemsTest
{
	TEST_CLASS(PathTest)
	{
	public:
		TEST_METHOD(TestRealPath)
		{
			Assert::AreEqual(LR"(C:\Users)"s, get_realpath(LR"(C:\Documents and Settings)"));
			Assert::AreEqual(LR"(C:\Users)"s, get_realpath(LR"(C:\Documents and Settings\)"));
			Assert::AreEqual(LR"(\\?\C:\Users)"s, get_realpath(LR"(C:\Documents and Settings)", false));

			//junctions
			Assert::AreEqual(LR"(C:\Users\desktop.ini)"s, get_realpath(LR"(C:\Documents and Settings\desktop.ini)"));
			Assert::AreEqual(LR"(C:\Users\Default)"s, get_realpath(LR"(C:\Documents and Settings\Default User)"));

			//symbolic links
			Assert::AreEqual(LR"(C:\Users\Public\Desktop\desktop.ini)"s, get_realpath(LR"(C:\Documents and Settings\All Users\Desktop\desktop.ini)"));
		}
	};
}

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

	TEST_CLASS(ProtectTest)
	{
	public:
		TEST_METHOD(TestToProtect)
		{
			using namespace mem;
			Assert::AreEqual(Protect::NoAccess, Protect::to_protect(PAGE_NOACCESS));
			Assert::AreEqual(Protect::Read, Protect::to_protect(PAGE_READONLY));
			Assert::AreEqual(Protect::Write, Protect::to_protect(PAGE_READWRITE));
			Assert::AreEqual(Protect::WriteCopy, Protect::to_protect(PAGE_WRITECOPY));

			Assert::AreEqual(Protect::Execute, Protect::to_protect(PAGE_EXECUTE));
			Assert::AreEqual(Protect::Execute | Protect::Read, Protect::to_protect(PAGE_EXECUTE_READ));
			Assert::AreEqual(Protect::Execute | Protect::Write, Protect::to_protect(PAGE_EXECUTE_READWRITE));
			Assert::AreEqual(Protect::Execute | Protect::WriteCopy, Protect::to_protect(PAGE_EXECUTE_WRITECOPY));

			Assert::AreEqual(Protect::Execute | Protect::Guard, Protect::to_protect(PAGE_EXECUTE | PAGE_GUARD));
			Assert::AreEqual(Protect::Execute | Protect::Read | Protect::NoCache, Protect::to_protect(PAGE_EXECUTE_READ | PAGE_NOCACHE));
			Assert::AreEqual(Protect::Execute | Protect::Write | Protect::WriteCombine, Protect::to_protect(PAGE_EXECUTE_READWRITE | PAGE_WRITECOMBINE));
		}

		TEST_METHOD(TestToFlProtect)
		{
			using namespace mem;
			Assert::AreEqual<DWORD>(PAGE_NOACCESS, Protect::to_flProtect(Protect::NoAccess));
			Assert::AreEqual<DWORD>(PAGE_READONLY, Protect::to_flProtect(Protect::Read));
			Assert::AreEqual<DWORD>(PAGE_READWRITE, Protect::to_flProtect(Protect::Write));
			Assert::AreEqual<DWORD>(PAGE_WRITECOPY, Protect::to_flProtect(Protect::WriteCopy));

			Assert::AreEqual<DWORD>(PAGE_EXECUTE, Protect::to_flProtect(Protect::Execute));
			Assert::AreEqual<DWORD>(PAGE_EXECUTE_READ, Protect::to_flProtect(Protect::Execute | Protect::Read));
			Assert::AreEqual<DWORD>(PAGE_EXECUTE_READWRITE, Protect::to_flProtect(Protect::Execute | Protect::Write));
			Assert::AreEqual<DWORD>(PAGE_EXECUTE_WRITECOPY, Protect::to_flProtect(Protect::Execute | Protect::WriteCopy));

			Assert::AreEqual<DWORD>(PAGE_EXECUTE | PAGE_GUARD, Protect::to_flProtect(Protect::Execute | Protect::Guard));
			Assert::AreEqual<DWORD>(PAGE_EXECUTE_READ | PAGE_NOCACHE, Protect::to_flProtect(Protect::Execute | Protect::Read | Protect::NoCache));
			Assert::AreEqual<DWORD>(PAGE_EXECUTE_READWRITE | PAGE_WRITECOMBINE, Protect::to_flProtect(Protect::Execute | Protect::Write | Protect::WriteCombine));
		}
	};
}
