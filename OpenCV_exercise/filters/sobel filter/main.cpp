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
		uchar *pointer_of_src = src.ptr<uchar>(j);
		cv::Vec3b *pointer_of_org = org.ptr<cv::Vec3b>(j);
		for (int i = 0; i < width; i++)
		{
			pointer_of_src[i] += (uchar)(pointer_of_org[i][0] * 0.114); //B
			pointer_of_src[i] += (uchar)(pointer_of_org[i][1] * 0.587); //G
			pointer_of_src[i] += (uchar)(pointer_of_org[i][2] * 0.299); //R
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
		uchar *pointer_of_padded_src = padded_src.ptr<uchar>(j);
		uchar *pointer_of_src = src.ptr<uchar>(j);
		for (int i = 0; i < width; i++)
		{
			pointer_of_padded_src[i + 1] = pointer_of_src[i];
		}
	}

	//�c�����t�B���^�K�p
	cv::Mat vertical_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		uchar *pointer_of_vertical_filtered = vertical_filtered.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
		{
			//��f�l�v�Z
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //�t�B���^�v�Z��f�̍����ʒu�w��
			{
				uchar *pointer_of_padded_src = padded_src.ptr<uchar>(b + j);
				for (int i = 0; i <= 2; i++) //�t�B���^�v�Z��f�̕��ʒu�w��
				{
					tmpValue += pointer_of_padded_src[a + i] * vertical_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			pointer_of_vertical_filtered[a] = tmpValue;
		}
	}

	//�c�����t�B���^�K�p�摜�̕\��
	imshow("vertical filtered image (progress)", vertical_filtered);
	cv::waitKey(0);

	//�������t�B���^�K�p
	cv::Mat horisontal_filtered = cv::Mat::zeros(cv::Size(width, height), CV_8U);
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		uchar *pointer_of_horisontal_filtered = horisontal_filtered.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
		{
			//��f�l�v�Z
			int tmpValue = 0;
			for (int j = 0; j <= 2; j++) //�t�B���^�v�Z��f�̍����ʒu�w��
			{
				uchar *pointer_of_padded_src = padded_src.ptr<uchar>(b + j);
				for (int i = 0; i <= 2; i++) //�t�B���^�v�Z��f�̕��ʒu�w��
				{
					tmpValue += pointer_of_padded_src[a + i] * horisontal_difference_arr[i + j * 3];
				}
			}
			tmpValue = scaling(tmpValue);
			pointer_of_horisontal_filtered[a] = tmpValue;
		}
	}

	//�������t�B���^�K�p�摜�̕\��
	imshow("horisontal filtered image (progress)", horisontal_filtered);
	cv::waitKey(0);

	//�c�������̓���
	for (int b = 0; b < height; b++) //�o�͉�f�̍����ʒu�w��
	{
		uchar *pointer_of_vertical_filtered = vertical_filtered.ptr<uchar>(b);
		uchar *pointer_of_horisontal_filtered = horisontal_filtered.ptr<uchar>(b);
		uchar *pointer_of_src = src.ptr<uchar>(b);
		for (int a = 0; a < width; a++) //�o�͉�f�̕��ʒu�w��
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
	//���͂ƕ���
	cv::Mat src = cv::imread("img/test.png");

	//�t�B���^����
	sobel_filter(src);

	//�o�͂Ɣ�r
	cv::imshow("result", src);
	cv::waitKey(0);

	return 0;
}