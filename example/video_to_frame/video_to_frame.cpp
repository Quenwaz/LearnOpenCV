#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write

#include <chrono>

void show_mat(const char* wintitle, cv::Mat img)
{
	cv::namedWindow(wintitle);
	cv::imshow(wintitle, img);
}


int main(int argc, char* argv[])
{
	if (argc < 4){
		fprintf(stderr, "usage: %s [mp4 file] [fps] [destination]", argv[0]);
		return EXIT_FAILURE;
	}

	auto exepath = argv[0];
	strrchr(exepath, '\\')[0] = '\0';
	try
	{
		const auto source_file = argv[1];
		const auto fps = std::atoi(argv[2]);
		const auto destination = argv[3];
	    cv::VideoCapture inputVideo(source_file);
	    if (!inputVideo.isOpened())
	    {
	        std::cout << "Camera not found." << std::endl;
	        return EXIT_FAILURE;
	    }
		
		size_t current_frame_index = 0;
	    while (true)
	    {
	        cv::Mat frame;
	        inputVideo >> frame;              
	        if (frame.empty()) break;         
			cv::waitKey(ceil(1000.0 / fps));

			char save_to_frame[_MAX_PATH] = {0};
			snprintf(save_to_frame, sizeof save_to_frame, "%s/frame_%d.jpg", destination, current_frame_index++);

			if (cv::imwrite(save_to_frame, frame)){
				std::string msg = "saved frame to ";
				msg += save_to_frame;
				cv::putText(frame, msg.c_str(), cv::Point(10, 50), CV_FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0),1,8);
			}
			show_mat("frame", frame);
			
	    }

	    return EXIT_SUCCESS;
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg.c_str() << std::endl;
	}
	
	return EXIT_FAILURE;
}