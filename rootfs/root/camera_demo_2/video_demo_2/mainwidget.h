#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTimer>

extern "C"{
#include "acq_img.h"
#include "pthread.h"
}

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
    void on_exit_pb_clicked();

    void on_init_pb_clicked();

    void on_start_pb_clicked();

    void on_stop_pb_clicked();
    void get_img();

private:
    void image_process(wwk_uchar_t *uc);

private:
    Ui::mainWidget *ui;

    QTimer *timer;
    pthread_t t;
    acq_img_t *acq;
    bool work_run;

};
#endif // MAINWIDGET_H
