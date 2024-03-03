#include <iostream>
#include <QtGlobal>
#include <conio.h>

using namespace std;

struct Point
{
    qint32 x,y;
};

struct Parameters
{
    qint32 a,b,p;
};

struct PairPoint
{
    Point a, b;
};

bool isEqually(Point *a, Point *b);
qint32 lambda(Point *a, Point *b, Parameters *equation);
Point sumPoints(Point *a, Point *b, Parameters *equation, bool positive);
Point multPointNum(Point *a,Parameters *equation, qint32 num);

int main()
{
    Parameters equation;
    Point G,a,Pa,Pb,K,Pm,tmp;
    PairPoint Cm;
    qint32 n,x,y,Na,Nb,k;
    qint64 y2,y1;
    bool flag;

    equation.a = 71; //7 variant
    equation.b = 781;
    equation.p = 2837;

    //поиск точки G и n

    for(x = 1,flag = true; x < equation.p && flag; ++x)
    {
        //y2 = (((x*x*x) % equation.p) + (equation.a*x) % equation.p + equation.b) % equation.p;
        y1 = (x*x*x + equation.a*x  + equation.b) % equation.p;

        for(y = 1; y < equation.p && flag; ++y)
        {
            y2 = ((y*y) % equation.p);
            if(y1 == y2)
            {
                G.x = x; G.y = y; a.x = x; a.y = y;

                n = 1;
                while((G.y != -a.y) && (n < equation.p))
                {
                    G = sumPoints(&a,&G,&equation,0); ++n;
                }

                if((G.y == -a.y) && (G.x == a.x))
                {
                    G.x = a.x; G.y = a.y; flag = false;
                   //cout << "main a: " << "(" << a.x << ";" << a.y << ")" << endl;
                   //cout << "main G: " << "(" << G.x << ";" << G.y << ")" << "  n= " << n << endl << endl; _getch();
                }
            }
        }

    }

    cout << "P: " << equation.p << endl << "a: " << equation.a << endl << "b: " << equation.b << endl << endl;

    cout << "G: " << "(" << G.x << ";" << G.y << ")" << endl;
    cout << "n: " << n << endl << endl;

    Na = 101;
    Nb = 103;

    Pa = multPointNum(&G,&equation,Na);
    Pb = multPointNum(&G,&equation,Nb);

    cout << "Na: " << Na << endl << "Nb: " << Nb << endl;
    cout << "Pa: " << "(" << Pa.x << ";" << Pa.y << ")" << endl;
    cout << "Pb: " << "(" << Pb.x << ";" << Pb.y << ")" << endl << endl;

    K = multPointNum(&Pb,&equation,Na);

    cout << "Ka: " << "(" << K.x << ";" << K.y << ")" << endl;

    K = multPointNum(&Pa,&equation,Nb);

    cout << "Kb: " << "(" << K.x << ";" << K.y << ")" << endl << endl;


    //-------------------------------------------------------------------
    Pm.x = 513;
    Pm.y = 167;
    k = 113;

    cout << "Text: " << "(" << Pm.x << ";" << Pm.y << ")" << endl;
    cout << "k: " << k << endl << endl;

    Cm.a = multPointNum(&G,&equation,k);

    tmp = multPointNum(&Pa,&equation,k);

    Cm.b = sumPoints(&tmp,&Pm,&equation,1);

    cout << "Encrypted text: " << "{(" << Cm.a.x << ";" << Cm.a.y << ")," << "(" << Cm.b.x << ";" << Cm.b.y << ")}" << endl;

    //---------------------------------------------------------------------
    tmp = multPointNum(&Cm.a,&equation,Na);
    tmp.y = tmp.y * (-1);

    Pm = sumPoints(&Cm.b,&tmp,&equation,1);
    cout << "Decrypted text: " << "(" << Pm.x << ";" << Pm.y << ")";

    _getch();
    return 0;
}

bool isEqually(Point *a, Point *b)
{
    return (a -> x == b -> x) && (a -> y == b -> y);
}

qint32 lambda(Point *a, Point *b, Parameters *equation)
{
    qint64 numerator, denominator;
    qint32 res;

    if(isEqually(a,b))
    {
        numerator = (3 * a->x * a->x + equation->a) % equation->p;
        denominator = (2 * a->y) % equation->p;
    }
    else
    {
        numerator = (b->y - a->y) % equation->p;
        denominator = (b->x - a->x) % equation->p;
    }

    if(denominator == 0) denominator += equation->p;

    //cout << "1)numerator: " << numerator << " denominator: " << denominator << endl; //_getch();

    if(denominator > 0)
    {
        while((numerator % denominator) != 0)
        {numerator += equation->p; /*if(numerator > 2000000) cout << "error1" << endl;*/}
    }
    else
    {
        while((numerator % denominator) != 0)
        {numerator -= equation->p; /*if(numerator < -2000000) cout << "error2" << endl;*/}
    }

    res = numerator/denominator;

    return res;
}

Point sumPoints(Point *a, Point *b, Parameters *equation, bool positive)
{
    Point res;
    qint32 Lambda;

    Lambda = lambda(a, b, equation);
    res.x = (Lambda * Lambda - a->x - b->x) % equation->p;
    res.y = (Lambda * (a->x - res.x) - a->y) % equation->p;

    if(positive)
    {
        if(res.x < 0) res.x += equation->p;
        if(res.y < 0) res.y += equation->p;
    }

    return res;
}

Point multPointNum(Point *a,Parameters *equation, qint32 num)
{
    Point res;
    qint32 i;

    res.x = a->x; res.y = a->y;

    for(i = 1; i < num; ++i)
    {
        res = sumPoints(&res,a,equation,1);
    }

    return res;
}
