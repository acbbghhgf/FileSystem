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

void mainWidget::tableview_add_item(QString region_value, int x1_value, int y1_value, int x2_value, int y2_value, int yz_value, int width_value, int height_value, int id_value)
{

    //add model item value
    QList<QStandardItem*> mlist;
    mlist.append(new QStandardItem(region_value));
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

    one_partiton_width_coe = 0;
    ope_partition_height_coe = 0;
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

void mainWidget::on_exit_pb_clicked()
{
    qDebug() << "process exit.";
    if(db.isOpen())
    {
        qDebug() << "db close.";
        db.close();
    }
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
    m_updatePixmap(process_mid_picture);

    update_display();
}


void mainWidget::m_updatePixmap(cv::Mat &dst)
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
    }
    for(int i = 0; i < height_num; i++){
        cv::line(process, cv::Point(edit_display_x1, edit_display_y1 + i * height_value), cv::Point(edit_display_x2, edit_display_y1 + i * height_value), cv::Scalar(0, 0, 255), 1, 8, 0);
    }
    cv::imwrite("/home/ww/bmp_demo/save.png", process);
    QImage image("/home/ww/bmp_demo/save.png");
    pixmap = QPixmap::fromImage(image);
    originalScene.addPixmap(pixmap.scaled(graphicsView_width, graphicsView_height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //add tableview data

    for(int i = 0; i < width_num; i++)
    {
        for(int j = 0; j < height_num; j++)
        {
            x1[i][j] = edit_display_x1 + i * width_value;
            y1[i][j] = edit_display_y1 + j * height_value;
            x2[i][j] = edit_display_x1 + (i + 1) * width_value;
            y2[i][j] = edit_display_y1 + (j + 1) * height_value;
        }
    }
    tableview_data_clear();
    for(int j = 0; j < height_num; j++)
    {
        for(int i = 0 ; i < width_num; i++)
        {
            tableview_add_item(QString("%1-%2").arg(i).arg(j), x1[i][j], y1[i][j], x2[i][j], y2[i][j], edit_display_yz, 1, 1, i+j);
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
    tableview_add_item(QString("0-0"), edit_display_x1, edit_display_y1, edit_display_x2, edit_display_y2, edit_display_width, edit_display_yz, edit_display_width, edit_display_height);
    m_updatePixmap(process_mid_picture);
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


void mainWidget::on_one_click_partition_pb_clicked()
{
    ope_partition_height_coe = edit_display_height;
    one_partiton_width_coe = edit_display_width;
    int width_value = (edit_display_x2 - edit_display_x1) / edit_display_width;
    int height_value = (edit_display_y2 - edit_display_y1) / edit_display_height;

    m_update_Onepartition_Pixmap(process_mid_picture, QImage::Format_RGB888, width_value, edit_display_width, height_value, edit_display_height);

    update_display();
}

void mainWidget::on_two_partition_pb_clicked()
{
    two_partition_width_coe = SPILT_WIDTH / one_partiton_width_coe;
    two_partition_height_coe = SPILT_HEIGHT / ope_partition_height_coe;
    int field_partition_width_coe = two_partition_width_coe;
    int field_partition_height_coe = two_partition_height_coe;
    qDebug() << "two_width_coe :" << two_partition_width_coe<< "two_height_coe :" << two_partition_height_coe;
    qDebug() << "field_width_coe :" << field_partition_width_coe << "field_height_coe :" << field_partition_height_coe;
    int field_x1[field_partition_width_coe][field_partition_height_coe];
    int field_y1[field_partition_width_coe][field_partition_height_coe];
    int field_x2[field_partition_width_coe][field_partition_height_coe];
    int field_y2[field_partition_width_coe][field_partition_height_coe];
    int field_width_value = 0;
    int field_height_value = 0;

    QString pa_region;
    int pa_x1, pa_y1, pa_x2, pa_y2, pa_yz, pa_width, pa_height, pa_id;
    QList<QStandardItem *> tmp_list;
    int counter = 0;
    int mode_count = model->rowCount();

    qDebug() << "current rows = " << model->rowCount() << "mode_count = " << mode_count;
    for(int i = 0 ; i < mode_count; i++)
    {
        //process row data:
        tmp_list = model->takeRow(0);
        if(tmp_list.isEmpty())
        {
            qDebug() << "tmp_list is null";
            break;
        }
        pa_region = tmp_list.takeFirst()->text();
        pa_x1 = tmp_list.takeFirst()->text().toInt();
        pa_y1 = tmp_list.takeFirst()->text().toInt();
        pa_x2 = tmp_list.takeFirst()->text().toInt();
        pa_y2 = tmp_list.takeFirst()->text().toInt();
        pa_yz = tmp_list.takeFirst()->text().toInt();
        pa_width = tmp_list.takeFirst()->text().toInt();
        pa_height = tmp_list.takeFirst()->text().toInt();
        pa_id = tmp_list.takeFirst()->text().toInt();
        //add insert database data
        field_width_value = (pa_x2 - pa_x1) / field_partition_width_coe;
        field_height_value = (pa_y2 - pa_y1) / field_partition_height_coe;

        for(int j = 0; j < field_partition_width_coe; j++)
        {
            for(int k = 0; k < field_partition_height_coe; k++)
            {
                field_x1[j][k] = pa_x1 + j * field_width_value;
                field_y1[j][k] = pa_y1 + j * field_height_value;
                field_x2[j][k] = pa_x1 + (j + 1) * field_width_value;
                field_y2[j][k] = pa_y1 + (j + 1) * field_height_value;
            }
        }
        //insert database small data
        for(int k = 0; k < field_partition_height_coe; k++)
        {
            for(int j = 0; j < field_partition_width_coe; j++)
            {

                //fen small insert databas
                QSqlQuery query;
                int field_id_value = j+ k * field_partition_width_coe + i * field_partition_width_coe * field_partition_height_coe;

                query.prepare("insert into test (field_id, x1, y1, x2, y2, yz) values (:field_id,:x1, :y1, :x2, :y2, :yz)");
                query.bindValue(":field_id", field_id_value);
                query.bindValue(":x1", field_x1[j][k]);
                query.bindValue(":y1", field_y1[j][k]);
                query.bindValue(":x2", field_x1[j][k]);
                query.bindValue(":y2", field_y2[j][k]);
                query.bindValue(":yz", edit_display_yz);
                if(!query.exec())
                {
                    qDebug()<<query.lastError();
                }
                else
                {
//                    qDebug()<<"inserted!";
                    counter++;
                    query.finish();
                }

            }
        }
    }
    qDebug() << "data base insert counter = " << counter;
}

void mainWidget::on_db_open_pb_clicked()
{
    qDebug() << "qsqldatabase::drivers = " << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("testDB.db");
    db.setUserName("root");
    db.setPassword("123456");
    qDebug() << "qsqldatabase set success";
}

void mainWidget::on_db_create_pb_clicked()
{
    bool isOk = db.open();
        if(!isOk){
            qDebug()<<"error info :"<<db.lastError();
        }
        else{
            QSqlQuery query;
            // create table test
            QString creatTableStr = "CREATE TABLE test\
                    ( \
                       field_id int, \
                        x1 int, \
                        y1 int, \
                        x2 int, \
                        y2 int, \
                        yz int, \
                        black_threshold_1 int, \
                        white_threshold_1 int, \
                        second_threshold_1 int \
                        black_threshold_2 int, \
                        white_threshold_2 int, \
                        second_threshold_2 int \
                        black_threshold_4 int, \
                        white_threshold_4 int, \
                        second_threshold_4 int \
                        black_threshold_8 int, \
                        white_threshold_8 int, \
                        second_threshold_8 int \
                        );";
            query.prepare(creatTableStr);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
                query.finish();
            }
            else{
                qDebug()<<"creat table success!";
                query.finish();
            }
        }
}

void mainWidget::on_db_select_pb_clicked()
{
    QSqlQuery query;
    //更新数据
    QString update_sql = "update test";
    query.prepare(update_sql);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }
    else
    {
        qDebug()<<"updated!";
        query.finish();
    }

    //查询所有数据
    QString select_all_sql = "select * from test";
    model->clear();
    //set model display
    model->setColumnCount(18);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("田字格序号"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("x1"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("y1"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("x2"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("y2"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("yz"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("黑色阈值_1"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("白色阈值_1"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("二次阈值_1"));
    model->setHeaderData(9, Qt::Horizontal, QString::fromLocal8Bit("黑色阈值_2"));
    model->setHeaderData(10, Qt::Horizontal, QString::fromLocal8Bit("白色阈值_2"));
    model->setHeaderData(11, Qt::Horizontal, QString::fromLocal8Bit("二次阈值_2"));
    model->setHeaderData(12, Qt::Horizontal, QString::fromLocal8Bit("黑色阈值_4"));
    model->setHeaderData(13, Qt::Horizontal, QString::fromLocal8Bit("白色阈值_4"));
    model->setHeaderData(14, Qt::Horizontal, QString::fromLocal8Bit("二次阈值_4"));
    model->setHeaderData(15, Qt::Horizontal, QString::fromLocal8Bit("黑色阈值_8"));
    model->setHeaderData(16, Qt::Horizontal, QString::fromLocal8Bit("白色阈值_8"));
    model->setHeaderData(17, Qt::Horizontal, QString::fromLocal8Bit("二次阈值_8"));

    ui->tableView->setColumnWidth(0,15);
    ui->tableView->setColumnWidth(1,15);
    ui->tableView->setColumnWidth(2,15);
    ui->tableView->setColumnWidth(3,15);
    ui->tableView->setColumnWidth(4,15);
    ui->tableView->setColumnWidth(5,15);
    ui->tableView->setColumnWidth(6,15);
    ui->tableView->setColumnWidth(7,15);
    ui->tableView->setColumnWidth(8,15);
    ui->tableView->setColumnWidth(9,15);
    ui->tableView->setColumnWidth(10,15);
    ui->tableView->setColumnWidth(11,15);
    ui->tableView->setColumnWidth(12,15);
    ui->tableView->setColumnWidth(13,15);
    ui->tableView->setColumnWidth(14,15);
    ui->tableView->setColumnWidth(15,15);
    ui->tableView->setColumnWidth(16,15);
    ui->tableView->setColumnWidth(17,15);

    query.prepare(select_all_sql);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }
    else
    {
        int i = 0;
        while(query.next())
        {
            QString field_id = query.value(0).toString();
            QString x1_value = query.value(1).toString();
            QString y1_value = query.value(2).toString();
            QString x2_value = query.value(3).toString();
            QString y2_value = query.value(4).toString();
            QString yz_value = query.value(5).toString();
            QString black_threshold_1 = query.value(6).toString();
            QString white_threshold_1 = query.value(7).toString();
            QString second_threshold_1 = query.value(8).toString();
            QString black_threshold_2 = query.value(9).toString();
            QString white_threshold_2 = query.value(10).toString();
            QString second_threshold_2 = query.value(11).toString();
            QString black_threshold_4 = query.value(12).toString();
            QString white_threshold_4 = query.value(13).toString();
            QString second_threshold_4 = query.value(14).toString();
            QString black_threshold_8 = query.value(15).toString();
            QString white_threshold_8 = query.value(16).toString();
            QString second_threshold_8 = query.value(17).toString();

            model->setItem(i,0, new QStandardItem(field_id));
            model->setItem(i,1, new QStandardItem(x1_value));
            model->setItem(i,2, new QStandardItem(y1_value));
            model->setItem(i,3, new QStandardItem(x2_value));
            model->setItem(i,4, new QStandardItem(y2_value));
            model->setItem(i,5, new QStandardItem(yz_value));
            model->setItem(i,6, new QStandardItem(black_threshold_1));
            model->setItem(i,7, new QStandardItem(white_threshold_1));
            model->setItem(i,8, new QStandardItem(second_threshold_1));
            model->setItem(i,9, new QStandardItem(black_threshold_2));
            model->setItem(i,10, new QStandardItem(white_threshold_2));
            model->setItem(i,11, new QStandardItem(second_threshold_2));
            model->setItem(i,12, new QStandardItem(black_threshold_4));
            model->setItem(i,13, new QStandardItem(white_threshold_4));
            model->setItem(i,14, new QStandardItem(second_threshold_4));
            model->setItem(i,15, new QStandardItem(black_threshold_8));
            model->setItem(i,16, new QStandardItem(white_threshold_8));
            model->setItem(i,17, new QStandardItem(second_threshold_8));
//            qDebug()<<QString("field_id:%1 x1:%2 y1:%3 x2:%4 y2:%5 yz:%6 \
                    black_t_1:%7 white_t_1:%8 second_t_1:%9").arg(field_id)\
                      .arg(x1_value).arg(y1_value).arg(x2_value).arg(y2_value)\
                      .arg(yz_value).arg(black_threshold_1).arg(white_threshold_1).arg(second_threshold_1);

            i++;
        }
        query.finish();
    }
}

void mainWidget::on_db_drop_pb_clicked()
{
    QSqlQuery query;
    QString drop_sql = "drop table test";
    query.prepare(drop_sql);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }
    else
    {
        qDebug()<<"updated!";
        query.finish();
    }
    model->clear();
}

void mainWidget::on_training_pb_clicked()
{
    make_picture_data(SPILT_HEIGHT, SPILT_WIDTH);
}


int mainWidget::update_database_threshold(cv::Mat &src, QString &pic_data, int pic_num)
{
    //src input black and white threshold.
    QString pic_data_ch = get_pic_data(pic_num);
    QByteArray pic_data_byte = QByteArray::fromBase64(pic_data_ch.toLocal8Bit());
    for(int i = 0; i< pic_data_byte.size(); ++i)
    {
        //process data base and update data base black and white
    }
}

void mainWidget::on_generate_partition_pb_clicked()
{
    //generate partition

    int dx = (edit_display_x2 - edit_display_x1) / SPILT_WIDTH;
    int dy = (edit_display_y2 - edit_display_y1) / SPILT_HEIGHT;
    for(int j = 0; j < SPILT_HEIGHT; ++j)
    {
        for(int i = 0; i < SPILT_WIDTH; ++i)
        {
            x1_gen_part_value[i][j] = edit_display_x1 + i * dx;
            y1_gen_part_value[i][j] = edit_display_y1 + j * dy;
            x2_gen_part_value[i][j] = edit_display_x1 + (i + 1) * dx;
            y2_gen_part_value[i][j] = edit_display_y1 + (j + 1) * dy;
        }
    }
    tableview_data_clear();
    for(int j = 0; j < SPILT_HEIGHT; j++)
    {
        for(int i = 0 ; i < SPILT_WIDTH; i++)
        {
            tableview_add_item(QString("%1-%2").arg(i).arg(j), x1_gen_part_value[i][j], y1_gen_part_value[i][j], x2_gen_part_value[i][j], y2_gen_part_value[i][j], edit_display_yz, 1, 1, i*SPILT_WIDTH+j*SPILT_HEIGHT);
        }
    }
}

void mainWidget::on_load_partition_pb_clicked()
{
    int counter = 0;
    //insert database small data
    for(int k = 0; k < SPILT_HEIGHT; k++)
    {
        for(int j = 0; j < SPILT_WIDTH; j++)
        {

            //fen small insert databas
            QSqlQuery query;
            int field_id_value = j + k * SPILT_WIDTH;

            query.prepare("insert into test (field_id, x1, y1, x2, y2, yz) values (:field_id,:x1, :y1, :x2, :y2, :yz)");
            query.bindValue(":field_id", field_id_value);
            query.bindValue(":x1", x1_gen_part_value[j][k]);
            query.bindValue(":y1", y1_gen_part_value[j][k]);
            query.bindValue(":x2", x2_gen_part_value[j][k]);
            query.bindValue(":y2", y2_gen_part_value[j][k]);
            query.bindValue(":yz", edit_display_yz);
            if(!query.exec())
            {
                qDebug()<<query.lastError();
            }
            else
            {
                //                    qDebug()<<"inserted!";
                counter++;
                query.finish();
            }

        }
    }
    qDebug() << "data base insert counter = " << counter;

}
