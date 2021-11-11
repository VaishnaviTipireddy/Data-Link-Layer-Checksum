// input libaries
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <iostream>
#include <math.h>
using namespace std;


//function declarations
int checksum(string msg, string gen);
int validate (string checksum, string gen);
int fourBitErrors(string msg, string gen);
int twoBitErrors(string msg, string gen);

//check command line option
int main(int argc, char **argv){
  int a;
  string gen = "10001000010100001";
  while ((a = getopt(argc, argv, "c:v:f:t:")) != -1){
    //switch statement where I call diff funcs
    //use getopt to parse arguments
    switch(a){
     case 'c':
       //call checksum
       checksum(optarg, gen);
       break;
     case 'v':
       //call validate
       validate(optarg, gen);
       break;
     case 'f':
       //call fourbiterrors
       fourBitErrors(optarg, gen);
       break;
     case 't':
       //call twobiterrors
       twoBitErrors(optarg, gen);
       break;
     default:
       return 2;
    }
  }

  if (optind == 1) //no argument specified
    return 2;
  return 0;
}

//XOR block and gen
string do_xor(string block, string gen){
  
  int len = gen.length();
  string rem = "";
  for (int i = 0; i < len; i++)
    {
      if (block[i] == gen[i])
	rem += '0';
      else
	rem += '1';

    }
  return rem;
} 


//recursively compute remainder of mod 2 arithmetic
string mod2(string msg, string gen, unsigned int genlen){

  //base case 
  if (msg.length() < genlen)
    return msg;
  

  if (msg[0] == '1'){
    string block = msg.substr(0, genlen);
    string ret = do_xor(block, gen);
    ret = ret.substr(1); 
    return mod2(ret.append(msg.substr(genlen)), gen, genlen);  
  }else{
    return mod2(msg.substr(1), gen, genlen);
  }


}


// -c  compute checksum
string checksum2(string msg, string gen){

  int genlen = gen.length();

  //left shift
  string leftshift = msg;
  for (int i = 1; i < genlen; i++)
    leftshift += '0';
  
  //compute mod 2 remainder
  string rem = mod2(leftshift, gen, genlen);
  
  //add remainder to msg
  string checksum = msg + rem;
  
  return checksum;
}

int checksum(string msg, string gen){
  string val = checksum2(msg, gen);
  cout << val << endl;
  return 0;

}

//-v, check if string is valid with attached crc
int validate2(string checksum, string gen){
  int genlen = gen.length();
  string rem = mod2(checksum, gen, genlen); 
  
  int retval = 1;
  for (int i = 0; i < genlen -1; i++){ //if all remainder bits are 0, output 1, else 0
    if (rem[i] != '0'){
      retval = 0;
      break;
    }
  }
  
  return retval;
}

int validate(string checksum, string gen){
  
  int retval = validate2(checksum, gen);
  cout << retval << endl;
  return 0;

}
// -f, output all undetected 4 bit errors (divisble by CRC)
int fourBitErrors(string msg, string gen){
  
  string chksum = checksum2(msg, gen);
  int len = chksum.length();
  string error = "";

  for (int x = 0; x < len; x++)
    error +='0';

  //check if possible 4-bit errors are undetected
  for (int i = len - 16 - 1; i >= 0; i--){
    error[i] = '1';
    for (int j = len - 12 - 1; j > i; j--){
      error[j] = '1';
      for (int k = len - 7 - 1; k > j; k--){
	error[k] = '1';
	for (int l = len - 5 - 1; l> k; l--){
	  error[l] = '1';
	  if (validate2(error, gen) == 1) //4-bit error is a multiple of generator
	    cout << do_xor(chksum, error) << endl; 
	  error[l] = '0';
	}
	error[k] = '0';
      }
      error[j] = '0';
    }
    error[i] = '0';
  }
  return 0;
}


//-t, fraction of undetected two bit errors
int twoBitErrors(string msg, string gen){
  
  string chksum = checksum2(msg, gen);
  int len = chksum.length();
  
  //compute total possible errors (len choose 2)
  double total = len * (len -1);
  total = total / 2;
  double undetected = 0;
  
  string error = "";
  for (int x = 0; x < len; x++)
    error +='0';

  //check if possible 2-bit errors are undetected
  for (int i = len - 16 - 1; i >= 0; i--){
    error[i] = '1';
    for (int j = len - 12 - 1; j > i; j--){
      error[j] = '1';
      if (validate2(error, gen)){
	//cout << do_xor(chksum, error) << endl; 
	undetected += 1; 
      }
      error[j] = '0';
    } 
    error[i] = '0';
  }

  cout << undetected / total << endl; 
  
  return 0;
}
