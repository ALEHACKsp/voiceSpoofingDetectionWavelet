/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 2 de abr de 2020
 *
 */

#include <cmath>
#include <stdexcept>
#include "../linearAlgebra/linearAlgebra.h"

double createAlpha(double samplingRate, double filterMaxFrequency, bool highPass = false)
{

	double alpha = M_PI * filterMaxFrequency / (samplingRate / 2);

	if (highPass)
	{
		return M_PI - alpha;
	}

	return alpha;
}

double* createLowPassFilter(int order, double samplingRate, double filterMaxFrequency)
{

	// Order MUST be odd
	if (order % 2 == 0)
	{
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *filter = new double[order + 1];

	//Calculating the alpha
	double alpha = createAlpha(samplingRate, filterMaxFrequency);

	double halfOrderSize = (double) (order / 2.0);

	for (int n = 0; n <= order; ++n)
	{

		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	linearAlgebra::normalizeVector(filter, order + 1);

	return filter;
}

double* createHighPassFilter(int order, double samplingRate, double filterStartFrequency)
{

	// Order MUST be odd
	if (order % 2 == 0)
	{
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	// Filter holder
	double *filter = new double[order + 1];

	//Calculating the alpha for high pass filter
	double alpha = createAlpha(samplingRate, filterStartFrequency, true);

	double halfOrderSize = (double) (order / 2.0);

	// Calculate low pass filter
	for (int n = 0; n <= order; ++n)
	{
		filter[n] = sin(alpha * (n - halfOrderSize)) / (M_PI * (n - halfOrderSize));
	}

	// normalizing data
	linearAlgebra::normalizeVector(filter, order + 1);

	// Builds the orthogonal vector
	// and return the final result (high pass filter)
	return linearAlgebra::calcOrthogonalVector(filter, order + 1);
}

double* createStopBandFilter(int order, double samplingRate, double startFrequency, double finalFrequency)
{

	// Order MUST be odd
	if (order % 2 == 0)
	{
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *lowPassMax = createLowPassFilter(order, samplingRate, finalFrequency);
	double *lowPassMin = createLowPassFilter(order, samplingRate, startFrequency);

	linearAlgebra::normalizeVector(lowPassMax, order + 1);
	linearAlgebra::normalizeVector(lowPassMin, order + 1);

	for (int i = 0; i < order + 1; i++)
	{
		lowPassMax[i] = lowPassMax[i] - lowPassMin[i];
	}

	delete[] lowPassMin;

	return lowPassMax;
}

double* bandStopFilter(int order, double samplingRate, double startFrequency, double finalFrequency)
{

	// Order MUST be odd
	if (order % 2 == 0)
	{
		throw std::runtime_error("Order MUST be an odd number!");
		return 0;
	}

	double *highPass = createHighPassFilter(order, samplingRate, startFrequency);
	double *lowPass = createLowPassFilter(order, samplingRate, finalFrequency);

	linearAlgebra::normalizeVector(highPass, order + 1);
	linearAlgebra::normalizeVector(lowPass, order + 1);

	for (int i = 0; i < order + 1; i++)
	{
		lowPass[i] = lowPass[i] + highPass[i];
	}

	return lowPass;
}

double* createTriangularWindow(int order)
{

	// order plus 1 is the amount of items
	double *w = new double[order + 1];

	// The reference point is amount of items divided by 2
	double referencePoint = order / 2.0;

	int n = 0;
	for (; n <= referencePoint; n++)
	{
		w[n] = 2.0 * n / order;
	}

	for (; n <= order; n++)
	{
		w[n] = 2.0 - 2.0 * n / order;
	}
	return w;
}

void applyWindow(double *filter, double *window, int order)
{
	do
	{
		filter[order] *= window[order];
	} while (order--);
}
