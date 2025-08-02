# Display Rendering Documentation

## Overview

This module handles the visual output of the emulator by converting the emulator’s frame buffer into a Qt-compatible image for rendering within the `MainWindow` class.

---

## Responsibilities

- Converts frame buffer bytes into displayable images
- Applies transformations to mimic the original arcade screen orientation
- Scales and overlays the color mask
- Maintains crisp pixel-based rendering

---

## Core Methods

### `paintEvent(QPaintEvent *event)`

- Retrieves the height of the menu bar to anchor image rendering
- Scales the raw frame (`currentRenderedImage`) and the overlay mask (`colorMask`)
- Uses `QPainter::CompositionMode_Multiply` to apply the color overlay
- Ensures pixel-perfect scaling with `Qt::FastTransformation`

### `on_frameBufferReceived(const frame_buffer_t *buffer)`

- Converts the incoming buffer into a `QBitmap`
- Rotates the bitmap -90° to correct the arcade screen tilt
- Inverts pixel colors to match PNG display expectations
- Updates `currentRenderedImage` and triggers a GUI repaint

---

## Design Considerations

- Optimized for speed and clarity (no interpolation)
- Separation of frame buffer data and display logic
- Only updates when valid buffer is received
- Display updates are isolated to `paintEvent()` for full control

---

## Image Notes

- Frame resolution: 224x256 (rotated)
- 8 pixels per byte, compatible with `QBitmap::fromData`
- Color masking provides visual fidelity over monochrome base

---
