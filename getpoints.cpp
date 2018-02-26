#include<iostream>
#include<math.h>
using namespace std;

int main(){
	int expr1,expr2;
	int count=0;
	for(int y=0;y<13;y++){
		expr1=pow(y,2);
		expr1=expr1 % 13;
		for(int x=0;x<13;x++){
			expr2=pow(x,3)+(1*x)+(1);
			expr2=expr2 % 13;
			if(expr2==expr1)
				cout<<++count<<"). POINT: "<<x<<" "<<y<<endl;
		}
	}
	return 0;
}