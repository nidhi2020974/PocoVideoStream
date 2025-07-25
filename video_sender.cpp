#include <iostream>
#include <opencv2/opencv.hpp>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetException.h>

int main() {
    try {
        Poco::Net::SocketAddress sa("127.0.0.1", 9000);
        Poco::Net::StreamSocket socket(sa);

        cv::VideoCapture cap(0);
        if (!cap.isOpened()) {
            std::cerr << "Camera open failed!" << std::endl;
            return -1;
        }

        cv::Mat frame;
        std::vector<uchar> buffer;

        while (true) {
            cap >> frame;
            if (frame.empty()) break;

            cv::imencode(".jpg", frame, buffer);
            int size = buffer.size();

            socket.sendBytes(&size, sizeof(int));                   
            socket.sendBytes(buffer.data(), buffer.size());        

            cv::imshow("Sender Preview", frame);
            if (cv::waitKey(1) == 27) break; 
        }

        socket.close();
    } catch (Poco::Exception& ex) {
        std::cerr << "Poco Exception: " << ex.displayText() << std::endl;
    }

    return 0;
}
