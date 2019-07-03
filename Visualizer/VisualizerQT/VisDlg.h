#ifndef VisDlg_H
#define VisDlg_H

#include "VisualizerCommon/Visualizer.h"
#include "ui_visualizer.h"
#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui
{
    class VisDlg;
}

class Ui::VisDlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit VisDlg(QWidget *parent = nullptr);
    ~VisDlg();

    void show();

    void SetVisualizer(Visualizer* v);
    void StartMinimized(boolean startmin);

private slots:
    void update();

    void show_hide();

    void on_lineEdit_Background_Brightness_textChanged(const QString &arg1);

    void on_lineEdit_Amplitude_textChanged(const QString &arg1);

    void on_lineEdit_Average_Size_textChanged(const QString &arg1);

    void on_lineEdit_Decay_textChanged(const QString &arg1);

    void on_lineEdit_Delay_textChanged(const QString &arg1);

    void on_lineEdit_Normalization_Offset_textChanged(const QString &arg1);

    void on_lineEdit_Normalization_Scale_textChanged(const QString &arg1);

    void on_lineEdit_Animation_Speed_textChanged(const QString &arg1);

    void on_comboBox_FFT_Window_Mode_currentIndexChanged(int index);

    void on_comboBox_Background_Mode_currentIndexChanged(int index);

    void on_comboBox_Foreground_Mode_currentIndexChanged(int index);

    void on_comboBox_Single_Color_Mode_currentIndexChanged(int index);

    void on_comboBox_Average_Mode_currentIndexChanged(int index);

    void on_pushButton_Save_Settings_clicked();

    void on_checkBox_Reactive_Background_clicked(bool checked);

    void on_comboBox_Audio_Device_currentIndexChanged(int index);

    void on_lineEdit_Filter_Constant_textChanged(const QString &arg1);

    void on_checkBox_Silent_Background_clicked(bool checked);

    void on_lineEdit_Background_Timeout_textChanged(const QString &arg1);

private:
    Ui::VisualizerDlg *ui;
    QSystemTrayIcon* trayIcon;
    QTimer* timer;
    QGraphicsScene* scene;
    QImage* image;
    QPixmap pixmap;
};
#endif // VisDlg_H
