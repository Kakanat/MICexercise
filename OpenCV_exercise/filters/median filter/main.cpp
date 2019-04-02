#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

cv::Mat src = cv::imread("img/test-noise.png"); //読み込み
cv::Mat org = src.clone();

int main()
{
	int kernel_size;
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;

	int width = org.cols;
	int height = org.rows;
	int channels = org.channels();

	for (int c = 0; c < channels; c++) //チャンネル指定
	{
		for (int b = (kernel_size - 1) / 2; b < height - (kernel_size - 1) / 2; b++) //出力画素の高さ位置指定
		{
			for (int a = (kernel_size - 1) / 2; a < width - (kernel_size - 1) / 2; a++) //出力画素の幅位置指定
			{
				//フィルタ内の画素値の中央値を計算
				std::vector<uchar> element;
				for (int j = -(kernel_size - 1) / 2; j < (kernel_size + 1) / 2; j++) //モザイク計算画素の高さ位置指定
				{
					for (int i = -(kernel_size - 1) / 2; i < (kernel_size + 1) / 2; i++) //モザイク計算画素の幅位置指定
					{
						element.emplace_back(org.data[((i + a) + (j + b) * width) * channels + c]);
					}
				}
				std::sort(element.begin(), element.end());
				src.data[(a + b * width) * channels + c] = element[(kernel_size * kernel_size - 1) / 2];
			}
		}
		std::cout << "channel " << c << " is done.\n";
	}

	imshow("result", src);
	imshow("original", org);
	cv::waitKey(0);

	return 0;
}