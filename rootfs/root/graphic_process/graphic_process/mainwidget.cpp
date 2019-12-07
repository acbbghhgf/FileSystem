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
    init_serial_port();
    // init database
    init_database();
}

mainWidget::~mainWidget()
{
    delete ui;
}

void mainWidget::tableview_init(void)
{
    //使用tableview
    model = new QStandardItemModel();
    model->setColumnCount(10);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("one_partition"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("two_partition"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("x1"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("y1"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("x2"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("y2"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("yz"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("width"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("height"));
    model->setHeaderData(9, Qt::Horizontal, QString::fromLocal8Bit("ID"));
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
    ui->tableView->setColumnWidth(9,20);
}


void mainWidget::init_database(void)
{
    qDebug() << "qsqldatabase::drivers = " << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("testDB.db");
    db.setUserName("root");
    db.setPassword("123456");
    qDebug() << "qsqldatabase set success";

    if(database_create_partiton_table())
        qDebug() << "create partition table error.";
    if(database_create_threshold_table())
        qDebug() << "create threshold table error.";
}

int mainWidget::database_create_partiton_table()
{
    bool isOk = db.open();
    if(!isOk){
        qDebug()<<"error info :"<<db.lastError();
        return -1;
    }
    else{
        QSqlQuery query;
        // create table test
        QString creatTableStr = "CREATE TABLE partiton\
                ( \
                    one_partiton char, \
                    two_partiton char, \
                    x1 int, \
                    y1 int, \
                    x2 int, \
                    y2 int, \
                    yz int, \
                    label int);";
        query.prepare(creatTableStr);
        if(!query.exec()){
            qDebug()<<"query error :"<<query.lastError();
            query.finish();
            return -1;
        }
        else{
            qDebug()<<"creat table success!";
            query.finish();
        }
    }
    return 0;
}

int mainWidget::database_create_threshold_table()
{
    bool isOk = db.open();
    if(!isOk){
        qDebug()<<"error info :"<<db.lastError();
        return -1;
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
                    second_threshold_1 int, \
                    black_threshold_2 int, \
                    white_threshold_2 int, \
                    second_threshold_2 int, \
                    black_threshold_4 int, \
                    white_threshold_4 int, \
                    second_threshold_4 int, \
                    black_threshold_8 int, \
                    white_threshold_8 int, \
                    second_threshold_8 int, \
                    judge_1 int, \
                    judge_2 int, \
                    judge_4 int, \
                    judge_8 int);";
        query.prepare(creatTableStr);
        if(!query.exec()){
            qDebug()<<"query error :"<<query.lastError();
            query.finish();
            return -1;
        }
        else{
            qDebug()<<"creat table success!";
            query.finish();
        }
    }
    return 0;
}

void mainWidget::tableview_data_clear(void)
{
    model->clear();
    model->setColumnCount(10);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("one_partition"));
    model->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("two_partition"));
    model->setHeaderData(2, Qt::Horizontal, QString::fromLocal8Bit("x1"));
    model->setHeaderData(3, Qt::Horizontal, QString::fromLocal8Bit("y1"));
    model->setHeaderData(4, Qt::Horizontal, QString::fromLocal8Bit("x2"));
    model->setHeaderData(5, Qt::Horizontal, QString::fromLocal8Bit("y2"));
    model->setHeaderData(6, Qt::Horizontal, QString::fromLocal8Bit("yz"));
    model->setHeaderData(7, Qt::Horizontal, QString::fromLocal8Bit("width"));
    model->setHeaderData(8, Qt::Horizontal, QString::fromLocal8Bit("height"));
    model->setHeaderData(9, Qt::Horizontal, QString::fromLocal8Bit("ID"));
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
    ui->tableView->setColumnWidth(9,20);
}

void mainWidget::tableview_add_item(QString one_partition, QString two_partition, int x1_value, int y1_value, int x2_value, int y2_value, int yz_value, int width_value, int height_value, int id_value)
{

    //add model item value
    QList<QStandardItem*> mlist;
    mlist.append(new QStandardItem(one_partition));
    mlist.append(new QStandardItem(two_partition));
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

void mainWidget::tableview_insert_item(int index, QString one_partition, QString two_partition, int x1_value, int y1_value, int x2_value, int y2_value, int yz_value, int width_value, int height_value, int id_value)
{
    //add model item value
    QList<QStandardItem*> mlist;
    mlist.append(new QStandardItem(one_partition));
    mlist.append(new QStandardItem(two_partition));
    mlist.append(new QStandardItem(QString("%1").arg(x1_value)));
    mlist.append(new QStandardItem(QString("%1").arg(y1_value)));
    mlist.append(new QStandardItem(QString("%1").arg(x2_value)));
    mlist.append(new QStandardItem(QString("%1").arg(y2_value)));
    mlist.append(new QStandardItem(QString("%1").arg(yz_value)));
    mlist.append(new QStandardItem(QString("%1").arg(width_value)));
    mlist.append(new QStandardItem(QString("%1").arg(height_value)));
    mlist.append(new QStandardItem(QString("%1").arg(id_value)));

    model->insertRow(index, mlist);
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
    QSqlQuery query;
    //drop table partiton
    query.clear();
    QString drop_sql_partition = "drop table partiton;";
    query.prepare(drop_sql_partition);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }
    else
    {
        qDebug()<<"updated!";
    }
    query.finish();

    //drop table test
    QString drop_sql = "drop table test";
    query.clear();
    query.prepare(drop_sql);
    if(!query.exec())
    {
        qDebug()<<query.lastError();
    }
    else
    {
        qDebug()<<"updated!";
    }
    query.finish();

    if(db.isOpen())
    {
        qDebug() << "db close.";
        db.close();
    }

    //qt close.
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
    QSqlQuery query;
    for(int j = 0; j < height_num; j++)
    {
        for(int i = 0 ; i < width_num; i++)
        {
            QString one_partition_label = QString("%1-%2").arg(i).arg(j);
            QString two_partition_label = "";
            tableview_add_item(one_partition_label, two_partition_label, x1[i][j], y1[i][j], x2[i][j], y2[i][j], edit_display_yz, 1, 1, i+j);
            QString update_one_partition = "update partiton set \
                                            one_partiton = :one_partiton \
                                            where x1 >= :x1 and x2 <= :x2 and y1 >= :y1 and y2 <= :y2;";
            query.clear();
            query.prepare(update_one_partition);
            query.bindValue(":one_partiton", one_partition_label);
            query.bindValue(":x1", x1[i][j]);
            query.bindValue(":x2", x2[i][j]);
            query.bindValue(":y1", y1[i][j]);
            query.bindValue(":y2", y2[i][j]);
            if(!query.exec()){
                qDebug()<<"query error :"<<query.lastError();
                qDebug() << "error update partition = " << query.executedQuery();
            }
            query.finish();
        }
    }

}

void mainWidget::on_modify_pb_clicked()
{

    int tmp_x1, tmp_y1, tmp_x2, tmp_y2,  tmp_width, tmp_height, yz, X, Y, id;
    QString one_partition, two_partition;
    if(!one_partition_modify_flag)
    {
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

        tableview_data_clear();
        tableview_add_item(QString("0-0"),QString("0-0"), edit_display_x1, edit_display_y1, edit_display_x2, edit_display_y2, edit_display_width, edit_display_yz, edit_display_width, edit_display_height);
        m_updatePixmap(process_mid_picture);
        update_display();
    }
    else
    {
        QList<QStandardItem*> mlist;
        mlist = model->takeRow(model_modify_index);
        if(mlist.isEmpty())
        {
            qDebug() << "get current list is null.";
            return;
        }
        one_partition = mlist.takeFirst()->text();
        two_partition = mlist.takeFirst()->text();
        tmp_x1 = mlist.takeFirst()->text().toInt();
        tmp_y1 = mlist.takeFirst()->text().toInt();
        tmp_x2 = mlist.takeFirst()->text().toInt();
        tmp_y2 = mlist.takeFirst()->text().toInt();
        yz = mlist.takeFirst()->text().toInt();
        tmp_width = mlist.takeFirst()->text().toInt();
        tmp_height = mlist.takeFirst()->text().toInt();
        id = mlist.takeFirst()->text().toInt();

        tmp_x1 = ui->x1_edit->text().toInt();
        tmp_y1 = ui->y1_edit->text().toInt();
        tmp_x2 = ui->x2_edit->text().toInt();
        tmp_y2 = ui->y2_edit->text().toInt();
        tmp_width = ui->width_edit->text().toInt();
        tmp_height = ui->height_edit->text().toInt();
        yz = ui->yz_edit->text().toInt();
        X = ui->X_edit->text().toInt();
        Y = ui->Y_edit->text().toInt();

        tableview_insert_item(model_modify_index, one_partition, two_partition, tmp_x1, tmp_y1, tmp_x2, tmp_y2, yz, tmp_width, tmp_height, id);
    }
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
    if(!one_partition_modify_flag)
        one_partition_modify_flag = true;
    //split 40 * 24 partition insert partition
    init_split_partition(edit_display_x1, edit_display_y1, edit_display_x2, edit_display_y2, edit_display_yz);

    ope_partition_height_coe = edit_display_height;
    one_partiton_width_coe = edit_display_width;
    int width_value = (edit_display_x2 - edit_display_x1) / edit_display_width;
    int height_value = (edit_display_y2 - edit_display_y1) / edit_display_height;

    m_update_Onepartition_Pixmap(process_mid_picture, QImage::Format_RGB888, width_value, edit_display_width, height_value, edit_display_height);

    update_display();

    //generate partition and insert partition
}

void mainWidget::init_split_partition(int x1, int y1, int x2, int y2, int yz)
{
    int dx = (x2 - x1) / SPILT_WIDTH;
    int dy = (y2 - y1) / SPILT_HEIGHT;
    QSqlQuery query;
    //table partiton clear:
    QString clear_sql_partition = "delete from partiton;";
    query.prepare(clear_sql_partition);
    if(!query.exec())
    {
        qDebug() << "error ops :" << query.executedQuery();
        qDebug()<<query.lastError();
    }
    query.finish();

    //insert partition database.
    QString insert_sql_partition = "insert into partiton (x1,y1,x2,y2,yz,label) values (:x1,:y1,:x2,:y2,:yz,:label)";
    for(int j = y1; j < y1 + (dy*SPILT_HEIGHT); j+=dy)
    {
        for(int k = x1; k < x1+(dx*SPILT_WIDTH); k+=dx)
        {
            int x1_value = k;
            int y1_value = j;
            int x2_value = k + dx;
            int y2_value = j + dy;
            int number = k + (j * SPILT_WIDTH);
            query.clear();
            query.prepare(insert_sql_partition);
            query.bindValue(":x1", x1_value);
            query.bindValue(":y1", y1_value);
            query.bindValue(":x2", x2_value);
            query.bindValue(":y2", y2_value);
            query.bindValue(":label", number);
            if(!query.exec())
            {
                qDebug() << "error ops :" << query.executedQuery();
                qDebug()<<query.lastError();
            }
            query.finish();
        }
    }
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

    QString one_partition;
    QString two_partition;
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
        one_partition = tmp_list.takeFirst()->text();
        two_partition = tmp_list.takeFirst()->text();
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


void mainWidget::on_training_pb_clicked()
{
    cv::Mat gray_src = cv::Mat::zeros(src_picture.size(), src_picture.type());
    cv::cvtColor(src_picture, gray_src, cv::COLOR_BGR2GRAY);
    update_database_threshold(gray_src, 18);
}


int mainWidget::update_database_threshold(cv::Mat &src, int pic_num)
{
    QSqlQuery query;
    //src input black and white threshold.
    QString pic_data_ch = get_pic_data(SPILT_HEIGHT, SPILT_WIDTH, pic_num);
    QByteArray pic_data_byte = pic_data_ch.toLatin1();

    unsigned char *data = src.data;
    int step = src.step;
    qDebug() << "pic_data.size = " << pic_data_byte.length();

    for(int i = 0; i< pic_data_byte.length(); ++i)
    {
        unsigned char ch = pic_data_byte[i];
//        qDebug() << "ch = " << ch;
        pic_parse_t ch_parse;
        bzero(&ch_parse, sizeof(ch_parse));
        //process data base and update data base black and white
        for(int n = 0; n < 2; ++n)
        {
            if(n%2 == 0)
            {
                if(ch & 0x80)
                    ch_parse.c8 = 1;
                else
                    ch_parse.c8 = 0;
                if(ch & 0x40)
                    ch_parse.c4 = 1;
                else
                    ch_parse.c4 = 0;
                if(ch & 0x20)
                    ch_parse.c2 = 1;
                else
                    ch_parse.c2 = 0;
                if(ch & 0x10)
                    ch_parse.c1 = 1;
                else
                    ch_parse.c1 = 0;
            }
            else
            {
                if(ch & 0x08)
                    ch_parse.c8 = 1;
                else
                    ch_parse.c8 = 0;
                if(ch & 0x04)
                    ch_parse.c4 = 1;
                else
                    ch_parse.c4 = 0;
                if(ch & 0x02)
                    ch_parse.c2 = 1;
                else
                    ch_parse.c2 = 0;
                if(ch & 0x01)
                    ch_parse.c1 = 1;
                else
                    ch_parse.c1 = 0;
            }
            //select x1 y1 and counter
            int field_id = -1;
            int x1_value = 0;
            int y1_value = 0;
            int x2_value = 0;
            int y2_value = 0;
            int yz_value = 0;
            int black_threshold_1 = 0;
            int white_threshold_1 = 0;
            int second_threshold_1 = 0;
            int black_threshold_2 = 0;
            int white_threshold_2 = 0;
            int second_threshold_2 = 0;
            int black_threshold_4 = 0;
            int white_threshold_4 = 0;
            int second_threshold_4 = 0;
            int black_threshold_8 = 0;
            int white_threshold_8 = 0;
            int second_threshold_8 = 0;
            QString select_sql = QString("select * from test where field_id == %1").arg(2*i+n);
            qDebug() << "select sql = " << select_sql;
            query.prepare(select_sql);
            if(!query.exec())
            {
                qDebug() << "not select field_id = " << 2*i +n;
                qDebug()<<query.lastError();
                query.finish();
                continue;
            }
            else
            {
                if(query.first())
                {
                    field_id = query.value(0).toInt();
                    x1_value = query.value(1).toInt();
                    y1_value = query.value(2).toInt();
                    x2_value = query.value(3).toInt();
                    y2_value = query.value(4).toInt();
                    yz_value = query.value(5).toInt();
                    black_threshold_1 = query.value(6).toInt() ? query.value(6).toInt() : 0;
                    white_threshold_1 = query.value(7).toInt() ? query.value(7).toInt() : 0;
                    second_threshold_1 = query.value(8).toInt() ? query.value(8).toInt() : 0;
                    black_threshold_2 = query.value(9).toInt() ? query.value(9).toInt() : 0;
                    white_threshold_2 = query.value(10).toInt() ? query.value(10).toInt() : 0;
                    second_threshold_2 = query.value(11).toInt() ? query.value(11).toInt() : 0;
                    black_threshold_4 = query.value(12).toInt() ? query.value(12).toInt() : 0;
                    white_threshold_4 = query.value(13).toInt() ? query.value(13).toInt() : 0;
                    second_threshold_4 = query.value(14).toInt() ? query.value(14).toInt() : 0;
                    black_threshold_8 = query.value(15).toInt() ? query.value(15).toInt() : 0;
                    white_threshold_8 = query.value(16).toInt() ? query.value(16).toInt() : 0;
                    second_threshold_8 = query.value(17).toInt() ? query.value(17).toInt() : 0;
                }
                else{
                    qDebug() << "query null is " << 2*i +n;
                    query.finish();
                    continue;
                }
                query.finish();
                int dx = (x2_value - x1_value) /2;
                int dy = (y2_value - y1_value) /2;

                for(int j = 0; j < 4; j++)
                {
                    int cs, ys;
                    cs = j /2;
                    ys = j%2;
                    int pcount = 0;
                    for(int m = y1_value + cs *dy; m <= y1_value +(cs + 1)*dy; m++)
                    {
                        for(int k = x1_value + ys *dx; k <= x1_value + (ys+1)*dx; k++)
                        {
                            int din = m * step + k;
                            if(data[din] < yz_value)
                                pcount++;
                        }
                    }
                    switch (j) {
                    case 0:
                        if(ch_parse.c1)
                            ch_parse.counter_black_c1 = pcount;
                        else
                            ch_parse.counter_white_c1 = pcount;
                        break;
                    case 1:
                        if(ch_parse.c2)
                            ch_parse.counter_black_c2 = pcount;
                        else
                            ch_parse.counter_white_c2 = pcount;
                        break;
                    case 2:
                        if(ch_parse.c4)
                            ch_parse.counter_black_c4 = pcount;
                        else
                            ch_parse.counter_white_c4 = pcount;
                        break;
                    case 3:
                        if(ch_parse.c8)
                            ch_parse.counter_black_c8 = pcount;
                        else
                            ch_parse.counter_white_c8 = pcount;
                        break;
                    }
                }
                // bi jiao threshold counter
                if((black_threshold_1 > ch_parse.counter_black_c1 && ch_parse.counter_black_c1 !=0) || black_threshold_1 == 0)
                    black_threshold_1 = ch_parse.counter_black_c1;
                if(white_threshold_1 < ch_parse.counter_white_c1 || white_threshold_1 == 0)
                    white_threshold_1 = ch_parse.counter_white_c1;
                if((black_threshold_2 > ch_parse.counter_black_c2 && ch_parse.counter_black_c2 != 0) || black_threshold_2 == 0)
                    black_threshold_2 = ch_parse.counter_black_c2;
                if(white_threshold_2 < ch_parse.counter_white_c2 || white_threshold_2 == 0)
                    white_threshold_2 = ch_parse.counter_white_c2;
                if((black_threshold_4 > ch_parse.counter_black_c4 && ch_parse.counter_black_c4 != 0) || black_threshold_4 == 0)
                    black_threshold_4 = ch_parse.counter_black_c4;
                if(white_threshold_4 < ch_parse.counter_white_c4 || white_threshold_4 == 0)
                    white_threshold_4 = ch_parse.counter_white_c4;
                if((black_threshold_8 > ch_parse.counter_black_c8 && ch_parse.counter_black_c8 != 0) || black_threshold_8 == 0)
                    black_threshold_8 = ch_parse.counter_black_c8;
                if(white_threshold_8 < ch_parse.counter_white_c8 || white_threshold_8 == 0)
                    white_threshold_8 = ch_parse.counter_white_c8;
            }

            QString update_sql_threshold = "update test set \
                    black_threshold_1 = :black_threshold_1, \
                    white_threshold_1 = :white_threshold_1, \
                    black_threshold_2 = :black_threshold_2, \
                    white_threshold_2 = :white_threshold_2, \
                    black_threshold_4 = :black_threshold_4, \
                    white_threshold_4 = :white_threshold_4, \
                    black_threshold_8 = :black_threshold_8, \
                    white_threshold_8 = :white_threshold_8  where field_id = :field_id;";


            query.clear();

            query.prepare(update_sql_threshold);
            query.bindValue(":black_threshold_1", black_threshold_1);
            query.bindValue(":white_threshold_1", white_threshold_1);

            query.bindValue(":black_threshold_2", black_threshold_2);
            query.bindValue(":white_threshold_2", white_threshold_2);

            query.bindValue(":black_threshold_4", black_threshold_4);
            query.bindValue(":white_threshold_4", white_threshold_4 );

            query.bindValue(":black_threshold_8", black_threshold_8);
            query.bindValue(":white_threshold_8", white_threshold_8);
            query.bindValue(":field_id", field_id);
            if(!query.exec())
            {
                qDebug() << query.executedQuery();
                qDebug() << "update field_id error -- 8, field_id = " << field_id;
                qDebug()<< query.lastError();
            }
            else
            {
                qDebug()<<"updated!";
            }
            query.finish();
        }
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
            tableview_add_item(QString("%1-%2").arg(i).arg(j),"", x1_gen_part_value[i][j], y1_gen_part_value[i][j], x2_gen_part_value[i][j], y2_gen_part_value[i][j], edit_display_yz, 1, 1, i*SPILT_WIDTH+j*SPILT_HEIGHT);
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

void mainWidget::on_train_threshold_pb_clicked()
{
    QSqlQuery query;
    QString update_threshold = "update test set \
            second_threshold_1 = (black_threshold_1 + white_threshold_1) /2,\
            second_threshold_2 = (black_threshold_2 + white_threshold_2) /2,\
            second_threshold_4 = (black_threshold_4 + white_threshold_4) /2,\
            second_threshold_8 = (black_threshold_8 + white_threshold_8) /2;";

    query.prepare(update_threshold);
    if(!query.exec())
    {
        qDebug() << query.executedQuery();
        qDebug() << "up second threshold error.";
        qDebug()<<query.lastError();
    }
    else
        qDebug() << "update threshold success.";
    query.finish();

    //update model judge value
    QString update_judge = "update test set \
            judge_1 = (black_threshold_1 - white_threshold_1),\
            judge_2 = (black_threshold_2 - white_threshold_2),\
            judge_4 = (black_threshold_4 - white_threshold_4),\
            judge_8 = (black_threshold_8 - white_threshold_8);";

    query.prepare(update_judge);
    if(!query.exec())
    {
        qDebug() << query.executedQuery();
        qDebug() << "up second threshold error.";
        qDebug()<<query.lastError();
    }
    else
        qDebug() << "update judge success.";
    query.finish();
}

void mainWidget::on_open_comm_pb_clicked()
{
    if(!comm_open_flag)
    {// open serial comm ops

        m_serial = new usbserial;
        comm_open_flag = true;
        ui->open_comm_pb->setText("close comm");
    }
    else
    {
        if(m_serial_name != ui->comm_box->currentText())
        {
            if(m_serial != nullptr)
                delete m_serial;
            qDebug() << "close current serial port name = " << m_serial_name;
            m_serial = new usbserial;
            comm_open_flag = true;
            ui->open_comm_pb->setText("close comm");
            return;
        }
        // close serial comm ops
        if(m_serial)
            delete m_serial;
        comm_open_flag = false;
        ui->open_comm_pb->setText("open comm");
    }
}


void mainWidget::init_serial_port(void)
{
    comm_open_flag = false;
    m_serial = nullptr;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        ui->comm_box->addItem(info.portName());
    }
}

void mainWidget::on_open_CCD_pb_clicked()
{
    std::string commnd;
    std::string commnd_lowercase;
    if(!ccd_open_flag)
    {
        ccd_open_flag = true;
        ui->open_CCD_pb->setText("close CCD");
        //TODO 串口指令写入 u_serial.send(commnd,commnd.size());
        commnd.clear();
        commnd = "NO";
        m_serial->send(const_cast<char *>(commnd.c_str()), commnd.size());
        qDebug() << "open ccd and send serial COMAND = NO;";
        usleep(300);

        m_videoCapture = cv::VideoCapture(0);
        if (!m_videoCapture.isOpened())
        {
          qDebug() << "open camera failed.";
        }
    }
    else
    {
        ccd_open_flag = false;
        ui->open_CCD_pb->setText("open CCD");

        commnd_lowercase.clear();
        commnd_lowercase = "no";
        m_serial->send(const_cast<char *>(commnd_lowercase.c_str()), commnd_lowercase.size());
        qDebug() << "open ccd and send serial COMAND = no;";
        usleep(300);

        m_videoCapture.release();
    }
}

void mainWidget::init_ccd_dev(void)
{
    ccd_open_flag = false;
}

void mainWidget::on_get_picture_pb_clicked()
{
    //display_model(cv::VideoCapture& v_capture, usbserial &u_serial)
    display_model(m_videoCapture, *m_serial);
}

void mainWidget::on_tableView_clicked(const QModelIndex &index)
{
    QList<QStandardItem *> tmp_list;
    int row_num =index.row();
    model_modify_index = index.row();
    tmp_list = model->takeRow(index.row());
    QString one_partition = tmp_list.takeFirst()->text();
    QString two_partition = tmp_list.takeFirst()->text();
    int pa_x1 = tmp_list.takeFirst()->text().toInt();
    int pa_y1 = tmp_list.takeFirst()->text().toInt();
    int pa_x2 = tmp_list.takeFirst()->text().toInt();
    int pa_y2 = tmp_list.takeFirst()->text().toInt();
    int pa_yz = tmp_list.takeFirst()->text().toInt();
    int pa_width = tmp_list.takeFirst()->text().toInt();
    int pa_height = tmp_list.takeFirst()->text().toInt();
    int pa_id = tmp_list.takeFirst()->text().toInt();

    ui->x1_edit->setText(QString("%1").arg(pa_x1));
    ui->x2_edit->setText(QString("%1").arg(pa_x2));
    ui->y1_edit->setText(QString("%1").arg(pa_y1));
    ui->y2_edit->setText(QString("%1").arg(pa_y2));
    ui->yz_edit->setText(QString("%1").arg(pa_yz));
    ui->width_edit->setText(QString("%1").arg(pa_width));
    ui->height_edit->setText(QString("%1").arg(pa_height));
    ui->id_edit->setText(QString("%1").arg(pa_id));

    tmp_list.clear();
    tmp_list.append(new QStandardItem(one_partition));
    tmp_list.append(new QStandardItem(two_partition));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_x1)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_y1)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_x2)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_y2)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_yz)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_width)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_height)));
    tmp_list.append(new QStandardItem(QString("%1").arg(pa_id)));

    model->insertRow(row_num, tmp_list);

}
