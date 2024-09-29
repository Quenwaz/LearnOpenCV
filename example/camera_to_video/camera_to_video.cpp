#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write


int main(int argc, char* argv[])
{
	try
	{
	    if (argc < 3)
	    {
	        std::cout << "Usage: "<<argv[0] <<" [outfilename] [fps] [minute:default 1]" << std::endl;
			std::cout << "eg.: " << argv[0] << " video 24" << std::endl;
	        return -1;
	    }

	    const std::string outfile = argv[1];
	    int nMinute = 1;
	    if (argc >= 4) {
	        nMinute = atoi(argv[3]);
	        nMinute = nMinute < 1 ? 1 : nMinute;
	    }
	    cv::VideoCapture inputVideo(0);
	    if (!inputVideo.isOpened())
	    {
	        std::cout << "Camera not found." << std::endl;
	        return -1;
	    }

		// 设置帧率
		int fps = atoi(argv[2]);
		fps = fps == 0 ? 24 : fps;
	    inputVideo.set(CV_CAP_PROP_FPS,fps) ;

		// 输出文件
	    std::string::size_type pAt = outfile.find_last_of('.');
	    const std::string NAME = outfile.substr(0, pAt)  + ".mp4";   // Form the new name with container
	    /*
	    CV_FOURCC('P', 'I', 'M', '1') = MPEG-1 codec
	    CV_FOURCC('M', 'J', 'P', 'G') = motion-jpeg codec
	    CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec 
	    CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec 
	    CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec 
	    CV_FOURCC('U', '2', '6', '3') = H263 codec 
	    CV_FOURCC('I', '2', '6', '3') = H263I codec 
	    CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec
	     */
		
	    const int ex =  CV_FOURCC('D', 'I', 'V', 'X');//CV_FOURCC('H', '2', '6', '4'); // static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));

		// 帧尺寸
	    cv::Size S = cv::Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
	        (int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

	    cv::VideoWriter outputVideo(NAME, ex, fps, S, true);
	    if (!outputVideo.isOpened())
	    {
	        std::cout << "Could not open the output video for write: " << NAME << std::endl;
	        return -1;
	    }

	    std::cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
	        << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;
	    // Transform from int to char via Bitwise operators
	    char EXT[] = { ex & 0XFF , (ex & 0XFF00) >> 8,(ex & 0XFF0000) >> 16,(ex & 0XFF000000) >> 24, 0 };
	    std::cout << "Input codec type: " << EXT << std::endl;

	    std::vector<cv::Mat> spl;
	    long second = 0;
	    while (true)
	    {
	        cv::Mat frame;
	        inputVideo >> frame;              // read
	        if (frame.empty()) break;         // check if at end

	        //outputVideo.write(res); //save or
	        outputVideo << frame;
	        ++second;
	        // Sleep(ceil(1000.0 / fps));
	        if (second >= (fps * 60 * nMinute))
	        {
	            break;
	        }
	    }

	    std::cout << "Finished writing" << std::endl;
	    return 0;
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg.c_str() << std::endl;
	}

}