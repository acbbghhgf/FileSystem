#include "mainwidget.h"
#include "ui_mainwidget.h"

//add v4l2 class and thread process

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    mdate.dev_name = "/dev/video0";
    int flag = init_dev(&mdate);
    if(flag == -1)
    {
        qDebug() << "Can't identify.";
    }
    else if(flag == -2)
    {
        qDebug() << "can not open device.";
    }
    else if(flag == 0)
    {
        qDebug() << "open device success.";
    }
//    start_capturing(&mdate);
}

mainWidget::~mainWidget()
{
    delete ui;
}


unsigned int mainWidget::getCurrentTime(QString str)
{
    struct timeval time_now = {0};
    unsigned int current = 0;
    gettimeofday(&time_now, NULL);
    current = time_now.tv_sec*1000*1000 + time_now.tv_usec;
    qDebug() << str << " : " << current;
    return current;
}


void mainWidget::on_start_pb_clicked()
{
    unsigned int time_start = 0;
    unsigned int time_end = 0;
    QImage image_raw;
    time_start = getCurrentTime("start obt image.");

    read_frame(&mdate);
    image_raw.loadFromData((const uchar*)mdate.buffers[mdate.buf.index].start, mdate.buffers[mdate.buf.index].length);
    return_data(&mdate);

    time_end = getCurrentTime("obt_image end.") - time_start;
    qDebug() << "time dealy :" << time_end;

    QGraphicsScene *originalScene = new QGraphicsScene;
    originalScene->addPixmap(QPixmap::fromImage(image_raw));
    ui->graphicsView->setScene(originalScene);
    ui->graphicsView->show();

    ui->textEdit->append("get picture...");
}

void mainWidget::on_stop_pb_clicked()
{
    qDebug() << "stop";
}

void mainWidget::on_exit_pb_clicked()
{
    qDebug() << "this function quit.";
    this->close();
}
