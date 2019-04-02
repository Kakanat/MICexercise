#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>

//正規化ガウス関数の用意
std::vector<double> gaussian_value_vec(double sigma, int kernel_size)
{
	std::vector<double> gaussian_value_vec;
	double total_gaussian_value = 0.0;
	for (int y = -(kernel_size - 1) / 2; y < (kernel_size + 1) / 2; y++)
	{
		for (int x = -(kernel_size - 1) / 2; x < (kernel_size + 1) / 2; x++)
		{
			gaussian_value_vec.emplace_back(exp(-(x * x + y * y) / (2 * sigma * sigma)));
			total_gaussian_value += exp(-(x * x + y * y) / (2 * sigma * sigma));
		}
	}
	for (int index_of_gaussian_value_vec = 0; index_of_gaussian_value_vec < kernel_size * kernel_size; index_of_gaussian_value_vec++)
	{
		gaussian_value_vec[index_of_gaussian_value_vec] = gaussian_value_vec[index_of_gaussian_value_vec] / total_gaussian_value;
	}

	return gaussian_value_vec;
}


void gaussian_filter(cv::Mat &src)
{
	int kernel_size;
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

	int width = src.cols;
	int height = src.rows;
	int channels = src.channels();
	std::vector<double> gaussian_kernel = gaussian_value_vec(variance, kernel_size);
	   
	//ゼロパディング（ゼロの用意）
	cv::Mat zero_padded_src = cv::Mat::zeros(cv::Size(width + kernel_size - 1, height + kernel_size - 1), CV_8UC3);

	//ゼロパディング（元画像をのせる）
	for (int c = 0; c < channels; c++) //チャンネル指定
	{
		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //縦方向
		{
			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //横方向
			{
				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = src.data[((a - (kernel_size - 1) / 2) + (b - (kernel_size - 1) / 2) * width) * channels + c];
			}
		}	
	}

	//ゼロパディング後画像の複製
	cv::Mat zero_padded_org = zero_padded_src.clone();
	
	//フィルタリング実行
	for (int c = 0; c < channels; c++) //チャンネル指定
	{
		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //出力画素の高さ位置指定
		{
			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //出力画素の幅位置指定
			{
				//画素値計算
				double tmpValue = 0.0;
				for (int j = -(kernel_size - 1) / 2; j < (kernel_size + 1) / 2; j++) //ガウシアン計算画素の高さ位置指定
				{
					for (int i = -(kernel_size - 1) / 2; i < (kernel_size + 1) / 2; i++) //ガウシアン計算画素の幅位置指定
					{
						//ここから要検討						
						tmpValue += zero_padded_org.data[((a + i) + (b + j) * zero_padded_src.cols) * channels + c] * gaussian_kernel[(i + (kernel_size - 1) / 2) + (j + (kernel_size - 1) / 2) * kernel_size];
					}
				}
				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = tmpValue;
			}
		}
		std::cout << "step " << c + 1 << " is done (total step is " << channels << ").\n";
	}

	//パディング部分の削除と出力ファイルへの書き出し
	for (int c = 0; c < channels; c++) //チャンネル指定
	{
		for (int b = 0; b < height; b++) //縦方向
		{
			for (int a = 0; a < width; a++) //横方向
			{
				src.data[(a + b * width) * channels + c] = zero_padded_src.data[((a + (kernel_size - 1) / 2) + (b + (kernel_size - 1) / 2) * zero_padded_src.cols) * channels + c];
			}
		}
	}	
}

int main()
{
	//入力と複製
	cv::Mat src = cv::imread("img/test.png");
	cv::Mat before_filtering_image = src.clone();

	//フィルタ処理
	gaussian_filter(src);

	//出力と比較
	imshow("result", src);
	imshow("original", before_filtering_image);
	cv::waitKey(0);

	return 0;
}