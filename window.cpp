#include "window.h"
#include "ui_window.h"
#include <QtWidgets>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<QKeyEvent>
#include<QPixmap>
#include<QMessageBox>
#include<QStringList>
#include<QTableWidget>
#include<fstream>

using namespace cv;
using namespace std;

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    //in a every fixed time,we will change image of the video .
    QObject::connect(&timeHeure, SIGNAL(timeout()), this, SLOT(updateImage()));
    // after the animation finishes,the hand control will be ended and the time wasted to shoot will be recorded and cleared.
    QObject::connect(ui->widget,SIGNAL(declenche(int)),this,SLOT(restart(int)));
    // to change the level
    QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(const QString &)),ui->widget,SLOT(changeLevelGame(const QString &)));
    // actions after clicking and releaseing these 3 buttons
    QObject::connect(ui->pushButton,SIGNAL(released()),this,SLOT(pushButtonReactToggle()));
    QObject::connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(pushButton2ReactToggle()));
    QObject::connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(pushButton3ReactToggle()));

    QObject::connect(ui->widget,SIGNAL(timeTorecord()),this,SLOT(record()));

    // set names of horizontal headers
    QStringList list;
    list<<"Score"<<"Player name";
    ui->tableWidget_2->setHorizontalHeaderLabels(list);
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget_3->setHorizontalHeaderLabels(list);

    //load the record document
    QString name=QString("%1/trebuchet.txt").arg(QDir::homePath());
    ifstream is(name.toStdString().c_str());
    if (is.good())
    {
        chargerOneTable(is,ui->tableWidget);
        chargerOneTable(is,ui->tableWidget_2);
        chargerOneTable(is,ui->tableWidget_3);
    }



    setFocusPolicy(Qt::StrongFocus);

    //initialize variables
    pressed=false;
    time=QTime(0,0,0,0);
    second=0;
    secondTarget=0;
    aboutLast=5;//diplaying the information about the version will last 5 seconds
    start=false;
    flagAbout=false;

    // set a timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    //set frame of the video and template
    int frameWidth;
    int frameHeight;
    if(videoCap.open(0))
    {
        //frameWidth=videoCap.get(CV_CAP_PROP_FRAME_WIDTH);
        //frameHeight=videoCap.get(CV_CAP_PROP_FRAME_HEIGHT);
       frameWidth=320;
       frameHeight=240;
//        cout<<"width:"<<frameWidth<<endl;
//        cout<<"height:"<<frameHeight<<endl;
//        frameWidth=frameWidth/2;
//        frameHeight=frameHeight/2;
//        cout<<"Set size to "<<frameWidth<<"x"<<frameHeight<<endl;
        videoCap.set(CV_CAP_PROP_FRAME_WIDTH,frameWidth);
        videoCap.set(CV_CAP_PROP_FRAME_HEIGHT,frameHeight);
        timeHeure.start(10);
        frame=Mat::zeros(videoCap.get(CV_CAP_PROP_FRAME_HEIGHT)/2, videoCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
    }
    else
    {
        exit(1);
    }


    // Definition of the template rectangle
    templateWidth=64;
    templateHeight=64;
    templateRect=new Rect((frameWidth-templateWidth)/2,2*(frameHeight/2-templateHeight)/3,templateWidth,templateHeight);
    ui->widget->initialPositionHand(frame.cols - templateWidth + 1,frame.rows - templateHeight + 1);
}

Window::~Window()
{
    QString name=QString("%1/trebuchet.txt").arg(QDir::homePath());
    //QString name=QString("./record/trebuchet.txt");
    ofstream os(name.toStdString().c_str());
    if (os.good())
    {
        sauverOneTable(os,ui->tableWidget);
        sauverOneTable(os,ui->tableWidget_2);
        sauverOneTable(os,ui->tableWidget_3);
    }
    delete ui;

}

/*void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}*/

void Window::paintEvent(QPaintEvent *e)
{
    QImage image2 = QImage((uchar*)(frame.data),frame.cols, frame.rows, QImage::Format_RGB888);
    //cout<<QPixmap::fromImage(image2).width()<<","<<QPixmap::fromImage(image2).height()<<endl;
    ui->label->setPixmap(QPixmap::fromImage(image2));
    //ui->label->resize(image2.size());
    ui->label->show();

}

void Window::updateImage()
{
    Mat frameTem=Mat::zeros(videoCap.get(CV_CAP_PROP_FRAME_HEIGHT), videoCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
    videoCap>>frameTem;
    frame=frameTem(Range(frame.rows,frameTem.rows),Range(0,frame.cols));
    if(frame.data)
    {
        cvtColor(frame, frame, CV_BGR2RGB);
        flip(frame,frame,1);
        if(pressed==false)
        {
            templateImage=Mat(frame,*templateRect).clone();
            rectangle(frame,*templateRect,Scalar( 0, 0, 255),2,8,0);

        }
        else
        {
            match();
            rectangle(frame,*templateRect,Scalar( 0, 255, 0),2,8,0);

        }
        this->update();
    }
}
void Window::keyPressEvent(QKeyEvent *e)
{
    pressed=true;
}

void Window::match()
{
    Mat resultImage;
    int result_cols =  frame.cols - templateImage.cols + 1;
    int result_rows = frame.rows - templateImage.rows + 1;
    resultImage.create( result_cols, result_rows, CV_32FC1 );

    matchTemplate( frame, templateImage, resultImage, TM_CCORR_NORMED );

    double minVal; double maxVal; Point minLoc; Point maxLoc;
    minMaxLoc( resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
    delete templateRect;
    templateRect=new Rect(maxLoc.x,maxLoc.y,templateWidth,templateHeight);
    ui->widget->handMouveSlowly(maxLoc.x,maxLoc.y);//the movement of the hand will control the widget opengl.

}

void Window::restart(int type)
{
    pressed=false;
    ui->widget->initialPositionHand(frame.cols - templateWidth + 1,frame.rows - templateHeight + 1);
    delete templateRect;
    templateRect=new Rect((frame.cols-templateWidth)/2,2*(frame.rows-templateHeight)/3,templateWidth,templateHeight);
    flag=true;// update the information in the textbrowser while playing the game
    if(type==0)//a projectile has been throwed,the time for aiming at the target should be cleared.
    {
            secondTarget=0;
    }
    else
    {
        secondTarget--;
        second--;
    }
}

void Window::gameStart()
{
    QString text=ui->comboBox->currentText();

    int niveauxDifficulty;
    if(text=="Easy")
        niveauxDifficulty=1;
    else if(text=="Medium")
        niveauxDifficulty=2;
    else
        niveauxDifficulty=3;
    player=new Player(ui->lineEdit->text(),niveauxDifficulty);
    ui->widget->getPlayer(player);//将玩家对象地址传到glwidget类中
    ui->widget->gameStart();
    start=true;
    flag=true;

}

void Window::updateTime()
{
    if(start&&!flagPause)
    {
        second+=1;
        secondTarget+=1;
    }
    updateTextBrowser();
}

void Window::updateTextBrowser()
{
    if(!start)
    {
        if(!flagAbout)
            ui->textBrowser->setText(QString("Choose the level difficulty and write your nickname,then press the bouton 'start' to let the game begin.After the target appears,pressed any keys to start to handle the trebuchet."));
        else
        {
            aboutLast--;
            ui->textBrowser->setText(QString("Programme name: 3DProjet,Version:1.0,Authors:XU Sixiang and QIU Chen"));
            if(aboutLast==0)
            {
                flagAbout=false;
                aboutLast=5;
            }
        }
    }
    else
    {
        if(flagPause)
            ui->textBrowser->setText("the games pauses,and please press the bouton 'resume' after you have had a break");
        else{
            if(flag)
            {
                textInbrowser1.clear();
                textInbrowser1=QString(" remaining set: ");
                textInbrowser1.append(QString::number(player->getSetRemaining()));
                textInbrowser1.append(QString(" total points: "));
                textInbrowser1.append(QString::number(player->getPointTotal()));
                textInbrowser1.append(QString(" previous point : "));
                textInbrowser1.append(QString::number(player->getPointPrevious()));
                flag=false;
            }
            QString textInbrowser2=QString(" global time: ");
            textInbrowser2.append(time.addSecs(second).toString());
            textInbrowser2.append(QString(" set time :"));
            textInbrowser2.append(time.addSecs(secondTarget).toString());
            textInbrowser2.append(textInbrowser1);
            textInbrowser2.append(QString("\nRemark:  The programme will call the physical function to calculate the trajectory,please be patient"));
            textInbrowser2.append(QString("\nSuggestion:  When your hand is near an edge,move it quickly toward another edge to fire"));
            ui->textBrowser->setText(textInbrowser2);
        }
    }
}

void Window::pushButtonReactToggle()
{
    if(!buttonState[0])
    {
        if(ui->lineEdit->text()=="")
        {
//            QMessageBox qmBox;
//            qmBox.setText("please input your player name first");
//            qmBox.exec();

        }
    else
        {
            ui->pushButton->setText(QString("Resume"));
            ui->pushButton_2->setText(QString("Pause"));
            ui->pushButton_3->setText((QString("Stop")));
            ui->comboBox->setEnabled(false);
            ui->lineEdit->setEnabled(false);
            buttonState[0]=true;
            buttonState[1]=true;
            buttonState[2]=true;
            ui->pushButton->setEnabled(false);
            gameStart();
        }
    }
    else//resume the game
    {
        ui->widget->designForResume();
        flagPause=false;
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(true);

    }
}

void Window::pushButton2ReactToggle()
{
    if(!buttonState[1])
    {
        flagAbout=true;
    }
    else//pause the game
    {
        ui->widget->designForPause();
        flagPause=true;
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setEnabled(true);
    }
}

void Window::pushButton3ReactToggle()
{
    if(!buttonState[2])
    {
        QApplication::quit();
    }
    else
    {

        ui->widget->designForPause();
        flagPause=true;
        ui->pushButton_2->setEnabled(false);
        ui->pushButton->setEnabled(true);//在手势控制时进行重来一盘时，先让游戏暂停在弹出对话框

        QMessageBox msgBox;
        msgBox.setText("The temporary record of this match will be lost.");
        msgBox.setInformativeText("Do you want to start a new game?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if(ret==QMessageBox::Yes)
        {
            oneGameFinish();
        }


    }
}

void Window::oneGameFinish()//当一场游戏结束或者中途按stop结束时恢复初始状态时调用
{
    ui->pushButton->setText(QString("Start"));
    ui->pushButton_2->setText(QString("About"));
    ui->pushButton_3->setText((QString("Exit")));
    ui->comboBox->setEnabled(true);
    ui->lineEdit->setEnabled(true);
    buttonState[0]=false;
    buttonState[1]=false;
    buttonState[2]=false;
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    start=false;
    flagPause=false;
    second=0;
    secondTarget=0;
    delete player;
    ui->widget->designForRestart();
}

void Window::record()
{
    int level=player->getLevelDifficulty();
    QTableWidget *table;
    switch (level) {
    case 1:
        table=ui->tableWidget;
        break;
    case 2:
        table=ui->tableWidget_2;
        break;
    case 3:
        table=ui->tableWidget_3;
        break;
    default:
        break;
    }
    int rowNum=table->rowCount();
    rowNum++;
    table->setRowCount(rowNum);
    int score=player->getPointTotal();
    QTableWidgetItem *item1=new QTableWidgetItem(intToString(score));
    QTableWidgetItem *item2=new QTableWidgetItem(player->getName());
    table->setItem(rowNum-1,0,item1);
    table->setItem(rowNum-1,1,item2);
    table->sortByColumn(0);
    int numDiffrent=1;
    for(int i=0;i<=rowNum-2;i++)
    {
        if(table->item(i,0)->text()!=table->item(i+1,0)->text())
            numDiffrent++;
    }

    cout<<"numDiffrent: "<<numDiffrent<<endl;

    if(numDiffrent>3)
    {
        QString str=table->item(rowNum-1,0)->text();
        table->removeRow(rowNum-1);
        do
        {
            table->removeRow(rowNum-1);
            rowNum--;
        }while(table->item(rowNum-1,0)->text()==str);
    }
    oneGameFinish();
}

QString Window::intToString(int score)
{
    if(score==100)
        return QString("100");
    else if((score/10)==0)
        return QString("00%1").arg(score);
    else
        return QString("0%1").arg(score);
}

void Window::sauverOneTable(ofstream &os,QTableWidget *table)
{
    int row=table->rowCount();
    os<<row<<endl;
    for(int i=0;i<row;i++)
        os<<table->item(i,0)->text().toStdString()<<";"<<table->item(i,1)->text().toStdString()<<endl;
}

void Window::chargerOneTable(ifstream &is, QTableWidget *table)
{
    int n;
    is>>n;
    is.ignore();
    table->setRowCount(n);
    for (int i=0;i<n;i++)
    {
        string scoreTemp;
        string playerTemp;
        getline(is,scoreTemp,';');
        getline(is,playerTemp);
        table->setItem(i,0,new QTableWidgetItem(QString::fromStdString(scoreTemp)));
        table->setItem(i,1,new QTableWidgetItem(QString::fromStdString(playerTemp)));
    }
}


