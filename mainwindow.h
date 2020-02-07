#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class StartGameButton;
class UsersField;
class ComputersField;
class OpponentOfUser;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void UserWon(const bool is_user_won);
    void SetVisiableStartGameButton(const bool flag);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    Ui::MainWindow *ui;
    OpponentOfUser* opponent_;
    UsersField* UF_;
    ComputersField* CF_;

    void StartGamePart();
    void FinishGamePart();
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void on_play_again_button__clicked();
    void on_exit_button__clicked();
    void on_start_game_button__clicked();
};

#endif // MAINWINDOW_H
