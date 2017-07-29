#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include"player.h"
#include<QImage>

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    void designForPause();
    void designForResume();
    void designForRestart();
    void gameStart();
    int getSetScore();
    void getPlayer(Player *player){this->player=player;}
    void handMouveSlowly(int x,int y);
    void initialPositionHand(double x,double y);
    void testMatlab(int angle);



signals:
    void declenche(int type);
    void timeTorecord();
    //call the prgramme that a complete match has been ended and tell the function record() in the class window


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    // slots for xyz-rotation slider
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    //according the level which a player choose,this function would change the distance between the target and the trebuchet.
    void changeLevelGame(const QString &text);

signals:
    // signaling rotation from mouse movement
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);

private:
    double angleHorizontal;
    double angleVertical;
    double angleRope;//the angle between the rope and the arm
    // angleHorizontal(vertical)=angleHorizontalUnit(VerticalUnit)*distance between the detected point of objet in current image and previous image
    double angleHorizontalUnit;
    double angleVerticalUnit;

    bool changeCamera;//alert whether changing the camera(we will change the camera to display the animation ).

    QImage tex[9];

    double cibleXZ[2];// target's coordinates(cibleXZ[0],0,cibleXZ[1]);

    GLuint GLtexture[9];// save textures
    GLuint GLdisplayList[8];//save object's lists

    QPoint lastPos;
    QPoint lastPosHand;

    int niveauxDifficulty;//level 1：easy，2：medium，3：difficult

    double posCamera[2];
    Player *player;
    // projectile landing's coordinates
    double projectileLandX;
    double projectileLandZ;
    double projectileLandY;

    bool start;//whether a match starts

    double xProjectile;
    double yProjectile;

    int xRot;
    int yRot;
    int zRot;

    void draw();
    void chargerImage(QString Image,int num);
    void displayList();
    void drawCylinder(GLdouble radius,GLdouble height,GLfloat texture);
    void DrawCircleArea(float cx, float cy, float cz, float r, int num_segments,GLfloat texture);
    void drawSphere(GLdouble radius,GLint slices,GLint stacks);
    void displayTexture();
    void glDrawCube();
    double getAngleRope();
    void getCibleXZ();//get randomly coordinates of a target.
    void getCameraPos(double x);//calculate the camera's position when displaying the animation
    unsigned int LoadTex(int num);
    double random(double start,double end);
    void setAngleHorizontalRotation(double angle);
    void setAngleVerticalRotation(double angle);
    void setXYCoordonneeProjectile();


};
#endif // GLWIDGET_H
