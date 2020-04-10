#include "graphics.hpp"

#include <iostream>
#include <cmath>

void gaussianBlur(uint8_t* data, int w, int h, uint stride, float r)
{
	// smooth core
	int smooth = r;
	float k = r * 0.8355427582103334; // c * sqrt(2 * pi) / 3
	r = 2 * r * r / 9;
	float weight[smooth + 1];
	float weightSum = 0;

	for (int i = 0; i <= smooth; i++)
	{
		weight[i] = exp(-i * i / r) / k;
		if (i != 0) weightSum += weight[i];
	}
	weightSum = weightSum * 2 + weight[0];

	for (int i = 0; i <= smooth; i++) // Нормализация
		weight[i] /= weightSum;

	// Создание дополнительной архитектуры
	float temp[smooth * 3];
	int temp_i = 0;

	// Вертикальный проход
	for (int x = 0; x < w; x++)
	{
		uint8_t* wdata = data + x * 4;

		for (int y = 0; y < h + smooth; y++)
		{
			// Рассчет цвета с координатами (x; y - smooth)

			temp_i = (temp_i + 1) % smooth;
			float tnR = 0, toR = temp[temp_i * 3 + 0]; // new, old
			float tnG = 0, toG = temp[temp_i * 3 + 1]; // new, old
			float tnB = 0, toB = temp[temp_i * 3 + 2]; // new, old

			// analize [y - smooth + 1; y]
			for (int i = 0; i <= smooth; i++)
			{
				// Учет точки (x; y - i)
				uint8_t* pix = wdata + stride * (y - i);
				if (y - i <= 0) pix = wdata;
				if (y - i >= h) pix = wdata + stride * (h - 1);

				if (i < smooth)
				{
					float& w = weight[smooth - i];
					toR += pix[0] * w; toG += pix[1] * w; toB += pix[2] * w;
				}
				float& w = weight[i];
				tnR += pix[0] * w; tnG += pix[1] * w; tnB += pix[2] * w;
			}

			temp[temp_i * 3 + 0] = tnR;
			temp[temp_i * 3 + 1] = tnG;
			temp[temp_i * 3 + 2] = tnB;

			if (y - smooth < 0) continue;

			uint8_t* pix = wdata + stride * (y - smooth);
			pix[0] = toR; pix[1] = toG; pix[2] = toB;
		}
	}

	// Горизонтальный проход
	for (int y = 0; y < h; y++)
	{
		uint8_t* wdata = data + y * stride;

		for (int x = 0; x < w + smooth; x++)
		{
			// Рассчет цвета с координатами (x - smooth; y)

			temp_i = (temp_i + 1) % smooth;
			float tnR = 0, toR = temp[temp_i * 3 + 0]; // new, old
			float tnG = 0, toG = temp[temp_i * 3 + 1]; // new, old
			float tnB = 0, toB = temp[temp_i * 3 + 2]; // new, old

			// analize [x - smooth + 1; x]
			for (int i = 0; i <= smooth; i++)
			{
				// Учет точки (x - i; y)
				uint8_t* pix = wdata + 4 * (x - i);
				if (x - i <= 0) pix = wdata;
				if (x - i >= w) pix = wdata + 4 * (w - 1);

				if (i < smooth)
				{
					float& w = weight[smooth - i];
					toR += pix[0] * w; toG += pix[1] * w; toB += pix[2] * w;
				}
				float& w = weight[i];
				tnR += pix[0] * w; tnG += pix[1] * w; tnB += pix[2] * w;
			}

			temp[temp_i * 3 + 0] = tnR;
			temp[temp_i * 3 + 1] = tnG;
			temp[temp_i * 3 + 2] = tnB;

			if (x - smooth < 0) continue;

			uint8_t* pix = wdata + 4 * (x - smooth);
			pix[0] = toR; pix[1] = toG; pix[2] = toB;
		}
	}
}
//
//void gaussianBlur2(uint8_t* data, int w, int h, uint stride, float r)
//{
//	// smooth core
//	int smooth = r + 1;
//	float k = r * 0.8355427582103334; // c * sqrt(2 * pi) / 3
//	r = 2 * r * r / 9;
//	float weight[smooth];
//
//	for (int i = 0; i < smooth; i++)
//		weight[i] = exp(-i * i / r) / k;
//
//	uint8_t temp[h * 3];
//
//	// Вертикальный проход
//	for (int x = 0; x < w; x++)
//	{
//		uint8_t* wdata = data + x * 4;
//
//		for (int y = 0; y < h; y++)
//		{
//			// Рассчет цвета с координатами (x; y)
//
//			float tnR = 0;
//			float tnG = 0;
//			float tnB = 0;
//
//			for (int i = -smooth+1; i < smooth; i++)
//			{
//				// Учет точки (x; y - i)
//				uint8_t* pix = wdata + stride * (y + i);
//				if (y + i < 0) pix = wdata;
//				if (y + i >= h) pix = wdata + stride * (h - 1);
//
//				float& w = weight[abs(i)];
//				tnR += pix[0] * w; tnG += pix[1] * w; tnB += pix[2] * w;
//			}
//
//			uint8_t* out = temp + y * 3;
//			out[0] = tnR; out[1] = tnG; out[2] = tnB;
//		}
//
//		for (int y = 0; y < h; y++)
//		{
//			uint8_t* in = temp + y * 3;
//			uint8_t* pix = wdata + stride * y;
//			pix[0] = in[0]; pix[1] = in[1]; pix[2] = in[2];
//		}
//	}
//}
