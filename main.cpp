#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* src = 0;
int x_sel, y_sel, width, height;

// рисуем область выделения
void drawSelected(IplImage* img, int x, int y, int w, int h, int add_value = 200)
{
    cvSetImageROI(img, cvRect(x, y, w, h));
    cvAddS(img, cvScalar(add_value), image);
}

// обработчик событий от мышки
void myMouseCallback( int event, int x, int y, int flags, void* param )
{
    IplImage* img = (IplImage*) param;

    switch( event )
    {
    case CV_EVENT_MOUSEMOVE:
        break;

    case CV_EVENT_LBUTTONDOWN:
        x_sel = x;
        y_sel = y;
        break;

    case CV_EVENT_LBUTTONUP:
        width = x - x_sel;
        height = y - y_sel;
        drawSelected(img, x_sel, y_sel, width, height);
        cvResetImageROI(image);
        cvShowImage("ROI", image);
        break;
    }
}

int main(int argc, char* argv[])
{
    // имя картинки задаётся первым параметром
    const char* filename = argc == 2 ? argv[1] : "Image0.jpg";
    // получаем картинку
    image = cvLoadImage(filename,1);
    // клонируем картинку
    src = cvCloneImage(image);

    printf("[i] image: %s\n", filename);
    assert( src != 0 );

    // окно для отображения картинки
    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);

    // вешаем обработчик мышки
    cvSetMouseCallback( "original", myMouseCallback, (void*) image);

    while(1)
    {
        // показываем картинку
        cvCopyImage( image, src );
        cvShowImage( "original", src );

        char c = cvWaitKey(33);
        if (c == 27)   // если нажата ESC - выходим
        {
            break;
        }
    }

    // освобождаем ресурсы
    cvReleaseImage(&image);
    cvReleaseImage(&src);
    // удаляем окно
    cvDestroyWindow("original");
    return 0;
}
