#include <iostream>
#include <cmath>
#include "codec.h"
#include <cstdlib>
#include <sys/stat.h>
#include <fstream>
#include <string>


using namespace std;

void usage ()
{
  cout << " ERROR! This won't work " << endl;
	exit (1);
}

bool isPrime ( int64_t n)	//checks if a no. is prime or not
  { 
    if ((n < 2) || (n % 2 == 0))
	return false;
    if (n == 2)
	return true;

    for (int64_t i = 3; i*i <= n; i+=2)  //checks divisibility by odd nos
       { 
	  if (n % i == 0)
		{
		  return false;
		}
	}
    return true;
}

int64_t gcd (int64_t a, int64_t b)	//the greatest common divider
{ 
  int64_t tmp;
	while( b!= 0)
	{
	  tmp = a % b;
	  a = b;
	  b = tmp;
	}
	  return a;
}

int64_t modInv(int64_t a, int64_t n)	//making a and b coprime
{
	int64_t 
		t = 0, tNew = 1, r = n, rNew = a, q, tmp;

  while (rNew != 0)
	{
	  q = r / rNew;

	  tmp = t - q * tNew;
	  t = tNew;
	  tNew = tmp;

	  tmp = r - q * rNew;
	  r = rNew;
	  rNew = tmp;
	}

  if (t < 0 )
	t +=n;
  return t;
}

int64_t modExp (int64_t b, int64_t e, int64_t m)
{
  int64_t ans = 1;

  while (e != 0)
  {
	if (e % 2 == 1)
  	  ans = ( ans * b) % m;
	b = (b * b)%m;
	e = e/2;
  }
  return ans;
}


int64_t getfSize(char *fn) {
    struct stat
        fs;

    stat(fn,&fs);

    return fs.st_size;
}

void keyGen (void)
{
  int64_t p,q,e=0,f=0,d=0,g=0,n;

  do {
    p = rand() % (65536) + 4096;
      }
  while (!isPrime(p));
  cout << " P is " << p << endl;
  do {
	q = rand() % (65536) + 4096;
     }
  while (!isPrime(q));

  f = (p - 1) * (q - 1);	//computing f: coprime of p and q
  n = p*q;

  do {
      e = rand() % (65536) + 4096;}
  while (gcd(e,f) !=1);
  cout << " q is " << q << " e is " << e << " F is " << f << endl;
  g = modInv(e,f);

  if ((g*e)%f != 1)
    {
	cout << "Error, e and f aren't coprime" << endl;
	return;
    }


  cout << p << " " << q << " " << f << endl;

  
  cout << "The Public keys are:" << " " << n << " " << e << endl;
  cout << "The Private Keys are: " << " " << n << " " << g << endl;}
 



void encrypt (char *filein, char *fileout, int64_t n, int64_t e){

  int64_t m,M,fSize;
  uint8_t c1,c2,c3;

  Codec64 codec;

  ifstream inputfile;
  ofstream outputfile;
  
  inputfile.open(filein);
  if (!outputfile)
    {
	cout << "ERROR! " << endl;
    }

  outputfile.open(fileout);
  if (!outputfile)
  {
	cout << "ERROR! " << endl;
  }

  codec.beginEncode();

  while (fSize > 0) {
    c2 = c3 = 0;
    c1 = inputfile.get();
  
  fSize--;
  if (fSize >= 0) {
  c2 = inputfile.get();
  fSize--;
  }

  if (fSize >= 0)
	{
	c3 = inputfile.get();
	fSize--;
	}

  m = c1 + 256 * (c2 + 256*c3);
  M = modExp(m,e,n);

codec.put64(M);
  while(codec.getEncodedChar(c1))
    outputfile << c1;

 

  while(codec.getEncodedChar(c3))
    outputfile << c3;

  inputfile.close();
  outputfile.close();
}
codec.endEncode();
}

void decrypt(char *filein, char *fileout, uint64_t n, uint64_t d){

  uint64_t m, M, fsize;
  uint8_t c1, c2, c3;

  Codec64 codec;

  ifstream inputfile;
  ofstream outputfile;

  inputfile.open( filein );
  if (!inputfile) {
    cout << " ERROR " << endl;
  }

   outputfile.open( fileout );
  if (!outputfile) {
    cout << " ERROR " << endl;
  }
  
  codec.beginDecode();
  while (!codec.get64(fsize)){
  	inputfile >> c1;
	codec.putEncodedChar(c1);
	}
  while (fsize > 0) 
  	{
	  while (! codec.get64(M))
		{
		  inputfile >> c1;
		}

  	  m = modExp(M,d,n);

  	  c1 = m % 256;
	  outputfile << c1;
	  fsize--;
	  m = m/256;
	}

	inputfile.close();
	outputfile.close();
	codec.endDecode();

}


int main (int argc, char *argv[]) 
{
  int64_t n,e,d;

  if (argc < 2)
  {
    usage();
     return 1;
  }
  if (argv [1][0] != '-')
  { 
    usage();
    return 1;
  }
  
  switch (argv[1][1]) {
	case 'k':
	  if (argc != 2)
	    {
	  	usage();
	    }
	  keyGen();
	  break;

	case 'e':
	  if (argc != 6)
	     {usage();}
	  n = atoll (argv[2]);
	  e = atoll (argv[3]);
	    //encrypt (argv[4], argv[5], n, e};
	    break;

	case 'd':
	  if (argc != 6)
	      usage();
	  n = atoll (argv[2]);
	  d = atoll (argv[3]);
	 //decrypt (argv[4], argv[5], n, d);
	  break;
	default:
		usage();
	}
}
				
























	  
