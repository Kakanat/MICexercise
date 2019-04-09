#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

void mosaic_filter(cv::Mat &src)
{
	cv::Mat org = src.clone();

	int kernel_size, K_h, K_w;
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;

	int width = org.cols;
	int height = org.rows;
	int channels = org.channels();


	for (int present_h = 0; present_h < height; present_h = present_h + kernel_size) //高さに関する処理
	{
		K_h = std::min(kernel_size, height - present_h);
		for (int present_w = 0; present_w < width; present_w = present_w + kernel_size) //幅に関する処理
		{
			K_w = std::min(kernel_size, width - present_w);
			for (int b = 0; b < K_h; b++) //出力画素の高さ位置指定
			{
				cv::Vec3b *pointer_of_src = src.ptr<cv::Vec3b>(present_h + b);
				for (int a = 0; a < K_w; a++) //出力画素の幅位置指定
				{
					//フィルタ内の画素値の平均を計算
					double tmpValueOfBlue = 0.0; //画素値計算用変数
					double tmpValueOfGreen = 0.0; //画素値計算用変数	
					double tmpValueOfRed = 0.0; //画素値計算用変数	
					for (int j = 0; j < K_h; j++) //モザイク計算画素の高さ位置指定
					{
						cv::Vec3b *pointer_of_org = org.ptr<cv::Vec3b>(present_h + j);
						for (int i = 0; i < K_w; i++) //モザイク計算画素の幅位置指定
						{
							tmpValueOfBlue += (double)(pointer_of_org[present_w + i][0]) / (K_w * K_h);
							tmpValueOfGreen += (double)(pointer_of_org[present_w + i][1]) / (K_w * K_h);
							tmpValueOfRed += (double)(pointer_of_org[present_w + i][2]) / (K_w * K_h);
						}
					}
					pointer_of_src[present_w + a][0] = (unsigned char)tmpValueOfBlue;
					pointer_of_src[present_w + a][1] = (unsigned char)tmpValueOfGreen;
					pointer_of_src[present_w + a][2] = (unsigned char)tmpValueOfRed;
				}
			}
		}
	}
}

int main()
{
	cv::Mat src = cv::imread("img/test2.png");
	cv::Mat before_filtering_image = src.clone();

	mosaic_filter(src);

	imshow("result", src);	imshow("original", before_filtering_image);
	cv::waitKey(0);

	return 0;
}