#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"
#include "usersfield.h"
#include "computersfield.h"
#include "ship.h"
#include "opponentofuser.h"

void MainWindow::UserWon(const bool is_user_won)
{
    CF_->InstallEventFilterOnCells(false, CF_);
    CF_->InstallEventFilterOnCells(true, this);
    if ( is_user_won )
    {
        ui->you_won_label_->show();
    }
    else
    {
        ui->you_lose_label_->show();
    }
    ui->play_again_button_->show();
    ui->exit_button_->show();
}

void MainWindow::SetVisiableStartGameButton(const bool flag)
{
    ui->start_game_button_->setVisible(flag);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon( QIcon("icon.icon") );
    setWindowTitle("BattleShip");
    setFixedSize(this->size());
    StartGamePart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartGamePart()
{
    ui->you_won_label_->hide();
    ui->you_lose_label_->hide();
    ui->play_again_button_->hide();
    ui->exit_button_->hide();
    SetVisiableStartGameButton(false);

    UF_ = new UsersField(this, 30, 30, this);
    UF_->show();
    opponent_ = new OpponentOfUser(UF_, UF_->GetCellMatrix());
    CF_ = new ComputersField(this, 400, 30, opponent_, this);
    CF_->show();

    UF_->AddOwnShip( new Ship(this, 1, Ship::DO::HORIZONTAL, QPoint(30,350), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 1, Ship::DO::HORIZONTAL, QPoint(30,350), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 1, Ship::DO::HORIZONTAL, QPoint(30,350), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 1, Ship::DO::HORIZONTAL, QPoint(30,350), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 2, Ship::DO::HORIZONTAL, QPoint(30,400), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 2, Ship::DO::HORIZONTAL, QPoint(30,400), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 2, Ship::DO::HORIZONTAL, QPoint(30,400), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 3, Ship::DO::HORIZONTAL, QPoint(30,450), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 3, Ship::DO::HORIZONTAL, QPoint(30,450), UF_, CF_) );
    UF_->AddOwnShip( new Ship(this, 4, Ship::DO::HORIZONTAL, QPoint(30,500), UF_, CF_) );

    CF_->PutShips();
    CF_->InstallEventFilterOnCells(true, this);
    show();
}

void MainWindow::FinishGamePart()
{

    delete opponent_;
    delete UF_;
    delete CF_;
    opponent_ = nullptr;
    UF_ = nullptr;
    CF_ = nullptr;

    // delete other widgets
    for ( auto w : findChildren<QWidget*>() )
    {
        if ( ! ( w->parent() == this ) )
        {
            continue;
        }
        if ( w->objectName() == "_layout" ||
             w->objectName() == "qt_rubberband" ||
             w->objectName() == "centralWidget" ||
             w->objectName() == "pushButton"
             )
        {
            continue;
        }
        delete w;
        w = nullptr;
    }
}

bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if ( event->type() == QEvent::MouseButtonRelease && dynamic_cast<Cell*>(target) != nullptr )
    {
        QApplication::beep();
        return true;
    }
    return false;
}

void MainWindow::on_play_again_button__clicked()
{
    FinishGamePart();
    StartGamePart();
}


void MainWindow::on_exit_button__clicked()
{
    exit(0);
}

void MainWindow::on_start_game_button__clicked()
{
    // user can shoot into ComputersField
    CF_->InstallEventFilterOnCells(false, this);
    CF_->InstallEventFilterOnCells(true, CF_);
    UF_->InstallIgnoreFilterOnOwnShips();
    CF_->InstallIgnoreFilterOnOwnShips();
    SetVisiableStartGameButton(false);
    UF_->SetItselfParentToOwnShips();
}
