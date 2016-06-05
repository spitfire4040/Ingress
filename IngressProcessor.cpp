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

using namespace std;

struct net
{
	long network;
	int range;
	string as;
};

int getdir (string dir, vector<string> &files);

bool compareNetwork(net &a, net &b);

bool Binary_Search(const vector<int> &numbers, int value);

int getFileSize(const string &inputFileName);

int main()
{
	// initialize variables
	ifstream fin1, fin2, fin3;
	ofstream fout1, fout2, fout3;
	string temp, token, delimiter, newNet, item, target, dir, inputFileName, line, src;
	long int network;
	char i;
	int range, num, counter = 0;
	net tempnet;
	bool flag;

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
    dir = "/home/jay/Ingress/as-list";

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

    flag = true;

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
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // iterate through all input files
    for (int x = 1; x < 2; x++)
    {
        // open data files for read
        fin2.open("/home/jay/M-Lab/Mlab/april_" + to_string(x) + "_unique_trace");

        // load each line into temp vector and process
        while (getline(fin2, temp))
        {
            ss << temp;

            while (getline(ss, token, ' '))
            {
                traceVect.push_back(token);
            }

            temp = traceVect[0];
            delimiter = ':';
            src = temp.substr(0, temp.find(delimiter));
            cout << src << endl;

            for (int i = 1; i < traceVect.size(); i++)
                cout << traceVect[i] + ' ';
            cout << endl;


            traceVect.clear();
            ss.clear();

        }

        fin2.close();

    }


    // take addresses one at a time and compare to subnet

    // if address in subnet add src address to vantage point list for that AS

    // get hop number of first address that matches for distance

    // get remaining addresses in list for visibility in that AS

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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



/*
int main() {
   
vector <int>my_numbers;
bool result;
int key;
  
for (int i = 0; i < 20; i++)
    {
        my_numbers.push_back(i);
    }

    cout << "enter number for search: ";
    cin >> key;

    result = Binary_Search(my_numbers, key);

    if (result == true)
    {
        cout << "found" << endl;
    }
    else
    {
        cout << "not found" << endl;
    }

return 0;
}
*/

bool Binary_Search(const vector< int> &my_numbers, int key) {

int iteration = 0, left = 0, right = my_numbers.size()-1, mid;

while (left <= right) {
    iteration++;
    mid = (int) ((left + right) / 2);
    if (key == my_numbers[mid]) {
        return 1;
    }
else if (key > my_numbers[mid])
    left = mid + 1;
else
    right = mid - 1;
}

return 0;

}


// get file size
int getFileSize(const string &inputFileName)
{
    ifstream file(inputFileName.c_str(), ifstream::in | ifstream::binary);

    if (!file.is_open())
    {
        return -1;
    }

    file.seekg(0, ios::end);
    int fileSize = file.tellg();
    file.close();

    return fileSize;
}