#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <vector>
#include <set>
#include <algorithm>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

struct net
{
	long network;
	int range;
	string as;
};

union iptolint
{
    char ip[16];
    unsigned long int addr;
};

struct sockaddr_in intIP;

int getdir (string dir, vector<string> &files);

bool compareNetwork(net &a, net &b);

bool Binary_Search(const vector< int> &my_numbers, int key);

unsigned long int conv(char []);

uint32_t get_mask( const int mask_length);

int main()
{
	// initialize variables
	ifstream fin1, fin2, fin3;
	ofstream fout1, fout2, fout3;
	string temp, token, delimiter, newNet, item, target, dir, inputFileName, line, src, ip;
	uint32_t network, mask;
	char i;
	int range, num, hop, counter = 0;
	net tempnet;
	bool flag = true, matchflag = false, result;

    // struct to convert ip to int
    union iptolint ipl;

	// initialize stringstream object
	stringstream ss;

	// initialize map to hold AS and subnet data
	vector<net> subnetMap;

	// initialize vector to hold subnet values
	vector<string> tempVect;

    // initialize vector to hold traces
    vector<string> traceVect;    

    // initialize vector to hold file names
    vector<string> files = vector<string>();

    // define directory
    dir = "/home/jay/Ingress/as-list-test";

    // call function to get file names
    getdir(dir,files);

    // iterate through file names and get subnets
    for (int i = 0; i < files.size(); i++)
    {
        fin1.open("/home/jay/Ingress/as-list/" + files[i]);
        while (getline(fin1, temp))
        {
        	ss << temp;
        	while (getline(ss, token, ','))
        	{
        		tempVect.push_back(token);
        	}

        	net newNet;
        	newNet.network = stol(tempVect[1]);
        	newNet.range = stoi(tempVect[2]);
        	newNet.as = tempVect[0];
        	subnetMap.push_back(newNet);
        	tempVect.clear();
        	ss.clear();

        }

        fin1.close();

    }

    // sort subnetMap based on network rank
    sort (subnetMap.begin(), subnetMap.end(), compareNetwork);

    while (flag == true)
    {
        for (int i = 0; i < subnetMap.size(); i++)
        {
            flag = false;
        	if ((subnetMap[i].network == subnetMap[i+1].network) && (subnetMap[i].range > subnetMap[i+1].range))
        	{
        		subnetMap.erase(subnetMap.begin()+(i));
                flag = true;
        	}

            if ((subnetMap[i].network == subnetMap[i+1].network) && (subnetMap[i].range < subnetMap[i+1].range))
            {
                subnetMap.erase(subnetMap.begin()+(i+1));
                flag = true;
            }
        }
        for (int j = 0; j < subnetMap.size(); j++)
        {
            cout << subnetMap[j].network << ' ' << subnetMap[j].range << endl;
        }
    }

    // iterate through all input files
    for (int x = 1; x < 2; x++)
    {
        // open data files for read
        fin2.open("/home/jay/M-Lab/Mlab/april_" + to_string(x) + "_unique_trace");

        // read each line from file
        while (getline(fin2, temp))
        {
            // push line to string stream
            ss << temp;

            // read from string stream and parse tokens using delimiter
            while (getline(ss, token, ' '))
            {
                // push each token to tempVect
                traceVect.push_back(token);
            }

            // take first element of vector, parse using delimiter, and take first part (src)
            temp = traceVect[0];

            // set delimiter
            delimiter = ':';

            // partition string and get src
            src = temp.substr(0, temp.find(delimiter));

            // set match flag
            matchflag = false;

            // iterate through the rest of trace string
            for (int i = 1; i < traceVect.size(); i++)
            {
                // take subsequent elements and parse at delimiter (addresses)
                temp = traceVect[i];

                // set delimiter
                delimiter = '-';

                // partition string and get ip
                ip = temp.substr(0, temp.find(delimiter));

                // set hop = current iteration
                hop = i;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // get subnet mask
                //mask = get_mask()

                // initialize stringstream object
                stringstream ss;

                // read ip into stringstream
                ss << ip;

                // stream into array ipl.ip
                ss >> ipl.ip;

                // convert ipl.ip into an integer
                ipl.addr = conv(ipl.ip);

                // print integer for testing
                cout << ipl.addr << endl;

                // search data structure for match

                // if match

                    //save src-distance in AS vantage point file

                    // save all other matching ips as visibility in AS file


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            }

            // clear vector and stringstream each time
            traceVect.clear();
            ss.clear();
        }

        // close data file
        fin2.close();
    }

    // end program
	return 0;
}



int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;

    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        files.push_back(string(dirp->d_name));
    }

    closedir(dp);
    return 0;
}



bool compareNetwork(net &a, net &b)
{
	return a.network < b.network;
}


bool Binary_Search(const vector< int> &my_numbers, int key)
{
    int iteration = 0, left = 0, right = my_numbers.size()-1, mid;

    while (left <= right)
    {
        iteration++;
        mid = (int) ((left + right) / 2);

        if (key == my_numbers[mid])
        {
            return 1;
        }

        else if (key > my_numbers[mid])
        {
            left = mid + 1;
        }

        else
        {
            right = mid - 1;
        }
    }

return 0;
}


unsigned long int conv(char ipadr[])
{
    unsigned long int num=0,val;
    char *tok,*ptr;
    tok=strtok(ipadr,".");
    while( tok != NULL)
    {
        val=strtoul(tok,&ptr,0);
        num=(num << 8) + val;
        tok=strtok(NULL,".");
    }
    return(num);
}


uint32_t get_mask( const int mask_length)
{
    if( mask_length > 31)
    {
        return 0xffffffff;
    }
    return (1 << (mask_length + 1)) - 1;
}