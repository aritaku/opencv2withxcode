//
//  main.cpp
//  opencv2withxcode
//
//  Created by 有村 琢磨 on 2015/03/03.
//  Copyright (c) 2015年 takuma arimura. All rights reserved.
//

//#include <iostream>
//#include <opencv2/opencv.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main () {
//    VideoCapture capture(0);
//    Mat frame;
//    
//    while (waitKey(1) !='q'){
//        capture >> frame;
//        imshow("sample", frame);
//    }
//    
//    return 0;
//}


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
    cv::Mat src_img = cv::imread("/Users/aritaku/Desktop/スクリーンショット 2015-03-03 4.43.25.png", 1);
    if (!src_img.data) return -1;
    
    cv::Mat dst_img, work_img;
    dst_img = src_img.clone();
    cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
    
    //Hough変換のための前処理(画像の平滑化を行わないと誤検出が発生しやすい)
    cv::GaussianBlur(work_img, work_img, cv::Size(11,11), 2,2);
    
    //Hough変換による炎の検出と検出した円の描画
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(work_img, circles, CV_HOUGH_GRADIENT, 1, 100, 20, 50);
    
    std::vector<cv::Vec3f>::iterator it = circles.begin();
    for(; it!=circles.end(); ++it) {
        cv::Point center(cv::saturate_cast<int>((*it)[0]), cv::saturate_cast<int>((*it)[1]));
        int radius = cv::saturate_cast<int>((*it)[2]);
        cv::circle(dst_img, center, radius, cv::Scalar(0,0,255), 2);
    }
    
    cv::namedWindow("HoughCircles", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("HoughCircles", dst_img);
    cv::waitKey(0);
}