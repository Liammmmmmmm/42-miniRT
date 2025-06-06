import sys
import base64
from PIL import Image

def image_to_base64(path, width, height):
    img = Image.open(path).convert("RGB")
    img = img.resize((width, height), Image.LANCZOS)

    rgb_bytes = bytearray(width * height * 3)
    i = 0
    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x, y))
            rgb_bytes[i] = r
            rgb_bytes[i+1] = g
            rgb_bytes[i+2] = b
            i += 3

    encoded = base64.b64encode(rgb_bytes)
    sys.stdout.buffer.write(encoded + b'\n')

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python image_to_base64.py <image.png> <width> <height>")
        sys.exit(1)

    image_path = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])
    image_to_base64(image_path, width, height)
