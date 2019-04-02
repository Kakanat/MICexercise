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

	//����
	cv::Mat org = src.clone();

	//���͉摜�̂P�`�����l����
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

	//�P�`�����l�����摜�̕\��
	imshow("original image", org);
	cv::waitKey(0);
	imshow("1 channeled original image (progress)", src);
	cv::waitKey(0);

}

//	//�[���p�f�B���O�i�[���̗p�Ӂj
//	cv::Mat zero_padded_src = cv::Mat::zeros(cv::Size(width + kernel_size - 1, height + kernel_size - 1), CV_8UC3);
//
//	//�[���p�f�B���O�i���摜���̂���j
//	for (int c = 0; c < channels; c++) //�`�����l���w��
//	{
//		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //�c����
//		{
//			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //������
//			{
//				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = src.data[((a - (kernel_size - 1) / 2) + (b - (kernel_size - 1) / 2) * width) * channels + c];
//			}
//		}
//	}
//
//	//�[���p�f�B���O��摜�̕���
//	cv::Mat zero_padded_org = zero_padded_src.clone();
//
//	//�t�B���^�����O���s
//	for (int c = 0; c < channels; c++) //�`�����l���w��
//	{
//		for (int b = (kernel_size - 1) / 2; b < height + (kernel_size - 1) / 2; b++) //�o�͉�f�̍����ʒu�w��
//		{
//			for (int a = (kernel_size - 1) / 2; a < width + (kernel_size - 1) / 2; a++) //�o�͉�f�̕��ʒu�w��
//			{
//				//��f�l�v�Z
//				double tmpValue = 0.0;
//				for (int j = -(kernel_size - 1) / 2; j < (kernel_size + 1) / 2; j++) //�K�E�V�A���v�Z��f�̍����ʒu�w��
//				{
//					for (int i = -(kernel_size - 1) / 2; i < (kernel_size + 1) / 2; i++) //�K�E�V�A���v�Z��f�̕��ʒu�w��
//					{
//						//��������v����						
//						tmpValue += zero_padded_org.data[((a + i) + (b + j) * zero_padded_src.cols) * channels + c] * gaussian_kernel[(i + (kernel_size - 1) / 2) + (j + (kernel_size - 1) / 2) * kernel_size];
//					}
//				}
//				zero_padded_src.data[(a + b * zero_padded_src.cols) * channels + c] = tmpValue;
//			}
//		}
//		std::cout << "step " << c + 1 << " is done (total step is " << channels << ").\n";
//	}
//
//	//�p�f�B���O�����̍폜�Əo�̓t�@�C���ւ̏����o��
//	for (int c = 0; c < channels; c++) //�`�����l���w��
//	{
//		for (int b = 0; b < height; b++) //�c����
//		{
//			for (int a = 0; a < width; a++) //������
//			{
//				src.data[(a + b * width) * channels + c] = zero_padded_src.data[((a + (kernel_size - 1) / 2) + (b + (kernel_size - 1) / 2) * zero_padded_src.cols) * channels + c];
//			}
//		}
//	}
//}

int main()
{
	//���͂ƕ���
	cv::Mat src = cv::imread("img/test.png");

	//�t�B���^����
	sobel_filter(src);

	//�o�͂Ɣ�r
	imshow("result", src);
	cv::waitKey(0);

	return 0;
}