#include <catch.hpp>
#include <IbWinCpp/Cpp/auto_cast.hpp>

TEST_CASE("auto_cast") {
    int a = 1;

    uintptr_t addr1 = ib::auto_cast(&a);
    CHECK(addr1 == (uintptr_t)&a);

    uintptr_t addr2 = ib::auto_reinterpret_cast(&a);
    CHECK(addr2 == (uintptr_t)&a);
}