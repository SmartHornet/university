#include <iostream>
#include <ios>
#include <conio.h>
#include <cstring>
#include <cstdlib>
#include <QString>
#include <QFile>
using namespace std;

template <typename T> void Bits(T n, string *str)
{
    int i,size;
    T _n;
    //char *tmp = new char[4];

    size = sizeof(n) * 8;
    //cout << "Size: " << size << " bits" << endl;

    //itoa(size,tmp,10);
    //str -> append("Size: "); str -> append(tmp);
    //str -> append(" bits"); str -> append(1,'\n');

    for(i = size; i > 0; --i)
    {
        _n = n >> (i - 1);
        str -> append(1, (char)((_n & 0x1) + 48));
    }
    //delete tmp;
}

unsigned short int sum(unsigned short int a, unsigned short int b)
{
    return ((a + b) & 65535);
}

unsigned short int mul(unsigned short int a, unsigned short int b)
{
    unsigned long tmpa,tmpb;
    unsigned short int res;

    if(a == 0) tmpa = 65536; else tmpa = a;
    if(b == 0) tmpb = 65536; else tmpb = b;
    if(a == 0 && b == 0) res = 1; else res = (unsigned short int)((tmpa * tmpb) % 65537);
    return res;
}

unsigned short int fromBinString(const char* str)//16
{
    unsigned short int res = 0,i,len = strlen(str);

    for(i = 0; i < len; ++i)
    {
        if(*(str + i) == '1'){res = res | (0x8000 >> i);}
    }

    return res;
}

unsigned short int clearHighBits(unsigned short int n, int numBits)
{
    unsigned short int tmp = 65535;
    tmp = tmp >> numBits;
    return n & tmp;

}

unsigned short int clearLowBits(unsigned short int n, int numBits)
{
    unsigned short int tmp = 65535;
    tmp = tmp << numBits;
    return n & tmp;
}

/*unsigned short int getSubKey(unsigned short int *key, int number)
{
    //int Lshift = 25, keySize = 16, keyLenght = 128, numKeys = 8;
    int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res;

    startBit = ((((number - 1) >> 3) * 25 + ((number - 1) & 7) * 16) % 129) + 1;
    endBit = (startBit + 15) % 129;

    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);

    startBit = startBit & 15;

    res1 = (clearHighBits(key[subKey1],startBit - 1) << (startBit - 1));
    res2 = (clearLowBits(key[subKey2],17 - startBit) >> (17 - startBit));
    res = res1 | res2;

    return res;
}*/

unsigned short int getSubKey(unsigned short int *key, int number)
{
    //int Lshift = 25, keySize = 16, keyLenght = 128, numKeys = 8;
    unsigned int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res,res11,res22;

    char *tmp = new char[4];
    string str1,str2,str,str3,key1,key2,str11,str22;
    QFile txtFile;
    txtFile.setFileName("3.txt");
    txtFile.open(QIODevice::Append | QIODevice::Text);


////////
    startBit = (((number - 1) >> 3) * 25 + 1 + (((number - 1) % 8) * 16)) % 129;
    endBit = (startBit + 15) % 129;
///////

    itoa(number,tmp,10);
    str3.append("-------- ");  str3.append(tmp); str3.append(" --------"); str3.append(1,'\n');

    itoa(startBit,tmp,10);
    str3.append("startBit1: "); str3.append(tmp); str3.append(1,'\n');

    itoa(endBit,tmp,10);
    str3.append("endBit1: "); str3.append(tmp); str3.append(1,'\n');

/////////
    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);
////////

    itoa(subKey1,tmp,10);
    str3.append("subKey1: "); str3.append(tmp); str3.append(1,'\n');

    itoa(subKey2,tmp,10);
    str3.append("subKey2: "); str3.append(tmp); str3.append(1,'\n');

////////
    startBit = startBit & 15;
////////

    itoa(startBit,tmp,10);
    str3.append("startBit2: "); str3.append(tmp); str3.append(1,'\n');


/////////
    res1 = clearHighBits(key[subKey1],startBit - 1);
    res2 = clearLowBits(key[subKey2],17 - startBit);
    res11 = res1 << (startBit - 1);
    res22 = res2 >> (17 - startBit);
    res = res11 | res22;

////////

    key1.append("key[subKey1]: ");
    key2.append("key[subKey2]: ");
    str1.append("res1        : ");
    str2.append("res2        : ");
    str11.append("res11       : ");
    str22.append("res22       : ");
    str.append("res         : ");

    Bits(key[subKey1], &key1);
    Bits(key[subKey2], &key2);
    Bits(res1, &str1);
    Bits(res2, &str2);
    Bits(res11, &str11);
    Bits(res22, &str22);
    Bits(res, &str);

    str3.append(key1);  str3.append(1,'\n');
    str3.append(key2);  str3.append(1,'\n');
    str3.append(str1);  str3.append(1,'\n');
    str3.append(str2);  str3.append(1,'\n');
    str3.append(str11);  str3.append(1,'\n');
    str3.append(str22);  str3.append(1,'\n');
    str3.append(str);  str3.append(2,'\n');

    txtFile.write(str3.c_str());

    txtFile.close();
    delete tmp;
    return res;
}

struct subKeys
{
    unsigned short int subKey1, subKey2, subKey3, subKey4, subKey5, subKey6;
};

struct values
{
    unsigned short int value1, value2, value3, value4;
};

void stage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues)
{
    unsigned short int v1,v2,v3,v4,ma1,ma2,tmp;

    v1 = mul(inSubKeys -> subKey1, inValues -> value1);
    v2 = sum(inSubKeys -> subKey2, inValues -> value2);
    v3 = sum(inSubKeys -> subKey3, inValues -> value3);
    v4 = mul(inSubKeys -> subKey4, inValues -> value4);

    ma1 = v1 ^ v3;
    ma2 = v2 ^ v4;

    ma1 = mul(ma1,inSubKeys -> subKey5);
    ma2 = sum(ma2,ma1);
    ma2 = mul(ma2,inSubKeys -> subKey6);
    ma1 = sum(ma1,ma2);

    v1 = v1 ^ ma2;
    v2 = v2 ^ ma1;
    v3 = v3 ^ ma2;
    v4 = v4 ^ ma1;

    tmp = v2;
    v2 = v3;
    v3 = tmp;

    outValues -> value1 = v1;
    outValues -> value2 = v2;
    outValues -> value3 = v3;
    outValues -> value4 = v4;
}

void endStage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues)
{
    outValues -> value1 = mul(inSubKeys -> subKey1,inValues -> value1);
    outValues -> value2 = sum(inSubKeys -> subKey2,inValues -> value3);
    outValues -> value3 = sum(inSubKeys -> subKey3,inValues -> value2);
    outValues -> value4 = mul(inSubKeys -> subKey4,inValues -> value4);
}

void getSubKeys(unsigned short int *key, struct subKeys *outSubKeys, int startKey)
{
    outSubKeys -> subKey1 = getSubKey(key,startKey);
    outSubKeys -> subKey2 = getSubKey(key,startKey + 1);
    outSubKeys -> subKey3 = getSubKey(key,startKey + 2);
    outSubKeys -> subKey4 = getSubKey(key,startKey + 3);
    outSubKeys -> subKey5 = getSubKey(key,startKey + 4);
    outSubKeys -> subKey6 = getSubKey(key,startKey + 5);
}

int main()
{
    unsigned short int key[8],k;
    unsigned long a1,a2,a3,a4;
    int i;
    string str,res;

    struct subKeys *subKeys_1 = new struct subKeys;
    struct values *values_1 = new struct values;

    a1 = 4050550955;
    a2 = 1605609391;
    a3 = 4072650953;
    a4 = 2152509687;

    key[0] = a1 >> 16;
    key[1] = a1 & 0xFFFF;
    key[2] = a2 >> 16;
    key[3] = a2 & 0xFFFF;
    key[4] = a3 >> 16;
    key[5] = a3 & 0xFFFF;
    key[6] = a4 >> 16;
    key[7] = a4 & 0xFFFF;

    values_1 -> value1 = 61904;
    values_1 -> value2 = 17310;
    values_1 -> value3 = 49332;
    values_1 -> value4 = 16838;

    cout << "SubKeys: " << endl;
    for(i = 1; i < 53; ++i)
    {
        k = getSubKey(key, i);
        cout << i << " - " << k << endl;
        /*cout << subKeys_1 -> subKey1 << "  " << subKeys_1 -> subKey2 << "  " <<subKeys_1 -> subKey3;
        cout << "  " << subKeys_1 -> subKey4 << "  " << subKeys_1 -> subKey5 << "  " <<subKeys_1 -> subKey6 << endl;*/
    }

    cout << "Key: " << endl;
    for(i = 0; i < 8; ++i)
    {
        str.clear();
        Bits(key[i],&str);
        cout << str << endl;
    }

    cout << endl << "Value: " << endl;
    str.clear();
    Bits(values_1 -> value1,&str);
    cout << str << endl;

    str.clear();
    Bits(values_1 -> value2,&str);
    cout << str << endl;

    str.clear();
    Bits(values_1 -> value3,&str);
    cout << str << endl;

    str.clear();
    Bits(values_1 -> value4,&str);
    cout << str << endl;

    cout << endl << "a: " << endl;

    str.clear();
    Bits(a1,&str);
    cout << str << endl; res.append(str); res.append(1,'\n');

    str.clear();
    Bits(a2,&str);
    cout << str << endl; res.append(str); res.append(1,'\n');

    str.clear();
    Bits(a3,&str);
    cout << str << endl; res.append(str); res.append(1,'\n');

    str.clear();
    Bits(a4,&str);
    cout << str << endl; res.append(str); res.append(1,'\n');

    QFile txtFile;
    txtFile.setFileName("2.txt");
    txtFile.open(QIODevice::Append | QIODevice::Text);
    txtFile.write(res.c_str());
    txtFile.close();

    delete values_1;
    delete subKeys_1;
    _getch();
    return 0;
}



/*cout << "Values: " << values_1 -> value1 << "  " << values_1 -> value2 << "  ";
cout << values_1 -> value3 << "  " << values_1 -> value4 << endl << endl;

for(i = 0; i < 8; ++i)
{
    getSubKeys(key, subKeys_1, i*6 + 1);

    cout << "Stage: " << i+1 << endl;
    cout << "Keys: " << hex << subKeys_1 -> subKey1 << "  " << subKeys_1 -> subKey2 << "  " <<subKeys_1 -> subKey3;
    cout << hex << "  " << subKeys_1 -> subKey4 << "  " << subKeys_1 -> subKey5 << "  " <<subKeys_1 -> subKey6 << endl;

    stage(subKeys_1, values_1, values_1);

    cout << "Values: " << hex << values_1 -> value1 << "  " << values_1 -> value2 << "  ";
    cout << values_1 -> value3 << "  " << values_1 -> value4 << endl << endl;
    _getch();
}

getSubKeys(key, subKeys_1, 49);
endStage(subKeys_1, values_1, values_1);

cout << "Stage: end" << endl;
cout << "Keys: " << hex << subKeys_1 -> subKey1 << "  " << subKeys_1 -> subKey2 << "  " <<subKeys_1 -> subKey3;
cout << hex << subKeys_1 -> subKey4 << endl;

cout << "Values: " << hex << values_1 -> value1 << "  " << values_1 -> value2 << "  ";
cout << values_1 -> value3 << " " << values_1 -> value4 << endl << endl;*/

/*cout << values_1 -> value1 << endl;
cout << values_1 -> value2 << endl;
cout << values_1 -> value3 << endl;
cout << values_1 -> value4 << endl;*/

/*cout << hex << values_1 -> value1 << endl;
cout << hex << values_1 -> value2 << endl;
cout << hex << values_1 -> value3 << endl;
cout << hex << values_1 -> value4 << endl;*/

//cout << (((values_1 -> value1 ^ values_1 -> value2) ^ values_1 -> value3) ^ values_1 -> value4);
