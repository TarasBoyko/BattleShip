#ifndef OPPONENTOFUSER_H
#define OPPONENTOFUSER_H

#include <QPoint>
#include <vector>
#include "cell.h"
#include "shipfinisher.h"
#include "abstractfield.h"

class UsersField;
using std::vector;

class OpponentOfUser
{
public:
    void MakeMove();
    OpponentOfUser(UsersField* newUsersField, const CellMatrix &new_CellMatrix);

protected:
    bool is_wounded_ships; // on users field
    UsersField* const users_field_;
    std::vector<QPoint> empty_cells_on_users_field_;
    ShipFinisher ship_finisher_;
    bool Shoot();
    void ReadEmptyCells();


};

#endif // OPPONENTOFUSER_H
