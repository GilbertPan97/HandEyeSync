#ifndef THREADWORKER_H
#define THREADWORKER_H

#include "qcustomplot.h"
#include "LineScannerInterface.h"  // Include the SensorApi header file

#include <iostream>
#include <QObject>
#include <QThread>

class ThreadWorker : public QObject {
    Q_OBJECT

public:
    explicit ThreadWorker(LineScannerInterface *sensorApi, QObject *parent = nullptr);

    explicit ThreadWorker();

public slots:
    void ContinueGrabPlot();
    void startGrabbing();
    void stopGrabbing();

signals:
    void updatePlot(); 

private:
    LineScannerInterface *sensorApi;
    bool isTreadGrabing_ = false;
};

#endif // THREADWORKER_H
