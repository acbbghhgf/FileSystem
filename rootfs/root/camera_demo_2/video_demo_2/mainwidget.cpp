#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    ui->init_pb->setEnabled(true);
    ui->start_pb->setEnabled(false);
    ui->stop_pb->setEnabled(false);
    ui->exit_pb->setEnabled(true);
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(get_img()));
//    timer->start(100);
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::get_img()
{
    int ret = 0;
    wwk_uchar_t *uc_image;
    ret = acq_image_getimag_work(acq, &uc_image);
    if(uc_image && ret != -1)
    {
        wwk_debug("obtin image length : %d\n", uc_image->len);
        image_process(uc_image);
        wwk_free(uc_image);
    }
    else
    {
        wwk_debug("obtion image error or null....\n");
//        sleep(1);
    }
}

void mainWidget::on_exit_pb_clicked()
{
    qDebug() << "func quit.";
    ui->textEdit->append("func quit...\n");
    this->close();
}

void mainWidget::on_init_pb_clicked()
{
    acq = acq_image_init();
    ui->init_pb->setEnabled(false);
    ui->start_pb->setEnabled(true);
}

void mainWidget::on_start_pb_clicked()
{
    acq->run = 1;
    pthread_create(&t, NULL, acq_image_work, (void *)acq);
    ui->stop_pb->setEnabled(true);
    ui->start_pb->setEnabled(false);
    update();
    timer->start(50);
    qDebug() << "pop queue quit.";
}

void mainWidget::on_stop_pb_clicked()
{
    int tmp = 0;
    int *ret = &tmp;
    acq->run = 0;
    pthread_join(t, (void **)&ret);
    timer->stop();
    qDebug() << "work thread quit.";
    ui->textEdit->append("work thread quit.");
    ui->start_pb->setEnabled(true);
    ui->stop_pb->setEnabled(false);
}

void mainWidget::image_process(wwk_uchar_t *uc)
{
    QImage image_raw;
    QPixmap image_pix;
    wwk_debug("image process uc len: %d\n", uc->len);
    image_raw.loadFromData(uc->data, uc->len, nullptr);
//    image_raw = QImage(uc->data, 640, 480, 8, QImage::Format_Grayscale8);
//    image_raw.save("/home/ww/newpicture", "PNG", 100);
    image_pix.loadFromData(uc->data, uc->len, nullptr, Qt::ColorOnly);
    QGraphicsScene *originalScene = new QGraphicsScene;
//    originalScene->addPixmap(QPixmap::fromImage(image_raw));
    originalScene->addPixmap(image_pix);
    ui->graphicsView->setScene(originalScene);
    ui->graphicsView->show();
    sleep(1);
    ui->textEdit->append("get picture...");
}
