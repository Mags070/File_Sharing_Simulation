#include<iostream>
#include "utils/Logger.hpp"
#include "crypto/Hash.hpp"
#include "utils/ProgressBar.hpp"
#include <thread>
#include<chrono>
using namespace std;

int main(int argc,char **argv){
    Logger::info("PeerShare started");
    Logger::success("Logger working perfectly!");
    Logger::warning("Be careful!");
    Logger::error("Something went wrong!");
    Progress bar(100);

    for(int i=0;i<=100;i++){
        bar.update(i);
        this_thread::sleep_for(chrono::milliseconds(20));
    }
    bar.finish();

    string test="hello world";
    string hashed=Hash::sha256(test);
    cout<<hashed<<endl;
   /* cout<<"Simulator Initialised"<<endl;
    cout<<"Usage"<<endl;
    cout << "peershare receive <port>\n";
    cout << "peershare send <file> <ip> <port>\n";*/

    return 0;
}
