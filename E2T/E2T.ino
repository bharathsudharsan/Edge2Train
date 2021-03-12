#define KERNEL_GAMMA 0.001
#include "iris.h"
//#include "breast_cancer.h"
//#include "digits.h"
#define TOTAL_SAMPLES (POSITIVE_SAMPLES + NEGATIVE_SAMPLES)
#pragma once

namespace Edge2Train {
    namespace SVM_train {
        typedef float (*kernelFunction)(float *x, float *w, unsigned int dimension);
        float linearKernel(float *x, float *w, unsigned int dimension) {
            float sum = 0;
            for (unsigned int i = 0; i < dimension; i++)
                sum += x[i] * w[i];
            return sum;   }

#if defined(KERNEL_GAMMA)
        float rbfKernel(float *x, float *w, unsigned int dimension) {
            float sum = 0;
            for(unsigned int i = 0; i < dimension; i++)
                sum += (x[i] - w[i]) * (x[i] - w[i]);
            return exp(-sum / (2 * KERNEL_GAMMA * KERNEL_GAMMA));    }
#endif
    } }

namespace Edge2Train {
    namespace SVM_train {
        template<unsigned int D>
        class SVMSMO {
        public:
            SVMSMO(kernelFunction kernel) :
                    _kernel(kernel),
                    _cache(NULL) {
                _params = {
                        .C = 1,
                        .tol = 1e-4,
                        .alphaTol = 1e-7,
                        .maxIter = 10000,
                        .passes = 10
                };   }

            void setC(float C) {
                _params.C = C;  }

            void setTol(float tol) {
                _params.tol = tol;    }
                
            void setAlphaTol(float alphaTol) {
                _params.alphaTol = alphaTol; }

            void setMaxIter(unsigned int maxIter) {
                _params.maxIter = maxIter;  }

            void setPasses(unsigned int passes) {
                _params.passes = passes;   }

            void setKernelCache(float *cache) {
                _cache = cache;  }

            void fit(float X[][D], int *y, unsigned int N) {
                _alphas = (float *) malloc(sizeof(float) * N);

                for (unsigned int i = 0; i < N; i++)
                    _alphas[i] = 0;

                unsigned int iter = 0;
                unsigned int passes = 0;
                
                if (_cache != NULL) {
                    for (unsigned int i = 0; i < N; i++) {
                        for (unsigned int j = 0; j < N; j++) {
                            _cache[i * N + j] = _kernel(X[i], X[j], D);
                        }        }         }

                while(passes < _params.passes && iter < _params.maxIter) {
                    float alphaChanged = 0;
                    for (unsigned int i = 0; i < N; i++) {
                        float Ei = margin(X, y, X[i], N) - y[i];
                        if ((y[i] * Ei < -_params.tol && _alphas[i] < _params.C) || (y[i] * Ei > _params.tol && _alphas[i] > 0)) {
                            unsigned int j = i;

                            while (j == i)
                                j = random(0, N);
                            float Ej = margin(X, y, X[j], N) - y[j];  float ai = _alphas[i]; float aj = _alphas[j];  float L = 0;  float H = 0;

                            if (y[i] == y[j]) {
                                L = max(0, ai + aj - _params.C);
                                H = min(_params.C, ai + aj); } 
                                else {
                                L = max(0, aj - ai);
                                H = min(_params.C, _params.C + aj - ai);     }
                            if (abs(L - H) < 1e-4)
                                continue;
                            double eta;
                            if (_cache != NULL)
                                eta = _cache[i * N + j] - _cache[i * N + i] - _cache[j * N + j];
                            else {
                                eta = _kernel(X[i], X[j], D) - _kernel(X[i], X[i], D) - _kernel(X[j], X[j], D);    }
                            if (eta >= 0)
                                continue;
                            eta *= 2;
                            float newaj = aj - y[j] * (Ei - Ej) / eta;
                            if (newaj > H)
                                newaj = H;
                            if (newaj < L)
                                newaj = L;
                            if (abs(aj - newaj) < 1e-4)
                                continue;
                            float newai = ai + y[i] * y[j] * (aj - newaj);
                            _alphas[i] = newai;
                            _alphas[j] = newaj;
                            float b1 = _b - Ei - y[i] * (newai - ai) * _kernel(X[i], X[i], D)
                                       - y[j] * (newaj - aj) * _kernel(X[i], X[j], D);
                            float b2 = _b - Ej - y[i] * (newai - ai) * _kernel(X[i], X[j], D)
                                       - y[j] * (newaj - aj) * _kernel(X[j], X[j], D);
                            _b = 0.5 * (b1 + b2);
                           if (newai > 0 && newai < _params.C)
                                _b = b1;
                            if (newaj > 0 && newaj < _params.C)
                                _b = b2;
                            alphaChanged++;  } } 
                    iter++;
                    if(alphaChanged == 0)
                        passes++;
                    else passes= 0;      }
               _y = y;
                _numSamples = N;  }     
                
            int predict(float X_train[][D], float x[D]) {
                return margin(X_train, _y, x, _numSamples, true) > 0 ? 1 : -1;      }
            float score(float X_train[][D], float X_test[][D], int y_test[], unsigned int testSize) {
                unsigned int correct = 0;
                for (unsigned int i = 0; i < testSize; i++)
                    if (predict(X_train, X_test[i]) == y_test[i])
                        correct += 1;
                return 1.0 * correct / testSize;      }

        protected:
            kernelFunction _kernel;
            struct {
                float C;  float tol;   float alphaTol;  unsigned int maxIter;  unsigned int passes;
            } _params;
            float _b = 0;
            unsigned int _numSamples;
            int *_y;
            float *_alphas;
            float *_cache;
            float margin(float X[][D], int *y, float x[D], unsigned int N, bool skipSmallAlfas = false) {
                float sum = _b;
                for(unsigned int i = 0; i < N; i++)
                    if ((!skipSmallAlfas && _alphas[i] != 0) || (skipSmallAlfas && _alphas[i] > _params.alphaTol))
                        sum += _alphas[i] * y[i] * _kernel(x, X[i], D);
                return sum;  }   };  }  }

using namespace Edge2Train::SVM_train;

float X_train[TOTAL_SAMPLES][Feature_dimension]; float X_test[TOTAL_SAMPLES][Feature_dimension];
int y_train[TOTAL_SAMPLES]; int y_test[TOTAL_SAMPLES];
float cache[TOTAL_SAMPLES * TOTAL_SAMPLES];
SVMSMO<Feature_dimension> classifier(linearKernel);
int temp, temp1 = 0;

void setup() {
    Serial.begin(115200);
    delay(3000);
    classifier.setC(10);
    classifier.setTol(1e-5);
    classifier.setMaxIter(10000);}

void loop() {
    int tp = 0; int tn = 0; int fp = 0; int fn = 0;

    int positive_sample_inputs = readSerialNumber("Enter the number of positive samples that need to be used for training? ", POSITIVE_SAMPLES);
    if (positive_sample_inputs > POSITIVE_SAMPLES - 1) {
        positive_sample_inputs = POSITIVE_SAMPLES - 1;    }
    int negative_sample_inputs = readSerialNumber("Enter the number of negative samples that need to be used for training? ", NEGATIVE_SAMPLES);
    if (negative_sample_inputs > NEGATIVE_SAMPLES - 1) {
        negative_sample_inputs = NEGATIVE_SAMPLES - 1;    }

    loadDataset(positive_sample_inputs, negative_sample_inputs);
    time_t start = millis();
    classifier.fit(X_train, y_train, positive_sample_inputs + negative_sample_inputs);
    Serial.print("It took ");
    temp1 = millis() - start;
    Serial.print(temp1);
    Serial.print("ms to train on ");
    Serial.print(positive_sample_inputs + negative_sample_inputs);
    Serial.println(" samples");

    start = millis();

    for (int i = 0; i < TOTAL_SAMPLES - positive_sample_inputs - negative_sample_inputs; i++) {
        int y_pred = classifier.predict(X_train, X_test[i]);
        int y_true = y_test[i];
        if (y_pred == y_true && y_pred ==  1) tp += 1;
        if (y_pred == y_true && y_pred == -1) tn += 1;
        if (y_pred != y_true && y_pred ==  1) fp += 1;
        if (y_pred != y_true && y_pred == -1) fn += 1;    }

    Serial.print("It took ");
    temp = millis() - start;
    Serial.print(temp);
    Serial.print("ms to test on ");
    Serial.print(TOTAL_SAMPLES - positive_sample_inputs - negative_sample_inputs);
    Serial.println(" samples");}

int readSerialNumber(String prompt, int maxAllowed) {
    Serial.print(prompt);
    Serial.print(" (");
    Serial.print(maxAllowed);
    Serial.print(" max) ");
    while (!Serial.available()) delay(1);
    int n = Serial.readStringUntil('\n').toInt();
    Serial.println(n);
    return n; }

void loadDataset(int positive_sample_inputs, int negative_sample_inputs) {
    int positiveTestSamples = POSITIVE_SAMPLES - positive_sample_inputs;

    for (int i = 0; i < positive_sample_inputs; i++) {
        memcpy(X_train[i], X_positive[i], Feature_dimension);
        y_train[i] = 1;    }

    for (int i = 0; i < negative_sample_inputs; i++) {
        memcpy(X_train[i + positive_sample_inputs], X_negative[i], Feature_dimension);
        y_train[i + positive_sample_inputs] = -1;    }

    for (int i = 0; i < positiveTestSamples; i++) {
        memcpy(X_test[i], X_positive[i + positive_sample_inputs], Feature_dimension);
        y_test[i] = 1;    }

    for (int i = 0; i < NEGATIVE_SAMPLES - negative_sample_inputs; i++) {
        memcpy(X_test[i + positiveTestSamples], X_negative[i + negative_sample_inputs], Feature_dimension);
        y_test[i + positiveTestSamples] = -1;    }}
