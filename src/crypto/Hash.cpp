#include "Hash.hpp"
#include<iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <iomanip>
#include <cstring>
using namespace std;

using uint8=uint8_t;
using uint32=uint32_t;
using uint64=uint64_t;
namespace{
    const uint32 k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    inline uint32 rotr(uint32 x,uint32 n){
        return (x>>n) | (x<<(32-n));
    }

    inline void transform(uint32 state[8],const uint8 block[64]){
            uint32 w[64];
            for(int i=0;i<16;i++){
                    w[i]=(block[i*4]<<24)|(block[i*4+1]<<16)|(block[i*4+2]<<8)|(block[i*4+3]);

            }   

            for(int i=16;i<64;i++){
                uint32 s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3);
                uint32 s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10);
                w[i]= w[i-16]+s0+w[i-7]+s1;



            }

            uint32 a=state[0],b=state[1],c=state[2],d=state[3],e=state[4],f=state[5],g=state[6],h=state[7];

            for (int i = 0; i < 64; ++i) {
            uint32 S1 = rotr(e,6) ^ rotr(e,11) ^ rotr(e,25);
            uint32 ch = (e & g) ^ (~e & f);
            uint32 temp1 = h + S1 + ch + k[i] + w[i];
            uint32 S0 = rotr(a,2) ^ rotr(a,13) ^ rotr(a,22);
            uint32 maj = (a & c) ^ (a & b) ^ (b & c);
            uint32 temp2 = S0 + maj;
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        state[0]+=a; state[1]+=b; state[2]+=c; state[3]+=d;state[4]+=e; state[5]+=f; state[6]+=g; state[7]+=h;


    }
}

string toHex(const uint8 *hash){
    stringstream ss;
    for(int i=0;i<32;i++){
        ss<<hex<<setw(2)<<setfill('0')<<(int)hash[i];
    }
    return ss.str();
}

string Hash::sha256(const vector<unsigned char>&data){
    uint32 state[8]={
        0x6a09e667,0xbb67ae85,
        0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,
        0x1f83d9ab,0x5be0cd19
    };

    uint64 bitlen=data.size()*8;
    vector<uint8>buffer(data.begin(),data.end());
    buffer.push_back(0x80);
    while((buffer.size()%64)!=56){
        buffer.push_back(0);
    }
    for (int i=7;i>=0;i--)
        buffer.push_back((bitlen >> (i * 8)) & 0xff);

    for (size_t i=0;i<buffer.size();i+=64)
        transform(state, &buffer[i]);
    uint8 hash[32];
    for (int i = 0; i < 8; i++) {
        hash[i * 4 + 0] = (state[i] >> 24) & 0xff;
        hash[i * 4 + 1] = (state[i] >> 16) & 0xff;
        hash[i * 4 + 2] = (state[i] >> 8) & 0xff;
        hash[i * 4 + 3] = (state[i]) & 0xff;
    }

    return toHex(hash);

}
string Hash::sha256(const string &data) {
    return sha256(std::vector<unsigned char >(data.begin(), data.end()));
}
string Hash::sha256_file(const string &filepath){
    ifstream file(filepath,ios::binary);
    if(!file){
        return "";

    }
    vector<unsigned char>buffer(
         (std::istreambuf_iterator<char>(file)),
         std::istreambuf_iterator<char>()
    );
    return sha256(buffer);
}
