#include <iostream>
#include <opencv2/opencv.hpp>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/NetException.h>

int main() {
    try {
        Poco::Net::ServerSocket server(9000);
        Poco::Net::StreamSocket client = server.acceptConnection();

        while (true) {
            int size = 0;
            int received = client.receiveBytes(&size, sizeof(int));
            if (received != sizeof(int)) break;

            std::vector<uchar> buffer(size);
            int totalReceived = 0;
            while (totalReceived < size) {
                int bytes = client.receiveBytes(buffer.data() + totalReceived, size - totalReceived);
                if (bytes <= 0) break;
                totalReceived += bytes;
            }

            cv::Mat img = cv::imdecode(buffer, cv::IMREAD_COLOR);
            if (img.empty()) continue;

            cv::imshow("Receiver", img);
            if (cv::waitKey(1) == 27) break; // ESC to stop
        }

        client.close();
    } catch (Poco::Exception& ex) {
        std::cerr << "Poco Exception: " << ex.displayText() << std::endl;
    }

    return 0;
}
