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

int main()
{
	// initialize variables
	ifstream fin1, fin2, fin3;
	ofstream fout1, fout2, fout3;
	string temp, token, newNet, item, target;
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

	// define directory
    string dir = "/home/jay/Ingress/as-list";

    // initialize vector to hold file names
    vector<string> files = vector<string>();

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

    for (int i = 0; i < subnetMap.size(); i++)
    {
        cout << subnetMap[i].network << ' ' << subnetMap[i].range << endl;	
    }

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