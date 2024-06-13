#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

pair<string, int> Input();
void Search(vector<vector<float> >, int, int);
float Accuracy(vector<vector<float> >, vector<int>);
long double Distance(vector<vector<float> >, vector<int>, int, int);

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
    cout << "Dataset has " << data.at(0).size()-1 << " features (not including class label), with " << data.size() << " instances." << endl;
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

    Search(data, return_vals.second, data_size-1);
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

void Search(vector<vector<float> > data, int choice, int data_length) {
    vector<int> curr_features;
    vector<int> avail_features;
    if (choice == 1) {
        //Forward Selection
        for (int i = 0; i < data_length; ++i) {
            avail_features.push_back(i+1);
        }
    } else {
        //Backward Elmination
        for (int i = 0; i < data_length; ++i) {
            curr_features.push_back(i+1);
            avail_features.push_back(i+1);
        }
    }

    if (choice == 1) {
        //Forward Selection
        int count = 0;
        float default_rate;
        for (int i = 0; i < data.size(); ++i) {
            if (data.at(i).at(0) == 1) {
                count++;
            }
        }
        if ((float)count / data.size() > .5) {
            default_rate = (float)count / data.size();
        } else {
            default_rate = (data.size()-count) / (float)data.size();
        }
        cout << "No Accuracy value with no features to test, the default rate is: " << default_rate*100 << "%" << endl << endl;
    } else {
        //Backward Elmination
        float acc = Accuracy(data, curr_features) * 100;
        cout << "Running Nearest Neighbor with all " << data.at(0).size()-1 << " features with 'leave one out' validation, I get an accuracy of " << acc << "%." << endl << endl;
    }

    cout << "Beginning Search" << endl << endl;

    float best_acc = 0;
    vector<int> best_features;

    while (avail_features.size() != 0) {
        vector<float> accs;
        if (choice == 1) {
            //Forward Selection
            for (int i = 0; i < avail_features.size(); ++i) {
                vector<int> frontier_search = curr_features;
                frontier_search.push_back(avail_features.at(i));
                float acc = Accuracy(data, frontier_search);
                accs.push_back(acc);
                cout << "Using features(s) {";
                for (int i = 0; i < frontier_search.size(); ++i) {
                    if (i == frontier_search.size() - 1) {
                        cout << frontier_search.at(i);
                    } else {
                        cout << frontier_search.at(i) << ",";
                    }
                }
                cout << "} accuracy is " << acc*100 << "%" << endl;
            }
        } else {
            //Backwards Elmination
            for (int i = 0; i < avail_features.size(); ++i) {
                vector<int> frontier_search = curr_features;
                frontier_search.erase(frontier_search.begin() + i);
                float acc = Accuracy(data, frontier_search);
                accs.push_back(acc);
                cout << "Using features(s) {";
                for (int i = 0; i < frontier_search.size(); ++i) {
                    if (i == frontier_search.size() - 1) {
                        cout << frontier_search.at(i);
                    } else {
                        cout << frontier_search.at(i) << ",";
                    }
                }
                cout << "} accuracy is " << acc*100 << "%" << endl;
            }
        }
        //Find Best feature, add to curr set, update best acc and avail_features, and print.
        //Also print if accuracy has decreased since the last step
        //Empty Accs
        auto max = max_element(accs.begin(), accs.end());
        int index = distance(accs.begin(), max);
        if (*max < best_acc) {
            cout << endl;
            cout << "Accuracy has decreased!! Still continuing" << endl;
        }

        if (choice == 1) {
            cout << endl;
            cout << "Best feature set was {";
            for (int i = 0; i < curr_features.size(); ++i) {
                cout << curr_features.at(i) << ",";
            }
            cout << avail_features.at(index) << "}, accuracy is " << *max*100 << "%" << endl;
            cout << endl;
        } else {
            cout << endl;
            cout << "Best feature set was {";
            for (int i = 0; i < curr_features.size(); ++i) {
                if (i != index) {
                    if (i == curr_features.size() - 1) {
                        cout << curr_features.at(i);
                    } else {
                        cout << curr_features.at(i) << ",";
                    }
                }
            }
            cout << "} accuracy is " << *max*100 << "%" << endl;
            cout << endl;
        }
        
        if (*max > best_acc) {
            best_acc = *max;
            if (choice == 1) {
                best_features = curr_features;
                best_features.push_back(avail_features.at(index));
            } else {
                best_features = curr_features;
                best_features.erase(best_features.begin()+index);
            }
        }
        //Update curr set and avail features
        if (choice == 1) {
            //Add to curr and remove from avail in Forward
            curr_features.push_back(avail_features.at(index));
            avail_features.erase(avail_features.begin()+index);
        } else {
            //Remove from curr and avail for backward
            curr_features.erase(curr_features.begin()+index);
            avail_features.erase(avail_features.begin()+index);
        }
        //Wipe Accs
        accs.erase(accs.begin(), accs.end());

    }
    cout << endl;
    cout << "Finished Search!!! The best feature subset is {";
    for (int i = 0; i < best_features.size(); ++i) {
        if (i == best_features.size() - 1) {
            cout << best_features.at(i);
        } else {
            cout << best_features.at(i) << ",";
        }
    }
    cout << "} accuracy is " << best_acc*100 << "%" << endl;
}

float Accuracy(vector<vector<float> > data, vector<int> feature_set) {
    int correct = 0;
    for (int i = 0; i < data.size(); ++i) {
        int label = data.at(i).at(0);
        int loc = -1;
        float nearest_neighbor_dist = numeric_limits<float>::max();
        int calc_label = -1;

        for (int k = 0; k < data.size(); ++k) {
            if (k != i) {
                long double distance = Distance(data, feature_set, i, k);
                if (distance < nearest_neighbor_dist) {
                    nearest_neighbor_dist = distance;
                    loc = k;
                    calc_label = data.at(k).at(0);
                }
            }
        }

        if (calc_label == label) {
            correct++;
            //cout << "Correct Updated!" << endl;
        }
    }
    float accuracy = (float)correct / (float)data.size();
    //cout << "Data size is " << data.size() << endl;
    //cout << "Correct is " << correct << endl;
    //cout << "Accuracy is " << accuracy << endl;
    return accuracy;
}

long double Distance(vector<vector<float> > data, vector<int> feature_set, int i, int k) {
    long double sum = 0;
    //cout << "i is: " << i << " and k is: " << k << endl;
    for (int j = 0; j < feature_set.size(); ++j) {
        long double diff = abs(data.at(i).at(feature_set.at(j)) - data.at(k).at(feature_set.at(j)));
        //cout << "Diff is: " << diff << " ";
        sum += diff * diff;
    }
    //cout << endl;
    return sqrt(sum);
}