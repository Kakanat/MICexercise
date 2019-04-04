#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <array>

//オーバーフローへの対処
int scaling(int x)
{
	if (x >= 255) return 255;
	else if (x <= 0) return 0;
	else return x;
}

void sobel_filter(cv::Mat &src)
{
	constexpr int kernel_size = 9;

	int width = src.cols;
	int height = src.rows;
	int channels = src.channels();

	std::array<int, kernel_size> vertical_difference_arr{ 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	std::array<int, kernel_size> horisontal_difference_arr{ 1, 2, 1, 0, 0, 0, -1, -2, -1 };

	//複製
	cv::Mat org = src.clone();

	//入力画像の１チャンネル化
	src = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			src.data[i + j * width] += (uchar)(org.data[(i + j * width) * channels + 0] * 0.114); //B
			src.data[i + j * width] += (uchar)(org.data[(i + j * width) * channels + 1] * 0.587); //G
			src.data[i + j * width] += (uchar)(org.data[(i + j * width) * channels + 2] * 0.299); //R
		}
	}

	//１チャンネル化画像の表示
	imshow("original image", org);
	cv::waitKey(0);
	imshow("1 channeled original image (progress)", src);
	cv::waitKey(0);

	//ゼロパディング画像の用意
	cv::Mat padded_src = cv::Mat::zeros(cv::Size(width + 2, height + 2), CV_8U);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			padded_src.data[(i + 1) + (j + 1) * padded_src.cols] = src.data[i + j * width];
		}
	}

	//縦方向フィルタ適用
	cv::Mat vertical_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			//画素値計算
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //フィルタ計算画素の高さ位置指定
			{
				for (int i = 0; i <= 2; i++) //フィルタ計算画素の幅位置指定
				{
					tmpValue += padded_src.data[(a + i) + (b + j) * padded_src.cols] * vertical_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			vertical_filtered.data[a + b * width] = tmpValue;
		}
	}

	//縦方向フィルタ適用画像の表示
	imshow("vertical filtered image (progress)", vertical_filtered);
	cv::waitKey(0);

	//横方向フィルタ適用
	cv::Mat horisontal_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			//画素値計算
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //フィルタ計算画素の高さ位置指定
			{
				for (int i = 0; i <= 2; i++) //フィルタ計算画素の幅位置指定
				{
					tmpValue += padded_src.data[(a + i) + (b + j) * padded_src.cols] * horisontal_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			horisontal_filtered.data[a + b * width] = tmpValue;
		}
	}

	//横方向フィルタ適用画像の表示
	imshow("horisontal filtered image (progress)", horisontal_filtered);
	cv::waitKey(0);

	//縦横方向の統合
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			int tmpValue = 0;
			tmpValue = (int)sqrt(vertical_filtered.data[a + b * width] * vertical_filtered.data[a + b * width] + horisontal_filtered.data[a + b * width] * horisontal_filtered.data[a + b * width]);
			tmpValue = scaling(tmpValue);
			src.data[a + b * width] = tmpValue;
		}
	}
}

int main()
{
	//入力と複製
	cv::Mat src = cv::imread("img/test.png");

	//フィルタ処理
	sobel_filter(src);

	//出力と比較
	cv::imshow("result", src);
	cv::waitKey(0);

	return 0;
}