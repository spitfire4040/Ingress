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
#include <stdlib.h>
#include <bitset>
#include <climits>

using namespace std;

struct net
{
	uint32_t network;
	int range;
	string as;
};

union iptolint
{
    char ip[16];
    uint32_t addr;
};

struct sockaddr_in intIP;

int getdir (string dir, vector<string> &files);

bool compareNetwork(net &a, net &b);

bool Binary_Search(const vector<net> &my_numbers, uint32_t key);

uint32_t conv(char []);

uint32_t get_mask( const int mask_length);


int main()
{
	// initialize variables
	ifstream fin1, fin2, fin3;
	ofstream fout1, fout2, fout3;
	string temp, token, delimiter, newNet, item, target, dir, inputFileName, line, src, dst, ip, visible, hop;
	uint32_t network, mask, address;
	char i;
	int range, num;
	net tempnet;
	bool flag = true, matchflag = false, result, capture;

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

    // initialize vector to hold head for parsing
    vector<string> head;

    vector<string> ipaddr;


// LOAD SUBNET VALUES INTO VECTOR

    // define directory
    string files[20] = {"AS4270", "AS5555", "AS6846" /*, "AS10697", "AS17471", "AS19184", "AS24085", "AS32647",
                        "AS41691", "AS44716", "AS49406", "AS52866", "AS52945", "AS57786", "AS58272",
                        "AS59457", "AS65001", "AS197155", "AS262748", "AS263657"*/};

    // iterate through file names and get subnets
    for (int i = 0; i < 3 /*20*/; i++)
    {
        fin1.open("/home/jay/Ingress/as-list/" + files[i]);

        // read each line in file
        while (getline(fin1, temp))
        {
            // push line to stringstream object
        	ss << temp;

            // stream back to tempVect, parse out ','
        	while (getline(ss, token, ','))
        	{
        		tempVect.push_back(token);
        	}

            // initialize struct net
        	net newNet;

            // populate struct
        	newNet.network = stol(tempVect[1]);
        	newNet.range = stoi(tempVect[2]);
        	newNet.as = tempVect[0];

            // push struct onto end of netMap vector
        	subnetMap.push_back(newNet);

            // clear vector and stringstream for next time
        	tempVect.clear();
        	ss.clear();
        }

        // close input file
        fin1.close();

        // open output files with AS names for later
        fout1.open("/home/jay/Ingress/results/" + files[i] + "_vantage_point.txt");
        fout2.open("/home/jay/Ingress/results/" + files[i] + "_visibility.txt");


// SORT SUBNETS IN VECTOR BASED ON VALUE, ELIMINATE ALL SUBNETS WITHIN SUBNETS

        // print file name for verification
        cout << files[i] << endl;

        // sort subnetMap based on network rank
        sort (subnetMap.begin(), subnetMap.end(), compareNetwork);

        // set flag to true
        while (flag == true)
        {
            // iterate through subnetMap
            for (int i = 0; i < subnetMap.size(); i++)
            {
                // flag false in case it is finished
                flag = false;

                // if two are the same, erase current and set flag to true for another pass
            	if ((subnetMap[i].network == subnetMap[i+1].network) && (subnetMap[i].range > subnetMap[i+1].range))
            	{
            		subnetMap.erase(subnetMap.begin()+(i));
                    flag = true;
            	}

                // if two are still the same, erase the next one too and set flag to true for another pass
                if ((subnetMap[i].network == subnetMap[i+1].network) && (subnetMap[i].range < subnetMap[i+1].range))
                {
                    subnetMap.erase(subnetMap.begin()+(i+1));
                    flag = true;
                }
            }
        }

        // reset flag to true for next time
        flag = true;


// ITERATE THROUGH DATA FILES AND MAKE COMPARISON OF ADDRESSES TO SUBNET VECTOR

        // iterate through all input files
        for (int x = 1; x < 4/*31*/; x++)
        {
            // open data files for read
            fin2.open("/home/jay/M-Lab/Mlab3day/april_" + to_string(x) + "_unique_trace");    // M-Lab
            //fin2.open("/home/jay/Ark/Output/day-" + to_string(x) + "/unique-traces.txt"); // Ark
            //in2.open("/home/jay/Desktop/Ripe/Day_" + to_string(x) + "_trace.txt");         // Ripe

            // print day for visual verification
            cout << "Day-" << to_string(x) << endl;

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

                // set match flag
                matchflag = false;

                ss.clear();

                // iterate through trace string
                for (int i = 0; i < traceVect.size(); i++)
                {
                    if (i == 0)
                    {
                        // take first element of vector, parse using delimiter, and take first part (src)
                        temp = traceVect[i];

                        // push temp to string stream
                        ss << temp;

                        // parse ss to head vector
                        while (getline(ss, token, ':'))
                        {
                            head.push_back(token);
                        }

                        // set values for src, dst
                        src = head[0];
                        dst = head[1];

                        // clear string stream
                        ss.clear();
                    }

                    else
                    {
                        // take subsequent elements and parse at delimiter (addresses)
                        temp = traceVect[i];

                        // push temp to string stream
                        ss << temp;

                        // parse ss to ipaddr vector
                        while (getline(ss, token, '-'))
                        {
                            ipaddr.push_back(token);
                        }

                        // set values for ip, hop
                        ip = ipaddr[0];
                        hop = ipaddr[1];

                        // clear stringstream and ipaddr vector
                        ss.clear();

                        //read ip into stringstream
                        ss << ip;

                        //stream into array ipl.ip
                        ss >> ipl.ip;

                        // convert ipl.ip into an integer
                        ipl.addr = conv(ipl.ip);

                        // search data structure for match using binary search
                        result = Binary_Search(subnetMap, ipl.addr);            // binary search

                        // if ip is in subnet, print data
                        if (result == true)
                        {
                            if (matchflag == false)
                            {
                                fout1 << src << '\t' << hop << endl;
                                matchflag = true;
                            }
                            fout2 << ipaddr[0] << endl;
                        }
                    }
                    head.clear();
                    ipaddr.clear();
                    ss.clear();

                }
                // clear vector and stringstream for next time
                traceVect.clear();
                ss.clear();
                matchflag == false;
            }

            // close data file
            fin2.close();
        }

        // clear subnetMap after each run
        subnetMap.clear();

        // close files
        fout1.close();
        fout2.close();
    }

    // end program
	return 0;
}

// function: compare network values
bool compareNetwork(net &a, net &b)
{
	return a.network < b.network;
}


// function: binary search
bool Binary_Search(const vector<net> &my_numbers, uint32_t key)
{
    // initialize variables
    int left = 0, right = my_numbers.size()-1, mid, range;
    uint32_t mask, upper, lower;
    string bits;

    // while partitions don't cross
    while (left <= right)
    {
        // set midpoint
        mid = (int) ((left + right) / 2);

        // get subnet mask from subnetMap[mid]
        range = get_mask(my_numbers[mid].range);

        //cout << "key: " << ' ' << key << endl;
        //cout << "subnet: " << my_numbers[mid].network << endl;
        //cout << "range: " << range << endl;
        //cout << "key & range: " << (key & range) << endl;
        //cout << "network & range: " << ((my_numbers[mid].network) & range) << endl;

        // AND key/mask...AND network/mask. See if they are equal
        if ((key & range) == ((my_numbers[mid].network) & range))
        {
            // found, return true
            return 1;
        }

        // if key.mask larger than mid, move right and start again
        else if ((key & range) > ((my_numbers[mid].network) & range))
        {
            left = mid + 1;
        }

        // else move left and start again
        else
        {
            right = mid - 1;
        }
    }

// if not found return false
return 0;
}


// function: convert string to int
uint32_t conv(char ipadr[])
{
    uint32_t num=0,val;
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


// function get mask, as an int value
uint32_t get_mask( const int mask_length)
{
    if( mask_length > 31)
    {
        return 0xffffffff;
    }
    return (1 << (mask_length + 1)) - 1;
}