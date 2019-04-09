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
		uchar *pointer_of_src = src.ptr<uchar>(j);
		cv::Vec3b *pointer_of_org = org.ptr<cv::Vec3b>(j);
		for (int i = 0; i < width; i++)
		{
			pointer_of_src[i] += (uchar)(pointer_of_org[i][0] * 0.114); //B
			pointer_of_src[i] += (uchar)(pointer_of_org[i][1] * 0.587); //G
			pointer_of_src[i] += (uchar)(pointer_of_org[i][2] * 0.299); //R
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
		uchar *pointer_of_padded_src = padded_src.ptr<uchar>(j);
		uchar *pointer_of_src = src.ptr<uchar>(j);
		for (int i = 0; i < width; i++)
		{
			pointer_of_padded_src[i + 1] = pointer_of_src[i];
		}
	}

	//縦方向フィルタ適用
	cv::Mat vertical_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		uchar *pointer_of_vertical_filtered = vertical_filtered.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			//画素値計算
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //フィルタ計算画素の高さ位置指定
			{
				uchar *pointer_of_padded_src = padded_src.ptr<uchar>(b + j);
				for (int i = 0; i <= 2; i++) //フィルタ計算画素の幅位置指定
				{
					tmpValue += pointer_of_padded_src[a + i] * vertical_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			pointer_of_vertical_filtered[a] = tmpValue;
		}
	}

	//縦方向フィルタ適用画像の表示
	imshow("vertical filtered image (progress)", vertical_filtered);
	cv::waitKey(0);

	//横方向フィルタ適用
	cv::Mat horisontal_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		uchar *pointer_of_horisontal_filtered = horisontal_filtered.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			//画素値計算
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //フィルタ計算画素の高さ位置指定
			{
				uchar *pointer_of_padded_src = padded_src.ptr<uchar>(b + j);
				for (int i = 0; i <= 2; i++) //フィルタ計算画素の幅位置指定
				{
					tmpValue += pointer_of_padded_src[a + i] * horisontal_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			pointer_of_horisontal_filtered[a] = tmpValue;
		}
	}

	//横方向フィルタ適用画像の表示
	imshow("horisontal filtered image (progress)", horisontal_filtered);
	cv::waitKey(0);

	//縦横方向の統合
	for (int b = 0; b < height; b++) //出力画素の高さ位置指定
	{
		uchar *pointer_of_vertical_filtered = vertical_filtered.ptr<uchar>(b);
		uchar *pointer_of_horisontal_filtered = horisontal_filtered.ptr<uchar>(b);
		uchar *pointer_of_src = src.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //出力画素の幅位置指定
		{
			int tmpValue = 0;
			tmpValue = (int)sqrt(pointer_of_vertical_filtered[a] * pointer_of_vertical_filtered[a] + pointer_of_horisontal_filtered[a] * pointer_of_horisontal_filtered[a]);
			tmpValue = scaling(tmpValue);
			pointer_of_src[a] = tmpValue;
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