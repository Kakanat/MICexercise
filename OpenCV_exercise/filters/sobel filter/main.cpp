#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <array>

//�I�[�o�[�t���[�ւ̑Ώ�
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

	//����
	cv::Mat org = src.clone();

	//���͉摜�̂P�`�����l����
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

	//�P�`�����l�����摜�̕\��
	imshow("original image", org);
	cv::waitKey(0);
	imshow("1 channeled original image (progress)", src);
	cv::waitKey(0);

	//�[���p�f�B���O�摜�̗p��
	cv::Mat padded_src = cv::Mat::zeros(cv::Size(width + 2, height + 2), CV_8U);
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			padded_src.data[(i + 1) + (j + 1) * padded_src.cols] = src.data[i + j * width];
		}
	}

	//�c�����t�B���^�K�p
	cv::Mat vertical_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
		{
			//��f�l�v�Z
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //�t�B���^�v�Z��f�̍����ʒu�w��
			{
				for (int i = 0; i <= 2; i++) //�t�B���^�v�Z��f�̕��ʒu�w��
				{
					tmpValue += padded_src.data[(a + i) + (b + j) * padded_src.cols] * vertical_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			vertical_filtered.data[a + b * width] = tmpValue;
		}
	}

	//�c�����t�B���^�K�p�摜�̕\��
	imshow("vertical filtered image (progress)", vertical_filtered);
	cv::waitKey(0);

	//�������t�B���^�K�p
	cv::Mat horisontal_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
		{
			//��f�l�v�Z
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //�t�B���^�v�Z��f�̍����ʒu�w��
			{
				for (int i = 0; i <= 2; i++) //�t�B���^�v�Z��f�̕��ʒu�w��
				{
					tmpValue += padded_src.data[(a + i) + (b + j) * padded_src.cols] * horisontal_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			horisontal_filtered.data[a + b * width] = tmpValue;
		}
	}

	//�������t�B���^�K�p�摜�̕\��
	imshow("horisontal filtered image (progress)", horisontal_filtered);
	cv::waitKey(0);

	//�c�������̓���
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
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
	//���͂ƕ���
	cv::Mat src = cv::imread("img/test.png");

	//�t�B���^����
	sobel_filter(src);

	//�o�͂Ɣ�r
	cv::imshow("result", src);
	cv::waitKey(0);

	return 0;
}