#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QString>

extern "C" {
#include "v4l2.h"
}

extern unsigned int getCurrentTime(char *str);

QT_BEGIN_NAMESPACE
namespace Ui { class mainWidget; }
QT_END_NAMESPACE

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    mainWidget(QWidget *parent = nullptr);
    ~mainWidget();

private slots:
    void on_start_pb_clicked();

    void on_stop_pb_clicked();

    void on_exit_pb_clicked();

private:
    Ui::mainWidget *ui;
    unsigned int getCurrentTime(QString str);

    pass_data mdate;
};
#endif // MAINWIDGET_H
