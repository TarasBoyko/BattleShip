#ifndef COMPUTERSFIELD_H
#define COMPUTERSFIELD_H

#include <QWidget>
#include <QPoint>
#include "abstractfield.h"
class OpponentOfUser;

const int kMaxNumberShipOnComputersField = 10;

class ComputersField : public AbstractField
{
public:
    ComputersField(QWidget *parent, const int &LT_x, const int &LT_y, OpponentOfUser* new_opponent, MainWindow *new_main_window);
    void PutShips();
    void InstallEventFilterOnCells(const bool flag, QObject *filter_object);
protected:
    using AbstractField::SetItselfParentToOwnShips;
private:
    void GenerateRandomShip(QPoint& LT_deck, Ship::DO &orientation);
    vector<Ship*> PutDeckEqualShips(const int& number_ships, const int& number_decks);
    virtual bool ShootIntoCell(const int& X, const int& Y) override;
    bool eventFilter(QObject *target, QEvent *event);
    OpponentOfUser* opponent_; // opponent of user
};

#endif // COMPUTERSFIELD_H
