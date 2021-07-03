# Support-Vector Machines (SVMs) Training and Inference on Microcontrollers (MCUs)

We provide *Edge2Train* (E2T) algorithm for all Arduino IDE-supported MCUs. E2T can be used for SVMs (for binary classification tasks) training and inference directly on MCU boards.

## Datasets, MCU boards chosen for SVMs training and inference on MCUs

### Datasets 

We converted all the listed datasets into MCU executable *.h* files and placed them inside the E2T folder. The datasets 1 to 3 are used for training binary SVM classifiers on MCUs using *Edge2Train*. The users have to uncomment their dataset of choice (header file at the beginning of the main algorithm program) to use it when training and inference on MCUs.

1. [Iris Flowers](https://archive.ics.uci.edu/ml/datasets/iris "Google's Homepage") (4 features, 3 classes, 150 samples): Using E2T, we train a binary classifier to distinguish Iris Setosa from other flowers based on the input features.
2. [Breast Cancer](https://www.kaggle.com/uciml/breast-cancer-wisconsin-data) (30 features, 2 classes, 569 samples): Here we train a binary classifier that can find the class names (malignant or benign) based on the input features.
3. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/) (64 features, 10 classes, 1797 samples): Here, we extracted data fields for digit 6, with positive and negative samples. Then using E2T, we trained a binary classifier on MCUs, that distinguishes digit 6 from other digits, based on the input features.

### MCU boards

Using Arduino IDE we upload the E2T algorithm along with the selected/uncommented dataset on the following popular boards and trained binary classifiers on MCUs.

1. [nRF52840 Adafruit Feather](https://www.adafruit.com/product/4062): ARM Cortex-M4 @64MHz, 1MB Flash, 256KB SRAM.
2. [STM32f103c8 Blue Pill](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html): ARM Cortex-M0 @72MHz, 128KB Flash, 20KB SRAM.
3. [Generic ESP32](https://www.espressif.com/en/products/devkits): Xtensa LX6 @240MHz, 4MB Flash, 520KB SRAM.
4. [ATSAMD21G18 Adafruit METRO](https://www.adafruit.com/product/3505): ARM Cortex-M0+ @48 MHz, 256kB Flash, 32KB SRAM. 

## Edge2train Performance Evaluation

After training we compare the accuracy, power consumed and inference time of our Edge2Trained models (trained on MCUs) with Python Scikit learn models (trained on high-resource CPUs). The CPU1 is a standard Ubuntu laptop with Intel (R) Core (TM) i7-5500 CPU @ 2.40 GHz. CPU2 is a Windows laptop with Intel (R) Core (TM) i7-8650U CPU @ 1.90 GHz. <br/>

![alt text](https://github.com/bharathsudharsan/Edge2Train/blob/main/setup.png)

As shown above, we received the training and inference result from MCUs via Serial port. We report the following results, using which we perform analysis.

1. Time and energy consumed to; Train SVMs on MCUs; Infer using thus trained SVMs on MCUs

2. Flash and SRAM consumed by Edge2Train on various MCU boards

3. Compare MCUs results (uses Edge2Train) with CPUs (uses Python scikit-learn) for the same datasets and tasks

The relationship between training time, training set size, and feature dimension

1. ESP32 took 15.58 secs to train for Iris. 28.24 secs for 64 features MNIST digits

2. Metro with 48 MHz clock and no FPS took 21.39 minutes for Iris (82x times slower than ESP32)

3. Training time grows swiftly with the number of training samples

4. Sudden peaks observed when Edge2Train consumes more time to find the optimal hyperplane. Trough points are observed when the optimal hyperplane is found quickly


**Tip:** When first time training SVMs on MCUs using E2T, we recommend to use better resource boards like ESP32 and Adafruit Feather nrf52, then move on to using the tiny ones like Arduino Nano, Uno, etc.

If the code is useful, please consider citing *Edge2Train* paper using the BibTex entry below.

```
@inproceedings{sudharsan2020edge2train,
  title={Edge2train: a framework to train machine learning models (svms) on resource-constrained iot edge devices},
  author={Sudharsan, Bharath and Breslin, John G and Ali, Muhammad Intizar},
  booktitle={Proceedings of the 10th International Conference on the Internet of Things (IoT’20)},
  year={2020}
}
```
