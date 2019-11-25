#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    graphicsView_width = ui->graphicsView->width();
    graphicsView_height = ui->graphicsView->height();
}

mainWidget::~mainWidget()
{
    delete ui;
}


void mainWidget::on_setHeader_pb_clicked()
{
    //设置表头
        QTableWidgetItem    *headerItem;
        QStringList headerText;
        headerText<<"序 号"<<"X1"<<"Y1"<<"X2"<<"Y2"<<"YZ"<<"theshold";  //表头标题用QStringList来表示
        ui->tableInfo->setHorizontalHeaderLabels(headerText);
        ui->tableInfo->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
        for (int i=0;i<ui->tableInfo->columnCount();i++)//列编号从0开始
        {
           headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
           QFont font=headerItem->font();//获取原有字体设置
           font.setBold(true);//设置为粗体
           font.setPointSize(8);//字体大小
           headerItem->setTextColor(Qt::red);//字体颜色
           headerItem->setFont(font);//设置字体
           ui->tableInfo->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
        }

//        ui->tableInfo->setItemDelegateForColumn(colScore,&spinDelegate);//设置自定义代理组件
}

void mainWidget::on_exit_pb_clicked()
{
    qDebug() << "process exit.";
    this->close();
}

void mainWidget::on_select_pb_clicked()
{
    pictrue_path = QFileDialog::getOpenFileName(
                this, "choose src Directory",
                "/home/ww/");

    if (pictrue_path.isEmpty())
    {
        qDebug() << "get filename path error.";
        return;
    }
    else
    {
        qDebug() << "get filename path success. filename = " << pictrue_path;
    }
    ui->picture_path_edit->setText(pictrue_path);
    QImage image(pictrue_path, "bmp");
    QPixmap pixmap = QPixmap::fromImage(image);
    originalScene.addPixmap(pixmap.scaled(graphicsView_width, graphicsView_height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->graphicsView->setScene(&originalScene);
    ui->graphicsView->show();


}
