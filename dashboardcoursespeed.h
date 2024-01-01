#ifndef DASHBOARDCOURSESPEED_H
#define DASHBOARDCOURSESPEED_H

#include <QWidget>

namespace Ui {
class DashboardCourseSpeed;
}

class DashboardCourseSpeed : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardCourseSpeed(QWidget *parent = nullptr);
    ~DashboardCourseSpeed();

protected:
    void paintEvent(QPaintEvent *);

    void drawCrown(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawIndicator(QPainter *painter);
    void drawNumericValue(QPainter *painter);

public slots:
    void UpdateAngle(double value);

private:
    QColor m_background;
    QColor m_foreground;


    int m_maxValue;
    int m_minValue;
    int m_startAngle;
    int m_endAngle;

    int m_scaleMajor;
    int m_scaleMinor;
    double m_value;
    int m_precision;
    QTimer *m_updateTimer;
    QString m_units;
    QString m_title;
    public Q_SLOTS:
    void UpdateAngle();

private:
    Ui::DashboardCourseSpeed *ui;
};

#endif // DASHBOARDCOURSESPEED_H
