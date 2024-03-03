#include <iostream>

using namespace std;

__int64 findD(__int64 a, __int64 b)
{
	__int64 x1=1, x2=0, x3=b, y1=0, y2=1, y3=a, q;

	while ((y3!=0) && (y3!=1))
	{
		q=x3/y3;
		__int64 t1=x1-q*y1;
		__int64 t2=x2-q*y2;
		__int64 t3=x3-q*y3;
		x1=y1; x2=y2; x3=y3;
		y1=t1; y2=t2; y3=t3;
	}
  
	if (y3=0) return 0;
	if (y3=1)  
		if (y2<0) return y2+b;
		else return y2;
}
/*
unsigned __int64 crypt(unsigned __int64 X, unsigned __int64 e, unsigned __int64 n)
{
	unsigned __int64 Y=1;

	while (e>0)
	{
		if ((e%2)==0)
		{
			cout<<"e="<<e<<endl;
			e/=2;
			X=(X*X)%n;
			cout<<"X="<<X<<endl;
		}
		else
		{
			cout<<"e="<<e<<endl;
			e--;
			Y=(Y*X)%n;			
			cout<<"Y="<<Y<<endl;
		}

	}

	return Y;

}
*/
 __int64 mult(__int64 a , __int64 b, __int64 c)
{
    __int64 res = 0;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            b--;
            res = (res + a) % c;
            
        }
        a = (a + a) % c;
        b /= 2;
    
    }
	//cout<<"\nres="<<res<<endl;
    return res;
    
}




int main()
{
	__int64 p=89329, q=89231, n=p*q, fn=(p-1)*(q-1), e=10001, d=findD(e,fn);
	__int64 S=16777216;
    
	__int64 X=174 + 171*256 + 165*65536 + 129*S;//X=2175118254;
	
    cout<<"X = "<<X<<endl<<endl;

	cout<<"p="<<p<<"\nq="<<q<<"\nn="<<n<<"\nfn="<<fn<<"\ne="<<e<<"\nd="<<d<<endl;

	 __int64 Y=1;
    
	while (e > 0)
    {
        if (e % 2 == 1)
        {
            e--;
            Y = mult(Y , X, n);	
        }
        e /= 2;
        X = mult(X , X, n);    
    }

	cout<<"Y = "<<Y<<endl<<endl;

	system("pause");
	return 0;
}
