// Bourgeois Noe
/*
compilation raises 2 warnings:

enigma.cpp: In function ‘char* enigma(char*)’:
enigma.cpp:40:34: warning: conversion to ‘std::__cxx11::basic_string<char>::size_type’ {aka ‘long unsigned int’} from ‘int’ may change the sign of the result [-Wsign-conversion]
   40 |         int a = int (str_message[c]);
      |                                  ^
enigma.cpp:45:50: warning: conversion to ‘std::vector<char>::size_type’ {aka ‘long unsigned int’} from ‘int’ may change the sign of the result [-Wsign-conversion]
   45 |             int permutation = (int) permutations[idx];
      |                                                  ^~~

not relevants "c" and "idx" being both above 0
*/

/*
Le chiffrement est un moyen de transformer une suite de caractères, en une autre suite, de telle
sorte que cette dernière soit codée à l’aide d’une information secrète. Sans cette information, il
serait impossible de déchiffrer la suite de caractère.
Le chiffrement Enigma est assez simple : lorsqu’un caractère est fourni à la machine, chaque
composant qui la constitue est chargé de remplacer le caractère reçu en entrée par un autre en
sortie.
*/

#include "enigma.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

int ORD_OF_CHAR_A = 65;

struct Rotor{
    //Un rotor est un composant chargé de faire correspondre chaque lettre fournie en
    //entrée par une autre lettre en sortie en se basant sur une permutation fixe.
    vector<char> permutations;
};

struct Reflector{
    //Le réflecteur se situe au bout de la chaîne, et est une dernière permutation effectuée.
    vector<char> reflection;
};

struct Machine
{
    Rotor rotors[3];
    Reflector reflector;
};


extern "C" char* enigma(char* message){
                           //A,  B,  C,  D,  E,  F,  G,  H,  I,  J,  K,  L,  M,  N,  O,  P,  Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z
    Rotor rotor1 =        {{'E','K','M','F','L','G','D','Q','V','Z','N','T','O','W','Y','H','X','U','S','P','A','I','B','R','C','J'}};
    Rotor rotor2 =        {{'A','J','D','K','S','I','R','U','X','B','L','H','W','T','M','C','Q','G','Z','N','P','Y','F','V','O','E'}};
    Rotor rotor3 =        {{'B','D','F','H','J','L','C','P','R','T','X','V','Z','N','Y','E','I','W','G','A','K','M','U','S','Q','O'}};
    Reflector reflector = {{'Y','R','U','H','Q','S','L','D','P','X','N','G','O','K','M','I','E','B','F','Z','C','W','V','J','A','T'}};

    /*
    L’envoi des signaux entrants, en bleu sur le Schéma 1. Le signal entrant est initialement la
    lettre tapée sur le clavier qui traverse les permutations de chaque rotor de droite à gauche.

    La lettre entrante du rotor de gauche est envoyée au réflecteur qui retourne une nouvelle
    lettre et refléte le résultat sur le rotor de gauche.
    */
    vector<char> to_reflector_steps_list[] =   {rotor3.permutations,
                                                rotor2.permutations,
                                                rotor1.permutations,
                                                reflector.reflection};

    /*
    La dernière étape correspond au signal renvoyé par le réflecteur aux rotors 
    en traversant les permutations de chaque rotor de manière inverse
    */                                            
    vector<char> from_reflector_steps_list[] = {rotor1.permutations,
                                                rotor2.permutations,
                                                rotor3.permutations};

    string str_message = message;
    cout << "message : " << message << endl;
    int msg_size = (int) str_message.size(); //unsigned long instead of int
    char *charPointerTo_encrypted_message = new char[200];

    int c = 0;
    for (c = 0; c < msg_size; c++)
    {
        cout << "message character : " << message[c] << endl;
        int a = int (str_message[c]);
        int idx = a - ORD_OF_CHAR_A;
        for (int p = 0; p < 4 ; p++)
        {
            //cout << " character idx : " << idx << endl;
            vector<char> permutations = to_reflector_steps_list[p];
            int permutation = (int) permutations[idx];
            idx = permutation - ORD_OF_CHAR_A;
        };
        char character = char(ORD_OF_CHAR_A + idx);
        for (int p = 0; p < 3 ; p++)
        {
            //cout << " character : " << character << endl;
            vector<char> permutations = from_reflector_steps_list[p];
            int idx = static_cast<int> (find(permutations.begin(), permutations.end(), character) - permutations.begin());
            
            character = char(ORD_OF_CHAR_A + idx);
        };
        cout << "encrypted message character: " << character << endl;
        charPointerTo_encrypted_message[c] = character;
    };
    cout << "charPointerTo_encrypted_message:" << charPointerTo_encrypted_message << endl;

    //c++;
    charPointerTo_encrypted_message[c] = '\0';

    cout << "charPointerTo_encrypted_message:" << charPointerTo_encrypted_message << endl;
    return charPointerTo_encrypted_message;
}
/*
int main(){
    cout << "votre message:";
    
    string input;

    cin >> input;
    
    char* char_arr;
    string str_obj(input);
    char_arr = &str_obj[0];
    
    cout << "envoi du message:";
    char* encrypted_msg = enigma(char_arr);
    cout << encrypted_msg << endl;
    return 0;
}
*/


