#include "Salon.h"
#include "interfataUtilizator.h"
#include "sablonGestiune.h"
#include "logger.h"
#include "angajatFactory.h"


int main() {

    GestiuneColectie<std::string> istoricActiuni;
    istoricActiuni.adauga("Deschidere salon");
    istoricActiuni.adauga("Incarcare catalog servicii");
    GestiuneColectie<int> coduriPromotionale;
    coduriPromotionale.adauga(1024);
    coduriPromotionale.adauga(2048);

    afiseazaElementeGeneric("Istoric Evenimente", istoricActiuni.getElemente());
    afiseazaElementeGeneric("Coduri de Discount Active", coduriPromotionale.getElemente());
    Logger::getInstance().log("Aplicatia a pornit cu succes.");

    auto angajatNou = AngajatFactory::creeazaAngajat("Junior", "Simona", "Cosmetica");
    if (angajatNou != nullptr) {
        Logger::getInstance().log("Fabrica a generat un angajat nou: " + angajatNou->getNume());
    }
    Salon salonulMeu("Beauty Lab");

    salonulMeu.adaugaServiciuInCatalog(Serviciu("Tuns Barbati", 50.0, 30, "Tuns"));
    salonulMeu.adaugaServiciuInCatalog(Serviciu("Tuns Femei", 90.0, 60, "Tuns"));
    salonulMeu.adaugaServiciuInCatalog(Serviciu("Vopsit Total", 200.0, 120, "Vopsit"));
    salonulMeu.adaugaServiciuInCatalog(Serviciu("Tratament Facial", 150.0, 45, "Cosmetica"));
    salonulMeu.adaugaServiciuInCatalog(Serviciu("Manichiura clasica", 80.0, 60, "Manichiura"));
    salonulMeu.adaugaServiciuInCatalog(Serviciu("Tratament hidratare", 250.0, 150, "Tratament"));

    //adaugam personal
    auto a1 = std::make_shared<StilistJunior>("Andreea", "Cosmetician", 35.0);
    a1->adaugaCompetenta("Cosmetica");
    a1->adaugaCompetenta("Tratament");

    auto a2 = std::make_shared<StilistSenior>("Matei", "Hair-Stilist", 0.12, 2);
    a2->adaugaCompetenta("Tuns");
    a2->adaugaCompetenta("Vopsit");

    auto a3 = std::make_shared<StilistJunior>("Elena", "Manichiurista", 40.0);
    a3->adaugaCompetenta("Manichiura");

    auto m1 = std::make_shared<ManagerSalon>("Gabriela", 5000.0);

    salonulMeu.adaugaAngajat(a1);
    salonulMeu.adaugaAngajat(a2);
    salonulMeu.adaugaAngajat(a3);
    salonulMeu.adaugaAngajat(m1);

    InterfataUtilizator interfata;
    interfata.ruleaza(salonulMeu);

    return 0;
}
