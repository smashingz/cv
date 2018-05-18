#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <imgproc/imgproc_c.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* src = 0;
IplImage* gray = 0;
IplImage* edges = 0;

int x_sel, y_sel;

// рисуем область выделения
void selectROI(IplImage* img, int x, int y, int x1, int y1, int add_value = 200) {
	int width, height, x_beg, y_beg;
	width = abs(x - x1);
	height = abs(y - y1);
	if ((x1 - x > 0) ^ (y1 - y > 0)) {
		x_beg = x1 < x ? x1 : x;
		y_beg = y1 < y ? y1 : y;
	}
	else {
		x_beg = y1 < y ? x1 : x;
		y_beg = x1 < x ? y1 : y;
	}
	cvSetImageROI(img, cvRect(x_beg, y_beg, width, height));
}

// обработчик событий от мышки
void myMouseCallback( int event, int x, int y, int flags, void* param ) {
	IplImage* img = (IplImage*) param;

	switch( event ) {
	case CV_EVENT_MOUSEMOVE:
		break;

	case CV_EVENT_LBUTTONDOWN:
		x_sel = x;
		y_sel = y;
		break;

	case CV_EVENT_LBUTTONUP:
		if ((x - x_sel == 0) && (y - y_sel == 0)) {
			break;
		}
		selectROI(img, x_sel, y_sel, x, y);
		gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		edges = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
		cvCvtColor(image, gray, CV_RGB2GRAY);
		cvCanny(gray, edges, 10, 100, 3);
		cvShowImage("cvCanny", edges );
		CvChain* chain = 0;
		CvMemStorage* storage = 0;
		storage = cvCreateMemStorage(0);
		cvFindContours(edges, storage, (CvSeq**)(&chain), sizeof(*chain), CV_RETR_EXTERNAL, CV_CHAIN_CODE);
		FILE * fp;
		fp = fopen ("out.txt", "w");
		for (; chain != NULL; chain = (CvChain*)chain->h_next) {
			CvSeqReader reader;
			int i, total = chain->total;
			cvStartReadSeq((CvSeq*)chain, &reader, 0);
			fprintf(fp, "--------------------chain\n");
			for (i = 0; i<total; i++) {
				char code;
				CV_READ_SEQ_ELEM(code, reader);
				fprintf(fp, "%d", code);
			}
		}
		cvResetImageROI(image);
		break;
	}
}

int main(int argc, char* argv[]) {
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

	// получаем границы
        cvCanny(gray, edges, 10, 100, 3);

	while(1) {
		// показываем картинки
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
	cvReleaseImage(&edges);
	// удаляем окно
	cvDestroyAllWindows();
	return 0;
}
