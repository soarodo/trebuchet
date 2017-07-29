#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include<QTimer>
#include<opencv2/opencv.hpp>
#include<QTime>
#include"player.h"
#include<QTableWidget>
#include<fstream>

namespace Ui {
class Window;
}
using namespace cv;

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

protected:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
private:
    Ui::Window *ui;

    int aboutLast;//seconds will last when clicking'about'
    bool buttonState[3]={false,false,false};//the two states of each button

    bool flag;
    bool flagAbout;//whether the button 'About' is clicked
    bool flagPause=false;//whether the button 'Pause' is clicked
    Mat frame;

    bool pressed;//whether press a key
    Player *player;

    QTimer timeHeure;
    Mat templateImage;
    int templateWidth;
    int templateHeight;
    VideoCapture videoCap;
    Rect *templateRect;
    QTime time;//global time
    QString textInbrowser1;

    int second;
    int secondTarget;
    bool start;// whether the game starts

    void chargerOneTable(ifstream &is,QTableWidget *table);
    void gameStart();
    QString intToString(int score);//change the format of the score in order to record it in the table.
    void sauverOneTable(ofstream &os,QTableWidget *table);
    void oneGameFinish();
    void updateTextBrowser();


private slots:
    void match();//locate the position of a hand in a frame of the video
    void restart(int type);
    void record();
    void pushButtonReactToggle();
    void pushButton2ReactToggle();
    void pushButton3ReactToggle();
    void updateTime();
    void updateImage();


};

#endif // WINDOW_H
