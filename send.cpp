// Bourgeois Noe

#include "enigma.hpp"
#include <chrono>

using namespace std;
int alphabet_len = 27;
char maj_min_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";

string Crypter::sanitize(string msg) 
{
    //prend en paramètre un message et s’occupe de le nettoyer. Un message nettoyé correspond à un message qui contient uniquement des caractères alphabétiques
//majuscules. Dans le cas où une lettre minuscule est entrée, la fonction doit la transformer en
//majuscule en utilisant la fonction toupper
    string sanitized;
    for (auto & c : msg) {
        int a = c;
        if ((64 < a ) & (a < 91)){
            char s = static_cast<char> (a);
            sanitized += s;
        }
        else if ((96 < a) & (a < 123)){
            char s = static_cast<char> (toupper(a));
            sanitized += s; 
        }
    }
    return sanitized;
}

string Cesar::caesar(string msg) 
{
    //Cette fonction prend un message, en clair, et l’information de décalage en paramètre et retourne la version chiffrée du message.
    string sanitized_msg = this->sanitize(msg);
    for (auto & c : sanitized_msg) {
        int a = c;
        if ((64 < a ) & (a < 91)){
            if (this->rightShift){
                if ((a - 64) <= this->shift){
                    //the shift is bigger than the letters between a & "A" number
                    c = char (90 - (this->shift - (a - 64))%alphabet_len );
                    //continue from Z in reverse order
                }                  
                else{
                    c = char (a - this->shift);
                }         
            }
            else{
                if (90 - a <= this->shift){
                    c = char (64 + (this->shift - (90 - a))%alphabet_len); 
                }                    
                else{
                    c = char (a + this->shift);
                }                  
            }           
        }         
    }  
    return sanitized_msg;
} 

void Crypter::send(string crypted_msg) 
{
    //Cette fonction se charge d’écrire un message dans un fichier.
    ofstream file(this->filename, ios_base::app);
    if(file.is_open()) 
    {
        file << crypted_msg << endl;
        file.close();
    }
    else cerr << "Impossible d’ouvrir le fichier " << filename << endl;
} 

int main(int argc, char *argv[]) 
{
    
    if (strcmp(argv[1], "enigma") == 0)
    {
        ArthurScherbius as = ArthurScherbius();
        as.filename = argv[2];


        cout << "Communication chiffrée avec Enigma via " << argv[2] << endl;
        string input;

        while(input != "end"){
            cout << "votre message:";   
            cin >> input;
            string sanitized_msg = as.sanitize(input);
            char* sanitized_msg_ptr = &sanitized_msg[0];
            char* crypted_msg = enigma(sanitized_msg_ptr);
            //cout << "enigma-crypted_msg back to send:" << crypted_msg << endl;
            cout << "message crypté à envoyer:" << crypted_msg << endl;

            as.send(crypted_msg);
            cout << "message crypté envoyé:" << crypted_msg << endl;
            delete[] crypted_msg;
        }
    }

    else if (strcmp(argv[1], "statistics") == 0)
    {
        Cesar c = Cesar();
        ArthurScherbius as = ArthurScherbius();
        c.rightShift = strcmp(argv[2], "D") == 0;
        c.shift = atoi(argv[3]); // atoi: conversion d'un string vers un entier
        c.filename = argv[4];
        as.filename = argv[4];

        int vec_size = 100;
        int caesar_microseconds[vec_size];
        int enigma_microseconds[vec_size];
        string input;

        for(int size=0; size<100; size++)
        {
            input += maj_min_alphabet[rand()%52];
            string sanitized_msg = c.sanitize(input);

            auto caesar_start = chrono::high_resolution_clock::now();
            string caesar_crypted_msg = c.caesar(sanitized_msg);
            auto caesar_stop = chrono::high_resolution_clock::now();
            auto caesar_duration = std::chrono::duration_cast<std::chrono::microseconds>(caesar_stop - caesar_start);

            caesar_microseconds[size] = caesar_duration.count();
            cout << input << " of size " << size + 1 << " caesar-crypted to : " << endl;
            cout << caesar_crypted_msg << " in " << caesar_microseconds[size] << " microseconds" << endl << endl;

            c.send(caesar_crypted_msg);

            auto enigma_start = chrono::high_resolution_clock::now();
            char *sanitized_msg_ptr = &sanitized_msg[0];
            char *enigma_crypted_msg = enigma(sanitized_msg_ptr);
            cout << "enigma-crypted_msg back to send:" << enigma_crypted_msg << endl;

            auto enigma_stop = chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(enigma_stop - enigma_start);

            enigma_microseconds[size] = duration.count();
            cout << input << " of size " << size + 1 << " enigma-crypted to : " << endl;
            cout << enigma_crypted_msg << " in " << enigma_microseconds[size] << " microseconds" << endl;

            as.send(enigma_crypted_msg);
            cout << " enigma_crypted_msg sent : " << endl;
            cout << enigma_crypted_msg << endl;

            delete[] enigma_crypted_msg;
            cout << " enigma_crypted_msg after delete : " << endl;
            cout << enigma_crypted_msg << endl;

            cout << endl << endl;
        };

        string execution_times_filename = "execution-times.txt";
        ofstream file(execution_times_filename, ios_base::app);
        if(file.is_open()) 
        {
            cout << "caesar_crypt_microseconds = [" << caesar_microseconds[0];
            file << "caesar_crypt_microseconds = [" << caesar_microseconds[0];


            for(int size = 1; size < 100; size++)
            {
                cout << ", " << caesar_microseconds[size];
                file << ", " << caesar_microseconds[size];

            }
            cout << "]" << endl;
            file << "]" << endl;

            cout << "enigma_crypt_microseconds = [" << enigma_microseconds[0];
            file << "enigma_crypt_microseconds = [" << enigma_microseconds[0];


            for(int size = 1; size < 100; size++)
            {
                cout << ", " << enigma_microseconds[size];
                file << ", " << enigma_microseconds[size];

            }
            cout << "]" << endl;
            file << "]" << endl;

            file.close();
        }
        else cerr << "Impossible d’ouvrir le fichier " << execution_times_filename << endl;
    }

    else{
        // Les arguments à fournir: G|D n fichier
        if (argc < 4) {
            cout << "Utilisation: ./send G|D décalage fichier ou ./send enigma fichier." << endl;
            return 1;
        };
        if (strcmp(argv[1], "G") != 0 && 
            strcmp(argv[1], "D") != 0 && 
            strcmp(argv[1], "enigma") != 0){
            cout << "Le premier argument doit être G ou D, " << argv[1] << " est incorrect." << endl;
            return 1;
        }
        Cesar c = Cesar();
        c.rightShift = strcmp(argv[1], "D") == 0;
        c.shift = atoi(argv[2]); // atoi: conversion d'un string vers un entier
        c.filename = argv[3];
        cout << "Communication chiffrée avec César " << argv[1] <<  " " << argv[2] <<" via " << argv[3] << endl;
        string input;

        while(input != "end"){
            cout << "votre message:";
            cin >> input;
            cout << "envoi du message:";
            string crypted_msg = c.caesar(input);
            cout << crypted_msg << endl;
            c.send(crypted_msg);
        }
    
    }
    return 0;
}
