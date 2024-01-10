#ifndef PUSHBUTTON_SWITCHAPPLE_H
#define PUSHBUTTON_SWITCHAPPLE_H

#include <QPushButton>
#include <QObject>
#include <QTimeLine>
#include <QDebug>
#include <QPainter>


class PushButton_SwitchAPPLE : public QPushButton
{
    Q_OBJECT
public:
    explicit  PushButton_SwitchAPPLE(QWidget *parent = nullptr);

    int status = 0;
    int span_length = 0;
    bool rl_flag = false;
    bool release_flag =true;
    QTimeLine *timeline;

    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timeOut(int x);
    void drawSpan(QPainter painter,int s);


};

#endif // PUSHBUTTON_SWITCHAPPLE_H
