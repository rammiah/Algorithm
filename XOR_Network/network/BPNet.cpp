//
// Created by Yaning Wang on 2018/3/12.
//

#include "BPNet.h"

BPNet::BPNet() {
    // 初始化随机数生成器
    srand((unsigned) time(nullptr));
    error = 100.f;

    // 初始化输入层
    for (int i = 0; i < IN_NODE_CNT; ++i) {
        inputLayer[i] = new InputNode();
        for (int j = 0; j < HIDE_NODE_CNT; ++j) {
            inputLayer[i]->weight.push_back(get_11Random());
            inputLayer[i]->wDeltaSum.push_back(0.f);
        }
    }

    // 初始化隐藏层
    for (int i = 0; i < HIDE_LAYER_CNT; ++i) {
        // 最后一层，下一层为输出层
        for (int j = 0; j < HIDE_NODE_CNT; ++j) {
            hiddenLayer[i][j] = new HiddenNode();
            hiddenLayer[i][j]->bias = get_11Random();
            if (i == HIDE_LAYER_CNT - 1) {
                for (int k = 0; k < OUT_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->weight.push_back(get_11Random());
                    hiddenLayer[i][j]->wDeltaSum.push_back(0.f);
                }
            } else {
                for (int k = 0; k < HIDE_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->weight.push_back(get_11Random());
                    hiddenLayer[i][j]->wDeltaSum.push_back(0.f);
                }
            }
        }
    }

    // 初始化输出层
    for (int i = 0; i < OUT_NODE_CNT; ++i) {
        outputLayer[i] = new OutputNode();
        outputLayer[i]->bias = get_11Random();
    }
}

// 前向传播
void BPNet::forwardPropagationEpoc() {
    // 输入应该在前向传播前已处理
    // 隐含层操作
#ifdef _DEBUG
    cout << "Into forward.\n";
#endif
    // 从前到后计算
    for (int i = 0; i < HIDE_LAYER_CNT; ++i) {
        if (i == 0) {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                double sum = 0;
                for (int k = 0; k < IN_NODE_CNT; ++k) {
                    sum += inputLayer[k]->value * inputLayer[k]->weight[j];
                }
                sum += hiddenLayer[i][j]->bias;
                hiddenLayer[i][j]->value = sigmoid(sum);
            }
        } else {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                double sum = 0;
                for (int k = 0; k < HIDE_NODE_CNT; ++k) {
                    sum += hiddenLayer[i - 1][k]->value * hiddenLayer[i - 1][k]->weight[j];
                }
                sum += hiddenLayer[i][j]->bias;
                hiddenLayer[i][j]->value = sigmoid(sum);
            }
        }
    }

    // 输出层操作
    for (int i = 0; i < OUT_NODE_CNT; ++i) {
        double sum = 0;
        for (int j = 0; j < HIDE_NODE_CNT; ++j) {
            sum += hiddenLayer[HIDE_LAYER_CNT - 1][j]->value * hiddenLayer[HIDE_LAYER_CNT - 1][j]->weight[i];
        }

        sum += outputLayer[i]->bias;
        outputLayer[i]->value = sigmoid(sum);
    }
#ifdef _DEBUG
    cout << "Out forward.\n";
#endif
}

// 反向传播
void BPNet::backPropagationEpoc() {
#ifdef _DEBUG
    cout << "In back.\n";
#endif
    // 计算delta
    for (int i = 0; i < OUT_NODE_CNT; ++i) {
        // 误差值
        double tempe = outputLayer[i]->value - outputLayer[i]->rightout;
        // 错误值加到error上面
        error += tempe * tempe / 2;
        outputLayer[i]->delta = tempe * (1 - outputLayer[i]->value) * outputLayer[i]->value;
        outputLayer[i]->deltaSum += outputLayer[i]->delta;
    }
    // 隐藏层
    for (int i = HIDE_LAYER_CNT - 1; i >= 0; --i) {
        if (i == HIDE_LAYER_CNT - 1) {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                double sum = 0;
                for (int k = 0; k < OUT_NODE_CNT; ++k) {
                    sum += outputLayer[k]->delta * hiddenLayer[i][j]->weight[k];
                }
                hiddenLayer[i][j]->delta = sum * (1 - hiddenLayer[i][j]->value) * hiddenLayer[i][j]->value;
                hiddenLayer[i][j]->deltaSum += hiddenLayer[i][j]->delta;
                for (int k = 0; k < OUT_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->wDeltaSum[k] += hiddenLayer[i][j]->value * outputLayer[k]->delta;
                }
            }
        } else {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                double sum = 0;
                for (int k = 0; k < HIDE_NODE_CNT; ++k) {
                    sum += hiddenLayer[i + 1][j]->delta * hiddenLayer[i][j]->weight[k];
                }
                hiddenLayer[i][j]->delta = sum * (1 - hiddenLayer[i][j]->value) * hiddenLayer[i][j]->value;
                hiddenLayer[i][j]->deltaSum += hiddenLayer[i][j]->delta;
                for (int k = 0; k < HIDE_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->wDeltaSum[k] += hiddenLayer[i][j]->value * hiddenLayer[i + 1][j]->delta;
                }
            }
        }
    }
    // 输入层
    for (int i = 0; i < IN_NODE_CNT; ++i) {
        for (int j = 0; j < HIDE_NODE_CNT; ++j) {
            inputLayer[i]->wDeltaSum[j] += inputLayer[i]->value * hiddenLayer[0][j]->delta;
        }
    }
#ifdef _DEBUG
    cout << "Out back.\n";
#endif
}

void BPNet::train(vector<Sample> &sampleGroup, double threshold) {
#ifdef _DEBUG
    cout << "In train.\n";
#endif
    int sampleNum = sampleGroup.size();
    while (error > threshold) {
        cout << "training error: " << error << "\n";
        error = 0.f;
        // 初始化全部误差
        // 其中的bias和weight不可以动
        for (int i = 0; i < IN_NODE_CNT; ++i) {
            inputLayer[i]->wDeltaSum.assign(inputLayer[i]->wDeltaSum.size(), 0.f);
        }
        for (int i = 0; i < HIDE_LAYER_CNT; ++i) {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                hiddenLayer[i][j]->wDeltaSum.assign(hiddenLayer[i][j]->wDeltaSum.size(), 0.f);
            }
        }

        for (int iter = 0; iter < sampleNum; ++iter) {
            setInput(sampleGroup[iter].in);
            setOutput(sampleGroup[iter].out);

            forwardPropagationEpoc();
            backPropagationEpoc();
        }
        // 一个集合完成后开始更新参数
        update_bias_weight(sampleNum);
    }
#ifdef _DEBUG
    cout << "Out train.\n";
#endif
}

void BPNet::predict(vector<Sample> &testGroup) {
#ifdef _DEBUG
    cout << "In predict.\n";
#endif
    int testNum = testGroup.size();

    for (int iter = 0; iter < testNum; ++iter) {
        testGroup[iter].out.clear();

        setInput(testGroup[iter].in);
        forwardPropagationEpoc();

        // 处理输出层
        for (int i = 0; i < OUT_NODE_CNT; ++i) {
            testGroup[iter].out.push_back(outputLayer[i]->value);
        }
    }

#ifdef _DEBUG
    cout << "Out predict.\n";
#endif

}

void BPNet::setInput(vector<double> &sampleIn) {
#ifdef _DEBUG
    cout << "In input.\n";
#endif
    for (int i = 0; i < IN_NODE_CNT; ++i) {
        inputLayer[i]->value = sampleIn[i];
    }

#ifdef _DEBUG
    cout << "Out input.\n";
#endif

}

void BPNet::setOutput(vector<double> &sampleOutput) {
#ifdef _DEBUG
    cout << "In output.\n";
#endif

    for (int i = 0; i < OUT_NODE_CNT; ++i) {
        outputLayer[i]->rightout = sampleOutput[i];
    }

#ifdef _DEBUG
    cout << "Out output.\n";
#endif
}

void BPNet::update_bias_weight(int dataSize) {
    for (int i = 0; i < IN_NODE_CNT; ++i) {
        for (int j = 0; j < HIDE_NODE_CNT; ++j) {
            inputLayer[i]->weight[j] -= LEARN_RATE * inputLayer[i]->wDeltaSum[j] / dataSize;
        }
    }
    // 训练一个数据集合之后开始反馈
    for (int i = 0; i < HIDE_LAYER_CNT; ++i) {
        if (i == HIDE_LAYER_CNT - 1) {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                hiddenLayer[i][j]->bias -= LEARN_RATE * hiddenLayer[i][j]->deltaSum / dataSize;
                // weight 更新
                for (int k = 0; k < OUT_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->weight[k] -= LEARN_RATE * hiddenLayer[i][j]->wDeltaSum[k] / dataSize;
                }
            }
        } else {
            for (int j = 0; j < HIDE_NODE_CNT; ++j) {
                hiddenLayer[i][j]->bias -= LEARN_RATE * hiddenLayer[i][j]->deltaSum / dataSize;
                for (int k = 0; k < HIDE_NODE_CNT; ++k) {
                    hiddenLayer[i][j]->weight[k] -= LEARN_RATE * hiddenLayer[i][j]->wDeltaSum[k] / dataSize;
                }
            }
        }
    }

    for (int i = 0; i < OUT_NODE_CNT; ++i) {
        outputLayer[i]->bias -= LEARN_RATE * outputLayer[i]->deltaSum / dataSize;
    }
}
