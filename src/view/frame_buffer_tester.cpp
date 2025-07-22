/**********************************************************
 * @file frame_buffer_tester.cpp
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

// Project includes.
#include "frame_buffer_tester.h"

// Qt Image tools.
#include <QImage>
#include <QTransform>

// Qt File and text processing tools.
#include <QDir>
#include <QStringList>
#include <QRegularExpression>

// Standard includes.
#include <algorithm> // For sort.
#include <vector> // For vector.

// Qt application tools.
#include <QElapsedTimer>


/***************** Macros and defines. ***********************/

/***************** Namespaces. ***********************/
using namespace frame_buffer_tester;


/***************** Local Classes. ***********************/

/***************** Local Functions. ***********************/

/**
 * @brief Auxiliary helper function to sort the input
 * image files by number, instead of alphabetically.
 *
 * Gets the number at the end of the file name, i.e.
 * for "frame15.png", it will return 15.
 *
 * @param filename Image file name.
 * @returns Frame number.
 */
static int getFileFrameNumber(const QString &filename)
{
    // Extract number from image file with regex.
    static const QRegularExpression re("frame(\\d+)\\.png");
    auto match = re.match(filename);
    if (match.hasMatch())
    {
        return match.captured(1).toInt();
    }
    return INT_MAX;
}

/***************** Global Class Functions. ***********************/

FrameBufferTester::FrameBufferTester(QObject *parent)
    : QObject{parent}
{
    // Search for all the frames in the frames resources file.
    QDir framesDir(FRAME_DIR);
    QStringList filters("frame*.png");

    QStringList frameFiles = framesDir.entryList(filters);

    // Read all frames, sort them in numerical order
    // by it's name 'frame##', i.e., frame12.
    std::vector<QString> sortedFrames(frameFiles.begin(), frameFiles.end());
    std::sort(sortedFrames.begin(), sortedFrames.end(), [](const QString &a, const QString &b) {
       return getFileFrameNumber(a) < getFileFrameNumber(b);
    });

    // Create new frame vector, and allocate
    // all converted frame buffer data.
    frameSamples = new frame_samples_t;
    frameSamples->reserve(sortedFrames.size());
    for (auto &frame : sortedFrames)
    {
        // Convert each frame to the frame data.
        QImage image = QImage(FRAME_DIR + frame);
        auto buffer = image2FrameBuffer(image);
        frameSamples->emplace_back(buffer);
    }

    // Initialize periodic timer, and set up signal
    // callbacks for frame updates.
    frameIdx = 0;
    refreshTimer.setInterval(16);
    refreshTimer.setTimerType(Qt::PreciseTimer);
    connect(&refreshTimer, &QTimer::timeout, this, &FrameBufferTester::on_refreshTimerTimeout);
    refreshTimer.start();
}

frame_buffer_t *FrameBufferTester::image2FrameBuffer(QImage &image)
{
    // Rotate the image 90 degrees to the right, to adjust
    // the original frame to the expected format of the
    // emulator.
    QImage rotatedImage = image.transformed(QTransform().rotate(90));

    // Ensure frame dimensions are correct.
    if (image.height() != FRAME_HEIGHT)
    {
        throw QString("Expected frame width at 256, instead got: ") + QString::number(image.height());
    }

    if (image.width() != FRAME_WIDTH)
    {
        throw QString("Expected frame height at 224, instead got: ") + QString::number(image.width());
    }

    // Allocate buffer frame.
    frame_buffer_t *buffer = new frame_buffer_t;
    buffer->fill(0);

    size_t bitpos = 0;
    size_t bytepos = 0;

    // Iterate through all pixels on the image, and convert
    // them to single bits, 8 per byte. All little endian.
    for (int j = 0; j < rotatedImage.height(); j++)
    {
        for (int i = 0; i < rotatedImage.width(); i++)
        {
            // Set bit when pixel is white, else black.
            uint8_t bitpixel = (qGray(rotatedImage.pixel(i, j)) == 255) ? (1) : (0);

            // Insert each bit sequenctially.
            buffer->at(bytepos) |= ((uint8_t)(bitpixel << bitpos) & 0xFF);

            // Set next byte.
            if (bitpos == 7)
            {
                bytepos++;
            }

            // Rotate the bit position.
            bitpos = (bitpos + 1) % 8;
        }
    }

    return buffer;
}

void FrameBufferTester::on_refreshTimerTimeout(void)
{
    // Emit signal with current buffer.
    emit updateFrameBuffer(frameSamples->at(frameIdx));

    // Iterate through all frame buffers.
    frameIdx = (frameIdx + 1) % frameSamples->size();
}





















