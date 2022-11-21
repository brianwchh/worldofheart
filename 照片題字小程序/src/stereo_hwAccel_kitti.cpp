
/*
  Author : Brian (Wu Cheng He)
  Email :  brianwchh@gmail.com
  github : https://github.com/brianwchh/zynq-VDMA-driver-StereoVisionApp.git

*/
#define u32 unsigned int

#include "stereo.h"

#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/opencv.hpp>

#include <sys/time.h>
#include <sstream>
#include <fstream>
#include <termios.h>


using namespace std ;
using namespace cv;


int main(int argc, char** argv)
{

    Mat imOrigin = imread("./originalImg.png",1);
    Mat imTraget = imread("./targetImg.png",1);
    Mat imOrigin_gray,imTraget_gray;

    if(imOrigin.empty()){
        cout << "could not read image " << endl;
    }

    cvtColor(imOrigin,imOrigin_gray,CV_BGR2GRAY);
    cvtColor(imTraget,imTraget_gray,CV_BGR2GRAY);

    for (int r=0; r<imOrigin.rows; r++)
        for (int c=0; c<imOrigin.cols; c++){
            uchar pixV_origin = imOrigin_gray.at<uchar>(r,c) ;
            uchar pixV_target = imTraget_gray.at<uchar>(r,c) ;
            if (pixV_target > pixV_origin){  // the white area, replaced with original color
                imTraget.at<cv::Vec3b>(r,c)[0] = imOrigin.at<cv::Vec3b>(r,c)[0] ;
                imTraget.at<cv::Vec3b>(r,c)[1] = imOrigin.at<cv::Vec3b>(r,c)[1] ;
                imTraget.at<cv::Vec3b>(r,c)[2] = imOrigin.at<cv::Vec3b>(r,c)[2] ;
            } 
            // else {  // the ink area and original area, preserved
                
            // }
            
        }

    imwrite("final.jpg", imTraget);

    return 0 ;
}


