#include "network/BPNet.h"


int main() {
//    std::cout << "Hello, World!" << std::endl;
    BPNet testNet;
    vector<double> sampleInput[4];
    vector<double> sampleOutput[4];
    // 0 ^ 0 = 0
    sampleInput[0].push_back(0);
    sampleInput[0].push_back(0);
    sampleOutput[0].push_back(0);
    // 0 ^ 1 = 0
    sampleInput[1].push_back(0);
    sampleInput[1].push_back(1);
    sampleOutput[1].push_back(1);
    // 1 ^ 0 = 1
    sampleInput[2].push_back(1);
    sampleInput[2].push_back(0);
    sampleOutput[2].push_back(1);
    // 1 ^ 1 = 1
    sampleInput[3].push_back(1);
    sampleInput[3].push_back(1);
    sampleOutput[3].push_back(0);
    // 测试
    vector<Sample> sampleInOut(4);
    for (int i = 0; i < 4; ++i) {
        sampleInOut[i].in = sampleInput[i];
        sampleInOut[i].out = sampleOutput[i];
    }

    testNet.train(sampleInOut, 0.0002);

    vector<Sample> testGroup(4);
    testGroup[0].in.push_back(0.1);
    testGroup[0].in.push_back(0.2);

    testGroup[1].in.push_back(0.15);
    testGroup[1].in.push_back(0.9);

    testGroup[2].in.push_back(1.1);
    testGroup[2].in.push_back(0.01);

    testGroup[3].in.push_back(0.88);
    testGroup[3].in.push_back(1.03);

    testNet.predict(testGroup);

    for (int i = 0; i < testGroup.size(); ++i) {
        cout << testGroup[i].in[0] << " ^ " << testGroup[i].in[1] << " = " << testGroup[i].out[0] << "\n";
    }

    return 0;
}