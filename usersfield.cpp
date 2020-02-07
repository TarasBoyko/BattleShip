#include "usersfield.h"
#include <QDebug>
#include <QDropEvent>
#include <QSound>
#include "mainwindow.h"

UsersField::UsersField(QWidget* parent, const int& LT_x, const int& LT_y, MainWindow* new_main_window) :
    AbstractField(parent, LT_x, LT_y, new_main_window)
{
    was_killed_ship_ = false;
    was_wounded_ship_ = false;
}

void UsersField::PutShipOnCells(Ship &ship, const int &x_coord, const int &y_coord)
{
    AbstractField::PutShipOnCells(ship, x_coord, y_coord);
    if ( number_ships_ == kMaxNumberShipOnAbstractField )
    {
        main_window_->SetVisiableStartGameButton(true);
    }
}

void UsersField::TakeOffShipFromCells(Ship &ship, const int &x_coord, const int &y_coord)
{
    AbstractField::TakeOffShipFromCells(ship, x_coord, y_coord);
    main_window_->SetVisiableStartGameButton(false);
}

bool UsersField::ShootIntoCell(const int &X, const int &Y)
{
    bool hit;
    hit = AbstractField::ShootIntoCell(X, Y);
    if ( hit )
    {
        if ( !was_killed_ship_ )
        {
            was_killed_ship_ = IsKilledShip(cell_matrix_[X][Y]);
        }
        if ( !IsKilledShip(cell_matrix_[X][Y]) )
        {
            was_wounded_ship_ = true;
        }
        if ( was_killed_ship_ )
        {
            QSound::play("killed_ship_sound.wav");//killed_ship_sound.wav
        }
        else
        {
            if ( was_wounded_ship_ )
            {
                QSound::play("rounded_ship_sound.wav");
            }
        }
    }
    else
    {

        was_killed_ship_ = false;
        was_wounded_ship_ = false;
    }
    if ( number_ships_ == 0 )
    {
        QSound::play("killed_ship_sound.wav");
        main_window_->UserWon(false);
    }
    return hit;
}
