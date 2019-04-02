#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <array>

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
			src.data[i + j * width] += org.data[(i + j * width) * channels + 0] * 0.114; //B
			src.data[i + j * width] += org.data[(i + j * width) * channels + 1] * 0.587; //G
			src.data[i + j * width] += org.data[(i + j * width) * channels + 2] * 0.299; //R
		}
	}

	//１チャンネル化画像の表示
	imshow("original image", org);
	cv::waitKey(0);
	imshow("1 channeled original image (progress)", src);
	cv::waitKey(0);

}

//	//ゼロパディング（ゼロの用意）
//	cv::Mat zero_padded_src = cv::Mat::zeros(cv::Size(width + kernel_size - 1, height + kernel_size - 1), CV_8UC3);
//
//	//ゼロパディング（元画像をのせる）
//	for (int c = 0; c < channels; c++) //チャンネル指定
//	{
//		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //縦方向
//		{
//			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //横方向
//			{
//				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = src.data[((a - (kernel_size - 1) / 2) + (b - (kernel_size - 1) / 2) * width) * channels + c];
//			}
//		}
//	}
//
//	//ゼロパディング後画像の複製
//	cv::Mat zero_padded_org = zero_padded_src.clone();
//
//	//フィルタリング実行
//	for (int c = 0; c < channels; c++) //チャンネル指定
//	{
//		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //出力画素の高さ位置指定
//		{
//			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //出力画素の幅位置指定
//			{
//				//画素値計算
//				double tmpValue = 0.0;
//				for (int j = -(kernel_size - 1) / 2; j < (kernel_size + 1) / 2; j++) //ガウシアン計算画素の高さ位置指定
//				{
//					for (int i = -(kernel_size - 1) / 2; i < (kernel_size + 1) / 2; i++) //ガウシアン計算画素の幅位置指定
//					{
//						//ここから要検討						
//						tmpValue += zero_padded_org.data[((a + i) + (b + j) * zero_padded_src.cols) * channels + c] * gaussian_kernel[(i + (kernel_size - 1) / 2) + (j + (kernel_size - 1) / 2) * kernel_size];
//					}
//				}
//				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = tmpValue;
//			}
//		}
//		std::cout << "step " << c + 1 << " is done (total step is " << channels << ").\n";
//	}
//
//	//パディング部分の削除と出力ファイルへの書き出し
//	for (int c = 0; c < channels; c++) //チャンネル指定
//	{
//		for (int b = 0; b < height; b++) //縦方向
//		{
//			for (int a = 0; a < width; a++) //横方向
//			{
//				src.data[(a + b * width) * channels + c] = zero_padded_src.data[((a + (kernel_size - 1) / 2) + (b + (kernel_size - 1) / 2) * zero_padded_src.cols) * channels + c];
//			}
//		}
//	}
//}

int main()
{
	//入力と複製
	cv::Mat src = cv::imread("img/test.png");

	//フィルタ処理
	sobel_filter(src);

	//出力と比較
	imshow("result", src);
	cv::waitKey(0);

	return 0;
}