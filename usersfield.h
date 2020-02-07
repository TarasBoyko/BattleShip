#ifndef USERSFIELD_H
#define USERSFIELD_H

#include <QWidget>
#include <QDragEnterEvent>

#include "abstractfield.h"

class UsersField : public AbstractField
{
public:
    UsersField(QWidget* parent, const int& LT_x, const int& LT_y, MainWindow* new_main_window);
    virtual bool ShootIntoCell(const int& X, const int& Y) override;

private:
    bool was_killed_ship_;
    bool was_wounded_ship_;
    virtual void PutShipOnCells(Ship &ship, const int &x_coord, const int &y_coord) override;
    virtual void TakeOffShipFromCells(Ship &ship, const int& x_coord, const int& y_coord);
};

#endif // USERSFIELD_H
