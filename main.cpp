#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* src = 0;

// рисуем целеуказатель
void drawTarget(IplImage* img, int x, int y, int radius)
{
        cvCircle(img,cvPoint(x, y),radius,CV_RGB(250,0,0),1,8);
        cvLine(img, cvPoint(x-radius/2, y-radius/2), cvPoint(x+radius/2, y+radius/2),CV_RGB(250,0,0),1,8);
        cvLine(img, cvPoint(x-radius/2, y+radius/2), cvPoint(x+radius/2, y-radius/2),CV_RGB(250,0,0),1,8);
}

// обработчик событий от мышки
void myMouseCallback( int event, int x, int y, int flags, void* param )
{
        IplImage* img = (IplImage*) param;

        switch( event ){
                case CV_EVENT_MOUSEMOVE:
                        break;

                case CV_EVENT_LBUTTONDOWN:
                        printf("%d x %d\n", x, y);
                        drawTarget(img, x, y, 10);
                        break;

                case CV_EVENT_LBUTTONUP:
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

        while(1){
                // показываем картинку
                cvCopyImage( image, src );
                cvShowImage( "original", src );

                char c = cvWaitKey(33);
                if (c == 27) { // если нажата ESC - выходим
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
