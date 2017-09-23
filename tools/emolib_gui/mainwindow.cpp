#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyseriesiodevice.h"
#include <QCameraInfo>
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QStandardPaths>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QBarSet *set0 = new QBarSet("Emotion");
    *set0 << 1 << 2 << 3 << 4 << 5 << 6;
    QBarSeries *series = new QBarSeries();
    series->append(set0);

    m_chart = new QChart();
    m_chart->addSeries(series);
    m_chart->setTitle("Classification Results");
    m_chart->setAnimationOptions(QChart::SeriesAnimations);

    m_categories << "Neutral" << "Happy" << "Angry" << "Sad" << "Surprise" << "Fear";
    m_axis = new QBarCategoryAxis();
    m_axis->append(m_categories);
    m_chart->createDefaultAxes();
    m_chart->setAxisX(m_axis, series);

    m_chart->legend()->setVisible(false);

    m_chart_view = new QChartView(m_chart);
    m_chart_view->setRenderHint(QPainter::Antialiasing);

    QGridLayout* layout = new QGridLayout(ui->m_graph_group);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_chart_view);

    setupAudioPage();
    setupCameraPage();
    setupImagePage();

    m_classifierTypes << "Facial Expression" << "Speech" << "Bi-Model";
    ui->m_cmbClassifier->addItems(m_classifierTypes);

    m_inputTypes << "Webcam/Mic" << "Files";
    ui->m_cmbInput->addItems(m_inputTypes);
    choosePage();
}

MainWindow::~MainWindow()
{
    m_camera->stop();
    m_device->close();
    m_audioInput->stop();
    delete ui;
}

void MainWindow::setupCameraPage()
{
    m_camera = new QCamera(QCameraInfo::defaultCamera());

    QCameraViewfinderSettings settings;

    settings.setMinimumFrameRate(30);
    m_camera->setViewfinderSettings(settings);
    m_camera->exposure()->setExposureCompensation(10.0);

    m_camera->setViewfinder(ui->m_camera_widget);
}

void MainWindow::setupAudioPage()
{
    m_audio_chart = new QChart;
    QChartView *chartView = new QChartView(m_audio_chart);
    chartView->setMinimumSize(800, 600);
    m_series = new QLineSeries;
    m_audio_chart->addSeries(m_series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 2000);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Audio level");
    m_audio_chart->setAxisX(axisX, m_series);
    m_audio_chart->setAxisY(axisY, m_series);
    m_audio_chart->legend()->hide();
    m_audio_chart->setTitle("Data from the microphone");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);
    ui->m_audio_page->setLayout(mainLayout);

    QAudioFormat formatAudio;
    formatAudio.setSampleRate(16000);
    formatAudio.setChannelCount(1);
    formatAudio.setSampleSize(16);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
    m_audioInput = new QAudioInput(inputDevices,formatAudio, this);

    m_device = new XYSeriesIODevice(m_series, this);
}

void MainWindow::setupImagePage()
{
//    ui->m_selectedImageLabel->setBackgroundRole(QPalette::Base);

}

void MainWindow::setupVideoPage()
{

}

void MainWindow::selectImagePage()
{
    m_camera->stop();
    m_device->close();
    m_audioInput->stop();
    ui->m_pageStack->setCurrentIndex(2);
}

void MainWindow::selectMicAudioPage()
{
    m_camera->stop();
    m_device->open(QIODevice::WriteOnly);
    m_audioInput->start(m_device);
    ui->m_pageStack->setCurrentIndex(1);
}

void MainWindow::selectCameraPage()
{
    m_device->close();
    m_audioInput->stop();
    m_camera->start();
    ui->m_pageStack->setCurrentIndex(0);
}

void MainWindow::on_m_cmbClassifier_currentIndexChanged(int index)
{
    choosePage();
}

void MainWindow::on_m_cmbInput_currentIndexChanged(int index)
{
    choosePage();
}

void MainWindow::choosePage()
{
    int index = ui->m_cmbClassifier->currentIndex();
    int inputIndex = ui->m_cmbInput->currentIndex();

    switch (index)
    {
        case 0: // Facial Expressions
        {
            if(inputIndex == 0) // Webcam/Mic
            {
                selectCameraPage();
            }
            else if(inputIndex == 1) // Files
            {
                selectImagePage();
            }
            break;
        }
        case 1: // Speech
        {
            if(inputIndex == 0) // Webcam/Mic
            {
                selectMicAudioPage();
            }
            else if(inputIndex == 1) // Files
            {

            }
            break;
        }
        case 2: // Bi-Model
        {
            if(inputIndex == 0) // Webcam/Mic
            {
                selectCameraPage();
            }
            else if(inputIndex == 1) // Files
            {

            }
            break;
        }
        default:
            break;
    }
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

bool MainWindow::loadImage(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    m_selectedImage = newImage.scaled(ui->m_image_page->size() * devicePixelRatio(), Qt::AspectRatioMode::KeepAspectRatio, Qt::SmoothTransformation);
    m_cachedPixmap = QPixmap::fromImage(m_selectedImage);
    m_cachedPixmap.setDevicePixelRatio(devicePixelRatio());

    ui->m_selectedImageLabel->setPixmap(m_cachedPixmap);

    return true;
}

void MainWindow::openImage()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadImage(dialog.selectedFiles().first())) {}
}

void MainWindow::on_pushButton_clicked()
{
    openImage();
}
