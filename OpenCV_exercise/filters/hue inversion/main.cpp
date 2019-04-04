#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <array>

uchar RGBrescaling(int x)
{
	if (x >= 255) return 255;
	else if (x <= 0) return 0;
	else return x;
}

std::array<int, 3> RGBtoHSVarr(int B, int G, int R)
{
	int H, S, V;
	int max_of_RGB = std::max({ R, G, B });
	int min_of_RGB = std::min({ R, G, B });

	V = max_of_RGB;

	//グレースケールの処理
	if (max_of_RGB == min_of_RGB)
	{
		S = 0; H = 0; //本来グレースケールにおいて色相は定義されない。それはできないため代わりに0としている。
	}
	//以下カラーの処理
	else if (max_of_RGB == R)
	{
		S = 255 * (max_of_RGB - min_of_RGB) / max_of_RGB;
		H = 60 * (G - B) / (max_of_RGB - min_of_RGB);
	}
	else if (max_of_RGB == G)
	{
		S = 255 * (max_of_RGB - min_of_RGB) / max_of_RGB;
		H = 60 * (B - R) / (max_of_RGB - min_of_RGB) + 120;
	}
	else if (max_of_RGB == B)
	{
		S = 255 * (max_of_RGB - min_of_RGB) / max_of_RGB;
		H = 60 * (R - G) / (max_of_RGB - min_of_RGB) + 240;
	}
	while (H < 0)	H += 360;
	while (H >= 360) H -= 360;

	std::array<int, 3> HSVarr = { H, S, V };
	return HSVarr;
}

std::array<int, 3> HSVtoBGRarr(int H, int S, int V)
{
	int R, G, B;

	if (S == 0)
	{
		R = V;
		G = V;
		B = V;
	}
	else
	{
		int H_i = (int)(H / 60.0);
		double F = H / 60.0 - H_i;
		int M = (int)(V * (1 - S / 255.0));
		int N = (int)(V * (1 - F * S / 255.0));
		int K = (int)(V * (1 - (1 - F) * S / 255.0));
		switch (H_i)
		{
		case 0:
			R = V; G = K; B = M;
			break;
		case 1:
			R = N; G = V; B = M;
			break;
		case 2:
			R = M; G = V; B = K;
			break;
		case 3:
			R = M; G = N; B = V;
			break;
		case 4:
			R = K; G = M; B = V;
			break;
		case 5:
			R = V; G = M; B = N;
			break;
		}
	}
	std::array<int, 3> BGRarr = { B, G, R };
	return BGRarr;
}

void hue_inversion(cv::Mat src)
{
	int width = src.cols;
	int height = src.rows;
	int channels = src.channels();

	for (int j = 0; j < height; j++) //変更画素の高さ指定
	{
		for (int i = 0; i < width; i++) //変更画素の幅指定
		{
			int B = src.data[(i + j * width) * channels + 0];
			int G = src.data[(i + j * width) * channels + 1];
			int R = src.data[(i + j * width) * channels + 2];
			int H = RGBtoHSVarr(B, G, R)[0];
			int S = RGBtoHSVarr(B, G, R)[1];
			int V = RGBtoHSVarr(B, G, R)[2];
			H -= 180;
			if (H < 0) H += 360;
			src.data[(i + j * width) * channels + 0] = RGBrescaling(HSVtoBGRarr(H, S, V)[0]);
			src.data[(i + j * width) * channels + 1] = RGBrescaling(HSVtoBGRarr(H, S, V)[1]);
			src.data[(i + j * width) * channels + 2] = RGBrescaling(HSVtoBGRarr(H, S, V)[2]);
		}
	}
}

int main(int argc, char*argv[])
{
	cv::Mat src = cv::imread(argv[1]);
	cv::Mat before_filtering_image = src.clone();

	hue_inversion(src);

	imshow("original", before_filtering_image);
	cv::waitKey(0);
	imshow("result", src);
	cv::waitKey(0);

	return 0;
}