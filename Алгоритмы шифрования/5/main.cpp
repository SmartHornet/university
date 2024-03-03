#include <iostream>
#include <QtGlobal>
#include <conio.h>
using namespace std;

//рекурсивный, бинарный поиск НОД
template <typename T> T gcd(T a, T b)
{
    if(!a) return b;//НОД(0, b) = b;
    if(!b) return a;//НОД(a, 0) = a;
    if((a == 1) || (b == 1)) return 1;//НОД(1, b) = 1; НОД(a, 1) = 1;
    if(a == b) return a;//НОД(a, a) = m;

    if(!((a&1)||(b&1))) return (gcd(a >> 1,b >> 1) << 1);//Если a, b чётные, то НОД(a, b) = 2*НОД(a/2, b/2);
    if((!(a&1)) && (b&1)) return gcd(a >> 1,b);//Если a чётное, b нечётное, то НОД(a, b) = НОД(a/2, b);
    if((a&1) && (!(b&1))) return gcd(a,b >> 1);//Если b чётное, a нечётное, то НОД(a, b) = НОД(a, b/2);
    //если сюда дошли, то точно a и b нечетные
    if(a > b) return gcd((a-b) >> 1,b); else return gcd((b-a) >> 1,a);
    //Если a, b нечётные и b > a, то НОД(a, b) = НОД((b-a)/2, a);
    //Если a, b нечётные и b < a, то НОД(a, b) = НОД((a-b)/2, b);
}

quint64 mulmod(quint64 x, quint64 y, quint64 n)
{
    if (y == 0) return 0;
    quint64 res = mulmod(x, y >> 1, n);
    res += res;
    res %= n;
    return (y & 1)? (x + res) % n : res;
}

quint64 powmod(quint64 x, quint64 y, quint64 n)
{
    quint64 res = 1;

    while(y)
    {
        if (y & 1) res = mulmod(res,x,n);
        y >>= 1;
        x = mulmod(x,x,n);
    }
    return res;
}

int main()
{
    quint64 n,Fn,p,q,e,d;
    quint32 text;
    quint64 x1,x2,x3,y1,y2,y3,t1,t2,t3,tmp,i;
    bool find;

    p = 90631; //7 variant
    q = 90527;

    //cout << "p: " << p << endl;
    //cout << "q: " << q << endl;

    n = p*q;
    Fn = (p-1)*(q-1);
    //Казм 138 160 167 172
    text = ((((quint32)138) << 24) | (((quint32)160) << 16) | (((quint32)167) << 8) | ((quint32)172));

    //cout << "n: " << n << endl;
    //cout << "Fn: " << Fn << endl;

    for(i = 10001,find = false; !find; ++i)
    {
        if(gcd(Fn,i) == 1) {e = i; find = true;}
    }

    //cout << "e: " << e << endl;

    x1 = 1; x2 = 0; x3 = Fn;
    y1 = 0; y2 = 1; y3 = e;

    while((y3 != 1) && (y3 != 0))
    {
        tmp = x3/y3;
        t1 = x1 - tmp*y1; t2 = x2 - tmp*y2; t3 = x3 - tmp*y3;
        x1 = y1; x2 = y2; x3 = y3;
        y1 = t1; y2 = t2; y3 = t3;
    }

    if(y3 == 0) d = 0; else d = y2;
    //cout << "d: " << d << endl;

    cout << "Shared Key: " << endl << "e: " << e << endl << "n: " << n << endl << endl;
    cout << "Private Key: " << endl << "d: " << d << endl << "n: " << n << endl << endl;

    cout << "text: " << text << endl;

    text = powmod(text,e,n);
    cout << "Encrypted text: " << text;
    _getch();
    return 0;
}
