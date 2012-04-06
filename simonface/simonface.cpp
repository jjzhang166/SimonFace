#include "simonface.h"
#include "ui_simonface.h"
#include <QtGui>

SimonFace::SimonFace(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SimonFace)
{
    ui->setupUi(this);
    connect(ui->pbCameraCalibration, SIGNAL(clicked()), this, SLOT(SimonFaceDetection()));
}

SimonFace::~SimonFace()
{
    delete ui;
}
void SimonFace::SimonFaceDetection(){
    CvHaarClassifierCascade *cascade;
    CvMemStorage            *storage;
    CvCapture *capture;
    int   i,duration=0;
    char  key;
    char      *filename = "/home/mmh/kde/src/SimonFace/simonface/data/haarcascade_frontalface_alt.xml";
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
    storage = cvCreateMemStorage( 0 );
    capture = cvCaptureFromFile("/home/mmh/kde/src/SimonFace/simonface/data/test.mpg");
    assert( cascade && storage && capture );

    while( duration < 20 ) {
      IplImage* frame = cvQueryFrame(capture);
      if( !frame ) break;
      CvSeq *faces = cvHaarDetectObjects(frame,cascade,storage,1.1,3,0,cvSize( 40, 40 ) );
      for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {
        CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );
        cvRectangle( frame,cvPoint( r->x, r->y ),cvPoint( r->x + r->width, r->y + r->height ),CV_RGB( 0, 255, 0 ), 1, 8, 0 );
      }
    cvCvtColor(frame,frame,CV_BGR2RGB);
    
    QImage image = QImage((unsigned char *)frame->imageDataOrigin,frame->width,frame->height,QImage::Format_RGB888);
    QGraphicsScene* scene = new QGraphicsScene(this);
    QPixmap tempimage=QPixmap::fromImage(image);
    ui->graphicsView_2->resize(211, 161); 
    QPixmap *backgroundPixmap = &tempimage;
    QPixmap sized = backgroundPixmap->scaled(QSize(ui->graphicsView_2->width(),ui->graphicsView_2->height()),Qt::KeepAspectRatioByExpanding);
    QImage sizedImage = QImage(sized.toImage());
    QGraphicsPixmapItem *sizedBackground = scene->addPixmap(QPixmap::fromImage(sizedImage));
    ui->graphicsView_2->setScene(scene);
    key = cvWaitKey(10);
    duration++;
    }
    cvReleaseCapture( &capture );
    cvReleaseHaarClassifierCascade( &cascade );
    cvReleaseMemStorage( &storage );
 return;
}