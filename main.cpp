#include <iostream>
#include <string>
#include "Salon.h"
#include <limits>


int main() {
    std:: string numeAngajat, specializare, numeClient, numeServiciu, raspuns;
    double pretServiciu;

    Salon salonulMeu("Beauty Lab");
    do {
        std::cout<<"Introducere programare: ";

        std::cout << "Nume angajat: ";
        std::cin >> numeAngajat;
        std::cout << "Specializare: ";
        std::cin >> specializare;

        std::cout<<"Tasteaza nume client";
        std::cin>> numeClient;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Tasteaza observatii speciale ( sau apasa Enter daca nu sunt): ";
        std::string obsClient;
        std::getline(std::cin, obsClient);


        Angajat ang(numeAngajat, specializare);
        Programare p(numeClient, ang, obsClient);

        std::string raspunsServiciu;
        do {
            std::cout<<"Nume serviciu:";
            std::getline(std::cin, numeServiciu);

            if (numeServiciu.empty()) continue;

            std::cout<< "Pret serviciu:";

            while (!(std::cin >> pretServiciu)) {
                std::cout<< "Introduceti un numar valid pentru pret:";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits < std::streamsize >::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            p.adaugaServiciu(Serviciu(numeServiciu, pretServiciu));

            std::cout<<"Mai adaugati un serviciu pentru " << numeClient << "? (da/nu): ";
            std::getline(std::cin, raspunsServiciu);
        }while (raspunsServiciu == "da" || raspunsServiciu == "DA" );
        if (p.estePremium()) {
            std::cout<<"Acesta este un client PREMIUM";
        }
        salonulMeu.adaugaProgramare(p);

        std::cout<<"\nMai doriti sa adaugati o programare pentru alt client? (da/nu):";
        std::cin>>raspuns;
    } while (raspuns == "da" || raspuns == "DA" );


    std:: cout<<salonulMeu <<"\n";
    salonulMeu.afiseazaRaportZilnic();
    return 0;
}
