#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>

using namespace std;

pair<string, int> Input();
void Search(int);

int main() {
    pair<string, int> return_vals = Input();
    //Read in values from selected data
    vector<vector<float> > data;

    int data_size = 0;
    float number_calc;
    ifstream inputFile_calc;
    inputFile_calc.open(return_vals.first);
    inputFile_calc >> number_calc;
    data_size++;
    while (!inputFile_calc.eof()) {
        inputFile_calc >> number_calc;
        data_size++;
        if (number_calc == 1 || number_calc == 2) {
            data_size--;
            break;
        }
    }
    inputFile_calc.close();

    float number;
    ifstream inputFile;

    inputFile.open(return_vals.first);
    vector<float> temp;
    while (!inputFile.eof()) {
        inputFile >> number;
        if (temp.size() == data_size) {
            data.push_back(temp);
            temp.erase(temp.begin(), temp.end());
        }
        //cout << number << endl;
        temp.push_back(number);
    }
    inputFile.close();

    /*
    bool error = false;
    //Print data for testing
    for (int i = 0; i < data.size(); ++i) {
        if (data.at(i).size() != data_size) {
            cout << "ERROR IN SIZE VALUE!!!!" << endl;
            error = true;
        } else {
            cout << "WE ARE FINE" << endl;
        }
        for (int j = 0; j < data.at(i).size(); ++j) {
            cout << data.at(i).at(j) << " ";
        }
        cout << endl;
    }
    cout << data_size << endl;
    cout << error << endl;
    cout << data.size() << " " << data.at(0).size() << endl;
    */

   
    return 0;
}

pair<string, int> Input() {
    cout << "Welcome to David Strathman's Feature Selection Algorithm" << endl;
    cout << "Type in the name of the file to test: ";
    string input;
    cin >> input;
    cout << endl << endl;

    cout << "Type the number of the algorithm you would like to use" << endl;
    cout << "1. Forward Selection" << endl;
    cout << "2. Backward Elmination" << endl;
    int x;
    cin >> x;

    pair<string, int> inputs;
    inputs.first = input;
    inputs.second = x;
    return inputs;
}