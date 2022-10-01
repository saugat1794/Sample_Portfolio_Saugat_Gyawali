#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

double sum(vector<double>);
double mean(vector<double>);
double median(vector<double>);
vector<double> range(vector<double>);
double covariance(vector<double>, vector<double>);
double correlation(vector<double>, vector<double>);
double calculateStandardDeviation(vector<double>);
void display(vector<double>);

int main() {
    ifstream inFS;
    string line;
    string rm_in, medv_in;
    const int MAX_LEN = 1000;
    vector<double> rm(MAX_LEN);
    vector<double> medv(MAX_LEN);
    

    cout << "Opening file Boston.csv" << endl;

    inFS.open("Boston.csv");

    if (!inFS.is_open()) {
        cout << "Could not open file" << endl;
        return 1;
    }

    cout << "Reading line 1" << endl;
    getline(inFS, line);

    cout << "Heading " << line << endl;

    int numberofObservation = 0;
    while (inFS.good()) {
        getline(inFS, rm_in, ',');
        getline(inFS, medv_in, '\n');
        rm.at(numberofObservation) = stof(rm_in);
        medv.at(numberofObservation) = stof(medv_in);

        numberofObservation++;
    }
    rm.resize(numberofObservation);
    medv.resize(numberofObservation);

    cout << "New Line Size = " << numberofObservation << endl;

    cout << "Closing file Boston.csv" << endl;
    inFS.close();

    cout << "******************** Stats of rm**************************" << endl;
    display(rm);
    cout << "************************************************************" << endl << endl;

    cout << "******************** Stats of medv**************************" << endl;
    display(medv);
    cout << "************************************************************" << endl << endl;

    cout << "Covariance = " << covariance(rm, medv) << endl;

    cout << "\nCorrelation = " << correlation(rm, medv) << endl;

    cout << "\n Program terminated." << endl;

    return 0;
}

double sum(vector<double> temp) {
    double sum = 0.0;
    for (int i = 0; i < temp.size(); i++) {
        sum += temp.at(i);
    }
    return sum;
}

double mean(vector<double> temp) {
    double avg = 0.0;
    int observation = temp.size();
    double sumofVector = sum(temp);
    return (double)sumofVector / (double)observation;

}

double median(vector<double> temp) {
    double median = 0.0;
    int size = temp.size();
    sort(temp.begin(), temp.end());
    if (size % 2 == 0) {
        return (double)(temp.at((size - 1) / 2) + temp.at(size / 2)) / 2.0;
    }else{
        return (double)temp.at(size / 2);
    }
}

vector<double> range(vector<double> temp) {
    vector<double> result;
    double left = (double)*min_element(temp.begin(), temp.end());
    result.push_back(left);
    double right = (double)*max_element(temp.begin(), temp.end());
    result.push_back(right);
    return result;
}

double covariance(vector<double> vect1, vector<double> vect2) {
    double sum = 0;
    double x_mean = mean(vect1);
    double y_mean = mean(vect2);

    for (int i = 0; i < vect1.size(); i++) {
        sum = (double)sum + (vect1.at(i) - x_mean)*(vect2.at(i) - y_mean)/(double)(vect1.size());
    }
    return sum;
}

double correlation(vector<double> vect1, vector<double> vect2) {
    double standard_deviation_X = calculateStandardDeviation(vect1);
    double standard_deviation_Y = calculateStandardDeviation(vect2);
    return (double)covariance(vect1, vect2) / (double)(standard_deviation_X * standard_deviation_Y);
}

double calculateStandardDeviation(vector<double> temp) {
    double standardDeviation = 0.0;
    for (int i = 0; i < temp.size(); i++) {
        standardDeviation += pow(temp.at(i) - mean(temp), 2);
    }
    return (double)sqrt(standardDeviation / temp.size());
}

void display(vector<double>temp) {
   
    cout << "Sum = " << sum(temp) << endl;
    cout << "Mean = " << mean(temp) << endl;
    cout << "Median = " << median(temp) << endl;
    vector<double> rangeofVector = range(temp);
    cout << "Range = " << rangeofVector.at(0) << " to " << rangeofVector.at(1) << endl;
   
}