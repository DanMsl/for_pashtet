#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
using namespace std;

//Функция транспонирования матрицы
 double** TransponMtx( double**matr, int n)
 {
	 for (int i = 0; i < n; i++){ 
        for (int j = 0; j < n; j++){
			cout<<matr[i][j]<<" ";
		}
		cout<<endl;
		}
	 double buf;
    for (int i = 0; i < n; i++){ 
        for (int j = 0; j < n; j++){
			buf = matr[j][i];
            matr[j][i] = matr[i][j];
			matr[i][j] = buf;
		}
		}
	return matr;
}
//Функция освобождения памяти
 void FreeMem(double **matr, int n)
{
    for(int i = 0; i < n; i++)
        delete [] matr[i];
    delete [] matr;
}
//функция вычеркивания строки и столбца
double** Get_matr(double **matr, int n, double **temp_matr, int indRow, int indCol)   
{
    int ki = 0; 
    for (int i = 0; i < n; i++){
        if(i != indRow){
            for (int j = 0, kj = 0; j < n; j++){
                if (j != indCol){
                    temp_matr[ki][kj] = matr[i][j];
                    kj++;
                }
            }
            ki++;       
        }
    }
return temp_matr;
}
//функция вычисления определителя матрицы
/*double Det(double **matr, int n)  
{
	//cout<<"hi"<<endl;
    double temp = 0;   //временная переменная для хранения определителя
    int k = 1;      //степень
	cout<<n<<endl;
    if(n < 1){
        cout<<"Не верный размер матрицы!!!" << endl;
        return 0;
    }
    else if (n == 1)
        temp = matr[0][0];
    else if (n == 2)
        temp = matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1];
    else{
        for(int i = 0; i < n; i++){
            int m = n - 1; 
            double **temp_matr = new double * [m];
            for(int j = 0; j < m; j++)
                temp_matr[j] = new double [m];
            Get_matr(matr, n, temp_matr, 0, i);
            temp = temp + k * matr[0][i] * Det(temp_matr, m);
            k = -k;
            FreeMem(temp_matr, m);
			//cout<<"hi"<<i<<endl;
        }
    }
	//cout<<"hi123"<<endl;
    return temp;
}*/
// Функция вывода матрицы
void PrintMatr(double **mas, int m) {
  int i, j;
  for (i = 0; i<m; i++) {
    for (j = 0; j<m; j++)
      cout << mas[i][j] << " ";
    cout << endl;
  }
}
// Получение матрицы без i-й строки и j-го столбца
void GetMatr(double **mas, double **p, int i, int j, int m) {
  int ki, kj, di, dj;
  di = 0;
  for (ki = 0; ki<m - 1; ki++) { // проверка индекса строки
    if (ki == i) di = 1;
    dj = 0;
    for (kj = 0; kj<m - 1; kj++) { // проверка индекса столбца
      if (kj == j) dj = 1;
      p[ki][kj] = mas[ki + di][kj + dj];
    }
  }
}

double Det(double **mas, int m) {
  int i, j, k, n;
  double **p;
  double d;
  p = new double*[m];
  for (i = 0; i<m; i++)
    p[i] = new double[m];
  j = 0; d = 0;
  k = 1; //(-1) в степени i
  n = m - 1;
  if (m<1) cout << "Определитель вычислить невозможно!";
  if (m == 1) {
    d = mas[0][0];
    return(d);
  }
  if (m == 2) {
    d = mas[0][0] * mas[1][1] - (mas[1][0] * mas[0][1]);
    return(d);
  }
  if (m>2) {
    for (i = 0; i<m; i++) {
      GetMatr(mas, p, i, 0, m);
      //cout << mas[i][j] << endl;
      //PrintMatr(p, n);
      d = d + k * mas[i][0] * Det(p, n);
      k = -k;
    }
  }
  return(d);
}
double* otvety(double *b,double*y,int k, int n)
{
	b[0]=y[0];
	b[k-2]=y[n-1];
	int nome=1;
	b[k-1]=0;
	int str=0;
	while (str < (k-3))
	{
		b[str]=y[nome];
		b[str+1]=y[nome];
		b[str+2]=0;
		b[str+3]=0;
		nome=nome+1;
		str=str+4;
	}

	return b;
}
double** SetMtx( double**a,double*x, int k, int n)
{
  a[0][0]=x[0]*x[0]*x[0];
	a[0][1]=x[0]*x[0];
	a[0][2]=x[0];
	a[0][3]=1;
	a[1][0]=6*x[0];
	a[1][1]=2;
	a[k-2][k-4]=x[n-1]*x[n-1]*x[n-1];
	a[k-2][k-3]=x[n-1]*x[n-1];
	a[k-2][k-2]=x[n-1];
	a[k-2][k-1]=1;
	a[k-1][k-4]=6*x[n-1];
	a[k-1][k-3]=2;
	int nom=1;
	int pal=2;
	int stl=0;
	while (stl<(k-4))
	{
		a[pal][stl]=x[nom]*x[nom]*x[nom];
		a[pal][stl+1]=x[nom]*x[nom];
		a[pal][stl+2]=x[nom];
		a[pal][stl+3]=1.0;
		a[pal+1][stl+4]=x[nom]*x[nom]*x[nom];
		a[pal+1][stl+5]=x[nom]*x[nom];
		a[pal+1][stl+6]=x[nom];
		a[pal+1][stl+7]=1.0;
		a[pal+2][stl]=x[nom]*x[nom]*3.0;
		a[pal+2][stl+1]=x[nom]*2.0;
		a[pal+2][stl+2]=1.0;
		a[pal+2][stl+3]=0.0;
		a[pal+2][stl+4]=x[nom]*x[nom]*(-3.0);
		a[pal+2][stl+5]=x[nom]*(-2.0);
		a[pal+2][stl+6]=-1.0;
		a[pal+3][stl]=x[nom]*6.0;
		a[pal+3][stl+1]=2.0;
		a[pal+3][stl+2]=0.0;
		a[pal+3][stl+3]=0.0;
		a[pal+3][stl+4]=x[nom]*(-6.0);
		a[pal+3][stl+5]=2.0*(-1.0);
		nom=nom+1;
		pal=pal+4;
		stl=stl+4;
	}
 return a;
 }
double** obrach(double**matr,int n)
{
	
    double **obr_matr = new double * [n];
    double **tobr_matr = new double * [n];
    for(int i = 0; i < n; i++){
        obr_matr[i] = new double[n];
        tobr_matr[i] = new double[n];
    }
	cout<<"hi"<<endl;
   double det = Det(matr, n);
   cout<<"hi"<<endl;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                int m = n - 1;
               double**temp_matr = new double * [m];
                for(int k = 0; k < m; k++)
                    temp_matr[k] = new double[m];
                Get_matr(matr, n, temp_matr, i, j);
                obr_matr[i][j] = pow(-1.0, i + j + 2) * Det(temp_matr, m) / det;
				FreeMem(temp_matr, m);
            }
        }   
    //Транспонирование матрицы
		cout<<"hi"<<endl;
    tobr_matr=TransponMtx(obr_matr, n);
	return tobr_matr;
}
int main()
{   int n,k;
	n=5;
  double  *x=new double[n];
	double *y=new double[n];
	k=(n-1)*4;
	x[0]=1;
	x[1]=2;
	x[2]=3;
	x[3]=5;
	x[4]=7;
	y[0]=5;
	y[1]=3;
	y[2]=1;
	y[3]=2;
	y[4]=7;
	double **a=new double*[k];
	for(int i=0;i<k;i++)
	{
		a[i]=new double[k];
	}
	for(int i=0;i<k;i++)
	{
	for(int j=0;j<k;j++)
	{
		a[i][j]=0;
	}
	}
	double* b=new double[k];
	for(int i=0;i<k;i++)
	{
		b[i]=0;
	}
	double* promka=new double[k];
	promka=otvety(b,y,k,n);
	double **matr=new double*[k];
	for(int i=0;i<k;i++)
	{
		matr[i]=new double[k];
	}
	matr=SetMtx(a,x,k,n);
	double**inv=new double*[k];
	for(int i=0;i<k;i++)
	{
		inv[i]=new double[k];
	}
	inv=obrach(matr,k);
	for(int i=0;i<k;i++)
	{
		for(int j=0;j<k;j++)
	{
		cout<<inv[i][j]<<" ";
		}
		cout<<"\n";
	}

	system("pause");
	for(int i = 0; i < k; i++)
	{
        delete [] matr[i];
	}
    delete  []matr;
	for(int i = 0; i < k; i++)
	{
        delete [] a[i];
	}
    delete  []a;
	delete  []x;
	delete  []y;
	delete []b;
	delete[]promka;
	 FreeMem(inv, k);
	 system("pause");
return 0;
}
 