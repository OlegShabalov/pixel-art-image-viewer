# pixel-art-image-viewer

## About

This image viewer for `Windows` is specialized for viewing `pixel art` images and also for use as a `reference board`.

The following key features are implemented for these tasks:
- Image display without anti-aliasing
- Large image scaling range (even two-pixel images can be scaled to full screen, and large images can also be scaled down to small sizes)
- Free movement of the image to any part of the window and outside the window
- 100% of the window area is used to display the image
- GUI is minimalistic and hides immediately when not needed
- There is a `"keep on top"` function
- Supports frame-by-frame playback of GIF animations
- Supports most popular image formats
- Viewer works in portable mode, all settings are saved inside the application folder

In addition to `pixel art` images, this program is great for displaying `any type` of image.

## Usage

To open an image using this application on Windows 10:
- Right-click on any image file
- Open with
- Choose another app
- More apps
- Look for another app on this PC
- Specify the path to the `.exe` file of this app

When you simply launch an application without specifying an image file, you will be able to select an image file from within the program.

## Build

All functionality has been verified on `Qt 5.15`.
Building under `Qt 6` should also work.

## Screenshots

The appearance of the application is very minimalistic:
![Screenshot 1](https://github.com/OlegShabalov/pixel-art-image-viewer/blob/master/screenshots/1.png "Screenshot 1")

GUI elements appear only when the mouse is near them:
![Screenshot 2](https://github.com/OlegShabalov/pixel-art-image-viewer/blob/master/screenshots/2.png "Screenshot 2")

All the window area is occupied by the image, there is nothing to obstruct the view:
![Screenshot 3](https://github.com/OlegShabalov/pixel-art-image-viewer/blob/master/screenshots/3.png "Screenshot 3")

A wide range of image scaling allows you to zoom in and out as you want:
![Screenshot 4](https://github.com/OlegShabalov/pixel-art-image-viewer/blob/master/screenshots/4.png "Screenshot 4")

## License

[MIT license](https://github.com/OlegShabalov/pixel-art-image-viewer/blob/master/LICENSE)
