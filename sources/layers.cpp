#ifndef PARAMS
#define PARAMS
#include "weights.hpp"
#endif
#include "layers.hpp"
void layer0(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	float tmp;
	float mul;

	precision_i in;
	precision_w wei;

	int i, j, d, m, n, k, batch, c_out, pads, f, tmk, stride, p1, p2, idx;

	unsigned int Idimensions[4] = {1, 3, 32, 32};
	unsigned int NewDimensions[4] = {1, 3, 32, 32};
	unsigned int Kdimensions[4] = {6, 3, 5, 5};
	unsigned int Odimensions[4] = {1, 6, 28, 28};

	stride = 1;
	pads = 0;
	precision_i tmp_input[3][5][32];

	for (i = 0; i < Odimensions[2]; i++)
	{

		if (i == 0)
		{

			for (m = 0; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{

					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (n < pads || n > NewDimensions[3] - 1 - pads || m < pads)
						{
							tmp_input[k][m][n] = 0;
						}
						else
						{
							tmp_input[k][m][n] = input0.read();
						}
					}
				}
			}
		}
		else
		{

			for (m = stride; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{
						tmp_input[k][m - stride][n] = tmp_input[k][m][n];
					}
				}
			}
			for (m = stride; m > 0; m--)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (n < pads || n > NewDimensions[3] - 1 - pads)
						{
							tmp_input[k][5 - m][n] = 0;
						}
						else
						{
							if (i == Odimensions[2] - 1 && m == pads && pads > 0)
							{
								tmp_input[k][5 - m][n] = 0;
							}
							else
							{
								tmp_input[k][5 - m][n] = input0.read();
							}
						}
					}
				}
			}
		}

		for (c_out = 0; c_out < Kdimensions[0]; c_out++)
		{

			for (j = 0; j < NewDimensions[3] - Kdimensions[3] + 1; j += stride)
			{
				tmp = 0.0;


				for (k = 0; k < Kdimensions[1] * Kdimensions[2] * Kdimensions[3]; k++)
				{

					tmk = k;
					d = int(tmk / (Kdimensions[2] * Kdimensions[3]));
					tmk = tmk - d * (Kdimensions[2] * Kdimensions[3]);
					m = int(tmk / (Kdimensions[3]));
					tmk = tmk - m * (Kdimensions[3]);
					n = tmk;

					in = tmp_input[d][m][n + j];

					idx = c_out * Kdimensions[1] * Kdimensions[2] * Kdimensions[3] + d * Kdimensions[2] * Kdimensions[3] + m * Kdimensions[3] + n;
					wei = params00[idx % 450];

					mul = in * wei;
					tmp = tmp + mul;
				}

				tmp += bias0[c_out];
				if (tmp < 0)
				{
					output0.write(0);
				}
				else
				{
					output0.write(tmp);
				}
			}
		}
	}
}

void layer1(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	unsigned int Idimensions[4] = {1, 6, 28, 28};
	unsigned int NewDimensions[4] = {1, 6, 28, 28};
	unsigned int Kdimensions[4] = {1, 1, 2, 2};
	unsigned int Odimensions[4] = {1, 6, 14, 14};

	unsigned int pads = 0;
	unsigned int stride = 2;
	unsigned int c_out, k, i, j, d, m, n, tmk, ligne;
	precision_i in;
	precision_i tmp, max;
	precision_i tmp_input[6][2][28];

	max = 0;

	ligne = 0;
	for (i = 0; i < Odimensions[2]; i++)
	{

		if (i == 0)
		{

			for (m = pads; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{

					for (n = 0; n < NewDimensions[3]; n++)
					{

						tmp_input[k][m][n] = input0.read();
					}
				}
				ligne++;
			}
		}
		else
		{

			for (m = stride; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						tmp_input[k][m - stride][n] = tmp_input[k][m][n];
					}
				}
			}
			for (m = stride; m > 0; m--)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (ligne >= NewDimensions[2])
						{
							tmp_input[k][2 - m][n] = 0;
						}
						else
						{
							tmp_input[k][2 - m][n] = input0.read();
						}
					}
				}
				ligne++;
			}
		}

		for (c_out = 0; c_out < NewDimensions[1]; c_out++)
		{

			for (j = 0; j < Odimensions[2]; j++)
			{
				max = 0;



				for (k = 0; k < Kdimensions[1] * Kdimensions[2] * Kdimensions[3]; k++)
				{

					tmk = k;
					m = int(tmk / (Kdimensions[3]));
					tmk = tmk - m * (Kdimensions[3]);
					n = tmk;

					if (NewDimensions[3] - 1 - n - j * stride < NewDimensions[3])
					{

						in = tmp_input[c_out][m][n + j * stride];
					}
					if (in > max)
					{
						max = in;
					}
				}

				output0.write(max);
			}
		}
	}
}

void layer2(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	float tmp;
	float mul;

	precision_i in;
	precision_w wei;

	int i, j, d, m, n, k, batch, c_out, pads, f, tmk, stride, p1, p2, idx;

	unsigned int Idimensions[4] = {1, 6, 14, 14};
	unsigned int NewDimensions[4] = {1, 6, 14, 14};
	unsigned int Kdimensions[4] = {16, 6, 5, 5};
	unsigned int Odimensions[4] = {1, 16, 10, 10};

	stride = 1;
	pads = 0;
	precision_i tmp_input[6][5][14];

	for (i = 0; i < Odimensions[2]; i++)
	{

		if (i == 0)
		{

			for (m = 0; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{

					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (n < pads || n > NewDimensions[3] - 1 - pads || m < pads)
						{
							tmp_input[k][m][n] = 0;
						}
						else
						{
							tmp_input[k][m][n] = input0.read();
						}
					}
				}
			}
		}
		else
		{

			for (m = stride; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{
						tmp_input[k][m - stride][n] = tmp_input[k][m][n];
					}
				}
			}
			for (m = stride; m > 0; m--)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (n < pads || n > NewDimensions[3] - 1 - pads)
						{
							tmp_input[k][5 - m][n] = 0;
						}
						else
						{
							if (i == Odimensions[2] - 1 && m == pads && pads > 0)
							{
								tmp_input[k][5 - m][n] = 0;
							}
							else
							{
								tmp_input[k][5 - m][n] = input0.read();
							}
						}
					}
				}
			}
		}

		for (c_out = 0; c_out < Kdimensions[0]; c_out++)
		{

			for (j = 0; j < NewDimensions[3] - Kdimensions[3] + 1; j += stride)
			{
				tmp = 0.0;


				for (k = 0; k < Kdimensions[1] * Kdimensions[2] * Kdimensions[3]; k++)
				{

					tmk = k;
					d = int(tmk / (Kdimensions[2] * Kdimensions[3]));
					tmk = tmk - d * (Kdimensions[2] * Kdimensions[3]);
					m = int(tmk / (Kdimensions[3]));
					tmk = tmk - m * (Kdimensions[3]);
					n = tmk;

					in = tmp_input[d][m][n + j];

					idx = c_out * Kdimensions[1] * Kdimensions[2] * Kdimensions[3] + d * Kdimensions[2] * Kdimensions[3] + m * Kdimensions[3] + n;
					wei = params02[idx];

					mul = in * wei;

					tmp = tmp + mul;
				}

				tmp += bias2[c_out];

				if (tmp < 0)
				{
					output0.write(0);
				}
				else
				{
					output0.write(tmp);
				}
			}
		}
	}
}

void layer3(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	unsigned int Idimensions[4] = {1, 16, 10, 10};
	unsigned int NewDimensions[4] = {1, 16, 10, 10};
	unsigned int Kdimensions[4] = {1, 1, 2, 2};
	unsigned int Odimensions[4] = {1, 16, 5, 5};

	int pads = 0;
	int stride = 2;
	unsigned int c_out, k, i, j, d, m, n, tmk, ligne;
	precision_i in;
	precision_i tmp, max;
	precision_i tmp_input[16][2][10];

	max = 0;

	ligne = 0;
	for (i = 0; i < Odimensions[2]; i++)
	{

		if (i == 0)
		{

			for (m = pads; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{

					for (n = 0; n < NewDimensions[3]; n++)
					{

						tmp_input[k][m][n] = input0.read();
					}
				}
				ligne++;
			}
		}
		else
		{

			for (m = stride; m < Kdimensions[2]; m++)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						tmp_input[k][m - stride][n] = tmp_input[k][m][n];
					}
				}
			}
			for (m = stride; m > 0; m--)
			{

				for (k = 0; k < NewDimensions[1]; k++)
				{
					for (n = 0; n < NewDimensions[3]; n++)
					{

						if (ligne >= NewDimensions[2])
						{
							tmp_input[k][2 - m][n] = 0;
						}
						else
						{
							tmp_input[k][2 - m][n] = input0.read();
						}
					}
				}
				ligne++;
			}
		}

		for (c_out = 0; c_out < NewDimensions[1]; c_out++)
		{

			for (j = 0; j < Odimensions[2]; j++)
			{
				max = 0;

				for (k = 0; k < Kdimensions[1] * Kdimensions[2] * Kdimensions[3]; k++)
				{

					tmk = k;
					m = int(tmk / (Kdimensions[3]));
					tmk = tmk - m * (Kdimensions[3]);
					n = tmk;

					if (NewDimensions[3] - 1 - n - j * stride < NewDimensions[3])
					{

						in = tmp_input[c_out][m][n + j * stride];
					}
					if (in > max)
					{
						max = in;
					}
				}

				output0.write(max);
			}
		}
	}
}

void layer4(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	float mul, acc;
	precision_w weightV;

	unsigned int Idimensions[2] = {1, 400};
	unsigned int Kdimensions[2] = {120, 400};

	precision_w wei;
	int idx;
	precision_i features[400] = {0};

	for (int i = 0; i < 5; i++)
	{
		for (int c_out = 0; c_out < 16; c_out++)
		{
			for (int j = 0; j < 5; j++)
			{
				int k = c_out * 25 + i * 5 + j;
				features[k] = input0.read();
			}
		}
	}
	for (int i = 0; i < Kdimensions[0]; i++)
	{
		acc = 0;

		for (int j = 0; j < Kdimensions[1]; j++)
		{
			idx = i * Kdimensions[1] + j;

			if (0 == (int)idx / 16384)
			{
				wei = params04[idx % 16384];
			}
			if (1 == (int)idx / 16384)
			{
				wei = params14[idx % 16384];
			}
			if (2 == (int)idx / 16384)
			{
				wei = params24[idx % 16384];
			}

			acc += features[j] * wei;
		}
		acc += bias4[i];

		if (acc < 0)
		{
			output0.write(0);
		}
		else
		{
			output0.write(acc);
		}
	}
}

void layer5(AXIstreamIN &input0, AXIstreamOUT &output0)
{

	float mul, acc;
	precision_w weightV;

	unsigned int Idimensions[2] = {1, 120};
	unsigned int Kdimensions[2] = {84, 120};

	precision_w wei;
	int idx;
	precision_i features[120] = {0};

	for (int k = 0; k < Idimensions[1]; k++)
	{
		features[k] = input0.read();
	}
	for (int i = 0; i < Kdimensions[0]; i++)
	{
		acc = 0.0;


		for (int j = 0; j < Kdimensions[1]; j++)
		{
			idx = i * Kdimensions[1] + j;
			wei = params05[idx % 10080];

			mul = features[j] * wei;

			acc = acc + mul;
		}
		acc += bias5[i];

		if (acc < 0)
		{
			output0.write(0);
		}
		else
		{
			output0.write(acc);
		}
	}
}

void layer6(AXIstreamIN &input0, AXIstreamFINAL &output0)
{

	float mul, acc;
	precision_w weightV;
	unsigned int Idimensions[2] = {1, 84};
	unsigned int Kdimensions[2] = {10, 84};

	precision_w wei;
	int idx;
	precision_i features[84] = {0};

	for (int k = 0; k < Idimensions[1]; k++)
	{
		features[k] = input0.read();
	}

	for (int i = 0; i < Kdimensions[0]; i++)
	{
		acc = 0.0;
		for (int j = 0; j < Kdimensions[1]; j++)
		{
			idx = i * Kdimensions[1] + j;

			wei = params06[idx % 840];

			mul = features[j] * wei;

			acc = acc + mul;
		}
		acc += bias6[i];

		AXIS_DATA data;
		data.data = *(ap_uint<32> *)&acc;
		data.keep = -1;

		if (i != 9)
		{
			data.last = 0;
		}
		else
		{
			data.last = 1;
		}
		output0 << data;
	}
}