#include<iostream>
#include "ProgressBar.hpp"

using namespace std;

Progress::Progress(size_t total,size_t width)
        :total(total),width(width){};
        
void Progress::update(size_t current){
        // Add a check to prevent division by zero, just in case
        if (total == 0) return; 
        
        float progress=(float)current/total;
        int pos=(int)(width*progress);
        
        cout<<"[";
        for(int i=0;i<width;i++){
            if(i<pos) cout<<"#";
            else{
                cout<<"-";
            }
        }
        
        int precent=(int)(progress*100.0f);
        
        // This line contains the essential fix: using the calculated 'precent' and '\r'
        cout<<"]"<<"Percent :"<<precent<<"%\r"; 
        
        cout.flush();
}

void Progress::finish(){
   cout<<"[";
    for(size_t i=0;i<width;i++){
        cout<<"#";
    }
    cout<<"]"<<"Percent :"<<100<<"%"<<"\n";
    cout<<""<<endl;
    cout.flush();
}