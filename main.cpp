#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <iomanip>
#include "Salon.h"


int main() {

    Salon salonulMeu("Beauty Lab");
    //adaugam personal
    Angajat a1("Andreea", "Cosmetician");
    a1.adaugaCompetenta(TipServiciu::COSMETICA);
    a1.adaugaCompetenta(TipServiciu::TRATAMENT);

    Angajat a2("Matei", "Hair-Stilist");
    a2.adaugaCompetenta(TipServiciu::TUNS);
    a2.adaugaCompetenta(TipServiciu::VOPSIT);

    Angajat a3("Elena", "Manichiurista");
    a3.adaugaCompetenta(TipServiciu::MANICHIURA); // Pentru manichiură spa/tratament

    salonulMeu.adaugaAngajat(a1);
    salonulMeu.adaugaAngajat(a2);
    salonulMeu.adaugaAngajat(a3);
    //catalog de servicii
    std:: vector<Serviciu> catalog = {
        Serviciu("Tuns Barbati", 50.0, 30, TipServiciu::TUNS),
        Serviciu("Tuns Femei", 90.0, 60, TipServiciu::TUNS),
        Serviciu("Vopsit Total", 200.0, 120, TipServiciu::VOPSIT),
        Serviciu("Tratament Facial", 150.0, 45, TipServiciu::COSMETICA),
        Serviciu("Manichiura clasica", 80.0, 60, TipServiciu::MANICHIURA),
        Serviciu("Tratament hidratare", 250.0, 150, TipServiciu::TRATAMENT)
    };

    std:: string raspunsClientNou;
    do {
        std::string numeClient;
        std::cout << "\n==============================================";
        std::cout << "\nSISTEM GESTIUNE PROGRAMARI - CLIENT NOU";
        std::cout << "\n==============================================";
        std::cout << "\nIntroduceti numele clientului: ";

        if (std:: cin.peek() == '\n') std::cin.ignore();
        std:: getline(std::cin, numeClient);

        std:: string raspunsAltaProgramare;
        do {
            std::cout << "\n--- SPECIALISTI DISPONIBILI ---\n";
            const auto& listaAngajati = salonulMeu.getAngajati();
            for (size_t i = 0; i < listaAngajati.size(); ++i) {
                std:: cout << i + 1 << ". " << listaAngajati[i].getNume() << " [" << listaAngajati[i].getSpecializare() << "]\n";
            }

            int optiuneAng;
            std::cout<< "Alegeti specialistul: ";
            while (!(std::cin >> optiuneAng) || optiuneAng < 1 || optiuneAng > (int)listaAngajati.size()) {
                std::cout<< "Selectie invalida.Reincercati: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//curatare buffer

            Angajat angAles = listaAngajati[optiuneAng - 1];
            IntervalOrar interval = {0,0,0,0};
            Programare p(numeClient, angAles, 0,0,0,interval);

            std::string altServiciu;
            do {
                std::cout <<"\n--- SERVICII PRESTATE DE " << angAles.getNume() << " ---\n";
                std::vector <int> indiciFiltrati;
                int contor = 1;
                for (size_t i = 0; i < catalog.size(); ++i) {
                    if (angAles.poatePresta(catalog[i].getTip())) {
                        std::cout << contor << ". " <<std::left <<std::setw(20) << catalog[i].getNume()
                        << " | " << catalog[i].getDurata() << " min | " <<catalog[i].getPret() <<" RON\n";

                        indiciFiltrati.push_back(i);
                        contor++;
                    }
                }

                if (indiciFiltrati.empty()) {
                    std::cout<<"Acest specialist nu are servicii configurate !\n";
                    break;
                }

                int alegereS;
                std::cout << "Alegeti serviciul: ";
                while (!(std::cin >> alegereS) || alegereS < 1 || alegereS > (int)indiciFiltrati.size()) {
                    std::cout<< "Selectie invalida: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                p.adaugaServiciu(catalog[indiciFiltrati[alegereS - 1]]);
                std::cout << "Adaugat cu succes. ";
                std::cout << "\nMai adaugati un serviciu la acest specialist? (da/nu): ";
                std::cin >> altServiciu;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }while (altServiciu == "da" || altServiciu == "DA");


            //calcul durata totala
            int durataTotala = p.calculeazaDurataTotala();
            if (durataTotala > 0) {
                //selectare data
                int zi, luna, an;
                std::cout<< "\nData dorita (zi luna an): ";
                std::cin >>zi >>luna >> an;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                //cautare disponibilitate
                salonulMeu.afiseazaDisponibilitateAngajat( angAles.getNume(), zi, luna, an, durataTotala);

                std::cout<< "\nDoriti sa rezervati un interval? (da/nu): \n";
                std:: string confirm;
                std::cin >> confirm;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (confirm == "da" || confirm == "DA") {
                    int h,m;
                    std::cout<< "Ora start programare: \n";
                    std::cin >>h>>m;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    IntervalOrar inter = {h, m, 0, 0};
                    int minF = h * 60 + m +durataTotala;
                    inter.oraFinal = minF / 60;
                    inter.minutFinal = minF % 60;

                    if (salonulMeu.esteAngajatDisponibil(angAles, zi, luna, an, inter)) {
                        //actualizare programare
                        p.setData(zi, luna, an);
                        p.setInterval(inter);

                        std::cout<< "Observatii: \n";
                        std::string obs;
                        std:: getline(std::cin, obs);
                        p.setObservatii(obs);

                        salonulMeu.adaugaProgramare(p);
                        std::cout<<"Rezervare confirmata! ";
                    } else {
                        std::cout<<"Intervalul nu mai este disponibil. ";
                    }
                }else {
                    std::cout <<"Doriti sa va programati la un alt specialist pentru aceste servicii?";
                    std::string vreauAltul;
                    std::getline(std::cin, vreauAltul);

                    if (vreauAltul == "da" || vreauAltul =="DA") {
                        std::cout<< "--- ALEGETI UN NOU SPECIALIST ---\n";
                        for (size_t i = 0; i < listaAngajati.size(); ++i) {
                            std::cout<< i + 1 << ". " << listaAngajati[i].getNume() << "\n";
                        }
                        int optiuneNoua;
                        std::cin >> optiuneNoua;
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                        angAles = listaAngajati[optiuneNoua - 1];
                        p.setStilist(angAles);

                        std::cout << "Disponibilitate " << angAles.getNume() << ".\n";
                        salonulMeu.afiseazaDisponibilitateAngajat(angAles.getNume(), zi, luna, an, durataTotala);
                    }
                }
            }

            std::cout<< "\nDoriti o alta programare pentru " << numeClient << "? (da/nu): ";
            std::cin >> raspunsAltaProgramare;
            std:: cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } while (raspunsAltaProgramare == "da" || raspunsAltaProgramare == "DA");

        std::cout<<"\nDoriti sa faceti o programare pentru alt client? (da/nu): ";
        std::cin >> raspunsClientNou;
    } while (raspunsClientNou =="da" || raspunsClientNou == "DA");

    std:: cout<<salonulMeu <<"\n";
    salonulMeu.afiseazaRaportZilnic();
    return 0;
}
