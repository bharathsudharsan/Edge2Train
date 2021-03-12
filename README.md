# Edge2Train: A Framework to Train Machine Learning Model (SVMs) on Resource-Constrained IoT Edge Devices

**Overview:** We provide *Edge2Train* algorithm for all Arduino IDE-supported MCUs. [Edge2Train](https://dl.acm.org/doi/abs/10.1145/3410992.3411014) can be used to train SVMs (for binary classification tasks) on MCUs.

**Datasets for onboard training:** We converted all the listed datasets into MCU executable *.h* files and placed them inside the E2T folder. The datasets 1 to 3 are used for training binary classifiers on MCUs using *Edge2Train*. The users have to uncomment their dataset of choice (header file at the beginning of the main algorithm program) to use it when training on MCUs.

1. [Iris Flowers](https://archive.ics.uci.edu/ml/datasets/iris "Google's Homepage"): Using *Edge2Train*, we train a binary classifier to distinguish Iris Setosa from other flowers based on the input features.
2. [Breast Cancer](https://www.kaggle.com/uciml/breast-cancer-wisconsin-data): Here we train a binary classifier that can find the class names (malignant or benign) based on the input features.
3. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/): Here, we extracted data fields for digit 6, with positive and negative samples. Then using *Edge2Train*, we trained a binary classifier on MCUs, that distinguishes digit 6 from other digits, based on the input features.

**Experiment:** Using Arduino IDE we uploaded our *Edge2Train* algorithm on popular [nRF52840 Adafruit Feather](https://www.adafruit.com/product/4062), [STM32f103c8 Blue Pill](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html), [Generic ESP32](https://www.espressif.com/en/products/devkits), [ATSAMD21G18 Adafruit METRO](https://www.adafruit.com/product/3505) boards and trained binary classifier using the above datasets.

**Tip:** Before using the *Edge2Train* for onboard training, for the first time, we recommend users to use better resource boards like ESP32 and Adafruit Feather nrf52, then move on to using the tiny ones like Arduino Nano, Uno, etc.

If you use the code in this repository in your work, please cite *Edge2Train* paper using the BibTex entry below.

```
@inproceedings{sudharsan2020edge2train,
  title={Edge2train: a framework to train machine learning models (svms) on resource-constrained iot edge devices},
  author={Sudharsan, Bharath and Breslin, John G and Ali, Muhammad Intizar},
  booktitle={Proceedings of the 10th International Conference on the Internet of Things},
  pages={1--8},
  year={2020}
}
```
