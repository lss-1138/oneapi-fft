
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<mpi.h>
#include<fstream>
#include<sstream>
#include<stdlib.h>
//#define N 8

using namespace std;

//定义复数的结构 
typedef struct {
    double real;
    double imag;
} Complex;


//复数的乘法运算  
Complex multiply(Complex* a, Complex* b)
{
    Complex c;
    c.real = (a->real * b->real) - (a->imag * b->imag);
    c.imag = (a->real * b->imag) + (b->real * a->imag);
    return c;
}

//复数的加法 
Complex add(Complex* a, Complex* b)
{
    Complex c;
    c.real = a->real+b->real;
    c.imag = a->imag+b->imag;
    return c;
}


int main(int argc, char** argv)
{
    int N=stoi(argv[1]);
    int size, rank;int NN,i;int k;int n;
    Complex Input[N];
    MPI_Init(NULL, NULL);//初始化 
    MPI_Comm_size(MPI_COMM_WORLD, &size);//获取进程数 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);//获取进程编号 
    int localN = (N/2)/size;//每个节点分配的数目 
    double EulerR[N/2], EulerI[N/2];
    double *ResultR = NULL, *ResultI = NULL;
    double tempEulerI[localN], tempEulerR[localN];
    double tempResultR[localN*2], tempResultI[localN*2];        
    struct timespec now, tmstart;//定义时间结构体，struct timespec精确到微秒级别 
    double mpist, mpiend;

    //从文件中读取向量输入       
    NN = N;
    string fileName="./data/";
    fileName+=to_string(NN);
    fileName+=".txt";
    ifstream in(fileName);
    string line;
    for(int i = 0;i<NN;i++){
    getline(in,line);
    stringstream ss(line);
    ss>> Input[i].real;
    ss>> Input[i].imag;
    }

    if(rank==0)
    {
        ResultR = new double[N];//进程0开辟一段地址来存结果 
        ResultI = new double[N];
        clock_gettime(CLOCK_REALTIME, &tmstart);
        mpist = MPI_Wtime();//MPI 计时函数
    }


    //计算欧拉数，即复单位圆周上的等分点值，安装不同进程计算，然后再聚合一起 
    double theta, ang = 4.0*M_PI/N;//计算一般点数的欧拉值 
    int x;
    for(x = 0; x < localN; x++){
        theta = (localN*rank + x)*ang;//利用进程号计算欧拉值 
        tempEulerR[x] = cos(theta);
        tempEulerI[x] = -sin(theta);
    }

    //做一个全收集，使得每个进程上都有欧拉值 ，因为Allgather的机制，不会乱序 
    MPI_Allgather(tempEulerR, localN, MPI_DOUBLE, EulerR, localN, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Allgather(tempEulerI, localN, MPI_DOUBLE, EulerI, localN, MPI_DOUBLE, MPI_COMM_WORLD);

    Complex even, odd;

    Complex twiddle, temp, euler, result;

    double PI2_by_N = 2*M_PI/N;

    int diff, idx;

    for(x = 0; x < localN; x++)
    {
        k = rank*localN + x;
        even.real = even.imag = odd.real = odd.imag = 0.0;//初始化为0 
        diff = (k - 1 +(N>>1)) % (N>>1);
        idx = 0; 

        for(n = 0; n < (N>>1); n++){
            euler.real = EulerR[idx];
            euler.imag = EulerI[idx];
            temp = multiply(&Input[n<<1], &euler);         
            even = add(&even, &temp);
            temp = multiply(&Input[(n<<1) +1], &euler);  
            odd = add(&odd, &temp);
            idx = (idx + diff + 1) % (N>>1);
        }

        theta = k*PI2_by_N;
        twiddle.real = cos(theta);
        twiddle.imag = -sin(theta);
        temp = multiply(&odd, &twiddle);

        result = add(&even, &temp);
        tempResultR[x] = result.real;
        tempResultI[x] = result.imag;

        temp.real = -temp.real;
        temp.imag = -temp.imag;
        result = add(&even, &temp);
        tempResultR[x+localN] = result.real;
        tempResultI[x+localN] = result.imag;
    }


    //通过收集，把结果都收集都进程0上面来 
    MPI_Gather(tempResultR, localN, MPI_DOUBLE, ResultR, localN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(tempResultI, localN, MPI_DOUBLE, ResultI, localN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Gather(tempResultR+localN, localN, MPI_DOUBLE, ResultR+(N/2), localN, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Gather(tempResultI+localN, localN, MPI_DOUBLE, ResultI+(N/2), localN, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    //输出结果
    if(rank == 0)
    {
        mpiend = MPI_Wtime();
        clock_gettime(CLOCK_REALTIME, &now);

        printf("problem size : %d\n", N);
        printf("Number of processes : %d\n", size);

        printf("run time: %.5f s\n", mpiend-mpist);

        printf("source: ");
        for(k = 0; k <= NN-1; k++){
            printf("%.3f+%.3fi\t", Input[k].real,Input[k].imag);
        }

        printf("\nresult: ");
        for(k = 0; k <= NN-1; k++){
            printf("%.3f+%.3fi\t", ResultR[k],ResultI[k]);
        }
    }
    MPI_Finalize();
}
