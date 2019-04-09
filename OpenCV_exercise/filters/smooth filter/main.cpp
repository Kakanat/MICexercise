#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

void smooth_filter(cv::Mat &src)
{
	cv::Mat org = src.clone();

	int kernel_size;
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;
	while (kernel_size % 2 == 0)
	{
		std::cout << "Please enter an odd number.\n";
		std::cin >> kernel_size;
	}

	int width = org.cols;
	int height = org.rows;
	int channels = org.channels();

	for (int c = 0; c < channels; c++)
	{
		for (int b = 0; b < height; b++) //フィルタの適用対象画素（高さ指定）
		{
			int start_h = std::max(0, b - (kernel_size - 1) / 2); //計算開始画素（高さ）
			int end_h = std::min(height - 1, b + (kernel_size - 1) / 2); //計算終了画素（高さ）
			cv::Vec3b *pointer_of_src = src.ptr<cv::Vec3b>(b);
			for (int a = 0; a < width; a++) //フィルタの適用対象画素（幅指定）
			{
				int start_w = std::max(0, a - (kernel_size - 1) / 2); //計算開始画素（幅）
				int end_w = std::min(width - 1, a + (kernel_size - 1) / 2); //計算終了画素（幅）
				double tmpValue = 0.0;
				int step = 0; //フィルタ内画素数
				for (int j = start_h; j <= end_h; j++)
				{
					cv::Vec3b *pointer_of_org = org.ptr<cv::Vec3b>(j);
					for (int i = start_w; i <= end_w; i++)
					{
						tmpValue += (double)pointer_of_org[i][c];
						step++;
					}
				}
				pointer_of_src[a][c] = (unsigned char)(tmpValue / step);
			}
		}
	}
}

int main()
{
	cv::Mat src = cv::imread("img/test.png");
	cv::Mat before_filtering_image = src.clone();

	smooth_filter(src);

	imshow("result", src);	imshow("original", before_filtering_image);
	cv::waitKey(0);

	return 0;
}