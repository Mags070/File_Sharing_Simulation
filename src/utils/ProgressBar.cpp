#include<iostream>
#include "ProgressBar.hpp"

using namespace std;

Progress::Progress(size_t total,size_t width)
        :total(total),width(width){};
void Progress::update(size_t current){
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
        cout<<"]"<<"Percent :"<<"Percent"<<100<<"%";
        cout.flush();
        
}

void Progress::finish(){
    update(total);
    cout<<""<<endl;
}