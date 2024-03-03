#include <iostream>
//#include <ios>
#include <conio.h>
#include <cstring>
//#include <cstdlib>
//#include <QString>
//#include <QFile>
using namespace std;

template <typename T> void bits(T n, string *str)
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

/*unsigned short int clearHighBits(unsigned short int n, int numBits)
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
}*/

/*unsigned short int getSubKey(unsigned short int *key, int number, int *keyIndexes)
{
    //int Lshift = 25, keySize = 16, keyLenght = 128, numKeys = 8;
    int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res;

    startBit = *(keyIndexes + (number - 1)*2);
    endBit = *(keyIndexes + (number - 1)*2 + 1);

    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);

    startBit = startBit & 15;

    res1 = (clearHighBits(key[subKey1],startBit - 1) << (startBit - 1));
    res2 = (clearLowBits(key[subKey2],17 - startBit) >> (17 - startBit));
    res = res1 | res2;

    return res;
}*/

/*unsigned short int getSubKey(unsigned short int *key, int number, int *keyIndexes)
{
    //int Lshift = 25, keySize = 16, keyLenght = 128, numKeys = 8;
    int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res,tmp = 65535;


    tmp = tmp >> numBits;
    return n & tmp;

    startBit = *(keyIndexes + number - 1);
    endBit = (startBit + 15) % 129;

    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);

    startBit = startBit & 15;

    res1 = (clearHighBits(key[subKey1],startBit - 1) << (startBit - 1));
    res2 = (clearLowBits(key[subKey2],17 - startBit) >> (17 - startBit));
    res = res1 | res2;

    return res;
}*/

unsigned short int getSubKey(unsigned short int *key, int *keyIndexes, int number)
{
    int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res,tmp = 65535;

    startBit = keyIndexes[number - 1];
    endBit = (startBit + 15) % 129;

    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);

    startBit = startBit & 15;

    res1 = ((key[subKey1] & (tmp >> (startBit - 1))) << (startBit - 1));
    res2 = ((key[subKey2] & (tmp << (17 - startBit))) >> (17 - startBit));
    res = res1 | res2;

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
    unsigned short int temp = inValues -> value2;
    outValues -> value1 = mul(inSubKeys -> subKey1,inValues -> value1);
    outValues -> value2 = sum(inSubKeys -> subKey2,inValues -> value3);
    outValues -> value3 = sum(inSubKeys -> subKey3,temp);
    outValues -> value4 = mul(inSubKeys -> subKey4,inValues -> value4);
}

void getSubKeys(unsigned short int *key, struct subKeys *outSubKeys,int *keyIndexes, int startKey)
{
    outSubKeys -> subKey1 = getSubKey(key,keyIndexes,startKey);
    outSubKeys -> subKey2 = getSubKey(key,keyIndexes,startKey + 1);
    outSubKeys -> subKey3 = getSubKey(key,keyIndexes,startKey + 2);
    outSubKeys -> subKey4 = getSubKey(key,keyIndexes,startKey + 3);
    outSubKeys -> subKey5 = getSubKey(key,keyIndexes,startKey + 4);
    outSubKeys -> subKey6 = getSubKey(key,keyIndexes,startKey + 5);
}


int main()
{
    unsigned long a1,a2,a3,a4;
    int i;
    struct subKeys *subKeys_1 = new struct subKeys;
    struct values *values_1 = new struct values;
    unsigned short int key[8],k;
    /*int keyIndexes[112] = {
        1,16,
        17,32,
        33,48,
        49,64,
        65,80,
        81,96,
        97,112,
        113,128,

        26,41,
        42,57,
        58,73,
        74,89,
        90,105,
        106,121,
        122,9,
        10,25,

        51,66,
        67,82,
        83,98,
        99,114,
        115,2,
        3,18,
        19,34,
        35,50,

        76,91,
        92,107,
        108,123,
        124,11,
        12,27,
        28,43,
        44,59,
        60,75,

        101,116,
        117,4,
        5,20,
        21,36,
        37,52,
        53,68,
        69,84,
        85,100,

        126,13,
        14,29,
        30,45,
        46,61,
        62,77,
        78,93,
        94,109,
        110,125,

        23,38,
        39,54,
        55,70,
        71,86,
        0,0,0,0,0,0,0,0};*/

    int keyIndexes[56] = {
        1,
        17,
        33,
        49,
        65,
        81,
        97,
        113,

        26,
        42,
        58,
        74,
        90,
        106,
        122,
        10,

        51,
        67,
        83,
        99,
        115,
        3,
        19,
        35,

        76,
        92,
        108,
        124,
        12,
        28,
        44,
        60,

        101,
        117,
        5,
        21,
        37,
        53,
        69,
        85,

        126,
        14,
        30,
        46,
        62,
        78,
        94,
        110,

        23,
        39,
        55,
        71,
        0,
        0};

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

    /*key[0] = 0x0001;
    key[1] = 0x0002;
    key[2] = 0x0003;
    key[3] = 0x0004;
    key[4] = 0x0005;
    key[5] = 0x0006;
    key[6] = 0x0007;
    key[7] = 0x0008;*/

    values_1 -> value1 = 61904;
    values_1 -> value2 = 17310;
    values_1 -> value3 = 49332;
    values_1 -> value4 = 16838;

    /*values_1 -> value1 = 0x000;
    values_1 -> value2 = 0x001;
    values_1 -> value3 = 0x002;
    values_1 -> value4 = 0x003;*/

    /*cout << hex <<"Values: " << endl << values_1 -> value1 << "  ";
    cout << hex <<values_1 -> value2 <<  "  ";
    cout << hex <<values_1 -> value3 <<  "  ";
    cout << hex <<values_1 -> value4 <<  "  " << endl;*/

    for(i = 1; i <= 43; i += 6)
    {
        getSubKeys(key,subKeys_1,keyIndexes,i);
        stage(subKeys_1,values_1,values_1);

        /*cout << hex <<"Keys: " << endl << subKeys_1 -> subKey1 << "  ";
        cout << hex <<subKeys_1 -> subKey2 << "  ";
        cout << hex <<subKeys_1 -> subKey3 << "  ";
        cout << hex <<subKeys_1 -> subKey4 << "  ";
        cout << hex <<subKeys_1 -> subKey5 << "  ";
        cout << hex <<subKeys_1 -> subKey6 << "  " << endl;

        cout << hex <<"Values: " << endl << values_1 -> value1 << "  ";
        cout << hex <<values_1 -> value2 <<  "  ";
        cout << hex <<values_1 -> value3 <<  "  ";
        cout << hex <<values_1 -> value4 <<  "  " << endl;
        _getch();*/
    }

    getSubKeys(key,subKeys_1,keyIndexes,49);
    endStage(subKeys_1,values_1,values_1);

    /*cout << hex <<"EKeys: " << endl << subKeys_1 -> subKey1 << "  ";
    cout << hex <<subKeys_1 -> subKey2 << "  ";
    cout << hex <<subKeys_1 -> subKey3 << "  ";
    cout << hex <<subKeys_1 -> subKey4 << "  " << endl;*/

    cout <<"EValues: " << endl << values_1 -> value1 << endl;
    cout <<values_1 -> value2 <<  endl;
    cout <<values_1 -> value3 <<  endl;
    cout <<values_1 -> value4 <<  endl << endl;

    /*for(i = 1; i < 53; ++i)
    {
        k = getSubKey(key,i,keyIndexes);
        cout << k << " - " << i << endl;
    }*/


    delete subKeys_1;
    delete values_1;
    return 0;

}
