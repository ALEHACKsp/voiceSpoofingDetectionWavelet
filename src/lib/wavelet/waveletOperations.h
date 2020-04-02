/**
 * @author André Furlan
 * @email ensismoebius@gmail.com
 * This whole project are under GPLv3, for
 * more information read the license file
 *
 * 1 de abr de 2020
 */
#ifndef SRC_LIB_WAVELET_WAVELETOPERATIONS_H_
#define SRC_LIB_WAVELET_WAVELETOPERATIONS_H_

#include <vector>

namespace wavelets {

	/**
	 * Given a vector calculates the corresponding orthogonal vector
	 * @param vector - source vector
	 * @return orthogonal vector
	 */
	std::vector<long double> calcOrthogonal(std::vector<double> &vector);

	/**
	 * Applies a wavelets transform over a
	 * signal using the Mallat algorithm
	 * @param signal - signal to be transformed
	 * @param lowpassfilter - the wavelet lowpass filter
	 * @param level - levels of the signal decomposition
	 * @param maxItens - the signal upper limit to be processed
	 * @return transformed signal
	 */
	std::vector<double> malat(std::vector<double> signal, std::vector<double> lowpassfilter, unsigned int level = 1, unsigned int maxItens = 0);
}

#endif /* SRC_LIB_WAVELET_WAVELETOPERATIONS_H_ */
