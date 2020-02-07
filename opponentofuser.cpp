#include "opponentofuser.h"
#include "abstractfield.h"
#include "usersfield.h"
#include <QDebug>

void OpponentOfUser::MakeMove()
{
    while ( Shoot() )
    {
        if ( users_field_->GetNumberShips() == 0 )
        {
            break;
        }
    }
}

OpponentOfUser::OpponentOfUser(UsersField* newUsersField, const CellMatrix& new_CellMatrix) :
    users_field_(newUsersField),
    ship_finisher_(new_CellMatrix)
{
    is_wounded_ships = false;
    empty_cells_on_users_field_.reserve(kNumberSquaresInRow * kNumberSquaresInRow);
}

// read empty (accoding to image, but not state) cells from UsersField
void OpponentOfUser::ReadEmptyCells()
{
    int i;
    int j;
    empty_cells_on_users_field_.clear();

    for ( j = 0; j < kNumberSquaresInRow; j++ )
    {
        for ( i = 0; i < kNumberSquaresInRow; i++ )
        {
            if ( users_field_->GetCell(i, j).GetImage() == Cell::SE::EMPTY )
            empty_cells_on_users_field_.push_back( users_field_->GetCell(i, j).GetCoord() );
        }
    }
}

bool OpponentOfUser::Shoot()
{
    QPoint point;
    ReadEmptyCells();
    if ( is_wounded_ships )
    {
        point = ship_finisher_.FinishShip();
    }
    else
    {
        int point_index = rand() % empty_cells_on_users_field_.size();
        point = empty_cells_on_users_field_[point_index];
    }

    bool  hit = users_field_->ShootIntoCell( point.x(), point.y() );
    // if it is not hit not change shooting strategy
    // if it is hit check shooting strategy
    if ( hit )
    {
        if ( !is_wounded_ships )
        {
            ship_finisher_.FinishShip(point);
            is_wounded_ships = true;
        }
    }

    if ( hit && !users_field_->IsKilledShip( users_field_->GetCellMatrix()[point.x()][point.y()]) )
    {

        is_wounded_ships = true;
    }
    if ( hit && users_field_->IsKilledShip( users_field_->GetCellMatrix()[point.x()][point.y()]) )
    {
        is_wounded_ships = false;
    }
    return hit;
}

