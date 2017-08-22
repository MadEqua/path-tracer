#pragma once

//TODO: multi-threading
class BloomPostProcess
{
public:
	BloomPostProcess(float *input, float *output, 
		int imageWidth, int imageHeight,
		int kernelWidth, int kernelHeight,
		float gaussianSigma, float luminosityThreshold);

	void process();

private:
	int imageWidth, imageHeight;
	int kernelWidth, kernelHeight;
	float gaussianSigma;
	float luminosityThreshold;

	float *input, *output;

	void filterLowLuminosity(float *in, float *out);
	void applyGaussianBlur(float *in, float *out);
	void sumImages(float *in, float *in2, float *out);

	float** createGaussianMatrix() const;
};
