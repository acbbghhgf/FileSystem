#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QtSql>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "picture_data.h"


/*
 * define defaule param, modify ext
 *
*/
#define DATAFILE    "testDB.db"
#define TMPPICTUREFILE  "save.png"
#define CREATE_SQL  "create table test (id int primary key, name varchar(30), age int)"
#define INSERT_SQL  "insert into test values (?, ?, ?)"
#define SELECT_SQL  "select id, name from test"
#define UPDATE_SQL  "update test"
#define CLEAR_SQL   "delete from test"
#define DELETE_SQL  "delete from test where id = ?"
#define DROP_SQL    "drop table test"

#define SPILT_WIDTH 40
#define SPILT_HEIGHT 24


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
    void on_select_pb_clicked();
    void update_display();

    void on_modify_pb_clicked();

    void on_x1_edit_textChanged(const QString &arg1);

    void on_y1_edit_textChanged(const QString &arg1);

    void on_x2_edit_textChanged(const QString &arg1);

    void on_y2_edit_textChanged(const QString &arg1);

    void on_yz_edit_textChanged(const QString &arg1);

    void on_width_edit_textChanged(const QString &arg1);

    void on_height_edit_textChanged(const QString &arg1);

    void on_X_edit_textChanged(const QString &arg1);

    void on_Y_edit_textChanged(const QString &arg1);

    void on_one_click_partition_pb_clicked();

    void on_two_partition_pb_clicked();

    void on_db_open_pb_clicked();

    void on_db_create_pb_clicked();

    void on_db_select_pb_clicked();

    void on_db_drop_pb_clicked();

    void on_training_pb_clicked();

    void on_generate_partition_pb_clicked();

    void on_load_partition_pb_clicked();

private:
    void static_data_init(void);
    void DataBase_init(void);
    void open_database_table(void);
    void create_datebase_table(void);

    void m_updatePixmap(cv::Mat &dst);
    void m_update_Onepartition_Pixmap(cv::Mat &dst, QImage::Format flag, int width_value, int width_num, int height_value, int height_num);

    void tableview_init(void);
    void tableview_data_clear(void);
    void tableview_add_item(QString region_value, int x1_value, int y1_value, int x2_value, int y2_value, int yz_value, int width_value, int height_value, int id_value);

    int update_database_threshold(cv::Mat &src, QString &pic_data, int pic_num);
private:
    Ui::mainWidget *ui;
    //use timer
    QTimer *timer;

    //use database
    QSqlDatabase db;//connect database

    //display table data
    QStandardItemModel *model;


    //picture filename path
    QString pictrue_path;
    //picture display
    QGraphicsScene originalScene;
    QPixmap pixmap;
    //scope big
    QRectF big_rect;

    int graphicsView_width;
    int graphicsView_height;
    int one_partiton_width_coe;
    int ope_partition_height_coe;
    int two_partition_width_coe;
    int two_partition_height_coe;

    //display scope x and y
    int edit_display_x1;
    int edit_display_x2;
    int edit_display_y1;
    int edit_display_y2;
    int edit_display_yz;
    int edit_display_id;
    int edit_display_X;
    int edit_display_Y;
    int edit_display_width;
    int edit_display_height;

    int x1_gen_part_value[SPILT_WIDTH][SPILT_HEIGHT];
    int y1_gen_part_value[SPILT_WIDTH][SPILT_HEIGHT];
    int x2_gen_part_value[SPILT_WIDTH][SPILT_HEIGHT];
    int y2_gen_part_value[SPILT_WIDTH][SPILT_HEIGHT];

    //src picture
    cv::Mat src_picture;
    cv::Mat process_mid_picture;
};
#endif // MAINWIDGET_H
