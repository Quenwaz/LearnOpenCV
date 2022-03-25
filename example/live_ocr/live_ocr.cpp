/*
 * segmented_word_recognition.cpp
 *
 * A demo program on segmented word recognition.
 * Shows the use of the OCRHMMDecoder API with the two provided default character classifiers.
 *
 * Created on: Jul 31, 2015
 *     Author: Lluis Gomez i Bigorda <lgomez AT cvc.uab.es>
 */

#include "opencv2/text.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>


struct NumberOcr
{
    explicit NumberOcr(std::string lex)
    {

        // character recognition vocabulary
        std::string voc = "0123456789";
        // Emission probabilities for the HMM language model (identity matrix by default)
        cv::Mat emissionProbabilities = cv::Mat::eye((int)voc.size(), (int)voc.size(), CV_64FC1);
        // Bigram transition probabilities for the HMM language model
        cv::Mat transitionProbabilities;

        if (lex.size()>0)
        {
            // lexicon provided as a list of comma separated words.
            // Build tailored language model for the provided lexicon
            std::vector<std::string> lexicon;
            size_t pos = 0;
            std::string delimiter = ",";
            std::string token;
            while ((pos = lex.find(delimiter)) != std::string::npos) {
                token = lex.substr(0, pos);
                lexicon.push_back(token);
                lex.erase(0, pos + delimiter.length());
            }
            lexicon.push_back(lex);
            cv::text::createOCRHMMTransitionsTable(voc,lexicon,transitionProbabilities);
        } else {
            // Or load the generic language model (from Aspell English dictionary)
            cv::FileStorage fs("F:/DevEnvs/CppLibrary/opencvsdk/etc/testdata/contrib/text/OCRHMM_transitions_table.xml", cv::FileStorage::READ);
            fs["transition_probabilities"] >> transitionProbabilities;
            fs.release();
        }

        ocrTes_ = cv::text::OCRTesseract::create(R"(D:\Program Files (x86)\Tesseract-OCR\tessdata)", "eng", "0123456789");

        ocrNM_  = cv::text::OCRHMMDecoder::create(
                                    cv::text::loadOCRHMMClassifierNM("F:/DevEnvs/CppLibrary/opencvsdk/etc/testdata/contrib/text/OCRHMM_knn_model_data.xml.gz"),
                                    voc, transitionProbabilities, emissionProbabilities);

        ocrCNN_ = cv::text::OCRHMMDecoder::create(
                                    cv::text::loadOCRHMMClassifierCNN("F:/DevEnvs/CppLibrary/opencvsdk/etc/testdata/contrib/text/OCRBeamSearch_CNN_model_data.xml.gz"),
                                    voc, transitionProbabilities, emissionProbabilities);
    }

    void operator()(cv::Mat& frame)
    {
        try{
            // be sure the frame is a binary image
            cv::Mat ocrframe;
            cvtColor(frame, ocrframe, cv::COLOR_BGR2GRAY);
            threshold(ocrframe, ocrframe, 100., 255, cv::THRESH_BINARY);
erode(ocrframe, ocrframe, getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3))); //腐蚀
            dilate(ocrframe, ocrframe, getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3))); //膨胀
            
        	cv::GaussianBlur(ocrframe, ocrframe, cv::Size(3, 3), 0, 0);
	        
            cv::imshow("binary", ocrframe);
            std::string output;
            double t_r = (double)cv::getTickCount();
            std::vector<cv::Rect> component_rect;
            std::vector<std::string> component_texts;
            std::vector<float> component_confidences;
            ocrTes_->run(ocrframe, output,&component_rect, &component_texts, &component_confidences, cv::text::OCR_LEVEL_WORD);
            output.erase(remove(output.begin(), output.end(), '\n'), output.end());
            for (size_t i = 0;i < component_texts.size(); ++i)
            {
                // if (component_confidences.at(i) < 60){
                //     continue;
                // }
                std::string number = component_texts.at(i);

                number = number.erase(number.find_last_not_of(" ") + 1);
                number = number.erase(0, number.find_first_not_of(" "));
                if (number.empty()){
                    continue;
                }


                cv::rectangle(frame, component_rect.at(i), cv::Scalar(0,255, 0), 2);
                cv::putText(frame, number, component_rect.at(i).tl(),  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));
            }


            std::cout << " OCR_Tesseract  output \"" << output << "\". Done in "
                << ((double)cv::getTickCount() - t_r)*1000/cv::getTickFrequency() << " ms." << std::endl;

            // t_r = (double)getTickCount();
            // ocrNM_->run(ocrframe, output);
            // cout << " OCR_NM         output \"" << output << "\". Done in "
            //     << ((double)getTickCount() - t_r)*1000/getTickFrequency() << " ms." << endl;

            // t_r = (double)getTickCount();
            // ocrCNN_->run(ocrframe, output);
            // cout << " OCR_CNN        output \"" << output << "\". Done in "
            //     << ((double)getTickCount() - t_r)*1000/getTickFrequency() << " ms." << endl;
        }
        catch (cv::Exception& e)
        {
            // std::cout << e.msg.c_str() << std::endl;
        }

    }

    cv::Ptr<cv::text::OCRTesseract>  ocrTes_;
    cv::Ptr<cv::text::OCRHMMDecoder> ocrNM_;
    cv::Ptr<cv::text::OCRHMMDecoder> ocrCNN_;
};




int main(int argc, char* argv[]) {
	try
	{
        
        NumberOcr number_ocr("");

        auto image = cv::imread(R"(C:\Users\Quenwaz\Desktop\test.jpg)");
        number_ocr(image);
        cv::imshow("live", image);
        for(;;)
        cv::waitKey(0);
	
	    cv::VideoCapture inputVideo(0);
	    if (!inputVideo.isOpened())
	    {
	        std::cout << "Camera not found." << std::endl;
	        return -1;
	    }

	    while (true)
	    {
	        cv::Mat frame;
	        inputVideo >> frame;              // read
	        if (frame.empty()) break;         // check if at end
            number_ocr(frame);
            cv::imshow("live", frame);
            cv::waitKey(10);

	    }

	    std::cout << "Finished writing" << std::endl;
	    return 0;
	}
	catch (cv::Exception& e)
	{
		std::cout << e.msg.c_str() << std::endl;
	}

}