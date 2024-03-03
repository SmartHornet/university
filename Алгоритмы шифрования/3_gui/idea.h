#ifndef IDEA_H
#define IDEA_H

#include <string>
#include <QDebug>

class Idea
{
    struct subKeys
    {
        unsigned short int subKey1, subKey2, subKey3, subKey4, subKey5, subKey6;
    } *subkeys_1;

    struct values
    {
        unsigned short int value1, value2, value3, value4;
    } *values_1;

    unsigned long _key1,_key2,_key3,_key4;
    unsigned short int *key;
    int *keyIndexes;

    unsigned short int sum(unsigned short int a, unsigned short int b);
    unsigned short int mul(unsigned short int a, unsigned short int b);
    unsigned short int getSubKey(unsigned short int *kkey, int *kkeyIndexes, int number);
    void getSubKeys(unsigned short int *kkey, struct subKeys *outSubKeys,int *kkeyIndexes, int startKey);
    void stage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues);
    void endStage(struct subKeys *inSubKeys, struct values *inValues, struct values *outValues);

public:

    //void bits(unsigned short int n, std::string *str);
    //unsigned short int fromBinString(const char* str);

    void setKey(unsigned long key1,unsigned long key2,unsigned long key3, unsigned long key4);
    void setValue(unsigned short int v1, unsigned short int v2, unsigned short int v3, unsigned short int v4);
    void encrypt();
    unsigned short int getValue1();
    unsigned short int getValue2();
    unsigned short int getValue3();
    unsigned short int getValue4();

    Idea();
    ~Idea();
};

#endif // IDEA_H
