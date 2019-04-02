#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>

cv::Mat src = cv::imread("img/test-noise.png"); //�ǂݍ���
cv::Mat org = src.clone();

int main()
{
	int kernel_size;
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;

	int width = org.cols;
	int height = org.rows;
	int channels = org.channels();

	for (int c = 0; c < channels; c++) //�`�����l���w��
	{
		for (int b = (kernel_size - 1) / 2; b < height - (kernel_size - 1) / 2; b++) //�o�͉�f�̍����ʒu�w��
		{
			for (int a = (kernel_size - 1) / 2; a < width - (kernel_size - 1) / 2; a++) //�o�͉�f�̕��ʒu�w��
			{
				//�t�B���^���̉�f�l�̒����l���v�Z
				std::vector<uchar> element;
				for (int j = -(kernel_size - 1) / 2; j < (kernel_size + 1) / 2; j++) //���U�C�N�v�Z��f�̍����ʒu�w��
				{
					for (int i = -(kernel_size - 1) / 2; i < (kernel_size + 1) / 2; i++) //���U�C�N�v�Z��f�̕��ʒu�w��
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