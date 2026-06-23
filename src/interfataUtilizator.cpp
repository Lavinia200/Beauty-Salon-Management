#include "InterfataUtilizator.h"
#include <iostream>
#include <limits>

void InterfataUtilizator::curataBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void InterfataUtilizator::ruleaza(Salon& salon) {
    int optiunePrincipala = 0;
    do {
        std::cout << "\n==============================================";
        std::cout << "\nSISTEM GESTIUNE SALON - " << "BEAUTY LAB";
        std::cout << "\n==============================================";
        std::cout << "\n[1] CLIENT";
        std::cout << "\n[2] MANAGER";
        std::cout << "\n[3] IESIRE APLICATIE";
        std::cout << "\n==============================================";
        std::cout << "\nAlegeti tipul de cont: ";

        if (!(std::cin >> optiunePrincipala)) {
            curataBuffer();
            continue;
        }
        if (optiunePrincipala == 1) {
            executaModClient(salon);
        } else if (optiunePrincipala == 2) {
            executaModManager(salon);
        }
    } while (optiunePrincipala != 3);

    std::cout << "\nSistemul s-a inchis. O zi buna!\n";
}
void InterfataUtilizator::executaModClient(Salon& salon) {
    std::string numeClient;
    std::cout << "\n--- INTERFATA CLIENT ---";
    std::cout << "\nIntroduceti numele dvs: ";
    if (std:: cin.peek() == '\n') std::cin.ignore();
    std:: getline(std::cin, numeClient);

    std::string raspunsAltaProgramare;
    do {
        try {
            std::cout << "\n--- SPECIALISTI DISPONIBILI ---\n";
            const auto& listaAngajati = salon.getAngajati();
            int contor = 1;
            for (const auto& ang : listaAngajati ) {
                std:: cout << contor++ << ". " << ang-> getNume()
                << " [" <<  ang->getSpecializare() << " - " << ang->getGrad() << "]\n";

            }

            int optiuneAng;
            std::cout<< "Alegeti specialistul dorit: ";
            while (!(std::cin >> optiuneAng) || optiuneAng < 1 || optiuneAng > (int)listaAngajati.size()) {
                std::cout<< "Selectie invalida.Reincercati: ";
                curataBuffer();
            }
            curataBuffer();

            const Angajat& angAles = *(listaAngajati[optiuneAng - 1]);
            IntervalOrar intervalGol = {0,0,0,0};
            Programare p(numeClient, angAles, 0,0,0,intervalGol);

            std::string altServiciu;
            do {
                std::cout <<"\n--- SERVICII PRESTATE DE " << angAles.getNume() << " ---\n";
                std::vector <int> indiciFiltrati;
                int contorS = 1;
                const auto& catalogSalon = salon.getCatalog();
                for (size_t i = 0; i < catalogSalon.size(); ++i) {
                    if (angAles.poatePresta(catalogSalon[i].getTip())) {
                        std::cout << contorS << ". " <<std::left <<std::setw(20) << catalogSalon[i].getNume()
                        << " | " << catalogSalon[i].getDurata() << " min | " <<catalogSalon[i].getPret() <<" RON";
                        if (catalogSalon[i].esteComplex()) {
                            std::cout << " [Serviciu complex]";
                        }
                        std::cout<< "\n";
                        indiciFiltrati.push_back(i);
                        contorS++;
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
                    curataBuffer();
                }
                p.adaugaServiciu(catalogSalon[indiciFiltrati[alegereS - 1]]);

                std::cout << "Adaugat cu succes. ";
                std::cout << "\nMai adaugati un serviciu la acest specialist? (da/nu): ";
                std::cin >> altServiciu;
                curataBuffer();
            }while (altServiciu == "da" || altServiciu == "DA");

            //calcul durata totala
            int durataTotala = p.calculeazaDurataTotala();
            if (durataTotala > 0) {
                //selectare data
                int zi, luna, an;
                std::cout<< "\nData dorita (zi luna an): ";
                std::cin >>zi >>luna >> an;
                curataBuffer();

                if (zi <  1 || zi > 31 || luna < 1 || luna > 12 || an < 2026 ||
                    (an == 2026 && luna < 5) ||
                    (an == 2026 && luna == 5 && zi <25 )) {
                    throw DataInvalidaException(zi, luna, an);
                }

                //cautare disponibilitate
                salon.afiseazaDisponibilitateAngajat( angAles.getNume(), zi, luna, an, durataTotala);

                std::cout<< "\nDoriti sa rezervati un interval? (da/nu): \n";
                std:: string confirm;
                std::cin >> confirm;
                curataBuffer();

                if (confirm == "da" || confirm == "DA") {
                    int h,m;
                    std::cout<< "Ora start programare: \n";
                    std::cin >>h>>m;
                    curataBuffer();

                    IntervalOrar inter = {h, m, 0, 0};
                    int minF = h * 60 + m +durataTotala;
                    inter.oraFinal = minF / 60;
                    inter.minutFinal = minF % 60;

                    if (salon.esteAngajatDisponibil(angAles, zi, luna, an, inter)) {
                        //actualizare programare
                        p.setData(zi, luna, an);
                        p.setInterval(inter);

                        std::cout<< "Observatii: \n";
                        std::string obs;
                        std:: getline(std::cin, obs);
                        p.setObservatii(obs);

                        salon.adaugaProgramare(p);
                        std::cout<<"Rezervare confirmata pentru " << p.getNumeClient() << std::endl;
                    } else {
                        throw SuprapunereIntervalException(angAles.getNume(), h, m);
                    }
                }
            }
        }
        catch (const SalonException& e) {
            std::cout << "\n [OPERATIE RESPINSA] " << e.what() << "\n";
            std::cout << "Rezervarea curenta a fost abandonata.\n";
        }

        std::cout<< "\nDoriti o alta programare? (da/nu): ";
        std::cin >> raspunsAltaProgramare;
        curataBuffer();
    } while (raspunsAltaProgramare == "da" || raspunsAltaProgramare == "DA");
}
void InterfataUtilizator::executaModManager(Salon& salon) {
    int optiuneManager = 0;
    do {
        std::cout << "\n----------------------------------------------";
        std::cout << "\n PANOU DE CONTROL MANAGER";
        std::cout << "\n----------------------------------------------";
        std::cout << "\n[1] Vizualizare programari dupa o anumita zi";
        std::cout << "\n[2] Vizualizare raport financiar pe o luna";
        std::cout << "\n[3] Vizualizare lista salarii personal";
        std::cout << "\n[4] Promovare angajat";
        std::cout << "\n[5] Schimbare Manager General";
        std::cout << "\n[6] Angajare personal nou (adaugare in sistem)";
        std::cout << "\n[7] Afisare raport general salon";
        std::cout << "\n[8] Modificare profil angajat";
        std::cout << "\n[9] Inapoi la meniul principal";
        std::cout << "\n----------------------------------------------";
        std::cout << "\nSelectati actiunea: ";

        if (!(std::cin >> optiuneManager)) {
            curataBuffer();
            continue;
        }

        if (optiuneManager == 1) {
            int z, l, a;
            std::cout<< "Introduceti data cautata (zi luna an): ";
            std::cin >> z >> l >> a;
            curataBuffer();
            salon.afiseazaProgramariDupaZi(z, l , a);
        }
        else if ( optiuneManager == 2) {
            int l, a;
            std::cout << "Intrduceti luna si anul: ";
            std::cin >>l >> a;
            curataBuffer();
            salon.afiseazaProgramariDupaLuna(l, a);
        }
        else if (optiuneManager == 3) {
            salon.vizualizeazaSalarii();
            std::cout << "\nNumarul total de angajati activi in memorie: " << Angajat::getNumarTotalAngajati() << "\n";
        }
        else if (optiuneManager == 4) {
            std::string numePromovare;
            std::cout << "Introduceti numele Juniorului pe care vreti sa-l promovati: ";
            if (std::cin.peek() == '\n') std::cin.ignore();
            std::getline(std::cin, numePromovare);

            if (salon.upgradeAngajatLaSenior(numePromovare)) {
                std::cout << numePromovare << " a fost promovat la gradul de Stilist Senior\n";
            } else {
                std::cout << "Eroare: angajatul nu a fost gasit sau nu este inregistrat ca Junior.\n";
            }
        }
        else if (optiuneManager == 5) {
            std::string numeNouManager;
            double salariuNou;
            std::cout << "Numele noului Manager General:\n ";
            if (std::cin.peek() == '\n') std::cin.ignore();
            std::getline(std::cin, numeNouManager);
            std::cout << "Salariul fix stabilit pentru noul manager:\n ";
            std::cin >> salariuNou;
            curataBuffer();

            salon.schimbaManagerul(numeNouManager, salariuNou);
            std::cout << "Managerul a fost inlocuit cu succes. Noul Manager este " << numeNouManager << ".\n";
        }
        else if (optiuneManager == 6) {
            std::string numeN, specN;
            int tipA = 0;
            std::cout << "\nTip angajat (1 - Junior, 2 -Senior): ";
            std::cin >> tipA;
            curataBuffer();

            std::cout << "\nNume angajat: ";
            std::getline(std::cin, numeN);
            std::cout << "\nSpecializare: ";
            std::getline(std::cin, specN);

            std::shared_ptr<Angajat> angajatNou = nullptr;
            if (tipA == 1) {
                double tarif;
                std::cout<< "\nTarif orar baza (RON/ora): ";
                std::cin >> tarif; curataBuffer();
                angajatNou = std::make_shared<StilistJunior>(numeN, specN, tarif);
            } else {
                double comision;
                std::cout << "\nProcent comision: ";
                std::cin >> comision; curataBuffer();
                angajatNou = std::make_shared<StilistSenior>(numeN, specN, comision, 0);
            }
            std::string raspunsServiciu;
            std::cout<< "\nDoriti sa configurati un serviciu prestat de " << numeN << "? (da/nu): ";
            std::cin >> raspunsServiciu;
            curataBuffer();
            while (raspunsServiciu == "da" || raspunsServiciu == "DA") {
                std::string numeServ, catServ;
                double pretServ;
                int durataServ;

                std::cout << "\n-> Nume serviciu: ";
                std::getline(std::cin, numeServ);
                std::cout << "-> Pret serviciu (RON): ";
                std::cin >> pretServ;
                std::cout << "->\nDurata estimata (minute): ";
                std::cin >> durataServ;
                curataBuffer();
                std::cout << "\n-> Categoria: ";
                std::getline(std::cin, catServ);

                Serviciu s(numeServ, pretServ, durataServ, catServ);
                salon.adaugaServiciuInCatalog(s);

                if (!angajatNou->poatePresta(catServ)) {
                    angajatNou->adaugaCompetenta(catServ);
                }
                std::cout << "\nServiciul \"" << numeServ << "\" a fost adugat iin catalog sub categoria \"" << catServ << "\".\n";
                std::cout << "\nMai adaugati un alt serviciu pentru acest angajat? (da/nu): ";
                std::cin >> raspunsServiciu;
                curataBuffer();
            }
            std::string RaspunsCompExistenta;
            std::cout << "\nDoriti  sa-i asociati si o competenta existenta din catalog? (da/nu): ";
            std::cin >> RaspunsCompExistenta;
            curataBuffer();
            if ( RaspunsCompExistenta == "da" || RaspunsCompExistenta == "DA") {
                std::cout << "\nIntroduceti numele categoriei existente: ";
                std::string compE;
                std::getline(std::cin, compE);
                angajatNou->adaugaCompetenta(compE);
                std::cout << "\nCompetenta \"" << compE << "\" a fost adaugata!\n";
            }
            salon.adaugaAngajat(angajatNou);
            std::cout << " Angajatul " << numeN << " a fost adaugat cu succes in echipa.\n";
        }
        else if (optiuneManager == 7) {
            std::cout << "\n";
            salon.afiseazaRaportZilnic();
        }
        else if (optiuneManager == 8) {
            std::cout << "Introduceti numele angajatului pentru editare profil: ";
            std::string numeA;
            if (std::cin.peek() == '\n') std::cin.ignore();
            std::getline(std::cin, numeA);

            bool gasit = false;
            const auto& personal = salon.getAngajati();
            for (auto& ang : personal) {
                if (ang->getNume() == numeA) {
                    gasit = true;
                    std::cout << "\nCe detalii doriti sa modificati pentru " << numeA << "?\n";
                    std::cout << "1.Schimba Specializarea (In prezent: " << ang->getSpecializare() << ")\n";
                    std::cout << "2. Modifica atribute specifice rangului de " << ang->getGrad() << "\n";
                    std::cout << "Selectie: ";
                    int alegereModif;
                    std::cin>> alegereModif;
                    curataBuffer();

                    if (alegereModif == 1) {
                        std::cout << "Intrduceti noua specializare: ";
                        std::string nouaSpec;
                        std::getline(std::cin, nouaSpec);
                        ang->setSpecializare(nouaSpec);
                        std::cout << "Specializarea a fost actualizata!\n";
                    }
                    else if (alegereModif == 2) {
                        ang->editeazaProfilSpecifice();
                    }
                    break;
                }
            }
            if (!gasit){
                std::cout << "Angajatul cu numele " << numeA << " nu a fost gasit\n";
            }
        }
    } while (optiuneManager != 9);
}