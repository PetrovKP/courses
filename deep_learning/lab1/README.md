# FCNN

## [Intro](../README.md)

## Разработанные скрипты

Вся проведенная работа в пошаговом виде содержится в Jupiter-ноутбуке [lab1.ipynb](./lab1.ipynb).
Файл включает в себя:
* Подготовку данных
* Создание моделей
* Обучение моделей
* Визуализацию каждого шага


## Конфигурации моделей

В данной работе были проведены эксперементы с тремя моделями: 

1. Baseline model

    ![equation](https://latex.codecogs.com/gif.latex?%24%24n%20%5Ctimes%2064%20%5Ctimes%2064%20%5Ctimes%203%20%5Crightarrow%20n%20%5Ctimes%20128%20%5Crightarrow%20RELU%20%5Crightarrow%20n%20%5Ctimes%2018%20%5Crightarrow%20SOFTMAX%24%24)  
    
    ![baseline model](./images/baseline_model.png)

2. Improved model

    ![equation](https://latex.codecogs.com/gif.latex?%24%24n%20%5Ctimes%2064%20%5Ctimes%2064%20%5Ctimes%203%20%5Crightarrow%20n%20%5Ctimes%201024%20%5Crightarrow%20RELU%20%5Crightarrow%20n%20%5Ctimes%20128%20%5Crightarrow%20RELU%20%5Crightarrow%20n%20%5Ctimes%2018%20%5Crightarrow%20SOFTMAX%24%24)
    
    ![improved model](./images/improved_model.png)
    
3. Custom model

    ![equation](https://latex.codecogs.com/gif.latex?%24%24n%20%5Ctimes%2064%20%5Ctimes%2064%20%5Ctimes%203%20%5Crightarrow%20n%20%5Ctimes%20%5Ctextbf%20s%20%5Crightarrow%20%5Ctextbf%20%7Bact%7D%20%5Crightarrow%20n%20%5Ctimes%2018%20%5Crightarrow%20SOFTMAX%24%24)
    
    Рассматривались конфигурации нейронной сети при параметрах:
    
    ![equation](https://latex.codecogs.com/gif.latex?%24%24%20%5Ctextbf%20s%20%5Cin%20%5C%7B256%2C%20512%2C%201024%5C%7D%3B%20%5Ctextbf%20%7Bact%7D%20%3D%20%5C%7Brelu%2C%20selu%2C%20sigmoid%5C%7D%24%24)
    
    Пример модели:

    ![equation](https://latex.codecogs.com/gif.latex?%24%24n%20%5Ctimes%2064%20%5Ctimes%2064%20%5Ctimes%203%20%5Crightarrow%20n%20%5Ctimes%20512%20%5Crightarrow%20ReLU%20%5Crightarrow%20n%20%5Ctimes%2018%20%5Crightarrow%20SOFTMAX%24%24)
    
    ![custom model](./images/custom_model.png)
    
## Результаты экспериментов

| Model                                      | time (sec) | training accuracy | validate accuracy | test accuracy |
|--------------------------------------------|------------|-------------------|-------------------|---------------|
| Baseline model                             | 738.15     | 0.488             | 0.442             | 0.439         |
| Improved model                             | 998.92     | 0.515             | 0.426             | 0.442         |
| Best of custom model (s=512, act='relu')   | 842.86     | 0.495             | 0.480             | 0.473         |

Лучшие результаты на тестовой выборке продемонстрировала custom model с параметрами s=512, act='relu' - *0.473*.

Визуализация предсказания лучшей модели (по топ-3):

![predicts](./images/visualized_results.png)