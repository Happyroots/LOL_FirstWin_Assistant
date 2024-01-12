#include "pushbutton_switchapple.h"


PushButton_SwitchAPPLE::PushButton_SwitchAPPLE(QWidget *parent) :
 QPushButton(parent)
{
    status = 0; //长期状态
    span_length = 0;
    rl_flag = false;
    release_flag =true;
    timeline = new QTimeLine;
//    connect(timeline,SIGNAL(frameChanged(int)),this,SLOT(timeOut(int)));
}

void PushButton_SwitchAPPLE::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(219,219,219));
    painter.setPen(pen);
//    painter.setBrush(Qt::green);
//    painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,10,10);
//    painter.setBrush(Qt::white);
    painter.setPen(pen);

    if(!rl_flag){
        switch(status)
        {
        case 0:
            painter.setBrush(Qt::gray);
            painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,10,10);
            painter.setBrush(Qt::white);
            painter.drawEllipse(this->width()+3-this->height(),1,this->height()-3,this->height()-3);

            break;
        case 1:
            painter.drawRoundedRect(1,1,span_length,this->height()-2,10,10);
            break;
        case 2:
            painter.setBrush(Qt::green);
            painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,10,10);
            painter.setBrush(Qt::white);
            painter.drawEllipse(1,1,this->height()-3,this->height()-3);
            rl_flag=true;status=0;
            emit syncStatusControl(rl_flag);
            break;
        }

    }
    else{
        switch(status)
        {
        case 0:
            painter.setBrush(Qt::green);
            painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,10,10);
            painter.setBrush(Qt::white);
            painter.drawEllipse(1,1,this->height()-3,this->height()-3);
            break;
        case 1:
            painter.drawRoundedRect(this->width()-1-span_length,1,span_length,this->height()-2,30,30);
            break;
        case 2://最后一步是完全体
            painter.setBrush(Qt::gray);
            painter.drawRoundedRect(1,1,this->width()-2,this->height()-2,10,10);
            painter.setBrush(Qt::white);
            painter.drawEllipse(this->width()+3-this->height(),1,this->height()-3,this->height()-3);
            rl_flag=false;status=0;
            emit syncStatusControl(rl_flag);
            break;
        }

    }
}

void PushButton_SwitchAPPLE::drawSpan(QPainter painter,int s)
{

}

void PushButton_SwitchAPPLE::mousePressEvent(QMouseEvent *e)
{
     timeline->setDuration(400);
     span_length = 0;
     status=1;
     if(!rl_flag){
         int r1 = this->height()-3;
         int r2 = (this->width()/4*3)>r1?(this->width()/4*3):r1;//(this->width()/4*1)>r1?(this->width()/4*1):r1;
         timeline->setFrameRange(r1,r2);


     }else{

         int r1 = this->height()-3;
         int r2 = (this->width()/4*3)>r1?(this->width()/4*3):r1;
         timeline->setFrameRange(r1,r2);

     }
     timeline->start();
}

//void PushButton_SwitchAPPLE::mouseMoveEvent(QMouseEvent *e)
//{
//}

void PushButton_SwitchAPPLE::mouseReleaseEvent(QMouseEvent *e)
{
    timeline->stop();
    status = 2;
    update();
}

void PushButton_SwitchAPPLE::timeOut(int x)
{
    status = 1;
    span_length = x;
    update();
}
