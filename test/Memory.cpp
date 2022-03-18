#include <catch.hpp>
#include <IbWinCpp/Memory.hpp>

TEST_CASE("Addr") {
    SECTION("test arithmetic") {
        ib::Addr a = (int*)0x10000000;

		CHECK((uintptr_t)(a + 3) == 0x10000003);
		CHECK((uintptr_t)a.offset(3) == 0x10000003);
		CHECK((uintptr_t)a.offset<ib::Byte>(3) == 0x10000003);
		CHECK((uintptr_t)a.offset<ib::Byte[3]>() == 0x10000003);

		a += 6;
		CHECK((uintptr_t)(a - 3) == 0x10000003);
		CHECK((uintptr_t)a.offset(-3) == 0x10000003);
		CHECK((uintptr_t)a.offset<ib::Byte>(-3) == 0x10000003);
		CHECK((uintptr_t)a.offset<ib::Byte[3]>(-1) == 0x10000003);
		a -= 6;
		CHECK((uintptr_t)a == 0x10000000);

		ib::Addr b = 0x10000003;
		CHECK(b - a == 3);

		CHECK((uintptr_t)b.align_up(1) == 0x10000003);
		CHECK((uintptr_t)b.align_up(4) == 0x10000004);
		CHECK((uintptr_t)b.align_up(16) == 0x10000010);
		CHECK((uintptr_t)b.align_down(4) == 0x10000000);
		b = 0x10000004;
		CHECK((uintptr_t)b.align_up(4) == 0x10000004);
		CHECK((uintptr_t)b.align_down(4) == 0x10000004);
    }

	SECTION("test conversion and comparison") {
		ib::Addr a = (int*)0x10000001;
		CHECK(a.p == (ib::Byte*)0x10000001);
		CHECK(a.ptr<float>() == (float*)0x10000001);

		CHECK(a);
		a = nullptr;
		CHECK(!a);
		a = 0x10000001;

		CHECK(a == (int*)0x10000001);
		CHECK(a < (int*)0x10000002);
		CHECK(a <= (int*)0x10000001);
		CHECK(a > (int*)0x10000000);
		CHECK(a >= (int*)0x10000001);

		CHECK(!(a != (int*)0x10000001));
		CHECK(!(a >= (int*)0x10000002));
		CHECK(!(a > (int*)0x10000001));
		CHECK(!(a <= (int*)0x10000000));
		CHECK(!(a < (int*)0x10000001));
	}

	SECTION("test reading and writing") {
		int a = 123;
		ib::Addr p = &a;

		CHECK(p.read<int>() == 123);
		CHECK((int)p.read() == 123);

		p.write<int>(456);
		CHECK(p.read<int>() == 456);

		uintptr_t b[2] = { 0, (uintptr_t)&a };
		p = &b;
		CHECK(p[0] == nullptr);
		CHECK(p[sizeof uintptr_t] == &a);
	}
}

TEST_CASE("Protect") {
	SECTION("test to_protect()") {
		using namespace ib::mem;
		CHECK(Protect::to_protect(PAGE_NOACCESS) == Protect::NoAccess);
		CHECK(Protect::to_protect(PAGE_READONLY) == Protect::Read);
		CHECK(Protect::to_protect(PAGE_READWRITE) == Protect::Write);
		CHECK(Protect::to_protect(PAGE_WRITECOPY) == Protect::WriteCopy);

		CHECK(Protect::to_protect(PAGE_EXECUTE) == Protect::Execute);
		CHECK(Protect::to_protect(PAGE_EXECUTE_READ) == (Protect::Execute | Protect::Read));  // the parentheses are needed by Catch2
		CHECK(Protect::to_protect(PAGE_EXECUTE_READWRITE) == (Protect::Execute | Protect::Write));
		CHECK(Protect::to_protect(PAGE_EXECUTE_WRITECOPY) == (Protect::Execute | Protect::WriteCopy));

		CHECK(Protect::to_protect(PAGE_EXECUTE | PAGE_GUARD) == (Protect::Execute | Protect::Guard));
		CHECK(Protect::to_protect(PAGE_EXECUTE_READ | PAGE_NOCACHE) == (Protect::Execute | Protect::Read | Protect::NoCache));
		CHECK(Protect::to_protect(PAGE_EXECUTE_READWRITE | PAGE_WRITECOMBINE) == (Protect::Execute | Protect::Write | Protect::WriteCombine));
	}

	SECTION("test to_flProtect()") {
		using namespace ib::mem;
		CHECK(Protect::to_flProtect(Protect::NoAccess) == PAGE_NOACCESS);
		CHECK(Protect::to_flProtect(Protect::Read) == PAGE_READONLY);
		CHECK(Protect::to_flProtect(Protect::Write) == PAGE_READWRITE);
		CHECK(Protect::to_flProtect(Protect::WriteCopy) == PAGE_WRITECOPY);

		CHECK(Protect::to_flProtect(Protect::Execute) == PAGE_EXECUTE);
		CHECK(Protect::to_flProtect(Protect::Execute | Protect::Read) == PAGE_EXECUTE_READ);
		CHECK(Protect::to_flProtect(Protect::Execute | Protect::Write) == PAGE_EXECUTE_READWRITE);
		CHECK(Protect::to_flProtect(Protect::Execute | Protect::WriteCopy) == PAGE_EXECUTE_WRITECOPY);

		CHECK(Protect::to_flProtect(Protect::Execute | Protect::Guard) == (PAGE_EXECUTE | PAGE_GUARD));
		CHECK(Protect::to_flProtect(Protect::Execute | Protect::Read | Protect::NoCache) == (PAGE_EXECUTE_READ | PAGE_NOCACHE));
		CHECK(Protect::to_flProtect(Protect::Execute | Protect::Write | Protect::WriteCombine) == (PAGE_EXECUTE_READWRITE | PAGE_WRITECOMBINE));

	}
}