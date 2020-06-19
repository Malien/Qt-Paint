#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PaintWindow; }
QT_END_NAMESPACE

class PaintWindow : public QMainWindow
{
    Q_OBJECT
    QColor color = QColor::fromRgb(0,0,0);

public:
    explicit PaintWindow(QWidget *parent = nullptr);
    ~PaintWindow() override;

private:
    Ui::PaintWindow *ui;
};
#endif // PAINTWINDOW_H
