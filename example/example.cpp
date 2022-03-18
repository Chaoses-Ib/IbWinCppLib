#include <iostream>
#include <IbWinCpp/WinCpp.hpp>

int main()
{
    std::cout << ib::ModuleFactory::Load(L"ntdll.dll").base.p << std::endl;
}