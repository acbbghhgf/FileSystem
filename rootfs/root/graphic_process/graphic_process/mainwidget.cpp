#include "mainwidget.h"
#include "ui_mainwidget.h"

mainWidget::mainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainWidget)
{
    ui->setupUi(this);
    graphicsView_width = ui->graphicsView->width();
    graphicsView_height = ui->graphicsView->height();
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(update_display()));
//    timer->start(1000);
    tableview_init();
    static_data_init();

}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::tableview_init(void)
{
    //使用tableview
    model = new QStandardItemModel();
    model->setColumnCount(9);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("region"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("x1"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("y1"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("x2"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("y2"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("yz"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("width"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("height"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("ID"));
    ui->tableView->setModel(model);
    //表头信息显示居左
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽不可变
    ui->tableView->setColumnWidth(0,20);
    ui->tableView->setColumnWidth(1,20);
    ui->tableView->setColumnWidth(2,20);
    ui->tableView->setColumnWidth(3,20);
    ui->tableView->setColumnWidth(4,20);
    ui->tableView->setColumnWidth(5,20);
    ui->tableView->setColumnWidth(6,20);
    ui->tableView->setColumnWidth(7,20);
    ui->tableView->setColumnWidth(8,20);
}

void mainWidget::tableview_data_clear(void)
{
    model->clear();
    model->setColumnCount(9);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("region"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("x1"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("y1"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("x2"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("y2"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("yz"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("width"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("height"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("ID"));
    ui->tableView->setModel(model);
    //表头信息显示居左
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //设置列宽不可变
    ui->tableView->setColumnWidth(0,20);
    ui->tableView->setColumnWidth(1,20);
    ui->tableView->setColumnWidth(2,20);
    ui->tableView->setColumnWidth(3,20);
    ui->tableView->setColumnWidth(4,20);
    ui->tableView->setColumnWidth(5,20);
    ui->tableView->setColumnWidth(6,20);
    ui->tableView->setColumnWidth(7,20);
    ui->tableView->setColumnWidth(8,20);
}

void mainWidget::tableview_add_item(int region_value, int x1_value, int y1_value, int x2_value, int y2_value, int yz_value, int width_value, int height_value, int id_value)
{

    //add model item value
    QList<QStandardItem*> mlist;
    mlist.append(new QStandardItem(QString("%1").arg(region_value)));
    mlist.append(new QStandardItem(QString("%1").arg(x1_value)));
    mlist.append(new QStandardItem(QString("%1").arg(y1_value)));
    mlist.append(new QStandardItem(QString("%1").arg(x2_value)));
    mlist.append(new QStandardItem(QString("%1").arg(y2_value)));
    mlist.append(new QStandardItem(QString("%1").arg(yz_value)));
    mlist.append(new QStandardItem(QString("%1").arg(width_value)));
    mlist.append(new QStandardItem(QString("%1").arg(height_value)));
    mlist.append(new QStandardItem(QString("%1").arg(id_value)));
    model->appendRow(mlist);
}

void mainWidget::static_data_init()
{
    //init edit
    QRegExp regx("[0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    ui->x1_edit->setValidator(validator);
    ui->x2_edit->setValidator(validator);
    ui->y1_edit->setValidator(validator);
    ui->y2_edit->setValidator(validator);
    ui->X_edit->setValidator(validator);
    ui->Y_edit->setValidator(validator);
    ui->yz_edit->setValidator(validator);
    ui->id_edit->setValidator(validator);
    ui->width_edit->setValidator(validator);
    ui->height_edit->setValidator(validator);

    edit_display_x1 = 0;
    edit_display_x2 = 0;
    edit_display_y1 = 0;
    edit_display_y2 = 0;
    edit_display_X = 0;
    edit_display_Y = 0;
    edit_display_yz = 0;
    edit_display_width = 0;
    edit_display_height = 0;
}

void mainWidget::DataBase_init(void)
{
    open_database_table();
}

void mainWidget::open_database_table()
{
    //打开数据表
    qDebug() << "qsqldatabase::drivers = " << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DATAFILE);
    db.setUserName("root");
    db.setPassword("123456");
}

void mainWidget::create_datebase_table(void)
{
    bool isOk = db.open();
        if(!isOk){
            qDebug()<<"error info :"<<db.lastError();
        }
        else{
            QSqlQuery query;
    //        query=QSqlQuery::QSqlQuery(db);
            QString creatTableStr = "CREATE TABLE test\
                    (                                       \
                        cust_id      char(10),     \
                        cust_name    char(250) \
                        );";
            query.prepare(creatTableStr);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
                query.finish();
    //            db.close();
            }
            else{
                qDebug()<<"creat table success!";
                query.finish();
            }
        }
    //    db.close();
}

void mainWidget::update_display()
{
    //update edit display
    ui->x1_edit->setText(QString::number(edit_display_x1));
    ui->x2_edit->setText(QString::number(edit_display_x2));
    ui->y1_edit->setText(QString::number(edit_display_y1));
    ui->y2_edit->setText(QString::number(edit_display_y2));
    ui->X_edit->setText(QString::number(edit_display_X));
    ui->Y_edit->setText(QString::number(edit_display_Y));
    ui->yz_edit->setText(QString::number(edit_display_yz));
    ui->height_edit->setText(QString::number(edit_display_height));
    ui->width_edit->setText(QString::number(edit_display_width));

    //update graph view display
    ui->graphicsView->setScene(&originalScene);
    ui->graphicsView->show();

}

void mainWidget::on_setHeader_pb_clicked()
{
    //设置表头
    qDebug() << "set header push button.";
}

void mainWidget::on_exit_pb_clicked()
{
    qDebug() << "process exit.";
    if(db.isOpen())
        db.close();
    this->close();
}

void mainWidget::on_select_pb_clicked()
{
    pictrue_path = QFileDialog::getOpenFileName(
                this, "choose src Directory",
                "/home/ww/bmp_demo");

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
    src_picture = cv::imread(pictrue_path.toStdString(), 1);
    process_mid_picture = src_picture.clone();
    qDebug() << "proce _ channel = " << process_mid_picture.channels();
    edit_display_x1 = 0;
    edit_display_x2 = process_mid_picture.cols;
    edit_display_y1 = 0;
    edit_display_y2 = process_mid_picture.rows;
    edit_display_yz = 49;
    edit_display_width = 1;
    edit_display_height = 1;

    qDebug() << "proc_cols = " << process_mid_picture.cols << " proc_rows = " << process_mid_picture.rows;
    m_updatePixmap(process_mid_picture, QImage::Format_RGB888);

    update_display();
}


void mainWidget::m_updatePixmap(cv::Mat &dst, QImage::Format flag)
{

    originalScene.clear();
    cv::Mat process = dst.clone();
//    cv::Rect rect(edit_display_x1, edit_display_y1, edit_display_x2 - edit_display_x1, edit_display_y2 - edit_display_y1);
//    cv::rectangle(process, rect, cv::Scalar(255, 0, 0), 1, 8, 0);
    cv::rectangle(process, cv::Point(edit_display_x1, edit_display_y1), cv::Point(edit_display_x2, edit_display_y2), cv::Scalar(255, 0, 0), 1, 8, 0);
//    cv::line(process, cv::Point(edit_display_x1+50, edit_display_y1), cv::Point(edit_display_x1+50, edit_display_y2), cv::Scalar(255, 0, 0), 1, 8, 0);
    cv::imwrite("/home/ww/bmp_demo/save.png", process);
    qDebug() << "proc_cols = " << dst.cols << " proc_rows = " << dst.rows << "graphic width =" << graphicsView_width << " graphic height =" << graphicsView_height;
    QImage image("/home/ww/bmp_demo/save.png");
    pixmap = QPixmap::fromImage(image);
    originalScene.addPixmap(pixmap.scaled(graphicsView_width, graphicsView_height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

void mainWidget::m_update_Onepartition_Pixmap(cv::Mat &dst, QImage::Format flag, int width_value, int width_num, int height_value, int height_num)
{
    int x1[width_num][height_num];
    int x2[width_num][height_num];
    int y1[width_num][height_num];
    int y2[width_num][height_num];
    originalScene.clear();
    cv::Mat process = dst.clone();
    cv::rectangle(process, cv::Point(edit_display_x1, edit_display_y1), cv::Point(edit_display_x2, edit_display_y2), cv::Scalar(0, 0, 255), 1, 8, 0);
    for(int i = 0; i < width_num; i++){
        cv::line(process, cv::Point(edit_display_x1 + i * width_value, edit_display_y1), cv::Point(edit_display_x1 + i *width_value, edit_display_y2), cv::Scalar(0, 0, 255), 1, 8, 0);
        for(int j = 0; j < height_num; j++)
        {
            x1[i][j] = edit_display_x1 + (i-1) *width_value;
            x2[i][j] = edit_display_x1 + i* width_value;
        }
    }
    for(int i = 0; i < height_num; i++){
        cv::line(process, cv::Point(edit_display_x1, edit_display_y1 + i * height_value), cv::Point(edit_display_x2, edit_display_y1 + i * height_value), cv::Scalar(0, 0, 255), 1, 8, 0);
        for(int j = 0; j < height_num; j++)
        {
            y1[i][j] = edit_display_x1 + (i-1) *width_value;
            y2[i][j] = edit_display_x1 + i* width_value;
        }
    }
    cv::imwrite("/home/ww/bmp_demo/save.png", process);
    QImage image("/home/ww/bmp_demo/save.png");
    pixmap = QPixmap::fromImage(image);
    originalScene.addPixmap(pixmap.scaled(graphicsView_width, graphicsView_height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //add tableview data

    tableview_data_clear();
    for(int i = 0 ; i <= width_num; i++)
    {
        for(int j = 0; j <= height_num; j++)
        {
            tableview_add_item(i+j, x1[i][j], y1[i][j], x2[i][j], y2[i][j], edit_display_yz, 1, 1, i+j);
        }
    }

}

void mainWidget::on_modify_pb_clicked()
{
    int tmp_x1, tmp_y1, tmp_width, tmp_height;
    qDebug() << "x1 " << edit_display_x1 << "x2 " << edit_display_x2 << "y1 " << edit_display_y1 << "y2 " << edit_display_y2;
    if(edit_display_x1 <= 10)
        tmp_x1 = edit_display_x1;
    else
        tmp_x1 = edit_display_x1 - 10;
    if(edit_display_y1 <= 10)
        tmp_y1 = edit_display_y1;
    else
        tmp_y1 = edit_display_y1 - 10;
    if((edit_display_x2 - edit_display_x1) <= 0)
        tmp_width = 0;
    else
        tmp_width = edit_display_x2 - edit_display_x1;
    if((edit_display_y2 - edit_display_y1) <= 0)
        tmp_height = 0;
    else
        tmp_height = edit_display_y2 - edit_display_y1;

//    process_mid_picture = process_mid_picture(cv::Rect(tmp_x1, tmp_y1, tmp_width, tmp_height));
    tableview_data_clear();
    tableview_add_item(0, edit_display_x1, edit_display_y1, edit_display_x2, edit_display_y2, edit_display_width, edit_display_yz, edit_display_width, edit_display_height);
    m_updatePixmap(process_mid_picture, QImage::Format_Grayscale8);
    update_display();
}

void mainWidget::on_x1_edit_textChanged(const QString &arg1)
{
    edit_display_x1 = ui->x1_edit->text().toInt();
}

void mainWidget::on_y1_edit_textChanged(const QString &arg1)
{
    edit_display_y1 = ui->y1_edit->text().toInt();
}

void mainWidget::on_x2_edit_textChanged(const QString &arg1)
{
    edit_display_x2 = ui->x2_edit->text().toInt();
}

void mainWidget::on_y2_edit_textChanged(const QString &arg1)
{
    edit_display_y2 = ui->y2_edit->text().toInt();
}

void mainWidget::on_yz_edit_textChanged(const QString &arg1)
{
    edit_display_yz = ui->yz_edit->text().toInt();
}

void mainWidget::on_width_edit_textChanged(const QString &arg1)
{
    edit_display_width = ui->width_edit->text().toInt();
}

void mainWidget::on_height_edit_textChanged(const QString &arg1)
{
    edit_display_height = ui->height_edit->text().toInt();
}

void mainWidget::on_X_edit_textChanged(const QString &arg1)
{
        edit_display_X = ui->X_edit->text().toInt();
}

void mainWidget::on_Y_edit_textChanged(const QString &arg1)
{
        edit_display_Y = ui->Y_edit->text().toInt();
}

void mainWidget::on_db_test_add_clicked()
{
    qDebug() << "test db push button.";
}

void mainWidget::on_one_click_partition_pb_clicked()
{
    int width_value = (edit_display_x2 - edit_display_x1) / edit_display_width;
    int height_value = (edit_display_y2 - edit_display_y1) / edit_display_height;

    m_update_Onepartition_Pixmap(process_mid_picture, QImage::Format_RGB888, width_value, edit_display_width, height_value, edit_display_height);

    update_display();
}
