// Trie.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <cstdio>
#include "Trie.h"

int main() {
    Trie trie;
    freopen("C:/Users/Yaning Wang/Documents/GitHub/Algorithm/Trie/text.txt", "r", stdin);
    std::string str;
    char buff[100];
    while (std::cin >> str) {
        trie.insert(str);
    }
    std::cout << "Trump in text: " << trie.contain("Trump") << "\n";
    auto words = trie.get_starts_with("ex");
    std::cout << "Starts with \"ex\": " << words.size() << "\n";
    for (auto &w : words) {
        std::cout << w << "\n";
    }
    return 0;
}

