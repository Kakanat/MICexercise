#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <algorithm>

bool compare(const std::pair<int, double>&i, const std::pair<int, double>&j)
{
	return i.second < j.second;
}

int getRowPosition(int total, int i)
{
	int step = 0;
	while (total > i)
	{
		total = total - i;
		step++;
	}
	return step;
}

void templateMatching(cv::Mat &src, cv::Mat &tmp)
{
	std::cout << "Now searching...\n";

	//複製
	cv::Mat org_src = src.clone();
	cv::Mat org_tmp = tmp.clone();

	//入力画像の１チャンネル化
	src = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8U);
	double mean_of_src = 0.0;
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

	//テンプレートの１チャンネル化
	tmp = cv::Mat::zeros(cv::Size(tmp.cols, tmp.rows), CV_8U);
	double mean_of_tmp = 0.0;
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

	std::vector<std::pair<int, double>> NCCandPositionVec; //位置と正規化相関係数のペア

	int t = 1;
	int process = 0;
	for (int b = 0; b <= src.rows - tmp.rows; b++)
	{
		
		for (int a = 0; a <= src.cols - tmp.rows; a++)
		{
			//位置ごとの正規化相互相関係数の計算
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
			NCCandPositionVec.emplace_back(a + b * (src.cols - tmp.cols + 1), normalizedCrossCorrelation);
		}
		process++;
		if (t * (src.rows - tmp.rows) - 1 < process * 10 && t * (src.rows - tmp.rows) < process * 10)
		{
			std::cout << t * 10 << "% finished...\n";
			t++;
		}
	}

	std::sort(NCCandPositionVec.begin(), NCCandPositionVec.end(), compare);
	std::pair<int, double> target_pair = NCCandPositionVec.back();
	int ansPosition = target_pair.first;
	int startPositionOfRow = getRowPosition(ansPosition, src.cols - tmp.cols + 1);
	int startPositionOfCol = target_pair.first - startPositionOfRow * (src.cols - tmp.cols + 1);

	src = org_src.clone();
	for (int j = startPositionOfRow; j < startPositionOfRow + org_tmp.rows; j++)
	{
		cv::Vec3b *pointer_of_src = src.ptr<cv::Vec3b>(j);
		for (int i = startPositionOfCol; i < startPositionOfCol + org_tmp.cols; i++)
		{
			pointer_of_src[i][0] = 255 - pointer_of_src[i][0];
			pointer_of_src[i][1] = 255 - pointer_of_src[i][1];
			pointer_of_src[i][2] = 255 - pointer_of_src[i][2];
		}
	}
}

int main(int argc, char*argv[])
{
	cv::Mat src = cv::imread(argv[1]);
	cv::Mat tmp = cv::imread(argv[2]);
	cv::Mat before_filtering_image = src.clone();
	cv::Mat tmp_org = tmp.clone();
	imshow("original", before_filtering_image);
	imshow("template", tmp_org);
	cv::waitKey(0);

	templateMatching(src, tmp);

	imshow("result", src);
	cv::waitKey(0);

	return 0;
}