#include "ThreadWorker.h"

ThreadWorker::ThreadWorker(LineScannerInterface *sensorApi, QObject *parent)
    : QObject(parent), sensorApi(sensorApi) {

}

ThreadWorker::ThreadWorker(){

}

void ThreadWorker::ContinueGrabPlot() {
    while (isTreadGrabing_) {
        if (!isTreadGrabing_)
            break;
        sensorApi->GrabOnce();      // Perform the grabbing operation
        emit updatePlot();          // Emit signal when data is ready

        QThread::msleep(100);       // Simulate delay without blocking the UI thread
    }
    std::cout << "Grab thread stoped.\n";
}

void ThreadWorker::startGrabbing() {
    std::cout << "Start grabbing.\n";
    isTreadGrabing_ = true;

    ContinueGrabPlot();     // Start grabbing frames
}

void ThreadWorker::stopGrabbing() {
    std::cout << "Set grab status as stop.\n";
    isTreadGrabing_ = false;      // Stop the grabbing process
}
