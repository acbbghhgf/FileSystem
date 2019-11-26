#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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
    void on_setHeader_pb_clicked();

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

private:
    void m_updatePixmap(cv::Mat &dst, QImage::Format flag);
private:
    Ui::mainWidget *ui;
    QTimer *timer;
    //  自定义单元格Type的类型，在创建单元格的Item时使用
    enum    CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore}; //各单元格的类型
    //  各字段在表格中的列号
    enum    FieldColNum{colName=0, colSex,colBirth,colNation,colScore,colPartyM};

    //picture filename path
    QString pictrue_path;
    //picture display
    QGraphicsScene originalScene;
    QPixmap pixmap;
    //scope big
    QRectF big_rect;

    int graphicsView_width;
    int graphicsView_height;
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

    //src picture
    cv::Mat src_picture;
    cv::Mat process_mid_picture;
};
#endif // MAINWIDGET_H
