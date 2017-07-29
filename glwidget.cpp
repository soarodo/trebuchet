        #include "glwidget.h"

    #include <QtWidgets>
    #include <QtOpenGL>
    #include<QDebug>
    #include<gl/glut.h>
    #include<iostream>
    #include<virtualTrebuchetV5.h>
    #include "glwidget.h"
    #include<cmath>
    #include <ctime>
    #include<QImage>
    #include<QPixmap>

    using namespace std;


//    #pragma comment ( lib,"libeng.lib" )
//    #pragma comment ( lib,"libmx.lib" )
//    #pragma comment ( lib,"libmat.lib" )
//    #pragma comment ( lib,"libmex.lib" )
    #pragma comment(lib,"virtualTrebuchetV5.lib")
    GLWidget::GLWidget(QWidget *parent)
        : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
    {
        xRot = 0;
        yRot = 0;
        zRot = 0;
        angleHorizontal=0;
        angleVertical=0;
        angleRope=getAngleRope();
        niveauxDifficulty=1;
        srand(unsigned(time(0)));
        setXYCoordonneeProjectile();
        start=false;
        changeCamera=false;
        QPixmap temp=QPixmap(":/textureImages/images/background.png");
        if(temp.isNull())
            cout<<"not loaded"<<endl;
        chargerImage(":/textureImages/images/bois.bmp",0);
        chargerImage(":/textureImages/images/fire.bmp",1);
        chargerImage(":/textureImages/images/redFire.bmp",2);
        chargerImage(":/textureImages/images/rope.bmp",3);
        chargerImage(":/textureImages/images/grass1.bmp",4);
        chargerImage(":/textureImages/images/rock.bmp",5);
        chargerImage(":/textureImages/images/cible1.bmp",6);
        chargerImage(":/textureImages/images/tseLogoBarriere.bmp",7);
        chargerImage(":/textureImages/images/grassWithLogo.bmp",8);




    }

    GLWidget::~GLWidget()
    {
    }

    QSize GLWidget::minimumSizeHint() const
    {

        //return QSize(50, 50);
        return QSize(500, 500);
    }

    QSize GLWidget::sizeHint() const
    {
        //return QSize(400, 400);
        return QSize(1000, 1000);
    }

    static void qNormalizeAngle(int &angle)
    {
        while (angle < 0)
            angle += 360;
        while (angle > 360)
            angle -= 360 ;
    }

    void GLWidget::setXRotation(int angle)
    {
        qNormalizeAngle(angle);
        if (angle != xRot) {
            xRot = angle;
            emit xRotationChanged(angle);
            updateGL();
        }
    }

    void GLWidget::setYRotation(int angle)
    {
        qNormalizeAngle(angle);
        if (angle != yRot) {
            yRot = angle;
            emit yRotationChanged(angle);
            updateGL();
        }
    }

    void GLWidget::setZRotation(int angle)
    {
        qNormalizeAngle(angle);
        if (angle != zRot) {
            zRot = angle;
            emit zRotationChanged(angle);
            updateGL();
        }
    }

    void GLWidget::setAngleHorizontalRotation(double angle)
    {
            angleHorizontal+=angle;
            updateGL();
    }

    void GLWidget::setAngleVerticalRotation(double angle)
    {

            angleVertical=angle;
            angleRope=getAngleRope();
            setXYCoordonneeProjectile();
            updateGL();
    }

    double GLWidget::getAngleRope()
    {
        double angleVerticalRadius=angleVertical/180*M_PI;
        /*if(angleVertical>-16.45)*/
            return acos((3.3-6*sin(angleVerticalRadius))/5)*180/M_PI+90-angleVertical;
        /*else
            return 90+abs(angleVertical);*/
    }

    void GLWidget::getCibleXZ()
    {

        switch (niveauxDifficulty) {
        case 1:
            cibleXZ[0]=random(-20,20);
            cibleXZ[1]=random(-35,-43);
            break;
        case 2:
            cibleXZ[0]=random(-20,20);
            cibleXZ[1]=random(-43,-51);
            break;
        case 3:
            cibleXZ[0]=random(-20,20);
            cibleXZ[1]=random(-51,-59);
            break;
        default:
            break;
        }
    }

    double GLWidget::random(double start, double end)
    {
         return start+(end-start)*rand()/(RAND_MAX + 1.0);
    }


    void GLWidget::setXYCoordonneeProjectile()
    {
        double angleVerticalRadius=angleVertical/180*M_PI;
        double angleRopeRadius=angleRope/180*M_PI;
        xProjectile=-6*cos(angleVerticalRadius)-5*cos(angleRopeRadius+angleVerticalRadius);
        yProjectile=-6*sin(angleVerticalRadius) - 5*sin(angleRopeRadius+angleVerticalRadius);
    }

    void GLWidget::getCameraPos(double x)
    {
        double distanceCamera=pow((x*x/4+15*15),0.5);
        double angle1=atan(30/x);
        double zabs=abs(x)*cos(angleHorizontal/180*M_PI);
        double angle2=asin(zabs/x);
        double angle=angle1+angle2;
        cout<<"angle1: "<<angle1<<" angle2: "<<angle2<<endl;
        posCamera[1]=-sin(angle)*distanceCamera;
        if(angleHorizontal>=0)
            posCamera[0]=-cos(angle)*distanceCamera;
        else
            posCamera[0]=cos(angle)*distanceCamera;
        //cout<<"distanceProjectile: "<<x<<" angleHorizontal: "<<angleHorizontal<<" posCameraX: "<<posCamera[0]<<" posCameraY: "<<posCamera[1]<<endl;
    }

    int GLWidget::getSetScore()
    {
        double distance=pow((pow((projectileLandX-cibleXZ[0]),2)+pow((projectileLandZ-cibleXZ[1]),2)),0.5);
        double a=4.0/10.0;
    //    cout<<"a: "<<a<<endl;
    //    cout<<"distance between projectile and cible: "<<distance<<endl;
        if(distance>=0&&distance<=a)
            return 10;
        else if(distance>a&&distance<=2*a)
            return 9;
        else if(distance>2*a&&distance<=3*a)
            return 8;
        else if(distance>3*a&&distance<=4*a)
            return 7;
        else if(distance>4*a&&distance<=5*a)
            return 6;
        else if(distance>5*a&&distance<=6*a)
            return 5;
        else if(distance>6*a&&distance<=7*a)
            return 4;
        else if(distance>7*a&&distance<=8*a)
            return 3;
        else if(distance>8*a&&distance<=9*a)
            return 2;
        else if(distance>9*a&&distance<=10*a)
            return 1;
        else
            return 0;
    }
    void GLWidget::initializeGL()
    {
        qglClearColor(Qt::black);

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glShadeModel(GL_SMOOTH);


        GLfloat ambientLight[]={0.2f,0.2f,0.2f,1.0f};
        GLfloat diffuseLight[]={0.8f,0.8f,0.8f,1.0f};
        GLfloat specularLight[]={1.0f,1.0f,1.0f,1.0f};
        //static GLfloat lightPosition[4] = { 0, 0, 3, 1.0 };
        static GLfloat lightPosition[4] = {0 , -1, 1, 0.0 };
        glEnable(GL_LIGHTING);
        glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
        glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glEnable(GL_LIGHT0);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
        glMaterialfv(GL_FRONT,GL_SPECULAR,specularLight);
        glMateriali(GL_FRONT,GL_SHININESS,50);
        glEnable(GL_BLEND);

    }

    void GLWidget::paintGL()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        if(!changeCamera)
            //gluLookAt(0,45,0,0,0,-40,0,1,0);
            gluLookAt(0,35,0,0,0,-60,0,1,0);
        else
            gluLookAt(posCamera[0],40,posCamera[1],projectileLandX,projectileLandY,projectileLandZ,0,1,0);
            //gluLookAt(-15,40,-18,projectileLandX,projectileLandY,projectileLandZ,0,1,0);
        //gluLookAt(-45,-20,-10,0,0,-40,0,1,0);
        glTranslatef(0.0, 0.0, -10.0);
        glRotatef(xRot , 1.0, 0.0, 0.0);
        glRotatef(yRot , 0.0, 1.0, 0.0);
        glRotatef(zRot , 0.0, 0.0, 1.0);
        draw();
    }

    void GLWidget::resizeGL(int width, int height)
    {
        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    #ifdef QT_OPENGL_ES_1
        glOrthof(-30, +30, -30, +30, 1,120 );
    #else
        glOrtho(-30, +30, -30, +30, 1, 120);
    #endif
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void GLWidget::mousePressEvent(QMouseEvent *event)
    {
        lastPos = event->pos();
    }


    void GLWidget::initialPositionHand(double x,double y)
    {
        lastPosHand.setX(x/2);
        lastPosHand.setY(y/2);
        angleHorizontalUnit=180/x;
        angleVerticalUnit=33/y;
    }

    void GLWidget::handMouveSlowly(int x,int y)
    {
        if(start)
        {
            double dx = (x - lastPosHand.x())*angleHorizontalUnit;
            double dy = y*angleVerticalUnit;
            if(abs(y- lastPosHand.y())>10)
            {
                emit declenche(0);
                if (!virtualTrebuchetV5Initialize())
                {
                    cout << "Could not initialize libmysvd!" << endl;
                    exit(0);
                }
                double angleIncalul=90+angleVertical;
                double data[1] = {angleIncalul};
                mwArray Aq0(1, 1, mxDOUBLE_CLASS, mxREAL);
                mwArray Aq(100, 1, mxDOUBLE_CLASS, mxREAL);
                mwArray Sq(100, 1, mxDOUBLE_CLASS, mxREAL);
                mwArray X3(60, 1, mxDOUBLE_CLASS, mxREAL);
                mwArray Y3(60, 1, mxDOUBLE_CLASS, mxREAL);
                Aq0.SetData(data, 1);
                virtualTrebuchetV5(4,Aq,Sq,X3,Y3,Aq0);
                int dimensionStage12=Aq.GetDimensions()(1,1);
                int dimensionStage3=X3.GetDimensions()(1,1);
                getCameraPos((double)X3(dimensionStage3,1));
                projectileLandX=0;
                projectileLandZ=0;
                projectileLandY=0;
                changeCamera=true;
                for(int i=1;i<=dimensionStage12;i++)
                {
                    angleVertical=(double)Aq(i,1)-90.0;
                    angleRope=(double)Sq(i,1);
                    setXYCoordonneeProjectile();
                    updateGL();
                }

                for(int i=1;i<=dimensionStage3;i++)
                {
                    xProjectile=(double)X3(i,1);
                    yProjectile=(double)Y3(i,1);
                    projectileLandX=-abs(xProjectile)*sin(angleHorizontal/180*M_PI);
                    projectileLandZ=-abs(xProjectile)*cos(angleHorizontal/180*M_PI);
                    projectileLandY=yProjectile;
                    updateGL();
                }
                changeCamera=false;
                projectileLandX=-abs(xProjectile)*sin(angleHorizontal/180*M_PI);
                projectileLandZ=-abs(xProjectile)*cos(angleHorizontal/180*M_PI);
                //cout<<"angleHorizontal: "<<angleHorizontal<<" projectileLandX: "<<projectileLandX<<" projectileLandZ: "<<projectileLandZ<<endl;
                virtualTrebuchetV5Terminate();
                int point=getSetScore();
                player->addPoints(point);
                angleHorizontal=0;
                angleVertical=0;
                angleRope=getAngleRope();
                setXYCoordonneeProjectile();
                getCibleXZ();
                if(player->getSetRemaining()==0)
                {
                    start=false;
                    emit timeTorecord();
                }
            }
            else
            {
                setAngleHorizontalRotation(dx);
                setAngleVerticalRotation(dy);

                lastPosHand.setX(x);
                lastPosHand.setY(y);
            }
        }
    }



    void GLWidget::mouseMoveEvent(QMouseEvent *event)
    {
        int dx = event->x() - lastPos.x();
        int dy = event->y() - lastPos.y();

        if (event->buttons() & Qt::LeftButton) {
            setXRotation(xRot +  dy);
            setYRotation(yRot +   dx);
        } else if (event->buttons() & Qt::RightButton) {
            setXRotation(xRot +  dy);
            setZRotation(zRot +  dx);
        }

        lastPos = event->pos();
    }

    void GLWidget::displayTexture()
    {
        GLtexture[0]= LoadTex(0);
        GLtexture[1]= LoadTex(1);
        GLtexture[2]= LoadTex(2);
        GLtexture[3]= LoadTex(3);
        GLtexture[4]=LoadTex(4);
        GLtexture[5]=LoadTex(5);
        GLtexture[6]=LoadTex(6);
        GLtexture[7]=LoadTex(7);
        GLtexture[8]=LoadTex(8);
    }

    void GLWidget::chargerImage(QString Image,int num)
    {
          QPixmap buf1=QPixmap(Image);
          QImage buf=buf1.toImage();
          //buf.load(Image);
          tex[num]=QGLWidget::convertToGLFormat(buf);

    }

    unsigned int GLWidget::LoadTex(int num)
    {
        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex[num].width(), tex[num].height(),GL_RGBA, GL_UNSIGNED_BYTE, tex[num].bits());
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        return Texture;
    }

    void GLWidget::draw()
    {
        //qglColor(Qt::red);
         displayList();
         glCallList (GLdisplayList[0]);
         glCallList(GLdisplayList[1]);
         glCallList(GLdisplayList[2]);
         if(start)
            glCallList(GLdisplayList[3]);



        glPushMatrix();
         glTranslatef(-30,5,-25);
         glRotated(90,0,1,0);
         glCallList((GLdisplayList[4]));
        glPopMatrix();
        glPushMatrix();
         glTranslatef(-30,5,-55);
         glRotated(90,0,1,0);
         glCallList((GLdisplayList[4]));
        glPopMatrix();
        glPushMatrix();
         glTranslatef(-15,5,-70);
         glCallList((GLdisplayList[4]));
        glPopMatrix();
        glPushMatrix();
         glTranslatef(15,5,-70);
         glCallList((GLdisplayList[4]));
        glPopMatrix();
        glPushMatrix();
         glTranslatef(30,5,-25);
         glRotated(-90,0,1,0);
         glCallList((GLdisplayList[4]));
        glPopMatrix();
        glPushMatrix();
         glTranslatef(30,5,-55);
         glRotated(-90,0,1,0);
         glCallList((GLdisplayList[4]));
        glPopMatrix();

        glPushMatrix();
         glTranslatef(-30,0,-10);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(-30,0,-40);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(-30,0,-70);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(0,0,-70);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(30,0,-70);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(30,0,-40);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(30,0,-10);
         glCallList(GLdisplayList[5]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(-15.0,0,0);
         glCallList(GLdisplayList[6]);
        glPopMatrix();

        glPushMatrix();
         glTranslatef(15.0,0,0);
         glCallList(GLdisplayList[6]);
        glPopMatrix();


         glDeleteLists(GLdisplayList[0],1);
         glDeleteLists(GLdisplayList[1],1);
         glDeleteLists(GLdisplayList[2],1);
         if(start)
            glDeleteLists(GLdisplayList[3],1);
         glDeleteLists(GLdisplayList[4],1);
         glDeleteLists(GLdisplayList[5],1);
         glDeleteLists(GLdisplayList[6],1);
         glDeleteTextures(1, GLtexture);
         glDeleteTextures(1, GLtexture+1);
         glDeleteTextures(1, GLtexture+2);
         glDeleteTextures(1, GLtexture+3);
         glDeleteTextures(1,GLtexture+4);
         glDeleteTextures(1,GLtexture+5);
         glDeleteTextures(1,GLtexture+6);
         glDeleteTextures(1,GLtexture+7);
         glDeleteTextures(1,GLtexture+8);
    }

    void GLWidget::displayList()
    {
        displayTexture();
        GLdisplayList[0]= glGenLists(1);
        GLdisplayList[1]=glGenLists(2);
        GLdisplayList[2]=glGenLists(3);
        GLdisplayList[3]=glGenLists(4);
        GLdisplayList[4]=glGenLists(5);
        GLdisplayList[5]=glGenLists(6);
        GLdisplayList[6]=glGenLists(7);




        float rate=0.25;
        float longArm=8;
       /* angleVertical=0;
         qNormalizeAngle(angleVertical);*/
        /*angleHorizontal=0;
        qNormalizeAngle(angleHorizontal);*/


        glNewList ( GLdisplayList[0], GL_COMPILE );
            glPushMatrix();
                glRotatef(angleHorizontal,0,1,0);

                glPushMatrix();
                    glEnable( GL_TEXTURE_2D );
                    glBindTexture(GL_TEXTURE_2D, GLtexture[0]);
                    glTranslatef(0,-3.05,0);
                    glScalef(1,0.5,2);
                    glColor3f(1,1,1);
                    glDrawCube();
                    glDisable( GL_TEXTURE_2D );
                glPopMatrix();

                glPushMatrix();
                    glEnable( GL_TEXTURE_2D );
                    glBindTexture(GL_TEXTURE_2D, GLtexture[0]);
                    glTranslatef(0,-2.6,0);
                    glRotatef(90,1,0,0);
                    glScalef(1,1,1);
                    glColor3f(1,0,0);
                    drawCylinder(2,0.2, GLtexture[0]);
                    glDisable( GL_TEXTURE_2D );
                glPopMatrix();

                glPushMatrix();
                    glPushMatrix();
                    glEnable( GL_TEXTURE_2D );
                    glBindTexture(GL_TEXTURE_2D, GLtexture[0]);
                        glTranslatef(0.75,-1.1,0);
                        glScalef(0.5,3,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(0.75,-1.18,0.55);
                        glRotatef(20,-1,0,0);
                        glScalef(0.5,3.16,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(0.75,-1.18,-0.55);
                        glRotatef(-20,-1,0,0);
                        glScalef(0.5,3.16,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glDisable( GL_TEXTURE_2D );
                glPopMatrix();

                glPushMatrix();
                    glPushMatrix();
                    glEnable( GL_TEXTURE_2D );
                    glBindTexture(GL_TEXTURE_2D, GLtexture[0]);
                        glTranslatef(-0.75,-1.1,0);
                        glScalef(0.5,3,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(-0.75,-1.18,0.55);
                        glRotatef(20,-1,0,0);
                        glScalef(0.5,3.16,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(-0.75,-1.18,-0.55);
                        glRotatef(-20,-1,0,0);
                        glScalef(0.5,3.16,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                    glPopMatrix();
                    glDisable( GL_TEXTURE_2D );
                glPopMatrix();

                glPushMatrix();
                    glEnable( GL_TEXTURE_2D );
                    glBindTexture(GL_TEXTURE_2D, GLtexture[0]);
                        glTranslatef(-0.5,0,0);
                        glRotatef(90,0,1,0);
                        glScalef(1,1,1);
                        glColor3f(1,0,0);
                        drawCylinder(0.24,1,GLtexture[0]);
                    glDisable( GL_TEXTURE_2D );
                glPopMatrix();


                glPushMatrix();
                    glRotatef(angleVertical,1,0,0);

                    glPushMatrix();
                        glEnable( GL_TEXTURE_2D );
                        glBindTexture(GL_TEXTURE_2D, GLtexture[1]);
                        glTranslatef(0,0,rate*longArm);
                        glScalef(0.4,0.3,longArm);
                        glColor3f(1,1,1);
                        glDrawCube();
                        glDisable( GL_TEXTURE_2D );
                    glPopMatrix();

                    glPushMatrix();
                        glEnable( GL_TEXTURE_2D );
                        glBindTexture(GL_TEXTURE_2D, GLtexture[2]);
                        glTranslatef(0,0,-rate*longArm-0.25);
                        glScalef(0.9,1,0.5);
                        glColor3f(1,1,1);
                        glDrawCube();
                        glDisable( GL_TEXTURE_2D );
                    glPopMatrix();

                    glPushMatrix();
                        glEnable( GL_TEXTURE_2D );
                        glBindTexture(GL_TEXTURE_2D, GLtexture[3]);
                        glTranslatef(0,0,(1-rate)*longArm);
                        glRotatef(angleRope,1,0,0);
                        glScalef(1,1,1);
                        glColor3f(1,1,1);
                        drawCylinder(0.1,5, GLtexture[3]);
                        glDisable( GL_TEXTURE_2D );
                    glPopMatrix();

                glPopMatrix();

            glPopMatrix();
        glEndList ();

        glNewList(GLdisplayList[1],GL_COMPILE);
            glPushMatrix();
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[4]);
                glBegin(GL_QUADS);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -3.3f,  6.0f);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, -3.3f,  -70.0f);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(30.0f, -3.3f,  -70.0f);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -3.3f,  6.0f);

                glEnd();
                glDisable( GL_TEXTURE_2D );
            glPopMatrix();
        glEndList();


        glNewList(GLdisplayList[2],GL_COMPILE);
            glPushMatrix();
                glRotatef(angleHorizontal,0,1,0);
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[5]);
                glTranslatef(0,yProjectile,-xProjectile);
                glScalef(1,1,1);
                drawSphere(0.5,4, 4);
                glDisable( GL_TEXTURE_2D );
            glPopMatrix();
        glEndList();


        glNewList(GLdisplayList[3],GL_COMPILE);
            glPushMatrix();
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[6]);
                glTranslatef(cibleXZ[0],-3.2,cibleXZ[1]);
                glRotatef(90,1,0,0);
                glScalef(1,1,1);
                glColor4f(1, 1, 1, 0.5);
                DrawCircleArea(0.0, 0.0, 0.0, 4, 50,GLtexture[6]);
                glDisable( GL_TEXTURE_2D );
            glPopMatrix();
        glEndList();

        glNewList(GLdisplayList[4],GL_COMPILE);
            glPushMatrix();
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[7]);
                glBegin(GL_QUADS);
                glColor4f(1, 1, 1, 1);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-15.0f, -3.3f,  0.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-15.0f, 16.7f,  0.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(15.0f, 16.7f,  0.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(15.0f, -3.3f,  0.0f);
                glEnd();
            glPopMatrix();
        glEndList();

        glNewList(GLdisplayList[5],GL_COMPILE);
            glPushMatrix();
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[3]);
                glTranslatef(0,-3.3,0);
                glRotated(90,-1,0,0);
                glScalef(1,1,1);
                glColor3f(1,1,1);
                drawCylinder(2,27, GLtexture[3]);
                glDisable( GL_TEXTURE_2D );
            glPopMatrix();
        glEndList();

        glNewList(GLdisplayList[6],GL_COMPILE);
            glPushMatrix();
                glEnable( GL_TEXTURE_2D );
                glBindTexture(GL_TEXTURE_2D, GLtexture[8]);
                glBegin(GL_QUADS);
                glColor4f(1, 1, 1, 1);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(-4.0f, -3.29f,  -4.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(-4.0f, -3.29f,  4.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(4.0f, -3.29f,  4.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(4.0f, -3.29f,  -4.0f);
                glEnd();
            glPopMatrix();
        glEndList();

    }

    void GLWidget::drawCylinder(GLdouble radius,GLdouble height,GLfloat texture)
    {

        GLUquadricObj *quadratic;
        quadratic=gluNewQuadric();
        gluQuadricNormals(quadratic, GLU_SMOOTH);
        gluQuadricTexture(quadratic, GL_TRUE);
        gluCylinder(quadratic,radius,radius,height,50,50);
        DrawCircleArea(0.0, 0.0, 0.0, radius, 50,texture);
        DrawCircleArea(0.0,0.0,height,radius,50,texture);
    }

    void GLWidget::drawSphere(GLdouble radius, GLint slices, GLint stacks)
    {
        GLUquadricObj *qObjet;
        qObjet=gluNewQuadric();
        gluQuadricNormals(qObjet, GLU_SMOOTH);
        gluQuadricTexture(qObjet, GL_TRUE);
        gluSphere(qObjet,radius,slices,stacks);
    }

    void GLWidget::DrawCircleArea(float cx, float cy, float cz, float r, int num_segments,GLfloat texture)
    {
        GLfloat texcoord[2];
        GLfloat vertex[4];
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture);

        const GLfloat delta_angle = 2.0*M_PI/num_segments;
        glBegin(GL_TRIANGLE_FAN);


        texcoord[0]=0.5;
        texcoord[1]=0.5;
        glTexCoord2fv(texcoord);
        vertex[0] = cx;
        vertex[1] = cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);

        //draw the vertex on the contour of the circle
        for(int i = 0; i < num_segments ; i++)
        {

            texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
            texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
            glTexCoord2fv(texcoord);

            vertex[0] = std::cos(delta_angle*i) * r + cx;
            vertex[1] = std::sin(delta_angle*i) * r + cy;
            vertex[2] = cz;
            vertex[3] = 1.0;
            glVertex4fv(vertex);
        }

        texcoord[0] = (1.0 + 1.0)*0.5;
        texcoord[1] = (0.0 + 1.0)*0.5;
        glTexCoord2fv(texcoord);

        vertex[0] = 1.0 * r + cx;
        vertex[1] = 0.0 * r + cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }

    void GLWidget::glDrawCube()

    {
        //glTranslatef(ppx, ppy, 0.0f);
        glBegin(GL_QUADS);




        glNormal3f( 0.0f, 0.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);



        glNormal3f( 0.0f, 0.0f,-1.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);



        glNormal3f( 0.0f, 1.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f,  0.5f,  0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);


        glNormal3f( 0.0f,-1.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);



        glNormal3f( 1.0f, 0.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, -0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);


        glNormal3f(-1.0f, 0.0f, 0.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f,  0.5f,  0.5f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);

        glEnd();

        update();
    }

    void GLWidget::changeLevelGame(const QString &text)
    {
        if(text=="Easy")
            niveauxDifficulty=1;
        else if(text=="Medium")
            niveauxDifficulty=2;
        else
            niveauxDifficulty=3;
    }

    void GLWidget::gameStart()
    {
        start=true;
        emit declenche(0);
        getCibleXZ();
    }

    void GLWidget::designForPause()
    {
        emit declenche(1);
        angleHorizontal=0;
        angleVertical=0;
        angleRope=getAngleRope();
        setXYCoordonneeProjectile();
        start=false;
    }

    void GLWidget::designForResume()
    {
        start=true;
    }

    void GLWidget::designForRestart()
    {
        start=false;
    }

