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

        std::cout<< "Tasteaza nume angajat si specializare: ";
        std::cin>> numeAngajat >>specializare;

        std::cout<<"Tasteaza nume client";
        std::cin>> numeClient;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout<<"Tasteaza observatii speciale ( sau apasa Enter daca nu sunt): ";
        std::string obsClient;
        std::getline(std::cin, obsClient);

        std::cout<<"Tasteaza nume serviciu si pret: ";
        std::cin>>numeServiciu >> pretServiciu;

        Angajat ang(numeAngajat, specializare);
        Programare p(numeClient, ang, obsClient);
        p.adaugaServiciu(Serviciu(numeServiciu, pretServiciu));

        if (p.estePremium()) {
            std::cout<<"Acesta este un client PREMIUM";
        }
        salonulMeu.adaugaProgramare(p);

        std::cout<<"\nMai doriti sa adaugati o programare? (da/nu):";
        std::cin>>raspuns;
    } while (raspuns == "da" || raspuns == "DA" );


    std:: cout<<salonulMeu <<"\n";
    salonulMeu.afiseazaRaportZilnic();
    return 0;
}
