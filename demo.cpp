#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "Halide.h"
#include "halide_pipeline.h"

//#define USE_PAPI

#ifdef USE_PAPI
#include <papi.h>
#else
#include <ctime>
#endif

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    long long int counter = 0;
    namedWindow("Edge Detection - Halide Image Processing DSL");
    std::string text2;

#ifdef USE_PAPI   
    float rtime, ptime;
    long long flpops;
    float mflops;

    PAPI_flops(&rtime, &ptime, &flpops, &mflops);
    PAPI_stop_counters();
#endif

    for(;;)
    {
    	Mat frame;

       	cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);

        buffer_t input_buf = {0}, output_buf = {0};
	input_buf.host  = edges.data;
	output_buf.host = edges.data;
	input_buf.stride[0] = output_buf.stride[0] = 1;
	input_buf.stride[1] = output_buf.stride[1] = frame.cols;
	input_buf.extent[0] = output_buf.extent[0] = frame.cols;
	input_buf.extent[1] = output_buf.extent[1] = frame.rows;
	input_buf.min[0] = output_buf.min[0] = 0;
	input_buf.min[1] = output_buf.min[1] = 0;
	input_buf.elem_size = output_buf.elem_size = 1;

#ifdef USE_PAPI	
	PAPI_start_counters();
	halide_pipeline(&input_buf, frame.rows, frame.cols, &output_buf);
	PAPI_stop_counters();
	PAPI_flops(&rtime, &ptime, &flpops, &mflops);
#else
	clock_t begin = clock();
	halide_pipeline(&input_buf, frame.rows, frame.cols, &output_buf);
	clock_t end = clock();
#endif	

	int image_size = frame.cols*frame.rows;
	//153*frame.rows*frame.cols+6
	int num_floating_operations = 125*frame.rows*frame.cols;
	double flop = image_size*num_floating_operations;
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	double flop_per_sec = flop/elapsed_secs;

	std::string text1 = "Halide Pipeline (Edge Detection)";

#ifdef USE_PAPI
	if (counter==0 || counter%10==0) // Change the Flops each 100 iterations
		text2 = "GFlops : " + std::to_string(mflops/1000);
#else
	if (counter==0 || counter%10==0) // Change the Flops each 100 iterations
		text2 = "GFlops : " + std::to_string(flop_per_sec/1000000000);
#endif	
	counter++;

	cv::putText(edges, text1, cv::Point(30, 40), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255), 1, 8, false);
       	cv::putText(edges, text2, cv::Point(30, 70), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255), 1, 8, false);

        imshow("edges", edges);
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}


