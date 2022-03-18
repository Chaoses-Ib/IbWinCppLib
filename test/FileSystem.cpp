#include <catch.hpp>
#include <IbWinCpp/FileSystem.hpp>

#include <iostream>

TEST_CASE("path_to_realpath") {
    SECTION("test junctions") {
        CHECK(ib::path_to_realpath(LR"(C:\Documents and Settings)") == LR"(C:\Users)");
        CHECK(ib::path_to_realpath(LR"(C:\Documents and Settings\)") == LR"(C:\Users)");
        CHECK(ib::path_to_realpath(LR"(C:\Documents and Settings)", false) == LR"(\\?\C:\Users)");

        CHECK(ib::path_to_realpath(LR"(C:\Documents and Settings\desktop.ini)") == LR"(C:\Users\desktop.ini)");
        CHECK(ib::path_to_realpath(LR"(C:\Documents and Settings\Default User)") == LR"(C:\Users\Default)");
    }
    
    SECTION("test junctions and symbolic links") {
        std::wstring realpath = ib::path_to_realpath(LR"(C:\Documents and Settings\All Users)");
        CHECK((realpath == LR"(C:\Users\Public)" || realpath == LR"(C:\ProgramData)"));
    }
}