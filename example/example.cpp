#include <iostream>
#include <IbWinCpp/WinCppLib.hpp>

using namespace std;
using namespace ib;

int main()
{
    //auto a = wzstring(L"abc"s);
    cout << (void*)ModuleFactory::Load(L"ntdll.dll").base << endl;
}