
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
//#include "RegionGrowing.h"

int main(int argc, char *argv[])
{
    cv::Mat_<uchar> img = cv::Mat::zeros(500, 500, CV_8UC1);
    
    //imread(filename, 1(=グレースケールで読み込む))
    cv::Mat src_img = cv::imread("/Users/arimuratakuma/Desktop/opencv2withxcode/grayhunabiki.png", 1);
    if (!src_img.data) return -1;
    
    //現在のTickCount
    int64 start = cv::getTickCount();
    
    
    cv::Mat dst_img, work_img;
    dst_img = src_img.clone();
    //cvtColor:色空間の変換
    cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
    
    //Hough変換のための前処理(画像の平滑化を行わないと誤検出が発生しやすい)
    cv::GaussianBlur(work_img, work_img, cv::Size(11,11), 2,2);
    
    //Hough変換による円の検出と検出した円の描画
    std::vector<cv::Vec3f> circles;
    
    cv::HoughCircles(work_img, circles, CV_HOUGH_GRADIENT,
                     1, // 大きい閾値
                     1000, // 円を検出する際の投票数の閾値
                     10, //円半径の最小値
                     30); //円半径の最大値
    
    
    std::vector<cv::Vec3f>::iterator it = circles.begin();
    for(; it!=circles.end(); ++it) {
        cv::Point center(cv::saturate_cast<int>((*it)[0]), cv::saturate_cast<int>((*it)[1]));
        int radius = cv::saturate_cast<int>((*it)[2]);
        cv::circle(dst_img, center, radius, cv::Scalar(0,0,255), 2);
    }

    int64 end = cv::getTickCount();
    double elapsedMsec = (end - start)*1000 / cv::getTickFrequency();
    std::cout<< elapsedMsec << "[ms]" << std::endl;
    
    cv::namedWindow("HoughCircles", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    cv::imshow("HoughCircles", dst_img);
    cv::waitKey(0);
}