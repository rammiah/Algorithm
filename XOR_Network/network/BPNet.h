//
// Created by Yaning Wang on 2018/3/12.
//

#ifndef NEUTRALNETWORK_BPNET_H
#define NEUTRALNETWORK_BPNET_H
//#define _DEBUG


#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
// 模拟异或运算
#define IN_NODE_CNT 2 // 输入节点数
#define HIDE_NODE_CNT 4 // 隐含节点数
#define HIDE_LAYER_CNT 1 // 隐含层数
#define OUT_NODE_CNT 1 // 输出节点数
#define LEARN_RATE 10 // 学习速率


inline double get_11Random() {
    return 2.0 * ((double)rand() / RAND_MAX) - 1;
}

inline double sigmoid(double x) {
    double ans = 1 / (1 + exp(-x));
    return ans;
}

struct InputNode {
    double value; // 输入的值
    vector<double> weight, wDeltaSum;
};

struct OutputNode {
    double value, delta, rightout, bias, deltaSum;
};

struct HiddenNode {
    double value, delta, bias, deltaSum;
    vector<double> weight, wDeltaSum;
};

struct Sample {
    vector<double> in, out;
};

class BPNet {
public:
    BPNet();
    void forwardPropagationEpoc();
    void backPropagationEpoc();
    void update_bias_weight(int dataSize);

    void train(vector<Sample> &sampleGroup, double threshold);
    void predict(vector<Sample> &testGroup);

    void setInput(vector<double> &sampleIn);
    void setOutput(vector<double> &sampleOutput);

public:
    double error;
    InputNode *inputLayer[IN_NODE_CNT];
    OutputNode *outputLayer[OUT_NODE_CNT];
    HiddenNode *hiddenLayer[HIDE_LAYER_CNT][HIDE_NODE_CNT];
};

#endif //NEUTRALNETWORK_BPNET_H
