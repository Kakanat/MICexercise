#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

cv::Mat src = cv::imread("img/test2.png"); //�ǂݍ���
cv::Mat org = src.clone();

int main()
{
	int kernel_size, K_h, K_w;
	std::cout << "Input the kernel size.\n";
	std::cin >> kernel_size;

	int width = org.cols;
	int height = org.rows;
	int channels = org.channels();

	for (int c = 0; c < channels; c++) //�`�����l���w��
	{
		for (int present_h = 0; present_h < height; present_h = present_h + kernel_size) //�����Ɋւ��鏈��
		{
			K_h = std::min(kernel_size, height - present_h);
			for (int present_w = 0; present_w < width; present_w = present_w + kernel_size) //���Ɋւ��鏈��
			{
				K_w = std::min(kernel_size, width - present_w);
				for (int b = 0; b < K_h; b++) //�o�͉�f�̍����ʒu�w��
				{
					for (int a = 0; a < K_w; a++) //�o�͉�f�̕��ʒu�w��
					{
						//�t�B���^���̉�f�l�̕��ς��v�Z
						double tmpValue = 0.0; //��f�l�v�Z�p�ϐ�						
						for (int j = 0; j < K_h; j++) //���U�C�N�v�Z��f�̍����ʒu�w��
						{
							for (int i = 0; i < K_w; i++) //���U�C�N�v�Z��f�̕��ʒu�w��
							{
								tmpValue += (double)org.data[((present_w + i) + (present_h + j) * width) * channels + c] / (K_w * K_h);
							}
						}
						src.data[((present_w + a) + (present_h + b) * width) * channels + c] = (unsigned char)tmpValue;
					}
				}
			}
		}
	}

	imshow("result", src);	imshow("original", org);
	cv::waitKey(0);

	return 0;
}

//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <stdio.h>
//
//void mosaicFilter(cv::Mat image)
//{
//	int kernel_size, K_h, K_w;
//	std::cout << "Input the kernel size.\n";
//	std::cin >> kernel_size;
//
//	int width = org.cols;
//	int height = org.rows;
//	int channels = org.channels();
//
//	for (int c = 0; c < channels; c++) //�`�����l���w��
//	{
//		for (int present_h = 0; present_h < height; present_h = present_h + kernel_size) //�����Ɋւ��鏈��
//		{
//			K_h = std::min(kernel_size, height - present_h);
//			for (int present_w = 0; present_w < width; present_w = present_w + kernel_size) //���Ɋւ��鏈��
//			{
//				K_w = std::min(kernel_size, width - present_w);
//				for (int b = 0; b < K_h; b++) //�o�͉�f�̍����ʒu�w��
//				{
//					for (int a = 0; a < K_w; a++) //�o�͉�f�̕��ʒu�w��
//					{
//						//�t�B���^���̉�f�l�̕��ς��v�Z
//						double tmpValue = 0.0; //��f�l�v�Z�p�ϐ�						
//						for (int j = 0; j < K_h; j++) //���U�C�N�v�Z��f�̍����ʒu�w��
//						{
//							for (int i = 0; i < K_w; i++) //���U�C�N�v�Z��f�̕��ʒu�w��
//							{
//								tmpValue += (double)org.data[((present_w + i) + (present_h + j) * width) * channels + c] / (K_w * K_h);
//							}
//						}
//						src.data[((present_w + a) + (present_h + b) * width) * channels + c] = (unsigned char)tmpValue;
//					}
//				}
//			}
//		}
//	}
//}
//
//int main()
//{
//	//�����O�摜�̓���
//	cv::Mat src = cv::imread("img/test.png");
//	cv::Mat org = src.clone();
//
//	//���U�C�N����
//	mosaicFilter(src);
//
//	//������摜�̏o��
//	imshow("result", src);	imshow("original", org);
//	cv::waitKey(0);
//
//	return 0;
//}

