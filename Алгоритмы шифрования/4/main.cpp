#include <iostream>
#include <QtGlobal>
#include <cstdlib>
#include <string>
#include <conio.h>
using namespace std;

#define rotLS(x,n) (((x) << ((n) & (sizeof(x) - 1))) | ((x) >> ((sizeof(x)*8) - ((n) & (sizeof(x) - 1)))))
//#define rotRS(x,n) (((x) >> (n)) | ((x) << ((sizeof(x)*8) - (n))))
#define max(a,b) ((a > b)? (a):(b))


/*template <typename T> void bits(T n, string *str)
{
    int i,size;
    T _n;
    char *tmp = new char[4];

    size = sizeof(n) * 8;
    //cout << "Size: " << size << " bits" << endl;

    itoa(size,tmp,10);
    str -> append("Size: "); str -> append(tmp);
    str -> append(" bits"); str -> append(1,'\n');

    for(i = size; i > 0; --i)
    {
        _n = n >> (i - 1);
        str -> append(1, (char)((_n & 0x1) + 48));
    }
    delete tmp;
}*/

quint64 getPw(quint8 w)
{
    quint64 Pw;
    switch(w)
    {
        case 16: Pw = 0xB7E1; break;
        case 32: Pw = 0xB7E15163; break;
        case 64: Pw = 0xB7E151628AED2A6B; break;
        default: Pw = 0;
    }
    return Pw;
}

quint64 getQw(quint8 w)
{
    quint64 Qw;
    switch(w)
    {
        case 16: Qw = 0x9E37; break;
        case 32: Qw = 0x9E3779B9; break;
        case 64: Qw = 0x9E3779B97F4A7C15; break;
        default: Qw = 0;
    }
    return Qw;
}


int main()
{
    int w = 16; //длина слова в битах 16,32,64
    int r = 4; //число раундов
    int b = 4; //длина ключа в байтах
    quint16 Pw = 0xB7E1;
    quint16 Qw = 0x9E37;
    quint32 key = 1515458602;
    quint32 text = 2164892041;

    int t = 2*(r + 1);
    int c = (w >> 3);
    int a;

    quint16 S[t],L[c],A,B;
    quint16 i,j,X,Y;

    S[0] = Pw;
    for(i = 1; i < t; ++i) S[i] = S[i-1] + Qw;

    L[0] = key >> 16;
    L[1] = key & 0xFFFF;

    b = max(t,c)*3;
    i = j = X = Y = 0;
    for(a = 0; a < b; ++a)
    {
        S[i] = rotLS(S[i] + X + Y,3); X = S[i]; i = (i+1) % t;
        L[j] = rotLS(L[j] + X + Y,X + Y); Y = L[j]; j = (j+1) % c;
    }

    A = text >> 16;
    B = text & 0xFFFF;

    A = ((A + S[0]) & 65535);
    B = ((B + S[1]) & 65535);

    for(a = 1; a <= r; ++a)
    {
        A = rotLS((A ^ B),B) + S[2*a];
        B = rotLS((B ^ A),A) + S[2*a + 1];
    }

    quint32 text1,text2;
    text1 = A; text1 = text1 << 16;
    text2 = B;

    text = text1 | text2;

    cout << text;

    _getch();
    return 0;
}
