
/*
#ifndef ENIGMA_HPP
#define ENIGMA_HPP

extern "C" char* enigma(char* message);

#endif
*/

#ifndef CRYPTER
#define CRYPTER

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
using namespace std;


class Crypter
{
public:
    string filename;
    string sanitize(string msg);
    void send(string crypted_msg);
};

class Cesar : public Crypter
{
public:
    bool rightShift;
    int shift;
    string caesar(string msg);
};

class ArthurScherbius : public Crypter
{
};


extern "C" char* enigma(char* message);

#endif
