#include "game2.h"
#include "ui_game2.h"
#include <QMessageBox>
#include <QTimer>
//QTimer *timer;
Game2::Game2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Game2)
{
    ui->setupUi(this);
    ui->text_UserName->hide();
    ui->label_Username->hide();
    Local_Participant.clear();
    Remote_Participant.clear();
    generic_Symbol = "Z";
    setButtonState(false);

    //timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update(QPushButton *pPushButton_temp)));

    QMainWindow::show();
}
/*function FindMyNextPosition()
{
    if ((a==2)&&(b==2)&&(c== 0)&&(temp=="")) temp="C";
    if ((a==2)&&(b== 0)&&(c==2)&&(temp=="")) temp="B";
    if ((a== 0)&&(b==2)&&(c==2)&&(temp=="")) temp="A";
    if ((a==2)&&(d==2)&&(g== 0)&&(temp=="")) temp="G";
    if ((a==2)&&(d== 0)&&(g==2)&&(temp=="")) temp="D";
    if ((a== 0)&&(d==2)&&(g==2)&&(temp=="")) temp="A";
    if ((a==2)&&(e==2)&&(i== 0)&&(temp=="")) temp="I";
    if ((a==2)&&(e== 0)&&(i==2)&&(temp=="")) temp="E";
    if ((a== 0)&&(e==2)&&(i==2)&&(temp=="")) temp="A";
    if ((b==2)&&(e==2)&&(h== 0)&&(temp=="")) temp="H";
    if ((b==2)&&(e== 0)&&(h==2)&&(temp=="")) temp="E";
    if ((b== 0)&&(e==2)&&(h==2)&&(temp=="")) temp="B";
    if ((d==2)&&(e==2)&&(f== 0)&&(temp=="")) temp="F";
    if ((d==2)&&(e== 0)&&(f==2)&&(temp=="")) temp="E";
    if ((d== 0)&&(e==2)&&(f==2)&&(temp=="")) temp="D";
    if ((g==2)&&(h==2)&&(i== 0)&&(temp=="")) temp="I";
    if ((g==2)&&(h== 0)&&(i==2)&&(temp=="")) temp="H";
    if ((g== 0)&&(h==2)&&(i==2)&&(temp=="")) temp="G";
    if ((c==2)&&(f==2)&&(i== 0)&&(temp=="")) temp="I";
    if ((c==2)&&(f== 0)&&(i==2)&&(temp=="")) temp="F";
    if ((c== 0)&&(f==2)&&(i==2)&&(temp=="")) temp="C";
    if ((g==2)&&(e==2)&&(c== 0)&&(temp=="")) temp="C";
    if ((g==2)&&(e== 0)&&(c==2)&&(temp=="")) temp="E";
    if ((g== 0)&&(e==2)&&(c==2)&&(temp=="")) temp="G";
}

function FindHisNextPosition()
{
    if ((a==1)&&(b==1)&&(c==0)&&(temp=="")) temp="C";
    if ((a==1)&&(b==0)&&(c==1)&&(temp=="")) temp="B";
    if ((a==0)&&(b==1)&&(c==1)&&(temp=="")) temp="A";
    if ((a==1)&&(d==1)&&(g==0)&&(temp=="")) temp="G";
    if ((a==1)&&(d==0)&&(g==1)&&(temp=="")) temp="D";
    if ((a==0)&&(d==1)&&(g==1)&&(temp=="")) temp="A";
    if ((a==1)&&(e==1)&&(i==0)&&(temp=="")) temp="I";
    if ((a==1)&&(e==0)&&(i==1)&&(temp=="")) temp="E";
    if ((a==0)&&(e==1)&&(i==1)&&(temp=="")) temp="A";
    if ((b==1)&&(e==1)&&(h==0)&&(temp=="")) temp="H";
    if ((b==1)&&(e==0)&&(h==1)&&(temp=="")) temp="E";
    if ((b==0)&&(e==1)&&(h==1)&&(temp=="")) temp="B";
    if ((d==1)&&(e==1)&&(f==0)&&(temp=="")) temp="F";
    if ((d==1)&&(e==0)&&(f==1)&&(temp=="")) temp="E";
    if ((d==0)&&(e==1)&&(f==1)&&(temp=="")) temp="D";
    if ((g==1)&&(h==1)&&(i==0)&&(temp=="")) temp="I";
    if ((g==1)&&(h==0)&&(i==1)&&(temp=="")) temp="H";
    if ((g==0)&&(h==1)&&(i==1)&&(temp=="")) temp="G";
    if ((c==1)&&(f==1)&&(i==0)&&(temp=="")) temp="I";
    if ((c==1)&&(f==0)&&(i==1)&&(temp=="")) temp="F";
    if ((c==0)&&(f==1)&&(i==1)&&(temp=="")) temp="C";
    if ((g==1)&&(e==1)&&(c==0)&&(temp=="")) temp="C";
    if ((g==1)&&(e==0)&&(c==1)&&(temp=="")) temp="E";
    if ((g==0)&&(e==1)&&(c==1)&&(temp=="")) temp="G";
}*/

void Game2::update()
{
    //QPushButton *pPushButton_temp;
    QMessageBox msgBox;
    msgBox.setText("Timer Expired - " + Local_Participant.toHtmlEscaped() + " Won");
    msgBox.exec();
    //pPushButton_temp->setDisabled(true);
    //pPushButton_temp->setText("X");
    //ui->Button_Submit->click();
}

Game2::~Game2()
{
    delete ui;
}

void Game2::on_pushButton_clicked()
{
    if (ui->Button_Submit->text()=="Start" || ui->Button_Submit->text()=="Register")
    {
        QMessageBox msgBox;
        msgBox.setText("Complete the registration before playing :)");
        msgBox.exec();
        return;
    }
    ui->pushButton->setText(Local_Symbol.c_str());
    ui->pushButton->setDisabled(true);
    /*QMessageBox msgBox;
    msgBox.setText(ui->pushButton->text());
    msgBox.exec();*/
}
bool Game2::Registration()
{
    QString myUserName = ui->text_UserName->toPlainText();
    if (myUserName == "")
    {
        QMessageBox msgBox;
        msgBox.setText("Enter Username");
        msgBox.exec();
        ui->text_UserName->setFocus();
        return false;
    }
    QString myIdentity = myUserName;
    ui->list_Participants->addItem(myIdentity);
    Local_Participant.append(myIdentity);
    ui->text_UserName->hide();
    ui->label_Username->hide();
    return true;

    // TODO - Sankar - Call Sandeep Method with Player name
}

void Game2::on_Button_Submit_clicked()
{
    if(ui->Button_Submit->text()=="Start")
    {
        ui->text_UserName->show();
        ui->label_Username->show();
        ui->text_UserName->setFocus();
        ui->Button_Submit->setText("Register");
    }
    else if (ui->Button_Submit->text()=="Register")
    {

        if ( Registration())
        {
            ui->Button_Submit->setText("Play");
        }
        else
        {
            ui->Button_Submit->setText("Register");
        }
    }
    else if (ui->Button_Submit->text()=="Play")
    {
        setButtonState(true);
        ui->Button_Submit->setText("Submit");
    }
    else
    {
        // Catch the PushButton Contents and send to Lower Layers.

        string szBoardInfo;
        getBoardInfo(szBoardInfo);
        QString BoardDetails = szBoardInfo.c_str();

        //timer->start(5000);

        QMessageBox msgBox;
        msgBox.setText("Board Contents " + BoardDetails.toHtmlEscaped());
        msgBox.exec();
        //QTimer::singleShot(5000, this, SLOT(update()));
        szPreviousState = szBoardInfo;

        /*QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(5000);*/
        QTimer::singleShot(5000, this, SLOT(update()));
    }
}

void Game2::setButtonState(bool bState)
{
    ui->pushButton->setEnabled(bState);
            ui->pushButton_2->setEnabled(bState);
            ui->pushButton_3->setEnabled(bState);
            ui->pushButton_4->setEnabled(bState);
            ui->pushButton_5->setEnabled(bState);
            ui->pushButton_6->setEnabled(bState);
            ui->pushButton_7->setEnabled(bState);
            ui->pushButton_8->setEnabled(bState);
            ui->pushButton_9->setEnabled(bState);
}

void Game2::getBoardInfo(string & szBoardInfo)
{
    szBoardInfo = getButtonStatus(ui->pushButton)   + getButtonStatus(ui->pushButton_2) +
                            getButtonStatus(ui->pushButton_3) + getButtonStatus(ui->pushButton_4) +
                            getButtonStatus(ui->pushButton_5) + getButtonStatus(ui->pushButton_6) +
                            getButtonStatus(ui->pushButton_7) + getButtonStatus(ui->pushButton_8) +
                            getButtonStatus(ui->pushButton_9);

}

string Game2::getButtonStatus(QPushButton *pPushButton_temp)
{
    if (pPushButton_temp->text() == "X")
    {
        return "X";
    }
    else if (pPushButton_temp->text() == "O")
    {
        return "O";
    }
    else
    {
        return "Z";
    }
}

void Game2::on_Button_EndGame_clicked()
{
    //ui->label_IP->
    /*QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.exec();*/
    QMainWindow::close();
}
