#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>

#include "streamSimulator.hpp"

using namespace std;

int main()
{
    streamSimulator* myStream = new streamSimulator();

    cv::namedWindow( "Stream", CV_WINDOW_AUTOSIZE );
    std::pair<unsigned char*, size_t> imageData;
    while( true )
    {
        imageData = myStream->getImage();
        std::vector<unsigned char> buffer( imageData.first, imageData.first + imageData.second );
        cv::Mat image = imdecode(buffer, cv::IMREAD_ANYCOLOR);
        if(image.empty())
            return 0;
        cv::imshow("Stream", image);
        cv::waitKey( myStream->getSynchroTime_ums() / 1000 );
//        usleep( myStream->getSynchroTime_ums() );
    }
    myStream->waitForThreadExecution();
}
