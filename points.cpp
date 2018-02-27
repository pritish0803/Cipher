#include<iostream>
#include<vector>
#include<math.h>
#define modulo(a,b) ( (((a)%(b))+(b)) % (b))
using namespace std;

struct point{
	int xx;
	int yy;
};

class Encription{
	private:
		vector <int> X;
		vector <int> Y;
	public:
		Encription(){
			this->X.clear();
			this->Y.clear();
		}
		void append_X(int);
		void append_Y(int);
		int get_X(int);
		int get_Y(int);
		int size_X();
		int size_Y();
};
void ellipticCurve_points(int,int,int,Encription &);
void display(Encription &);
Encription generateCipher(int, int, Encription &);
struct point pointMultipication(int, int, int, int, int, int);
int inverseModulo(int,int);
int gcd(int a, int b);
int power(int x, unsigned int y, unsigned int m);


void Encription::append_X(int data){
	X.push_back(data);
}
void Encription::append_Y(int data){
	Y.push_back(data);
}
int Encription::get_X(int index){
	return X.at(index);
}
int Encription::get_Y(int index){
	return Y.at(index);
}
int Encription::size_X(){
	return X.size();
}
int Encription::size_Y(){
	return Y.size();
}

int main(){
	Encription obj;
	int p,a,b;
	cout<<"Elliptical curve: Y*Y=(X*X*X) + a*X + b"<<endl;
	cout<<"enter P a b:"<<endl;
	cin>>p>>a>>b;
	ellipticCurve_points(p,a,b,obj);
	//display(obj);
	Encription cipher;
	cipher=generateCipher(p,a,obj);
	for(int i=0;i<cipher.size_X();i++)
		cout<<"Ciper Point :"<<cipher.get_X(i)<<" "<<cipher.get_Y(i)<<endl;
	return 0;
}

int inverseModulo(int a, int m)
{
	while(a<0){
		a=a+m;
	}
	int x,g = gcd(a, m);
	if (g != 1)
		cout << "Inverse doesn't exist of : "<<a<<endl;
	else
	{
		x=power(a, m-2, m);
		cout << "Modular multiplicative inverse of: "<<a<<" is "
			<< x<<endl;
	}
	return x;
}

// To compute x^y under modulo m
int power(int x, unsigned int y, unsigned int m)
{
	if (y == 0)
		return 1;
	int p = modulo(power(x, y/2, m),m);
	p = modulo((p * p),m);

	return (modulo(y,2) == 0)? p : modulo((x * p),m);
}

// Function to return gcd of a and b
int gcd(int a, int b)
{
	if (a == 0)
		return b;
	return gcd(modulo(b,a), a);
}



void ellipticCurve_points(int p,int a,int b,Encription &obj){
		int expr1,expr2;
		for(int y=0;y<13;y++){
			expr1=pow(y,2);
			expr1=expr1 % 13;
			for(int x=0;x<13;x++){
				expr2=pow(x,3)+(a*x)+(b);
				expr2=modulo(expr2,p);
				if(expr2==expr1)
					obj.append_X(x);
					obj.append_Y(y);
			}
		}
	}

void display(Encription &obj){
	for(int i=0;i<obj.size_X();i++){
		cout<<obj.get_X(i)<<" "<<obj.get_Y(i)<<endl;
	}
}

Encription generateCipher(int p, int a, Encription &obj){
	Encription temp;
	struct point e1={0,0};
	struct point e2={0,0};
	struct point C1={0,0};
	struct point C2={0,0};
	struct point P={0,0};

	//choose e1
	int idx = rand() % obj.size_X();
	e1.xx = obj.get_X(idx);
	e1.yy = obj.get_Y(idx);
	//check if e1.yy is "0". if YES rechoose e1
	while(e1.yy==0){
		idx = rand() % obj.size_X();
		e1.xx = obj.get_X(idx);
		e1.yy = obj.get_Y(idx);
	}

	//choose a random "d"
	int d = (rand() % p) + 1;



	//testing******************************************
	//delete these later********************************
	//e1.xx=2;
	//e1.yy=22;
	//d=4;
	

	//calculate e2=d*e1
	e2.xx=e1.xx;
	e2.yy=e1.yy;
	for(int i=0;i<d;i++)
		e2=pointMultipication(e2.xx,e1.xx,e2.yy,e1.yy,p,a);


	//Create CIPHER C1=r*e1
	//sender selects a r
	int r;
	cout<<"Choose a key value(r): ";
	cin>>r;
	C1.xx=e1.xx;
	C1.yy=e1.yy;
	for(int i=0;i<r;i++)
		C1=pointMultipication(C1.xx,e1.xx,C1.yy,e1.yy,p,a);
	temp.append_X(C1.xx);
	temp.append_Y(C1.yy);


	//Create CIPHER C2=P+(r*e2)
	cout<<"enter the Plain Point : ";
	cin>>P.xx>>P.yy;
	struct point expr={0,0}; //expr=(r*e2);
	expr.xx=e2.xx;
	expr.yy=e2.yy;
	for(int i=0;i<r;i++)
		expr=pointMultipication(expr.xx,e2.xx,expr.yy,e2.yy,p,a);
	C2=pointMultipication(P.xx,expr.xx,P.yy,expr.yy,p,a);
	temp.append_X(C2.xx);
	temp.append_Y(C2.yy);
	return temp;
}


 struct point pointMultipication(int x2, int x1, int y2, int y1,int p, int a){
 	struct point temp1={0,0};
 	long long int expr1,expr2,expr,lambda;
 	if(x1==x2){
 		expr1=(3*pow(x1,2)+ a);
 		expr1=modulo(expr1,p);
 		expr2=inverseModulo((2*y1),p);
 	}
 	else{
 		expr1=(y2-y1);
 		expr1=modulo(expr1,p);
 		expr2=inverseModulo((x2-x1),p);
 	}
 	expr=expr1 * expr2;
 	lambda=modulo(expr,p);
 	temp1.xx=pow(lambda,2)-x1-x2;
 	temp1.yy=lambda*(x1-temp1.xx) - y1;
 	cout<<"lambda: "<<lambda<<endl;
 	cout<<"xx :"<<temp1.xx<<" yy:"<<temp1.yy<<endl;
	return temp1;
}





