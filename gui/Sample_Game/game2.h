#ifndef GAME2_H
#define GAME2_H

#include <QMainWindow>
#include <string.h>
#include <QPushButton>

using namespace std;

namespace Ui {
class Game2;
}

class Game2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game2(QWidget *parent = 0);
    ~Game2();

private slots:

    void on_pushButton_clicked();

    void on_Button_Submit_clicked();

    void on_Button_EndGame_clicked();

    void update();

    bool Registration();

    string getButtonStatus(QPushButton *pPushButton_temp);

    void getBoardInfo(string & szBoardInfo);

    void setButtonState(bool bState);

private:
    Ui::Game2 *ui;
    QString Local_Participant;
    QString Remote_Participant;
    string Local_Symbol;
    string Remote_Symbol;
    string generic_Symbol;
    string szPreviousState;
};

#endif // GAME2_H
