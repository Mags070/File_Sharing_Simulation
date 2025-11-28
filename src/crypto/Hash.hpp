#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Hash{
    public:
    static string sha256(const vector<unsigned char>&data);
    static string sha256(const string &data);
    static string sha256_file(const string &filepath);
};