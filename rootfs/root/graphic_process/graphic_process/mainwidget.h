#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsScene>
#include <QLabel>
#include <QDebug>
#include <opencv2/core.hpp>

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

private:
    Ui::mainWidget *ui;
    //  自定义单元格Type的类型，在创建单元格的Item时使用
    enum    CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore}; //各单元格的类型
    //  各字段在表格中的列号
    enum    FieldColNum{colName=0, colSex,colBirth,colNation,colScore,colPartyM};

    //picture filename path
    QString pictrue_path;
    //picture display
    QGraphicsScene originalScene;
    int graphicsView_width;
    int graphicsView_height;
};
#endif // MAINWIDGET_H
