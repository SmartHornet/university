#include "idea.h"

Idea::Idea()
{
    key = new unsigned short int[8];

    subkeys_1 = new subKeys;
    values_1 = new values;

    int i;
    int _keyIndexes[54] =
    {1,17,33,49,65,81,97,113,
    26,42,58,74,90,106,122,10,
    51,67,83,99,115,3,19,35,
    76,92,108,124,12,28,44,60,
    101,117,5,21,37,53,69,85,
    126,14,30,46,62,78,94,110,
    23,39,55,71,0,0};

    keyIndexes = new int[54];
    for(i = 0; i < 54; ++i) keyIndexes[i] = _keyIndexes[i];
}

Idea::~Idea()
{
    delete key;
    delete subkeys_1;
    delete values_1;
    delete keyIndexes;
}


unsigned short int Idea::sum(unsigned short int a, unsigned short int b)
{
    return ((a + b) & 65535);
}

unsigned short int Idea::mul(unsigned short int a, unsigned short int b)
{
    unsigned long tmpa,tmpb;
    unsigned short int res;

    if(a == 0) tmpa = 65536; else tmpa = a;
    if(b == 0) tmpb = 65536; else tmpb = b;
    if(a == 0 && b == 0) res = 1; else res = (unsigned short int)((tmpa * tmpb) % 65537);
    return res;
}

unsigned short int Idea::getSubKey(unsigned short int *kkey, int *kkeyIndexes, int number)
{
    int startBit, endBit, subKey1, subKey2;
    unsigned short int res1,res2,res,tmp = 65535;

    startBit = kkeyIndexes[number - 1];
    endBit = (startBit + 15) % 129;

    subKey1 = ((startBit - 1) >> 4);
    subKey2 = ((endBit - 1) >> 4);

    startBit = startBit & 15;

    res1 = ((kkey[subKey1] & (tmp >> (startBit - 1))) << (startBit - 1));
    res2 = ((kkey[subKey2] & (tmp << (17 - startBit))) >> (17 - startBit));
    res = res1 | res2;

    return res;
}

void Idea::getSubKeys(unsigned short int *kkey, struct subKeys *outSubKeys,int *kkeyIndexes, int startKey)
{
    outSubKeys -> subKey1 = getSubKey(kkey,kkeyIndexes,startKey);
    outSubKeys -> subKey2 = getSubKey(kkey,kkeyIndexes,startKey + 1);
    outSubKeys -> subKey3 = getSubKey(kkey,kkeyIndexes,startKey + 2);
    outSubKeys -> subKey4 = getSubKey(kkey,kkeyIndexes,startKey + 3);
    outSubKeys -> subKey5 = getSubKey(kkey,kkeyIndexes,startKey + 4);
    outSubKeys -> subKey6 = getSubKey(kkey,kkeyIndexes,startKey + 5);
}

void Idea::stage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues)
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

void Idea::endStage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues)
{
    unsigned short int temp = inValues -> value2;
    outValues -> value1 = mul(inSubKeys -> subKey1,inValues -> value1);
    outValues -> value2 = sum(inSubKeys -> subKey2,inValues -> value3);
    outValues -> value3 = sum(inSubKeys -> subKey3,temp);
    outValues -> value4 = mul(inSubKeys -> subKey4,inValues -> value4);
}

/*void Idea::bits(unsigned short int n, std::string *str)
{
    int i;
    unsigned short int _n;

    for(i = 16; i > 0; --i)
    {
        _n = n >> (i - 1);
        str -> append(1, (char)((_n & 0x1) + 48));
    }
}

unsigned short int Idea::fromBinString(const char* str)
{
    unsigned short int res = 0,i,len = strlen(str);

    for(i = 0; i < len; ++i)
    {
        if(*(str + i) == '1'){res = res | (0x8000 >> i);}
    }

    return res;
}*/

void Idea::encrypt()
{
    int i;
    key[0] = _key1 >> 16;
    key[1] = _key1 & 0xFFFF;
    key[2] = _key2 >> 16;
    key[3] = _key2 & 0xFFFF;
    key[4] = _key3 >> 16;
    key[5] = _key3 & 0xFFFF;
    key[6] = _key4 >> 16;
    key[7] = _key4 & 0xFFFF;

    for(i = 1; i <= 43; i += 6)
    {
        getSubKeys(key,subkeys_1,keyIndexes,i);
        stage(subkeys_1,values_1,values_1);
    }

    getSubKeys(key,subkeys_1,keyIndexes,49);
    endStage(subkeys_1,values_1,values_1);
}

void Idea::setKey(unsigned long key1, unsigned long key2, unsigned long key3, unsigned long key4)
{
    _key1 = key1; _key2 = key2; _key3 = key3; _key4 = key4;
}

void Idea::setValue(unsigned short v1, unsigned short v2, unsigned short v3, unsigned short v4)
{
    values_1 -> value1 = v1; values_1 -> value2 = v2;
    values_1 -> value3 = v3; values_1 -> value4 = v4;
}

unsigned short Idea::getValue1(){return values_1 -> value1;}
unsigned short Idea::getValue2(){return values_1 -> value2;}
unsigned short Idea::getValue3(){return values_1 -> value3;}
unsigned short Idea::getValue4(){return values_1 -> value4;}
