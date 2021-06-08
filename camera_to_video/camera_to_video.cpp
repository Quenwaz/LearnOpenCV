#include <iostream> // for standard I/O
#include <string>   // for strings
#include <Windows.h>
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui/highgui.hpp>  // Video write


int main(int argc, char* argv[])
try
{
    if (argc < 5)
    {
	    std::cout << "Not enough parameters" << std::endl;
        return -1;
    }

    const std::string source = argv[1];            // the source file name
    const bool askOutputType = argv[3][0] == 'Y';  // If false it will use the inputs codec type

    cv::VideoCapture inputVideo(atoi(argv[1]));        // Open input
    if (!inputVideo.isOpened())
    {
	    std::cout << "Could not open the input video." << source << std::endl;
        return -1;
    }
    inputVideo.set(CV_CAP_PROP_FPS, 24);

    std::string::size_type pAt = source.find_last_of('.');   // Find extension point
    const std::string NAME = source.substr(0, pAt) + argv[2][0] + ".mp4";   // Form the new name with container
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
    int ex = CV_FOURCC('H', '2', '6', '4');// static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form

    // Transform from int to char via Bitwise operators
    char EXT[] = { ex & 0XFF , (ex & 0XFF00) >> 8,(ex & 0XFF0000) >> 16,(ex & 0XFF000000) >> 24, 0 };

    cv::Size S = cv::Size((int)inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    //Acquire input size
                          (int)inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

    if (askOutputType) ex =-1;
    auto fps = inputVideo.get(CV_CAP_PROP_FPS);
    fps = fps == 0 ? 24 : fps;
    cv::VideoWriter outputVideo(NAME, ex, fps, S, true);                                       // Open the output
    if (!outputVideo.isOpened())
    {
	    std::cout << "Could not open the output video for write: " << source << std::endl;
        return -1;
    }

    union { int v; char c[5]; } uEx;
    uEx.v = ex;                              // From Int to char via union
    uEx.c[4] = '\0';

    std::cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
        << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << std::endl;
    std::cout << "Input codec type: " << EXT << std::endl;

    int channel = 2;    // Select the channel to save
    switch (argv[2][0])
    {
    case 'R': {channel = 2; break; }
    case 'G': {channel = 1; break; }
    case 'B': {channel = 0; break; }
    }
    cv::Mat src, res;
    std::vector<cv::Mat> spl;
    long second = 0;
    while (true) //Show the image captured in the window and repeat
    {
        inputVideo >> src;              // read
        if (src.empty()) break;         // check if at end

        // split(src, spl);                 // process - extract only the correct channel
        // for (int i = 0; i < 3; ++i)
        //     if (i != channel)
        //         spl[i] = Mat::zeros(S, spl[0].type());
        // merge(spl, res);
        res = src;
        //outputVideo.write(res); //save or
    	
        outputVideo << res;
    	++second;
        Sleep(ceil(1000.0 / fps));
    	if(second >= (fps * 60))
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