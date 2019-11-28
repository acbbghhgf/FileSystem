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

    static_data_init();

}

mainWidget::~mainWidget()
{
    delete ui;
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
    //use default param:
//    QString dbFile = QFileDialog::getOpenFileName(this, "select database file", "", "SQL Lite database(*.db *.db3)");
    QString dbFile = DATAFILE;
    if(dbFile.isEmpty())
    {
        qDebug() << "database filename is null.";
        return;
    }
    DB = QSqlDatabase::addDatabase("QSQLITE");// use sqlite3 qt driver
    DB.setDatabaseName(dbFile);

    if(!DB.open())//open database
    {
        qDebug() << "open database " << dbFile << "ERROR.";
        return;
    }
    //open database table
    open_database_table();

}

void mainWidget::open_database_table()
{
    //打开数据表
    tabModel=new QSqlTableModel(this,DB);//数据表
    tabModel->setTable("employee"); //设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);//数据保存方式，OnManualSubmit , OnRowChange
    tabModel->setSort(tabModel->fieldIndex("empNo"),Qt::AscendingOrder); //排序
    if (!(tabModel->select()))//查询数据
    {
        qDebug() << "select data from database error.";
        return;
    }

    //字段显示名
        tabModel->setHeaderData(tabModel->fieldIndex("number"),Qt::Horizontal,"序号");
        tabModel->setHeaderData(tabModel->fieldIndex("x1"),Qt::Horizontal,"x1");
        tabModel->setHeaderData(tabModel->fieldIndex("y1"),Qt::Horizontal,"y1");
        tabModel->setHeaderData(tabModel->fieldIndex("x2"),Qt::Horizontal,"x2");

        tabModel->setHeaderData(tabModel->fieldIndex("y2"),Qt::Horizontal,"y2");
        tabModel->setHeaderData(tabModel->fieldIndex("yz"),Qt::Horizontal,"yz");
        tabModel->setHeaderData(tabModel->fieldIndex("width"),Qt::Horizontal,"宽");
        tabModel->setHeaderData(tabModel->fieldIndex("height"),Qt::Horizontal,"高");


        theSelection=new QItemSelectionModel(tabModel);//关联选择模型
    //theSelection当前项变化时触发currentChanged信号
        connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)),
                this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    //选择行变化时
        connect(theSelection,SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this,SLOT(on_currentRowChanged(QModelIndex,QModelIndex)));

        ui->tableView->setModel(tabModel);//设置数据模型
        ui->tableView->setSelectionModel(theSelection); //设置选择模型

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
    edit_display_width = 40;
    edit_display_height = 24;

    qDebug() << "proc_cols = " << process_mid_picture.cols << " proc_rows = " << process_mid_picture.rows;
    m_updatePixmap(process_mid_picture, QImage::Format_RGB888);

    update_display();
}


void mainWidget::m_updatePixmap(cv::Mat &dst, QImage::Format flag)
{

    originalScene.clear();
    cv::Mat process = dst.clone();
    cv::Rect rect(edit_display_x1, edit_display_y1, edit_display_x2 - edit_display_x1, edit_display_y2 - edit_display_y1);
    cv::rectangle(process, rect, cv::Scalar(255, 0, 0), 1, 8, 0);
    cv::imwrite("/home/ww/bmp_demo/save.png", process);
    qDebug() << "proc_cols = " << dst.cols << " proc_rows = " << dst.rows << "graphic width =" << graphicsView_width << " graphic height =" << graphicsView_height;
    QImage image("/home/ww/bmp_demo/save.png");
    pixmap = QPixmap::fromImage(image);
    originalScene.addPixmap(pixmap.scaled(graphicsView_width, graphicsView_height, Qt::KeepAspectRatio, Qt::SmoothTransformation));

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
