#include <iostream>
#include <QtGlobal>
#include <QList>
#include <conio.h>
using namespace std;

quint16 mulmod(quint16 x, quint16 y, quint16 n)
{
    if (y == 0) return 0;
    quint16 res = mulmod(x, y >> 1, n);
    res += res;
    res %= n;
    return (y & 1)? (x + res) % n : res;
}

quint16 powmod(quint16 x, quint16 y, quint16 n)
{
    quint16 res = 1;

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
    quint16 q,i,a,tmp,Xa,Xb,Ya,Yb,k;
    QList<quint16> factors; //разложение числа q-1 на простые множители
    QList<quint16>::iterator ifactors;
    bool find;

    /////////////////////////////////////////
    q = 2963;  //7 variant
    //q = 11;

    //q - 1 = 2962 = 2*1481
    factors.append(2);
    factors.append(1481);
    /*factors.append(2);
    factors.append(5);*/
    ////////////////////////////////////////

    //нахождение первообразного корня
    for(i = 101, find = false; !find && (i < q); i++)
    {
        //cout << "i = " << i << endl;
        find = true;
        ifactors = factors.begin();
        while(ifactors != factors.end() && find)
        {
            tmp = powmod(i,(q-1)/(*ifactors),q);
            if(tmp == 1) find = false;
            //cout << "   factors: " << *ifactors << " >>>> tmp = " << tmp << endl;
            ++ifactors;
        }
        //cout << "end" << endl;
    }
    a = i - 1;

    /////////////////////////////////////////
    Xa = 1286;
    Xb = 1895;
    /////////////////////////////////////////

    Ya = powmod(a,Xa,q);
    Yb = powmod(a,Xb,q);

    k = powmod(Yb,Xa,q);

    cout << "q = " << q << endl << "a = " << a << endl << endl;
    cout << "Private keys: " << endl << "Xa = " << Xa << endl << "Xb = " << Xb << endl << endl;
    cout << "Shared keys: " << endl << "Ya = " << Ya << endl << "Yb = " << Yb << endl << endl;
    cout << "k = " << k;

    k = powmod(Ya,Xb,q);
    cout << endl << "k = " << k;
    _getch();
    return 0;
}
