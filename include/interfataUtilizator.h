
#ifndef INTERFATA_UTILIZATOR_H
#define INTERFATA_UTILIZATOR_H

#include "Salon.h"
class InterfataUtilizator {
private:
    void curataBuffer() const;
    void executaModClient(Salon& salon);
    void executaModManager(Salon& salon);
public:
    void ruleaza(Salon& salon);
};
#endif