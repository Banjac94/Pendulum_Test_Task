#include <QApplication>
#include <QChart>
#include <QChartView>
#include <QDateTimeAxis>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineSeries>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QValueAxis>

// QT_CHARTS_USE_NAMESPACE

class MeasurementPlot : public QMainWindow {
public:
    MeasurementPlot() { setupUI(); }

private:
    void setupUI() {
        QChart *chart = new QChart;
        chart->setTitle("Measurement Data");
        QDateTimeAxis *axisX = new QDateTimeAxis;
        QValueAxis *axisY = new QValueAxis;
        axisX->setTitleText("Timestamp");
        axisX->setFormat("yyyy-MM-dd hh:mm:ss");
        axisY->setTitleText("Value");

        chart->addAxis(axisX, Qt::AlignBottom);
        chart->addAxis(axisY, Qt::AlignLeft);
        
        chart->legend()->setVisible(false);

       
        QString folderPath = QFileDialog::getExistingDirectory(
            this, "Select Folder Containing SampleFiles", QDir::homePath());

        if (folderPath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Folder path not selected.");
            return;
        }

        QString fileName = QInputDialog::getText(this, "Enter File Name",
                                                 "Enter the name of the file you want to load:");

        if (fileName.isEmpty()) {
            QMessageBox::critical(this, "Error", "File name not entered.");
            return;
        }

        QString filePath = folderPath + "/" + fileName;

        loadMeasurementData(filePath, chart);

        QChartView *chartView = new QChartView(chart);
        setCentralWidget(chartView);
    }

    void loadMeasurementData(const QString &filePath, QChart *chart) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "Could not open the file: " + filePath);
            return;
        }

        QTextStream in(&file);
        QLineSeries *series = new QLineSeries;

        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith("#")) {
            } else {
                break;
            }
        }

        while (!in.atEnd()) {
            QStringList data = in.readLine().split(" ");
            if (data.size() == 2) {
                double timestamp = data[0].toDouble();
                double value = data[1].toDouble();
                qDebug() << "Timestamp:" << timestamp << "Value:" << value;
                series->append(timestamp, value);
            } else {
                QMessageBox::critical(this, "Error",
                                      "Invalid data format in the file.");
                delete series;
                return;
            }
        }

        chart->addSeries(series);
        series->attachAxis(chart->axisX());
        series->attachAxis(chart->axisY());

        file.close();
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MeasurementPlot mainWindow;
    mainWindow.resize(800, 600);
    mainWindow.show();
    return app.exec();
}
