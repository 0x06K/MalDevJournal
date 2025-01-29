#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// Function to capture the full screen
void CaptureScreen(const char *filename) {
    // Get the handle to the screen's device context (DC)
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMemDC = CreateCompatibleDC(hdcScreen);

    // Get the width and height of the screen
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Create a compatible bitmap to store the screen capture
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);

    // Select the compatible bitmap into the memory device context
    SelectObject(hdcMemDC, hbmScreen);

    // Copy the screen into the bitmap
    BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

    // Create a BITMAPFILEHEADER and BITMAPINFOHEADER for the BMP file
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = -height; // Negative to specify top-down bitmap
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = ((width * 3 + 3) & ~3) * height; // Align to 4-byte boundary
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    bfh.bfType = 0x4D42; // 'BM' in hexadecimal
    bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bih.biSizeImage;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Create the file to save the bitmap
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error: Could not create file %s\n", filename);
        return;
    }

    // Write the headers
    fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, file);

    // Get the bitmap bits from the memory DC and write them to the file
    BYTE *pixels = (BYTE *)malloc(bih.biSizeImage);
    GetDIBits(hdcMemDC, hbmScreen, 0, height, pixels, (BITMAPINFO *)&bih, DIB_RGB_COLORS);
    fwrite(pixels, bih.biSizeImage, 1, file);

    // Clean up
    free(pixels);
    fclose(file);

    // Release resources
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);

    printf("Screenshot saved as %s\n", filename);
}

int main() {
    // Capture the screen and save it as screenshot.bmp
    CaptureScreen("screenshot.bmp");
    return 0;
}
