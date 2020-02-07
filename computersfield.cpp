#include "computersfield.h"
#include "mainwindow.h"
#include <QDebug>
#include <typeinfo>
#include <QObject>
#include <QSound>
#include <QApplication>
#include "opponentofuser.h"
#include "shipfinisher.h"
ComputersField::ComputersField(QWidget* parent,
                               const int& LT_x,
                               const int& LT_y,
                               OpponentOfUser* new_opponent,
                               MainWindow* new_main_window) :
    AbstractField(parent, LT_x, LT_y, new_main_window)
{
    opponent_ = new_opponent;
}
    // generate random ship and put them on ComputerField
void ComputersField::PutShips()
{
    PutDeckEqualShips(1, 4);
    PutDeckEqualShips(2, 3);
    PutDeckEqualShips(3, 2);
    PutDeckEqualShips(4, 1);
    SetItselfParentToOwnShips();
    for ( Ship* i : own_ships_ )
    {
        i->hide();
    }
}

void ComputersField::InstallEventFilterOnCells(const bool flag, QObject* filter_object)
{
    if( flag )
    {
        for ( int j = 0; j < kNumberSquaresInRow; j++ )
        {
            for ( int i = 0; i < kNumberSquaresInRow; i++ )
            {
                cell_matrix_[i][j].installEventFilter(filter_object);
            }
        }
    }
    else
    {
        for ( int j = 0; j < kNumberSquaresInRow; j++ )
        {
            for ( int i = 0; i < kNumberSquaresInRow; i++ )
            {
                cell_matrix_[i][j].removeEventFilter(filter_object);
            }
        }
    }
}

void ComputersField::GenerateRandomShip(QPoint &LT_deck, Ship::DO& orientation)
{
    LT_deck.rx() = rand()%kNumberSquaresInRow;
    LT_deck.ry() = rand()%kNumberSquaresInRow;
    *(int*)&orientation = rand()%2;
}

vector<Ship*> ComputersField::PutDeckEqualShips(const int &number_ships, const int &number_decks)
{
    vector<Ship*> generated_ships;
    int number_placed_ships = 0;
    QPoint random_LT_deck;
    Ship::DO random_orientation;
    GenerateRandomShip(random_LT_deck, random_orientation);
    do
    {
        if ( CanPutShip(random_LT_deck, number_decks, random_orientation ) )
        {
            Ship* newShip = new Ship(parentWidget(), number_decks, random_orientation, random_LT_deck, this, nullptr);
            SetShip(*newShip, random_LT_deck.x(), random_LT_deck.y() );
            number_placed_ships++;
            generated_ships.push_back(newShip);
            AddOwnShip(newShip);
        }
        GenerateRandomShip(random_LT_deck, random_orientation);
    } while( number_placed_ships < number_ships );
    return generated_ships;
}

bool ComputersField::ShootIntoCell(const int &X, const int &Y)
{
    bool hit;
    hit = AbstractField::ShootIntoCell(X, Y);
    if ( hit )
    {
        if ( IsKilledShip(cell_matrix_[X][Y]) )
        {
            QSound::play("killed_ship_sound.wav");
        }
        else
        {
            QSound::play("rounded_ship_sound.wav");
        }
    }
    // take up killed ship to emphasize that he was killed
    if ( IsKilledShip( cell_matrix_[X][Y]) )
    {
        for ( Ship* i : own_ships_ )
        {
            if ( cell_matrix_[X][Y].geometry().intersects( i->geometry() ) )
            {
                i->show();
                break;
            }
        }
    }
    if ( number_ships_ == 0 )
    {
        main_window_->UserWon(true);
    }
    return hit;
}

bool ComputersField::eventFilter(QObject *target, QEvent *event)
{

    Cell* cell = dynamic_cast<Cell*>(target);
    if ( cell == NULL )
    {
        return false;
    }

    if ( event->type() == QEvent::MouseButtonRelease )
    {
        // if user try to shoot into POINT or WOUNDED_DECK will play error sound
        // and move will not take into consideration
        if ( cell->GetImage() == Cell::SE::POINT || cell->GetImage() == Cell::SE::WOUNDED_DECK )
        {
            QApplication::beep();
            return false;
        }
        bool hit = ShootIntoCell(cell->X(), cell->Y());

        if ( !hit )
        {
            opponent_->MakeMove();
            return false;
        }
    }
    return false;
}

