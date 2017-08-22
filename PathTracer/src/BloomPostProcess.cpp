#include "BloomPostProcess.h"

#include <iostream>
#include <cmath>

#include <glm/glm.hpp>
#include "Utils.h"


BloomPostProcess::BloomPostProcess(float *input, float *output,
	int imageWidth, int imageHeight,
	int kernelWidth, int kernelHeight,
	float gaussianSigma, float luminosityThreshold) :
	input(input), output(output),
	imageWidth(imageWidth), imageHeight(imageHeight),
	kernelWidth(kernelWidth), kernelHeight(kernelHeight),
	gaussianSigma(gaussianSigma),
	luminosityThreshold(luminosityThreshold) {
}

void BloomPostProcess::process() {

	float *temp = new float[imageWidth * imageHeight * 3];
	memset(temp, 0, imageWidth * imageHeight * 3 * sizeof(float));

	filterLowLuminosity(input, temp);
	//stbi_write_hdr("temp.hdr", imageWidth, imageHeight, 3, temp);
	applyGaussianBlur(temp, output);
	//stbi_write_hdr("temp2.hdr", imageWidth, imageHeight, 3, output);
	sumImages(input, output, output);

	delete[] temp;
}

void BloomPostProcess::filterLowLuminosity(float *in, float *out) {
	
	const glm::vec3 BLACK(0.0f);
	
	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {		
			glm::vec3 color = Utils::getPixelFromImage(in, imageWidth, imageHeight, i, j);
			float lum = Utils::getLuminanceFromRgb(color);
			Utils::setPixelToImage(out, 
				imageWidth, imageHeight,
				i, j,
				lum > luminosityThreshold ? color : BLACK);
		}
	}
}

void BloomPostProcess::applyGaussianBlur(float *in, float *out) {
	float** gausianMatrix = createGaussianMatrix();

	int halfKernelWidth = kernelWidth / 2;
	int halfKernelHeight = kernelHeight / 2;

	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {

			glm::vec3 sum(0.0f);

			for (int h = -halfKernelHeight; h < halfKernelHeight; ++h) {
				for (int w = -halfKernelWidth; w < halfKernelWidth; ++w) {

					int x = glm::clamp(i + w, 0, imageWidth - 1);
					int y = glm::clamp(j + h, 0, imageHeight - 1);

					float weight = gausianMatrix[h + halfKernelHeight][w + halfKernelWidth];
					sum += weight * Utils::getPixelFromImage(in, imageWidth, imageHeight, x, y);
				}
			}

			Utils::setPixelToImage(out, imageWidth, imageHeight, i, j, sum);
		}
	}

	//Delete Gausian Matrix
	for (int i = kernelHeight; i < kernelHeight; ++i) {
		delete[] gausianMatrix[i];
	}
	delete[] gausianMatrix;
}

float** BloomPostProcess::createGaussianMatrix() const {
	
	float **gausianMatrix = new float*[kernelHeight];
	for (int i = 0; i < kernelHeight; ++i) {
		gausianMatrix[i] = new float[kernelWidth];
	}

	float sum = 0.0f;

	int halfKernelWidth = kernelWidth / 2;
	int halfKernelHeight = kernelHeight / 2;

	for (int i = -halfKernelHeight; i < halfKernelHeight; i++) {
		for (int j = -halfKernelWidth; j < halfKernelWidth; j++) {

			int x = j + halfKernelWidth;
			int y = i + halfKernelHeight;

			float temp = exp(-(i*i + j*j) / (2.0f * gaussianSigma*gaussianSigma)) / (2.0f * PI*gaussianSigma*gaussianSigma);
			gausianMatrix[y][x] = temp;
			sum += temp;
		}
	}

	for (int i = 0; i < kernelHeight; i++) {
		for(int j = 0; j < kernelWidth; j++) {
			gausianMatrix[i][j] /= sum;
		}
	}

	return gausianMatrix;
}

void BloomPostProcess::sumImages(float *in, float *in2, float *out) {
	for (int i = 0; i < imageWidth; i++) {
		for (int j = 0; j < imageHeight; j++) {
			glm::vec3 a = Utils::getPixelFromImage(in, imageWidth, imageHeight, i, j);
			glm::vec3 b = Utils::getPixelFromImage(in2, imageWidth, imageHeight, i, j);
			glm::vec3 sum = a + b;
			Utils::setPixelToImage(out, imageWidth, imageHeight, i, j, sum);
		}
	}
}