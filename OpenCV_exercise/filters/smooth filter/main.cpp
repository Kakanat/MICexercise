#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

cv::Mat src = cv::imread("img/test.png"); //�ǂݍ���
cv::Mat org = src.clone();

int main()
{
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
		for (int b = 0; b < height; b++) //�t�B���^�̓K�p�Ώۉ�f�i�����w��j
		{
			int start_h = std::max(0, b - (kernel_size - 1) / 2); //�v�Z�J�n��f�i�����j
			int end_h = std::min(height - 1, b + (kernel_size - 1) / 2); //�v�Z�I����f�i�����j
			for (int a = 0; a < width; a++) //�t�B���^�̓K�p�Ώۉ�f�i���w��j
			{
				int start_w = std::max(0, a - (kernel_size - 1) / 2); //�v�Z�J�n��f�i���j
				int end_w = std::min(width - 1, a + (kernel_size - 1) / 2); //�v�Z�I����f�i���j
				double tmpValue = 0.0;
				int step = 0; //�t�B���^����f��
				for (int j = start_h; j <= end_h; j++)
				{
					for (int i = start_w; i <= end_w; i++)
					{
						tmpValue += (double)org.data[(i + j * width) * channels + c];
						step++;
					}
				}
				src.data[(a + b * width) * channels + c] = (unsigned char)(tmpValue / step);
			}
		}
	}

	imshow("result", src);
	imshow("original", org);
	cv::waitKey(0);

	return 0;
}