#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <fstream>

using namespace std;

double getf1(double d);
double getf2(double d);
double getf3(double d);
double get1x52(double d);
int prevpos(int a);
int centpos(int a);
double minLati = 35.2772;
double minLongi = 126.8682;
double maxLati = 35.2801;
double maxLongi = 126.8718;
double org_x = 35.2787;
double org_y = 126.8700;
double alt = 43;
double measureLength = 4782.9;
double tt_interval = 2.6685;

int main(){
    clock_t startTime = clock();
    ifstream LaC_file("Lati_cutted.csv");
    ifstream LoC_file("Longi_cutted.csv");
    ifstream M_file("M_cutted.csv");
    ofstream result_file("result.csv");
    ofstream bmf_file("bmf.csv");
    string str_buf;
    double val_Lati,val_Longi,val_M;
    double buff_M[100] = {0};
    double buff_M_filtered[100] = {0};
    double buff_Lati[100] = {0};
    double buff_Longi[100] = {0};
    double filter_window[3][100][100];
    double sum_M = 0;
    int pos_buff = 0;
    int count = 0;
    int i,j,k;
    double windowtarg,windowscaler;
    double convsum1,convsum2,convsum3;
    double convres;
    //filter window pre-calculation
    for(i = 0;i < 100;i++){
        for(j = 0;j < 100;j++){
            windowtarg = (i-49)*tt_interval/(j+1);
            windowscaler = 0.1/(j+1);
            filter_window[0][i][j] = windowscaler*getf1(windowtarg);
            filter_window[1][i][j] = windowscaler*getf2(windowtarg);
            filter_window[2][i][j] = windowscaler*getf3(windowtarg);
        }
    }
    //filter processing
    while(getline(M_file,str_buf)){
        val_M = stod(str_buf);
        getline(LaC_file,str_buf);
        val_Lati = stod(str_buf);
        getline(LoC_file,str_buf);
        val_Longi = stod(str_buf);
        if((val_Lati > minLati)&&(val_Lati < maxLati)&&(val_Longi > minLongi)&&(val_Longi < maxLongi)){
            if(count%10!=0){
                count++;
                continue;
            }
            count++;
            sum_M -= buff_M[pos_buff];
            buff_M[pos_buff] = val_M;
            buff_Lati[pos_buff] = val_Lati;
            buff_Longi[pos_buff] = val_Longi;
            sum_M += buff_M[pos_buff];
            buff_M_filtered[centpos(pos_buff)] = buff_M[centpos(pos_buff)] - sum_M/100;
            bmf_file << pos_buff;
            bmf_file << ",";
            bmf_file << buff_M[pos_buff];
            bmf_file << ",";
            bmf_file << buff_M[centpos(pos_buff)];
            bmf_file << ",";
            bmf_file << buff_M_filtered[centpos(pos_buff)];
            bmf_file << ",";
            bmf_file << sum_M/100;
            bmf_file << "\n";

            for(i = 0;i < 100;i++){
                convsum1 = 0;
                convsum2 = 0;
                convsum3 = 0;
                for(j = 0;j < 100;j++){
                    convsum1 += filter_window[0][j][i]*buff_M_filtered[(pos_buff+j+1)%100];
                    convsum2 += filter_window[1][j][i]*buff_M_filtered[(pos_buff+j+1)%100];
                    convsum3 += filter_window[2][j][i]*buff_M_filtered[(pos_buff+j+1)%100];
                }
                convres = sqrt(convsum1*convsum1+convsum2*convsum2+convsum3*convsum3);
                result_file << convres;
                if(i < 99){
                    result_file << ",";
                }
            }
            result_file << "\n";




            pos_buff++;
            if(pos_buff > 99){
                pos_buff = 0;
            }
        }
    }

    clock_t endTime = clock();
    clock_t elapsed = endTime - startTime;
    double timeInSecond = (double)(elapsed/CLOCKS_PER_SEC);
    cout << elapsed << endl;
    return 0;
}

double getf1(double d){
    double c1,c2,c3;
    c1 = sqrt(128.0/M_PI/3.0);
    c2 = d*d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double getf2(double d){
    double c1,c2,c3;
    c1 = sqrt(128.0/M_PI/5.0);
    c2 = d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double getf3(double d){
    double c1,c2,c3;
    c1 = sqrt(24.0/M_PI/5.0);
    c2 = 1-(5.0/3.0)*d*d;
    c3 = get1x52(d);
    return c1*c2*c3;
}

double get1x52(double d){
    return pow(1+d*d,-2.5);
}

int prevpos(int a){
    if(a == 0) return 99;
    return a-1;
}

int centpos(int a){
    if(a < 50) return a+50;
    return a-50;
}


/*

Dongwoo Lee
오후 10:15
1. 고정 변수 define으로 헤더에 정의
2. 전역변수 사용 자제 
3. 계산 시간 측정시 파일저장 제외
4. 2중 for문 사용시 안쪽 for문의 변수를 가장 array의 안쪽으로 
5. array 사용시 1차원 array 사용이 적합함
6.  main문 에 줄 코드가 아니라 함수 형태로 간단히 정의 
7. array 안에 비슷한 값을 사용시에 사전에 정의해서 적용
8. 변수명 이름 규칙 (대문자 쓸지? / 아래바 쓸지 통일해야함)
9. 하드 코딩 하지 말것 for 문에 들어가는 100 같은 숫자는 다 define 에서 정의해서 변수로 사용해야함.

*/