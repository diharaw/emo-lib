#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QCamera>
#include <QElapsedTimer>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
QT_END_NAMESPACE

class QMediaRecorder;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_m_cmbClassifier_currentIndexChanged(int index);

    void on_m_cmbInput_currentIndexChanged(int index);

private:
    void setupCameraPage();
    void setupAudioPage();
    void setupImagePage();
    void setupVideoPage();

    void selectMicAudioPage();
    void selectCameraPage();
    void choosePage();

private:
    Ui::MainWindow*   ui;
    QChartView*       m_chart_view;
    QChart*           m_chart;
    QChart*           m_audio_chart;
    QMediaRecorder*   m_mediaRecorder;
    QCamera*          m_camera;
    QStringList       m_categories;
    QStringList       m_classifierTypes;
    QStringList       m_inputTypes;
    QBarCategoryAxis* m_axis;
    XYSeriesIODevice* m_device;
    QLineSeries*      m_series;
    QAudioInput*      m_audioInput;
};

#endif // MAINWINDOW_H
