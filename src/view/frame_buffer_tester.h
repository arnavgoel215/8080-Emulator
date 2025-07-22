/**********************************************************
 * @file frame_buffer_tester.h
 * 
 * @brief Helper class used to test frame data.
 * This class reads test frame images, converts them
 * to the expected emulation output format, and sends
 * them to the main application to test the video update
 * performance and correct functionality..
 * 
 * @author Sergio Chavarria
 * 
 *********************************************************/
#ifndef FRAME_BUFFER_TESTER_H
#define FRAME_BUFFER_TESTER_H


/***************** Include files. ***********************/

// Standard includes.
#include <cstdint>

// Qt tools includes.
#include <QObject>
#include <QTimer>

// Project includes.
#include "common_frame_cfg.h"


/***************** Namespaces. ***********************/
namespace frame_buffer_tester
{

    
/***************** Global Types. ***********************/

/**
 * @brief Auxiliary vector of multiple frame buffers.
 */
using frame_samples_t = std::vector<frame_buffer_t*>;


/***************** Global Classes. ***********************/

/**
 * @brief FrameBufferTester. Helper class to test video buffer.
 */
class FrameBufferTester : public QObject
{
    Q_OBJECT
public:

    /***************** Public Class Functions. ***********************/

    /**
     * @brief Default constructor for QObjects.
     * 
     * This constructors reads embedded test image frames
     * and converts them to the expected format used by the
     * emulation output, then stores them in a local vector
     * which is used on the udpate frame function.
     * 
     * @param parent QObject parent class.
     */
    explicit FrameBufferTester(QObject *parent = nullptr);
    
    /**
     * @brief Default destructor.
     * 
     * Needs to be called explicitly to remove all temporary
     * allocated frame data.
     */
    ~FrameBufferTester()
    {
        // Stop timer to avoid calling frame update signals.
        refreshTimer.stop();

        // Delete all frame data.
        if (nullptr != frameSamples)
        {
            for (auto f : *frameSamples)
            {
                delete f;
            }

            delete frameSamples;
        }
    }

signals:

    /***************** Public Signal Functions. ***********************/

    /**
     * @brief Output signal to notify new frame data.
     * 
     * @param Pointer to buffer data to send to application.
     */
    void updateFrameBuffer(const frame_buffer_t *buffer);

private slots:

    /***************** Private Slot Functions. ***********************/
    
    /**
     * @brief Internal slot for the refresh timer timeout.
     * 
     * Used to update and send the frame data every 16 ms.
     */
    void on_refreshTimerTimeout(void);

private:

    /***************** Private class functions. ***********************/
    
    /**
     * @brief Auxiliary function to convert a frame image data
     * into the emulator output format.
     * 
     * @param image processed image object.
     */
    frame_buffer_t *image2FrameBuffer(QImage &image);

    /***************** Private class elements. ***********************/

    /**
     * @brief QRC resource path for image files.
     * 
     * The image files are embedded into the application resources.
     */
    const QString FRAME_DIR {":/resources/"};

    /**
     * @brief Local frame samples vector.
     * 
     * Used to store all frame data in a single collection.
     */
    frame_samples_t *frameSamples;

    /**
     * @brief Frame update period.
     * 
     * Not exactly 16.667 ms to achieve 60 Hz, but close enough.
     */
    static constexpr int FRAME_TIMER_PERIOD_MS = 16;

    /**
     * @brief Timer used to create periodic signal
     * callbacks to update the frame buffer.
     */
    QTimer refreshTimer;

    /**
     * @brief Current frame index.
     * 
     * The application cycles through all the existing frames,
     * then circles back to index 0.
     */
    int frameIdx = 0;
};

} // namespace frame_buffer_tester

#endif // FRAME_BUFFER_TESTER_H
