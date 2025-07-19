########################################################################
# CODE GENERATED WITH ChatGPT
#
# This is an auxiliary script to allow us to create test image frames
# to the original resolution of the space invaders game at 244x256,
# all black and white.
#
# CODE PROMPT: "Take this picture, change the resolution to 244x256,
# and normalize all pixels to all white if the current pixel is not black"
# ...
# SECOND CODE PROMPT: "Instead of setting all non-black pixels, provide a
# threshold of at least 10 points of brightness in any color,
# and anything below that should be black"
#
# The resulting code was generated below:
########################################################################

from PIL import Image
import numpy as np

# Load the image
image_path = "space_invaders.png"  # Replace with your image path
original_image = Image.open(image_path).convert("RGB")

# Resize the image to 244x256
resized_image = original_image.resize((244, 256))

# Convert to numpy array
image_array = np.array(resized_image)

# Create a mask for pixels where any channel (R, G, or B) is >= 10
mask = np.any(image_array >= 80, axis=-1)

# Initialize the result array with all black pixels
thresholded_array = np.zeros_like(image_array)

# Set pixels to white where the mask is True
thresholded_array[mask] = [255, 255, 255]

# Convert back to image
thresholded_image = Image.fromarray(np.uint8(thresholded_array))

# Show or save the processed image
thresholded_image.save("output.png")
