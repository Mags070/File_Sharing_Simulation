#include "Logger.hpp"
#include<iostream>
using namespace std;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define BLUE    "\033[34m"

void Logger::info(const string &msg ){
    cout<<BLUE<<"[INFO]"<<RESET<<msg<<endl;

}

void Logger::success(const string &msg){
    cout<<GREEN<<"[SUCCESS] "<<RESET<<msg<<endl;
}

void Logger::error(const string &msg){
    cout<<RED<<"[ERROR] "<<RESET<<msg<<endl;
}

void Logger::warning(const string &msg){
    cout<<YELLOW<<"[WARNING]"<<RESET<<msg<<endl;
}
