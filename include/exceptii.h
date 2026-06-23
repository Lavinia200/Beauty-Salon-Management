#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <string>
#include <exception>
#include <utility>


class SalonException : public std::exception {
protected:
    std::string mesaj;
public:
    explicit SalonException(std::string m) : mesaj(std::move(m)) {}
    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class DataInvalidaException : public SalonException {
public:
    DataInvalidaException(int z, int l, int a)
        : SalonException("Eroare de Validare : Data introdusa (" +
                         std::to_string(z) + "/" + std::to_string(l) + "/" + std::to_string(a) +
                         ") este invalida sau incorecta calendaristic !" ) {}
};

class SuprapunereIntervalException : public SalonException {
public:
    SuprapunereIntervalException (const std::string& numeAngajat, int h, int m)
        : SalonException("Eroare de Rezervare:  Stilistul " + numeAngajat +
            " este deja ocupat la ora " +  std::to_string(h) + ":" + (m==0 ? "00" : std::to_string(m)) + "!") {}
};

#endif