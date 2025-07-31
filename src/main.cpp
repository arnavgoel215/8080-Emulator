#include "controller.hpp"
#include "emulator.hpp"
#include "mainwindow.h"

#include <thread>
#include <QApplication>
#include <QElapsedTimer>

// Approximate period to run 60 Frames Per Second.
constexpr qint64 FRAME_NS = 1000000000 / 60;

/**
 * @brief Controller thread.
 * 
 * Runs a single frame every 1/60 Hz ~= 16.666 ms.
 * 
 * @param appRunning Pointer communicating if the other thread is running.
 * @param ctrl Controller class pointer.
 */
void runFrames(bool *appRunning, Controller *ctrl)
{
    QElapsedTimer timer;
    timer.start();

    while (*appRunning)
    {
        // Approximate of 60 FPS.
        if (timer.nsecsElapsed() >= FRAME_NS)
        {
            // Restart timer.
            timer.start();

            // Emulate single frame.
            ctrl->runFrame();
        }

        // Sleep thread maybe??
    }
}

/**
 * @brief Main entry function for Space Invaders Emulator.
 * 
 * Runs two threads, one executing the View (GUI),
 * and the other one runs the Controller module,
 * which coordinates the tasks for executing the emulation.
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Emulator model;
    Controller controller(&model, &w);

    // Create separate thread for Controller.
    bool applicationRunning = true;
    std::thread frames_thread(&runFrames, &applicationRunning, &controller);

    // Run and execute GUI in this thread.
    w.show();
    int ret = a.exec();
    
    // GUI is exited, communicate termination to other thread, and join.
    applicationRunning = false;
    frames_thread.join();
    return ret;
}
