#include "network/BPNet.h"


int main() {
//    std::cout << "Hello, World!" << std::endl;
    BPNet testNet;
    vector<Sample> sampleInOut(8);
    // 0 ^ 0 = 0
    sampleInOut[0].in.push_back(0);
    sampleInOut[0].in.push_back(0);
    sampleInOut[0].out.push_back(0);
    // 0 ^ 1 = 0
    sampleInOut[1].in.push_back(0);
    sampleInOut[1].in.push_back(1);
    sampleInOut[1].out.push_back(1);
    // 1 ^ 0 = 1
    sampleInOut[2].in.push_back(1);
    sampleInOut[2].in.push_back(0);
    sampleInOut[2].out.push_back(1);
    // 1 ^ 1 = 1
    sampleInOut[3].in.push_back(1);
    sampleInOut[3].in.push_back(1);
    sampleInOut[3].out.push_back(0);
    sampleInOut[4] = sampleInOut[0];
    sampleInOut[5] = sampleInOut[1];
    sampleInOut[6] = sampleInOut[2];
    sampleInOut[7] = sampleInOut[3];
    // 训练
    testNet.train(sampleInOut, 0.0002);
    // 测试
    vector<Sample> testGroup(4);
    testGroup[0].in.push_back(0.1);
    testGroup[0].in.push_back(0.1);

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