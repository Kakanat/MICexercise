#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>

void gaussian_filter()
{
	int kernel_size, horizontal_distance, vertical_distance;
	double variance;

	//カーネルサイズの入力
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;
	while (kernel_size % 2 == 0)
	{
		std::cout << "Enter an odd number for the kernel size.\n";
		std::cin >> kernel_size;
	}

	//分散の入力
	std::cout << "Input the variable size.\n";
	std::cin >> variance;
	while (variance <= 0)
	{
		std::cout << "Enter a postive number for the variance.\n";
		std::cin >> variance;
	}

	//画素ごとの非正規化ガウス関数の計算
	std::vector<uchar> gaussian_vector;
	double gaussian = exp(-(horizontal_distance * horizontal_distance + vertical_distance * vertical_distance) / (2 * variance * variance));


}

int main()
{
	//読み込みと複製
	cv::Mat src = cv::imread("img/test.png");
	cv::Mat org = src.clone();

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