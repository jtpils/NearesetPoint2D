// BFS, Breadth First Search

#include <iostream>
// 在这里使用队列，利用它先进先出的特性。因为僵尸自爆是有顺序的，先被感染的僵尸必须先自爆，
// 这也是为了遵循BFS的原理，如同水面的涟漪一层层地往外扩展
#include <queue>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>
#include <chrono>

using std::cout;
using std::endl;

int main() {
    cv::Mat image = cv::imread("../data/segmentation.png");
    // 生死薄，记录哪些是活人哪些是死人，刚开始全都活着
    cv::Mat image_DeathNote(image.size(), CV_8UC1, cv::Scalar::all(1));
    int rows = image.rows;
    int cols = image.cols;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (image.at<cv::Vec3b>(i, j) != cv::Vec3b(55, 80, 156)) {
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
            } else {
//                cout << i << ", " << j << endl;
            }
        }
    }
    cv::Mat image_show = image.clone();

    // 第一个被感染的“神”
    cv::Point2i cami(0, 0);
    cv::Vec3b mortal(0, 0, 0);

    // 首先确定一个种子僵尸，然后它不断地自爆，感染上下左右4个活人，自爆后自己就死了也不能再次被感染了(因为已经炸了，渣都不剩了)，
    // 然后4个被感染的人再次自爆去感染它四周的人。这样，经过多轮自爆后，毒气慢慢地就会延伸到“神”所在的区域，
    // 找出第一个被感染的“神”是谁
    std::queue<cv::Point2i> vZombiePixels; // 已经感染毒气，但还没自爆的（也就是还没死的）僵尸的集合
    cv::Point2i seedPt(920, 1275);  // 第一个被下蛊虫的人
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
    if (image.at<cv::Vec3b>(seedPt.x, seedPt.y) != mortal) {
        // 如果第一个被感染的人恰好是神，那不用找了，直接是它
        cami = cv::Point2i(seedPt.x, seedPt.y);
    } else {
        vZombiePixels.push(seedPt);
        image_DeathNote.at<uchar>(seedPt.x, seedPt.y) = 0;
    }

    // 还有僵尸没自爆。因为到最后我们肯定是需要所有人都感染病毒而死，并且死了后马上爆炸，炸得整幅图渣都不剩.除非已经找到了一个神
    while (!vZombiePixels.empty()) {
        std::queue<cv::Point2i> vTempPixels = vZombiePixels;
        int numPixel = 0;
//        while (!vTempPixels.empty()) {
//            cv::Point2i curPixel = vTempPixels.front();
////            cv::circle(image_show, cv::Point2i(curPixel.y, curPixel.x), 5, cv::Scalar(100, 200, 0));
//            image_show.at<cv::Vec3b>(curPixel.x, curPixel.y) = cv::Vec3b(100, 200, 0);
//            vTempPixels.pop();
//            numPixel++;
//        }
//        cout << "numPixel = " << numPixel << endl;
//        cv::namedWindow("propagation", CV_WINDOW_NORMAL);
//        cv::imshow("propagation", image_show);
//        cv::waitKey(1);

        // 取出队列最底部的元素，让它自爆：先死，然后再扩散病毒给活人
        cv::Point2i curPixel = vZombiePixels.front();

        int x = curPixel.x;
        int y = curPixel.y;
        // 给我死，在生死薄上记下来.或者说已经炸了，尸骨无存了，没办法再被感染了
        image_DeathNote.at<uchar>(x, y) = 0;

        // 把队列最底部的元素pop出来，因为它的尸体已经炸了
        vZombiePixels.pop();

        // 死了后实体自爆散发毒气，去感染四周的活人（至于周围那些被感染后已经死了的人，它炸得尸体都没有了，病毒自然也感染不了了）
        if (x >= 1) {  // 可以往上扩展
            // 病毒扩散到了“神”域
            if (image.at<cv::Vec3b>(x - 1, 1) != mortal) {
                cami = cv::Point2i(x - 1, 1);
                break;
            }

            if (image_DeathNote.at<uchar>(x - 1, y) == 1) {
                vZombiePixels.emplace(x - 1, y);
                image_DeathNote.at<uchar>(x - 1, y) = 0;
            }
        }

        if (x <= rows - 2) {  // 可以往下扩展
            if (image.at<cv::Vec3b>(x + 1, y) != mortal) {
                cami = cv::Point2i(x + 1, y);
                break;
            }

            if (image_DeathNote.at<uchar>(x + 1, y) == 1) {
                vZombiePixels.emplace(x + 1, y);
                image_DeathNote.at<uchar>(x + 1, y) = 0;
            }
        }

        if (y >= 1) {  // 可以往左扩展
            if (image.at<cv::Vec3b>(x, y - 1) != mortal) {
                cami = cv::Point2i(x, y - 1);
                break;
            }

            if (image_DeathNote.at<uchar>(x, y - 1) == 1) {
                vZombiePixels.emplace(x, y - 1);
                image_DeathNote.at<uchar>(x, y - 1) = 0;
            }
        }

        if (y <= cols - 2) {  // 可以往右扩展
            if (image.at<cv::Vec3b>(x, y + 1) != mortal) {
                cami = cv::Point2i(x, y + 1);
                break;
            }

            if (image_DeathNote.at<uchar>(x, y + 1) == 1) {
                vZombiePixels.emplace(x, y + 1);
                image_DeathNote.at<uchar>(x, y + 1) = 0;
            }
        }
    }
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    double ttrack = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();
    cout << "ttrack = " << ttrack << endl << endl;
    cout << "cami is " << cami << endl;
    cout << "image.at<cv::Vec3b>(cami) is " << image.at<cv::Vec3b>(cami.x, cami.y) << endl;
    cv::circle(image, cv::Point2i(seedPt.y, seedPt.x), 5, cv::Scalar(100, 200, 0));
    cv::circle(image, cv::Point2i(cami.y, cami.x), 5, cv::Scalar(100, 200, 0));
    cv::namedWindow("segmentation", CV_WINDOW_NORMAL);
    cv::imshow("segmentation", image);

    {
//        cv::Mat im = image.clone();
//        im.setTo(cv::Scalar::all(255));
//        cv::circle(im, cv::Point2i(cami.y, cami.x) , 5, cv::Scalar(100, 200, 0));
//        cv::namedWindow("cami", CV_WINDOW_NORMAL);
//        cv::imshow("cami", im);
    }

    cv::waitKey();
}
