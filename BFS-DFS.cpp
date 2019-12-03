//
// Created by lightol on 2019/12/2.
//

// BFS, Breadth First Search
// DFS, Depth First Search

#include <iostream>
// 在这里使用队列，利用它先进先出的特性
// 这也是为了遵循BFS的原理，如同水面的涟漪一层层地往外扩展
#include <queue>

// 在这里使用栈，利用它后进先出的特性。这样就遵循了DFS的原理，如同贪吃蛇一般依次往前进
#include <stack>

#include <opencv2/core.hpp>
#include <cv.hpp>
#include <set>

using std::cout;
using std::endl;

void DFS() {
    cv::Mat image(60, 60, CV_8UC1, cv::Scalar::all(255));
    int rows = image.rows;
    int cols = image.cols;
    cout << "image.size is " << image.size() << endl;

    // 首先确定一个种子僵尸，然后它不断地自爆，感染上下左右4个活人，自爆后自己就死了也不能再次被感染了(因为已经炸了，渣都不剩了)，
    // 然后4个被感染的人再次自爆去感染它四周的人。这样，经过多轮自爆后，毒气慢慢地就会延伸到“神”所在的区域，
    // 找出第一个被感染的“神”是谁
    std::stack<cv::Point2i> vZombiePixels; // 已经感染毒气，但还没自爆的（也就是还没死的）僵尸的集合
    cv::Point2i seedPt(30, 30);  // 第一个被下蛊虫的人
    vZombiePixels.push(seedPt);
    image.at<uchar>(seedPt.x, seedPt.y) = 0;

    // 还有僵尸没自爆。因为到最后我们肯定是需要所有人都感染病毒而死，并且死了后马上爆炸，炸得整幅图渣都不剩.除非已经找到了一个神
//    cv::Mat image_show = image.clone();
    while (!vZombiePixels.empty()) {
        cv::Mat image_show(image.size(), CV_8UC3, cv::Scalar::all(255));
        auto vTempPixels = vZombiePixels;
        while (!vTempPixels.empty()) {
            cv::Point2i curPixel1 = vTempPixels.top();
            image_show.at<cv::Vec3b>(curPixel1.x, curPixel1.y) = cv::Vec3b(100, 200, 0);
            vTempPixels.pop();
        }
        cout << "numPixel = " << vZombiePixels.size() << endl;
        cv::namedWindow("propagation", CV_WINDOW_NORMAL);
        cv::imshow("propagation", image_show);
        cv::waitKey(1);

        // 取出队列最底部的元素，让它自爆：先死，然后再扩散病毒给活人
        cv::Point2i curPixel = vZombiePixels.top();

        int x = curPixel.x;
        int y = curPixel.y;

        // 把队列最底部的元素pop出来，因为它的尸体已经炸了
        vZombiePixels.pop();

        // 死了后实体自爆散发毒气，去感染四周的活人（至于周围那些被感染后已经死了的人，它炸得尸体都没有了，病毒自然也感染不了了）
        if (x >= 1) {  // 可以往上扩展
            if (image.at<uchar>(x - 1, y) != 0) {
                vZombiePixels.emplace(x - 1, y);
                image.at<uchar>(x - 1, y) = 0;
            }
        }

        if (x <= rows - 2) {  // 可以往下扩展
            if (image.at<uchar>(x + 1, y) != 0) {
                vZombiePixels.emplace(x + 1, y);
                image.at<uchar>(x + 1, y) = 0;
            }
        }

        if (y >= 1) {  // 可以往左扩展
            if (image.at<uchar>(x, y - 1) != 0) {
                vZombiePixels.emplace(x, y - 1);
                image.at<uchar>(x, y - 1) = 0;
            }
        }

        if (y <= cols - 2) {  // 可以往右扩展
            if (image.at<uchar>(x, y + 1) != 0) {
                vZombiePixels.emplace(x, y + 1);
                image.at<uchar>(x, y + 1) = 0;
            }
        }

        cv::namedWindow("image", CV_WINDOW_NORMAL);
        cv::imshow("image", image);
//        cv::waitKey(10);
    }
}

void BFS() {
    cv::Mat image(60, 60, CV_8UC1, cv::Scalar::all(255));
    int rows = image.rows;
    int cols = image.cols;
    cout << "image.size is " << image.size() << endl;

    // 首先确定一个种子僵尸，然后它不断地自爆，感染上下左右4个活人，自爆后自己就死了也不能再次被感染了(因为已经炸了，渣都不剩了)，
    // 然后4个被感染的人再次自爆去感染它四周的人。这样，经过多轮自爆后，毒气慢慢地就会延伸到“神”所在的区域，
    // 找出第一个被感染的“神”是谁
    std::queue<cv::Point2i> vZombiePixels; // 已经感染毒气，但还没自爆的（也就是还没死的）僵尸的集合
    cv::Point2i seedPt(30, 30);  // 第一个被下蛊虫的人
    vZombiePixels.push(seedPt);
    // 入栈的同时自己就死了，就不能再被感染了
    image.at<uchar>(seedPt.x, seedPt.y) = 0;

    // 还有僵尸没自爆。因为到最后我们肯定是需要所有人都感染病毒而死，并且死了后马上爆炸，炸得整幅图渣都不剩.除非已经找到了一个神
//    cv::Mat image_show = image.clone();
    while (!vZombiePixels.empty()) {
//        cv::Mat image_show = image.clone();
        cv::Mat image_show(image.size(), CV_8UC3, cv::Scalar::all(255));
        auto vTempPixels = vZombiePixels;
        while (!vTempPixels.empty()) {
            cv::Point2i curPixel = vTempPixels.front();
//            cout << "[" << curPixel.x << ", " << curPixel.y << "] ";
            image_show.at<cv::Vec3b>(curPixel.x, curPixel.y) = cv::Vec3b(100, 200, 0);
            vTempPixels.pop();
        }
        cout << "numPixel = " << vZombiePixels.size() << endl;
        cv::namedWindow("propagation", CV_WINDOW_NORMAL);
        cv::imshow("propagation", image_show);
//        cv::waitKey(1);
//        cv::waitKey();

        // 取出队列最底部的元素，让它自爆：先死，然后再扩散病毒给活人
        cv::Point2i curPixel = vZombiePixels.front();

        int x = curPixel.x;
        int y = curPixel.y;

        // 把队列最底部的元素pop出来，因为它的尸体已经炸了
        vZombiePixels.pop();

        // 死了后实体自爆散发毒气，去感染四周的活人（至于周围那些被感染后已经死了的人，它炸得尸体都没有了，病毒自然也感染不了了）
        if (x >= 1) {  // 可以往上扩展
            if (image.at<uchar>(x - 1, y) != 0) {
                vZombiePixels.emplace(x - 1, y);
                // 给我炸，尸骨无存了，没办法再被感染了
                image.at<uchar>(x - 1, y) = 0;
            }
        }

        if (x <= rows - 2) {  // 可以往下扩展
            if (image.at<uchar>(x + 1, y) != 0) {
                vZombiePixels.emplace(x + 1, y);
                image.at<uchar>(x + 1, y) = 0;
            }
        }

        if (y >= 1) {  // 可以往左扩展
            if (image.at<uchar>(x, y - 1) != 0) {
                vZombiePixels.emplace(x, y - 1);
                image.at<uchar>(x, y - 1) = 0;
            }
        }

        if (y <= cols - 2) {  // 可以往右扩展
            if (image.at<uchar>(x, y + 1) != 0) {
                vZombiePixels.emplace(x, y + 1);
                image.at<uchar>(x, y + 1) = 0;
            }
        }

        cv::namedWindow("image", CV_WINDOW_NORMAL);
        cv::imshow("image", image);
        cv::waitKey(10);
    }
}



int main() {
    DFS();

    BFS();
}
