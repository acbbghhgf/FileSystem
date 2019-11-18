#include "mainwidget.h"
#include "ui_mainwidget.h"

//add v4l2 class and thread process

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    pd.dev_name = "/dev/video0";
    int flag = init_dev(&pd);
    if(flag == -1){
        qDebug() << "no device";
        ui->textEdit->append("no device /dev/video0\n");
        return ;
    }
    else if(flag == -2){
        qDebug() << "device is wrong.";
        ui->textEdit->append("device is wrong\n");
        return ;
    }
    else if(flag == -3){
        qDebug() << "can not open device.";
        ui->textEdit->append("can not open device.\n");
        return ;
    }
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
    time_start = getCurrentTime("start obt image.");
    QImage image_raw;
//    QImage image_raw_rgb;
    for(int i = 0; i< 8; i++){
        read_frame(&pd);
        return_data(&pd);
    }
//    read_frame(&pd);
//    return_data(&pd);
    qDebug() << "buferrs index " << pd.buf.index;
    image_raw.loadFromData((const uchar*)pd.buffers[pd.buf.index].start, pd.buffers[pd.buf.index].length);

//    convert_rgb_to_gray_output(&pd);

    time_end = getCurrentTime("obt image end.") - time_start;
    qDebug() << "time delay :" << time_end;

//    image_raw_rgb.loadFromData((const uchar*)pd.outputframe.start, pd.outputframe.length);
//    image_raw.loadFromData((const uchar*)pd.outputgray.start, pd.outputgray.length);
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
