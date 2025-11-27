#include<iostream>
#include<string>
using namespace std;

class Progress{
    private:
        size_t total;
        size_t width;
    public:
        Progress(size_t total,size_t width=40);
        void update(size_t current);
        void finish();

    

};