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
    axisY->setTitleText("Value");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    QString fileName = QInputDialog::getText(this, "Entering a file name","Enter a file name:");

    // HERE YOU NEED TO PUT PATH TO SAMPLE FILES, BECAUSE THIS IS PATH TO SAMPLE FILES ON MY PC.
    QString filePath = "C:/Users/banja/OneDrive/Desktop/TeskTaskEnglish/SampleFiles/" + fileName;

    loadMeasurementData(filePath, chart);

    QChartView *chartView = new QChartView(chart);
    setCentralWidget(chartView);
  }

  void loadMeasurementData(const QString &filePath, QChart *chart) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::critical(this, "Error", "Could not open the file.");
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
        series->append(timestamp, value);
      } else {
        QMessageBox::critical(this, "Error",
                              "Invalid data format in the file.");
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
  mainWindow.show();
  return app.exec();
}
