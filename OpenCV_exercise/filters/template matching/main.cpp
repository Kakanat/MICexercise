#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>

void templateMatching(cv::Mat src, cv::Mat tmp)
{
	//����
	cv::Mat org_src = src.clone();
	cv::Mat org_tmp = tmp.clone();

	double mean_of_src = 0.0;
	double mean_of_tmp = 0.0;

	//���͉摜�̂P�`�����l����
	src = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8U);
	for (int j = 0; j < src.rows; j++)
	{
		uchar *pointer_of_src = src.ptr<uchar>(j);
		cv::Vec3b *pointer_of_org_src = org_src.ptr<cv::Vec3b>(j);
		for (int i = 0; i < src.cols; i++)
		{
			pointer_of_src[i] += (uchar)(pointer_of_org_src[i][0] * 0.114); //B
			pointer_of_src[i] += (uchar)(pointer_of_org_src[i][1] * 0.587); //G
			pointer_of_src[i] += (uchar)(pointer_of_org_src[i][2] * 0.299); //R
			mean_of_src += pointer_of_src[i] / (src.cols * src.rows);
		}
	}
	
	//�e���v���[�g�̂P�`�����l����
	tmp = cv::Mat::zeros(cv::Size(tmp.cols, tmp.rows), CV_8U);
	for (int j = 0; j < tmp.rows; j++)
	{
		uchar *pointer_of_tmp = tmp.ptr<uchar>(j);
		cv::Vec3b *pointer_of_org_tmp = org_tmp.ptr<cv::Vec3b>(j);
		for (int i = 0; i < tmp.cols; i++)
		{
			pointer_of_tmp[i] += (uchar)(pointer_of_org_tmp[i][0] * 0.114); //B
			pointer_of_tmp[i] += (uchar)(pointer_of_org_tmp[i][1] * 0.587); //G
			pointer_of_tmp[i] += (uchar)(pointer_of_org_tmp[i][2] * 0.299); //R
			mean_of_tmp += pointer_of_tmp[i] / (tmp.cols * tmp.rows);
		}
	}

	std::vector<double> normalizedCrossCorrelationVec; //�ړI�ʒu�̌����Ɏg�p����x�N�^�[
	std::vector<double> positionVec; //�ړI�ʒu�̌����Ɏg�p����x�N�^�[
	std::vector<std::pair<int, double>> NCCandPositionVec;

	for (int b = 0; b <= src.rows - tmp.rows; b++) //�ύX��f�̍����w��
	{
		for (int a = 0; a <= src.cols - tmp.rows; a++) //�ύX��f�̕��w��
		{
			//���K�����ݑ��֌W���̌v�Z
			double numerator = 0.0;
			double denominator_src = 0.0;
			double denominator_tmp = 0.0;
			for (int j = 0; j < tmp.rows; j++)
			{
				uchar *pointer_of_src = src.ptr<uchar>(b + j);
				uchar *pointer_of_tmp = tmp.ptr<uchar>(j);
				for (int i = 0; i < tmp.cols; i++)
				{
					numerator += (pointer_of_src[a + i] - mean_of_src) * (pointer_of_tmp[i] - mean_of_tmp);
					denominator_src += (pointer_of_src[a + i] - mean_of_src) * (pointer_of_src[a + i] - mean_of_src);
					denominator_tmp += (pointer_of_tmp[i] - mean_of_tmp) * (pointer_of_tmp[i] - mean_of_tmp);
				}
			}
			double normalizedCrossCorrelation = numerator / (sqrt(denominator_src) * sqrt(denominator_tmp));
			NCCandPositionVec.emplace_back(a + b * src.cols, normalizedCrossCorrelation);
		}
	}

}

int main(int argc1, char*argv1[], int argc2, char*argv2[])
{
	cv::Mat src = cv::imread(argv1[1]);
	cv::Mat tmp = cv::imread(argv2[1]);
	cv::Mat before_filtering_image = src.clone();

	templateMatching(src, tmp);

	imshow("original", before_filtering_image);
	cv::waitKey(0);
	imshow("result", src);
	cv::waitKey(0);

	return 0;
}