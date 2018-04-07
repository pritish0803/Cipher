#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cmath>


using namespace std;
void ec_points(int a, int b, int p)
{
cout << "Points of Elliptic Curve" << endl;
cout << "---------------------------------------------------------" << endl;
for (int x = 0; x < p; x++)
  {
    for (int y = 0; y < p; y++)
      {
  int k = y * y;
  int m = (x * x * x) + a * x + b; if (k % p == m % p)
      {
        cout << "(" << x << "," << y << ")" << endl;
      }
    }
  }
}
static int EGCD(int a, int b, int& u, int &v)
{
  u=1; v=0; int g=a; int u1=0; int v1=1; int g1=b;
  while (g1 != 0)
  {
  int q = g/g1;
  int t1 = u - q*u1;
  int t2 = v - q*v1;
  int t3 = g - q*g1; u = u1; v = v1; g = g1; u1 = t1; v1 = t2; g1 = t3; }
  return g; 
}

static int InvMod(int x, int n) // Solve linear congruence equation x * z == 1 (mod n)for z
{
  n = abs(n);
  x = x % n; // % is the remainder function, 0 <= x % n < |n|
   int u,v,g,z;
    g = EGCD(x, n, u,v);
  if (g != 1)
  { z = 0;
  }
  else
  {z = u % n;
  }
  return z;
}

int NegMod (int a, int p)
{
  int b = a * -1;
  int n = ceil((float)b / p);
  return (n * p) - b;
}


void add_points (int xp, int yp, int xq, int yq, int &xr, int &yr, int p)
{
  int s;
  int n = yp - yq;
  int d = xp - xq;
  if (d < 0)
  {
   n*=-1; d*=-1;
  }
  int x = InvMod(d, p);
  if (n * x > 0)
  {
  s = (n * x) % p; }
  else {
  s = NegMod(n * x, p);
  }
  int xr_ = (s * s - xp - xq);
  if (xr_ < 0)
  xr = NegMod (xr_, p);
  else
  xr = xr_ % p;
  int yr_ = (-yp + s * (xp - xr));
  if (yr_ < 0)
  yr = NegMod(yr_, p);
  else
  yr = yr_ % p; }
void add_double (int xp, int yp, int &xr, int &yr, int a, int p)
{
  int s;
  int n = 3 * xp * xp + a;
  int d = 2 * yp;
  if (d < 0) {
  n *= -1; d *= -1; }
  int x = InvMod(d, p);

   if (n * x > 0)
  {
    s = (n * x) % p;
  }
  else
  {
    s = NegMod(n * x, p);
  }
  int xr_ = (s * s - 2 * xp);
  if (xr_ < 0)
  xr = NegMod (xr_, p);
  else
  xr = xr_ % p;
  int yr_ = (-yp + s * (xp - xr));
  if (yr_ < 0)
  yr = NegMod(yr_, p);
  else
  yr = yr_ % p;
}
void scalar_multiplicaiton (int xp, int yp, int k, int a, int p, int &PUx, int &PUy)
{
  if (k == 2) {
  add_double(xp, yp, PUx, PUy, a, p);
  }
  else if (k > 2) {
  add_double(xp, yp, PUx, PUy, a, p);
  for (int i = 0; i < k - 2; i++)
  {
  int xq=PUx; int yq=PUy; PUx=PUy=0;

   add_points(xp, yp, xq, yq, PUx, PUy, p); }
  } 
  else {
  cout << "Wrong key" << endl; }
}
void key_generation (int Px, int Py, int k, int a, int p, int &PUx, int &PUy)
{
  scalar_multiplicaiton(Px, Py, k, a, p, PUx, PUy);
  return; 

}


void encryption (int Mx, int My, int k, int a, int p, int PUx, int PUy, int &Cx, int &Cy) 
{
  int xr, yr;
  scalar_multiplicaiton(PUx, PUy, k, a, p, xr, yr);
  add_points(Mx, My, xr, yr, Cx, Cy, p);
}


void decryption (int Cx, int Cy, int k, int a, int p, int x1, int y1, int &Mx, int &My) 
{
  int xr, yr;
  scalar_multiplicaiton(x1, y1, k, a, p, xr, yr);
  add_points(Cx, Cy, xr, -yr, Mx, My, p);
}


void Ecc()
{
  int a, b, p;


   cout << "put a prime number: ";
   cin >> p;
  bool check;


  do {
  check = false;
  cout << "put a value for a: ";
  cin >> a; cout << "put a value for b: ";
  cin >> b;

  if (((4 * a * a * a + 27 * b * b) % p) == 0) 

  {
  cout << "Your values do not satisfied the condition" << endl; 
  cout << "Please put values again" << endl;
  check = true;
  }

  } while (check);
  cout << "----------------------------------------------------------" << endl;
  ec_points(a, b, p);
  int Px, Py, PUAx, PUAy, PUBx, PUBy, Mx, My, Cx, Cy, m, n;
  cout << "---------------------------" << endl;
  cout << "Key " << endl;
  cout << "---------------------------" << endl;
  cout << "Select a base point (x,y) from the curve: ";
  cin >> Px >> Py;
  cout << "Select a private key for person1: ";
  cin >> m;
  key_generation(Px, Py, m, a, p, PUAx, PUAy);
  cout << "Public key of Person1 is (" << PUAx << "," << PUAy << ")" << endl;
  cout << "Select a private key for Person2: ";


   cin >> n;
  key_generation(Px, Py, n, a, p, PUBx, PUBy);
  cout << "Public key of Person2 is (" << PUBx << "," << PUBy << ")" << endl;
  cout << "----------------------------------------------------" << endl;
  cout << "Encryption/Decryption" << endl;
  cout << "----------------------------------------------------" << endl;
  cout << "Select a Message point (x,y) from the curve (for encryption): ";
  cin >> Mx >> My;
  int x1, y1;
  scalar_multiplicaiton(Px, Py, n, a, p, x1, y1);
  encryption(Mx, My, m, a, p, PUBx, PUBy, Cx, Cy);
  cout << "Cipher is (" << Cx << "," << Cy << ")" << endl;
  
  cout << "Person1 send message pair(("<< x1 << "," << y1 << "),(" << Cx << ","
  << Cy << "))" << endl;
  cout << "||-----------------------------------------||" << endl;
  cout << "Person2 receive the message and start decrypting" << endl; 
  decryption(Cx, Cy, m, a, p, x1, y1, Mx, My);
  cout << "Decrypted message is (" << Mx << "," << My << ")" << endl;
}


bool CheckIsPrime(long int num) {
if(num < 2) 
  return false; 
long int i = 2;
while(i <= num / 2)
{
if(!(num % i)) return false; i++;


 }
return true; }
long int Multiply(long int num1,long int num2) 
{
  return num1 * num2; 
}
bool CheckCoPrime (long int num1, long int num2)
{ long int lowest;
if (num1 > num2) lowest = num2;

else lowest = num1;

long int i = 2;
bool coprime = true; 
while (i < lowest) 
{
  if (!(num1 % i) && !(num2 % i)) coprime = false;
  i++; 
}
return coprime; 

}
long int FindE(long int phi_n) 
{
long int e = 0; 
do {
cout << "Choose an integer number e (e must be co prime of phi_n): ";
cin >> e;
} while (!CheckCoPrime(phi_n, e));

 return e; }
long int FindD(long int phi_n, long int e) 
{
int a=phi_n,b=e;
long int x=0,y=1,u=1,v=0,m,n,q,r;
long int gcd = b;
while (a != 0) {
q = gcd / a;
r = gcd % a;
m = x - u * q;
n = y - v * q;
gcd = a;
a = r;
x = u;
y = v;
u = m;
v = n;
}
if (y < 1) {
y = phi_n + y; }
return y; }

long int Encrypt_Decrypt(long int t, long int e, long int n) {
long int rem;long int x = 1; while (e != 0) {
rem = e % 2; e= e/2;
if (rem == 1) x = (x * t) % n;
t = (t * t) % n; }
return x; }
void EncDecStr (long int e, long int n) {


  char *str = new char[1000]; 
  char *str1 = new char[1000]; 
  cout << "\nEnter a string: ";
  cin >> str;
  cout << "Encrypting using Public Key: " << endl;
  int i = 0;
  while (i != strlen(str)) 
  {
  str1[i] = Encrypt_Decrypt(str[i], e, n);
  i++; }
  cout << str1 << endl; 
  }
void EncDecNum (long int n1, long int n2) 
{
  long int pn;
  cout << "\nEnter an integer number: ";
  cin >> pn;
  cout << Encrypt_Decrypt(pn, n1, n2) << endl;
}
void generate_key (long int &n, long int &e, long int &d) 
{
  long int p, q, phi_n, pt, ct; 
  do {
  cout << "Enter a prime number: ";
  cin >> p;
  } while (!CheckIsPrime(p)); 
  do {
  cout << "Enter another prime number: ";

   cin >> q;
  } while (!CheckIsPrime(q));
  n = Multiply(p,q);
  cout << "n is " << n << endl;
  phi_n = Multiply (p-1,q-1);
  cout << "phi_n is " << phi_n << endl;
  e = FindE(phi_n);
  cout << "e is " << e << endl;
  if (!e) {
  cout << "Choose two suitable prime number" << endl;
  exit(1); }
  d = FindD(phi_n, e);
  cout << "d is " << d << endl; 
}
void throuput(){
  cout<<"Throuput Ecc security "<<endl;
  cout<<"Number Of Node(n) | RSA | ECC "<<endl;
  cout<<"--------------------------------------------- "<<endl;
  int x,y;
  for(int n=10;n<=50;n+=5){
  x=323.48+5.28*n; y=318.56+9.57*n;
  cout<<n<<" "<<"|"<<floor(x)<<" | "<<floor(y)<<endl; }
}
void delay(){


 cout<<"Delay Rsa ,Ecc  "<<endl; 
 cout<<"Number Of Node(n) | RSA | ECC "<<endl; 
 cout<<"--------------------------------------------- "<<endl;
int x,y;
for(int n=10;n<=50;n+=5){ x=273.60-2.78*n; y=281.56-4.63*n;
cout<<n<<" "<<"|"<<floor(x)<<" | "<<floor(y)<<endl; }
}
void jitter(){
cout<<"Jitter Rsa ,Ecc to security  "<<endl;
cout<<"Number Of Node(n) | RSA | ECC "<<endl; cout<<"--------------------------------------------- "<<endl; int x,y;
for(int n=10;n<=50;n+=5){
x=47.62-0.94*n; y=49.62-0.78*n;
cout<<n<<" "<<"|"<<floor(x)<<" | "<<floor(y)<<endl; }
}
void rsa(){
cout << endl << endl << " R.S.A ALGORITHM USING C++ to secure message in Mobile Adhoc" << endl << endl;
long int n, d = 0, e;
generate_key(n, e, d);
cout << "Public Key : ("<<e<<","<<n<<")" << endl;


 cout << "Private Key : ("<<d<<","<<n<<")" << endl;
cout << endl << "Press 1: for encrypting numbers & 2: for encrypting string: ";
int choice;
cin >> choice;
switch (choice) {
case 1: EncDecNum(e, n);
        break;
case 2: EncDecStr(e, n);
        break;
default:
        cout << "Wrong choice. Try again." << endl;
        exit(1);
}
cout << endl << "Press 1: for decrypting numbers & 2: for decrypting string: ";
cin >> choice;
switch (choice) {
case 1: EncDecNum(d, n); break;
case 2: EncDecStr(d, n); break;
default:cout << "Wrong choice. Try again." << endl;
        exit(1); }
}
void main_enc(){ int ch;
cout << endl<<"==================== \n";
cout<<"Press 1: for  RSA \n Press 2: for  ECC \nPress any key to exit "<<endl;
cin>>ch;
switch (ch) {
case 1: rsa();break;
case 2: Ecc();break;
default: cout <<"Wrong choice. Try again." << endl;exit(1); }


}
void main_per(){
int ch;
cout << endl << "==================== ";
cout << endl << "Press 1: Throuput Ecc security  ";
cout << endl << "Press 2: Delay Ecc security ";


cout << endl << "Press any key: exit ";
cout << endl << "==================== "<<endl;
cin >> ch;
switch (ch) {
case 1:
throuput(); break;
case 2:
delay(); break;
case 3:
jitter(); break;
default:
cout << "Wrong choice. Try again." << endl; exit(1);
} }
int main() {
int ch;
cout << endl << "==================== ";
cout << endl << "Press 1: Secure Massage ";
cout << endl << "Press 2: Performance .... ";
cout << endl << "Press 3: exit ";
cout << endl << "==================== "<<endl;
cin >> ch; switch (ch) {
case 1: main_enc(); break;


case 2: main_per(); break;
default:
cout << "Wrong choice. Try again." << endl; exit(1);
}
return 0; }
