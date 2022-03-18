#include <catch.hpp>
#include <IbWinCpp/Cpp/zstring.hpp>

TEST_CASE("zstring") {
    const wchar_t* p = L"test";
    ib::wzstring zs1 = p;
    CHECK(zs1.p == p);

    std::wstring s = L"test";
    ib::wzstring zs2 = s;
    CHECK(zs2.p == s.c_str());
}