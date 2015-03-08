
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    cv::Mat_<uchar> img = cv::Mat::zeros(500, 500, CV_8UC1);
    
    cv::Mat src_img = cv::imread("/Users/aritaku/Documents/PupilImages/スクリーンショット .png", 1);
    //imread(filename, 1(=グレースケールで読み込む))
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
    
    //マウスの瞳孔半径を検出出来ないのはHoughCircles, のパラメーターを変化させれば対応出来る可能性あり。検出する半径を変えていく。
    cv::HoughCircles(work_img, circles, CV_HOUGH_GRADIENT,
                     1, // 大きい閾値
                     1000, // 円を検出する際の投票数の閾値
                     1, //円半径の最小値
                     20); //円半径の最大値
    
    
    //cv::Vec3fってなに
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