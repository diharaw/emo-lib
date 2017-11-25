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
#include <QLabel>
#include <QAudioBuffer>
#include <string>
#include <QVideoWidget>
#include <QFuture>
#include <classifier.hpp>
#include <input_image_builder.hpp>
#include <input_audio_builder.hpp>
#include <thread_safe_queue.hpp>
#include <thread>

class WavFile;

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeriesIODevice;

QT_BEGIN_NAMESPACE
class QAudioInput;
class QAudioOutput;
class QAudioProbe;
class QVideoProbe;
QT_END_NAMESPACE

class QMediaRecorder;
class QAudioRecorder;
class QMediaPlayer;

struct EmotionResult
{
    float values[6];
};

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

    void on_m_btnPlay_clicked();

    void on_m_btnBrowse_clicked();

    void on_m_btnClassify_clicked();

private:
    void initializeClassifiers();
    
    void setupCameraPage();
    void setupAudioPage();
    void setupImagePage();
    void setupVideoPage();

    void selectImagePage();
    void selectMicAudioPage();
    void selectFileAudioPage();
    void selectCameraPage();
    void selectVideoPage();
    void choosePage();
    void openImage();
    bool loadImage(const QString &);

    void openAudio();
    bool loadAudio(const QString &);

    void openVideo();
    bool loadVideo(const QString &);
    
public slots:
    void processFrame(QVideoFrame frame);
    void processBuffer(QAudioBuffer buffer);

private:
    Ui::MainWindow*   ui;
    QChartView*       m_chart_view;
    QChart*           m_chart;
    QChart*           m_audio_chart;
    QBarSet*          m_emotionSet;
    QAudioRecorder*   m_audioRecorder;
    QMediaPlayer*     m_mediaPlayer;
    QCamera*          m_camera;
    WavFile*          m_file;
    QImage            m_selectedImage;
    QPixmap           m_cachedPixmap;
    QStringList       m_categories;
    QStringList       m_classifierTypes;
    QStringList       m_inputTypes;
    QVideoProbe*      m_videoProbe;
    QAudioProbe*      m_audioProbe;
    QVideoWidget*     m_videoWidget;
    QBarCategoryAxis* m_axis;
    XYSeriesIODevice* m_device;
    QLineSeries*      m_series;
    QAudioInput*      m_audioInput;
    std::vector<float> m_results;
    QFuture<std::vector<float>> m_classification_future;
    QAudioOutput*     m_audioOutput = nullptr;
    std::string               m_selected_image;
    emolib::Classifier        m_classifier;
    emolib::InputImageBuilder m_image_builder;
    emolib::InputAudioBuilder m_audio_builder;
    emolib::ThreadSafeQueue<QVideoFrame> m_video_frame_queue;
    emolib::ThreadSafeQueue<EmotionResult> m_result_queue;
};

#endif // MAINWINDOW_H
