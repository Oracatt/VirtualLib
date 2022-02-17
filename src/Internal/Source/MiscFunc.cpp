#include "../Include/MiscFunc.h"
#include <ctime>
#include <sstream>

const char CCH[] = "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

wstring c2w(LPCSTR sText)
{
    DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, sText, -1, NULL, 0);

    wchar_t* pwText = NULL;
    pwText = new wchar_t[dwNum + 10];
    if (!pwText)
    {
        delete[] pwText;
        pwText = NULL;
    }
    unsigned nLen = MultiByteToWideChar(CP_ACP, 0, sText, -1, pwText, dwNum + 10);
    if (nLen >= 0)
    {
        pwText[nLen] = 0;
    }

    wstring ret = pwText;
    delete[] pwText;
    return ret;
}

string GetRandomString()
{
    ostringstream s;
    srand((unsigned)time(NULL));
    char ch[33] = { 0 };
    for (int i = 0; i < 32; ++i)
    {
        int x = rand() / (RAND_MAX / (sizeof(CCH) - 1));
        ch[i] = CCH[x];
    }
    s << ch;
    return s.str();
}

bool isContain(string str1, string str2) 
{
    if (str1.find(str2) != string::npos) {
        return true;
    }
    else {
        return false;
    }
}