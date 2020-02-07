#include "abstractfield.h"
#include <QApplication>
#include <QLine>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QDebug>
#include <string>
#include <iostream>
#include <stdio.h>
using std::string;

AbstractField::AbstractField(QWidget* parent, const int& LT_x, const int& LT_y, MainWindow* new_main_window) :
    QWidget(parent)
{
    int i;
    int j;

    main_window_ = new_main_window;
    DrawField(parent, LT_x, LT_y);
    // put cells on AbstractField
    for ( i = 0; i < kNumberSquaresInRow; i++ )
    {
        for ( j = 0; j < kNumberSquaresInRow; j++ )
        {
            cell_matrix_[i][j].PutOnAbstractField(this, i, j);
        }
    }
    number_ships_ = 0;
}

void AbstractField::SetShip(Ship &newShip, const int& x_coord, const int& y_coord)
{
    QPoint new_coordinate;
    new_coordinate.rx() = x_coord*(kSquareLength + kLineWidth) + kLineWidth;
    new_coordinate.ry() = y_coord*(kSquareLength + kLineWidth) + kLineWidth;
    newShip.move( mapTo(parentWidget(), new_coordinate) );
    PutShipOnCells(newShip, x_coord, y_coord);
}

// convert ship's coordinates to cell coordinates relate to AbstractField
QPoint AbstractField::ShipToCellCoordinate(const Ship &ship) const
{
    QPoint point_in_abstract_field_coord = ship.pos() - pos();
    QPoint cell_coord;
    cell_coord.rx() = floor(((double)point_in_abstract_field_coord.x() - kLineWidth) / (kSquareLength + kLineWidth) + 0.5);
    cell_coord.ry() = floor(((double)point_in_abstract_field_coord.y() - kLineWidth) / (kSquareLength + kLineWidth) + 0.5);
    return cell_coord;
}

// if it is possibility to put ship return true
// else return false
bool AbstractField::CanPutShip(const QPoint &LT_angle, const int &number_decks, const Ship::DO &orientation) const
{
    int i;
    int j;
    QPoint RB_angle;
    // define right bottom angle of ship
    if ( orientation == Ship::DO::HORIZONTAL )
    {
        RB_angle.rx() = LT_angle.x() + number_decks - 1;
        RB_angle.ry() = LT_angle.y();
    }
    else
    {
        RB_angle.rx() = LT_angle.x();
        RB_angle.ry() = LT_angle.y() + number_decks - 1;
    }
    // if ship contour is out of AbstractField return false
    // else return true
    if ( LT_angle.x() < 0 ||
         LT_angle.y() < 0 ||
         RB_angle.x() >= kNumberSquaresInRow ||
         RB_angle.y() >= kNumberSquaresInRow )
    {
        return false;
    }
    // if other ships position allows to put ship return true
    // else return false
    for ( i = LT_angle.x() - 1; i <= RB_angle.x() + 1; i++ )
    {
        for ( j = LT_angle.y() - 1; j <= RB_angle.y() + 1; j++ )
        {
            if ( i >= 0 && j >= 0 && i <= 9 && j <= 9 )
            {
                if ( cell_matrix_[i][j].GetState() == Cell::SE::DECK )
                {
                    return false;
                }
            }
        }
    }
    return true;
}

void AbstractField::PutShipOnCells(Ship &ship, const int &x_coord, const int &y_coord)
{
    number_ships_++;
    SetShipOnCellsAsSE(ship, x_coord, y_coord, Cell::SE::DECK);
}

void AbstractField::TakeOffShipFromCells(Ship &ship, const int &x_coord, const int &y_coord)
{
    number_ships_--;
    SetShipOnCellsAsSE(ship, x_coord, y_coord, Cell::SE::EMPTY);
}

const Cell &AbstractField::GetCell(const int &X, const int &Y)
{
    return cell_matrix_[X][Y];
}

// if at the intersection X and Y is unharmed deck return true
// else return false

bool AbstractField::ShootIntoCell(const int &X, const int &Y)
{
    if ( cell_matrix_[X][Y].GetState() == Cell::SE::DECK )
    {
        cell_matrix_[X][Y].SetImage(Cell::SE::WOUNDED_DECK);
        cell_matrix_[X][Y].SetState(Cell::SE::WOUNDED_DECK);
        cell_matrix_[X][Y].raise();
        if ( IsKilledShip(cell_matrix_[X][Y]) )
        {
            number_ships_--;
            PutDotsAroundKilledShip(cell_matrix_[X][Y]);
        }
        return true;
    }
    // if user shooting into empty cell
    if ( cell_matrix_[X][Y].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[X][Y].SetImage(Cell::SE::POINT);
        cell_matrix_[X][Y].SetState(Cell::SE::POINT);
        return false;
    }
    return false;
}

void AbstractField::AddOwnShip(Ship *new_ship)
{
    own_ships_.push_back(new_ship);
}

int AbstractField::GetNumberShips() const
{
    return number_ships_;
}

const vector<Ship *> &AbstractField::GetOwnShips() const
{
    return own_ships_;
}

// own ships will not react to events associated with mouse
void AbstractField::InstallIgnoreFilterOnOwnShips()
{
    for ( Ship* i : own_ships_ )
    {
        i->installEventFilter(this);
    }
}

void AbstractField::SetItselfParentToOwnShips()
{
    for ( Ship* i : own_ships_ )
    {
        QPoint coord = i->mapToGlobal(QPoint(0,0));
        i->setParent(this);
        i->move( coord - mapToGlobal(QPoint(0,0)) );
        i->show();
    }
}

const CellMatrix &AbstractField::GetCellMatrix() const
{
    return cell_matrix_;
}

// set need size and mark the rows and columns
void AbstractField::DrawField(QWidget *parent, const int &LT_x, const int &LT_y)
{
    int i;
    QLabel* label;

    setGeometry(LT_x,
                LT_y,
                (kNumberSquaresInRow + 1)*kLineWidth + kNumberSquaresInRow*kSquareLength,
                (kNumberSquaresInRow + 1)*kLineWidth + kNumberSquaresInRow*kSquareLength);
    for (QString ch : {"A","B","C","D","E","F","G","H","I","J"} )
    {
        label = new QLabel(parent);
        label->setGeometry((pos().x() ) + (ch.toStdString()[0] - 'A')*(kSquareLength+kLineWidth) + kLineWidth,
                            pos().y() - 25,
                            25,
                            25);
        label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; font-weight:600;\">" +
                       ch +
                       "</span></p></body></html>");
        label->show();
    }
    for ( i = 0; i < kNumberSquaresInRow; i++ )
    {
        label = new QLabel(parent);
        label->setGeometry(pos().x() - 25,
                           pos().y() + (i)*(kSquareLength+kLineWidth) + kLineWidth,
                           25,
                           25);
        label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; font-weight:600;\">" +
                       QString::number(i+1) +
                       "</span></p></body></html>");
        label->show();
    }
}

void AbstractField::SetShipOnCellsAsSE(const Ship &ship, const int &x_coord, const int &y_coord, const Cell::SE new_state)
{
    int i;
    if ( ship.GetOrientation() == Ship::DO::HORIZONTAL )
    {
        for ( i = 0; i < ship.GetNumberDecks(); i++ )
        {
            cell_matrix_[x_coord + i][y_coord].SetState(new_state);
        }
    }
    else
    {
        for ( i = 0; i < ship.GetNumberDecks(); i++ )
        {
            cell_matrix_[x_coord][y_coord + i].SetState(new_state);
        }
    }
}

// define if ship, that has this Cell(parameter of function) is killed
bool AbstractField::IsKilledShip(const Cell &wounded_deck)
{
    int i;
    for ( i = 0; cell_matrix_[wounded_deck.X() - i][wounded_deck.Y()].GetState() == Cell::SE::WOUNDED_DECK; )
    {
        i++;
        if ( wounded_deck.X() - i < 0 )
        {
            i--;
            break;
        }
    }
    if ( cell_matrix_[wounded_deck.X() - i][wounded_deck.Y()].GetState() == Cell::SE::DECK )
    {
        return false;
    }

    for ( i = 0; cell_matrix_[wounded_deck.X() + i][wounded_deck.Y()].GetState() == Cell::SE::WOUNDED_DECK; )
    {
        i++;
        if ( wounded_deck.X() + i == kMaxNumberShipOnAbstractField )
        {
            i--;
            break;
        }
    }
    if ( cell_matrix_[wounded_deck.X() + i][wounded_deck.Y()].GetState() == Cell::SE::DECK )
    {
        return false;
    }

    for ( i = 0; cell_matrix_[wounded_deck.X()][wounded_deck.Y() + i].GetState() == Cell::SE::WOUNDED_DECK; )
    {
        i++;
        if ( wounded_deck.Y() + i == kMaxNumberShipOnAbstractField )
        {
            i--;
            break;
        }
    }
    if ( cell_matrix_[wounded_deck.X()][wounded_deck.Y() + i].GetState() == Cell::SE::DECK )
    {
        return false;
    }

    for ( i = 0; cell_matrix_[wounded_deck.X()][wounded_deck.Y() - i].GetState() == Cell::SE::WOUNDED_DECK; )
    {
        i++;
        if ( wounded_deck.Y() - i < 0 )
        {
            i--;
            break;
        }
    }
    if ( cell_matrix_[wounded_deck.X()][wounded_deck.Y() - i].GetState() == Cell::SE::DECK )
    {
        return false;
    }
    return true;
}

// Put dots around killed ship, that has this Cell(parameter of function).
// Coordinates of cell should be valid.
void AbstractField::PutDotsAroundKilledShip(const Cell &cell)
{
    int i;
    for ( i = 0; cell.X() + i < kNumberSquaresInRow && cell_matrix_[cell.X() + i][cell.Y()].GetState() == Cell::SE::WOUNDED_DECK; i++ )
    {
        PutDotsOnEmptyCellsAroundCell( cell_matrix_[cell.X() + i][cell.Y()] );
    }

    for ( i = 0; cell.Y() + i < kNumberSquaresInRow && cell_matrix_[cell.X()][cell.Y() + i].GetState() == Cell::SE::WOUNDED_DECK; i++ )
    {
        PutDotsOnEmptyCellsAroundCell( cell_matrix_[cell.X()][cell.Y() + i] );
    }

    for ( i = 0; cell.X() - i >= 0 && cell_matrix_[cell.X() - i][cell.Y()].GetState() == Cell::SE::WOUNDED_DECK; i++ )
    {
        PutDotsOnEmptyCellsAroundCell( cell_matrix_[cell.X() - i][cell.Y()] );
    }

    for ( i = 0; cell.Y() - i >= 0 && cell_matrix_[cell.X()][cell.Y() - i].GetState() == Cell::SE::WOUNDED_DECK; i++ )
    {
        PutDotsOnEmptyCellsAroundCell( cell_matrix_[cell.X()][cell.Y() - i] );
    }
}

bool AbstractField::eventFilter(QObject *target, QEvent *event)
{
    if ( ( event->type() == QEvent::MouseButtonPress ||
           event->type() == QEvent::MouseMove ||
           event->type() == QEvent::MouseButtonRelease )
         && dynamic_cast<Ship*>(target) != nullptr )
    {
        return true;
    }
    return true;
}

// coordinates of cell should be valid
void AbstractField::PutDotsOnEmptyCellsAroundCell(const Cell &cell)
{
    if ( cell.X() - 1 >= 0 && cell.Y() - 1 >= 0 && cell_matrix_[cell.X() - 1][cell.Y() - 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() - 1][cell.Y() - 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() - 1][cell.Y() - 1].SetImage(Cell::SE::POINT);
    }
    if ( cell.Y() - 1 >= 0 && cell_matrix_[cell.X()][cell.Y() - 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X()][cell.Y() - 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X()][cell.Y() - 1].SetImage(Cell::SE::POINT);
    }
    if ( cell.X() + 1 < kNumberSquaresInRow && cell.Y() - 1 >= 0 && cell_matrix_[cell.X() + 1][cell.Y() - 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() + 1][cell.Y() - 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() + 1][cell.Y() - 1].SetImage(Cell::SE::POINT);
    }
    if ( cell.X() - 1 >= 0 && cell_matrix_[cell.X() - 1][cell.Y()].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() - 1][cell.Y()].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() - 1][cell.Y()].SetImage(Cell::SE::POINT);
    }
    if ( cell.X() + 1 < kNumberSquaresInRow && cell_matrix_[cell.X() + 1][cell.Y()].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() + 1][cell.Y()].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() + 1][cell.Y()].SetImage(Cell::SE::POINT);
    }
    if ( cell.X() - 1 >= 0 && cell.Y() + 1 < kNumberSquaresInRow && cell_matrix_[cell.X() - 1][cell.Y() + 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() - 1][cell.Y() + 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() - 1][cell.Y() + 1].SetImage(Cell::SE::POINT);
    }
    if ( cell.Y() + 1 < kNumberSquaresInRow && cell_matrix_[cell.X()][cell.Y() + 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X()][cell.Y() + 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X()][cell.Y() + 1].SetImage(Cell::SE::POINT);
    }
    if ( cell.X() + 1 < kNumberSquaresInRow && cell.Y() + 1 < kNumberSquaresInRow && cell_matrix_[cell.X() + 1][cell.Y() + 1].GetState() == Cell::SE::EMPTY )
    {
        cell_matrix_[cell.X() + 1][cell.Y() + 1].SetState(Cell::SE::POINT);
        cell_matrix_[cell.X() + 1][cell.Y() + 1].SetImage(Cell::SE::POINT);
    }
}

// draw grid
void AbstractField::paintEvent(QPaintEvent *)
{
    int i;
    QPainter* painer = new QPainter(this);
    QPen p;

    p.setWidth(kLineWidth);
    painer->setPen(p);
    for ( i = 0; i < kNumberSquaresInRow + 1; i++ )
    {
        painer->drawLine(0,
                         kLineWidth/2 + i*(kSquareLength+kLineWidth),
                         kNumberSquaresInRow*(kLineWidth + kSquareLength),
                         kLineWidth/2 + i*(kSquareLength+kLineWidth) );
    }
    for ( i = 0; i < kNumberSquaresInRow + 1; i++ )
    {
        painer->drawLine(kLineWidth/2 + i*(kSquareLength+kLineWidth) ,
                         0,
                         kLineWidth/2 + i*(kSquareLength+kLineWidth),
                         (kNumberSquaresInRow+1)*kLineWidth + kNumberSquaresInRow*kSquareLength);
    }
    painer->end();
}

