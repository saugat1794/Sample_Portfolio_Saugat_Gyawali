#include<iostream>
#include<fstream>
#include<String>
#include<vector>
#include<sstream>
#include<algorithm>
#include<Eigen/Dense>
#include<chrono>


using namespace Eigen;
using namespace std;
using namespace chrono;

const int MAX_LEN = 1050;
vector<string> sex;
vector<string> survived;
void setValue(string);
double alpha = 0.001;
double e = 2.71828;
int epoch = 10;
MatrixXd weight(2, 1);

int read_data(string);
void setvalue(string);
void display(vector<string>);
MatrixXd set_datamatrix(vector<string>);
MatrixXd set_weight();
MatrixXd multiply_matrix(MatrixXd, MatrixXd);
MatrixXd train_data();
vector<int> test_data();
MatrixXd set_testData();
double checkAccuracy(vector<int>);
double checkSensitivity(vector<int>);
double checkSpecificity(vector<int>);
int numberofIteration = 52;

int main() {
    
	read_data("titanic_project.csv");
    auto start = steady_clock::now();
    MatrixXd final_weight = train_data();
    auto end = steady_clock::now();
    duration<double> elapsed_time = end - start;
    cout << "********************************************" << endl;
    cout << "             Coefficients                   " << endl;
    cout << "********************************************" << endl;
    cout << "w0 = " << final_weight(0) << endl;
    cout << "w1 = " << final_weight(1) << endl;
    cout << "********************************************" << endl;
    cout << "Required equation: " << endl;
    cout << "Survived = " << final_weight(0) << " + " << final_weight(1) << " age " << endl;
    cout << "********************************************" << endl;

    vector<int> result = test_data();
    cout << "********************************************" << endl;
    cout << "                  Metrics                   " << endl;
    cout << "********************************************" << endl;
    cout << "Accuracy = " << checkAccuracy(result) << endl;
    cout << "Sensitivity = " << checkSensitivity(result) << endl;
    cout << "Specificity = " << checkSpecificity(result) << endl;

    cout << "********************************************" << endl;
    cout << "Number of run times = " << numberofIteration << endl;
    cout << "********************************************" << endl;
    
    cout << "********************************************" << endl;
    cout << "elapsed time for training data: " << elapsed_time.count() << " second " << endl;
    cout << "********************************************" << endl;
    //cout << "elapsed time: " << elapsed_time.count() << "s\n";
    return 0;
}

int read_data(string filename) {
    fstream inFS;
    string line;
   // string sex_temp, survived_temp;
    vector<string> lineAll;

    std::cout << "Opening file " << filename << endl;

    inFS.open(filename);

    if (!inFS.is_open()) {
        std::cout << "Could not open a file" << endl;
        return 1;
    }

    //std::cout << "Reading line 1" << endl;
    std::getline(inFS, line);
    //std::cout << "Heading " << line << endl;

    while (inFS.good()) {
        std::getline(inFS, line);
        setValue(line);
    }
}

void setValue(string line) {
    // survived @ 2nd column, and sex @ 3rd column. Started from index 0.
    stringstream s_stream(line);
    vector<string> result;
    while (s_stream.good()) {
        string substr;
        getline(s_stream, substr, ','); //get first string delimited by comma
        result.push_back(substr);
    }
    sex.push_back(result.at(3));
    survived.push_back(result.at(2));

}

double signoid(int z) {
    return double(1/(1 + pow(e, -z)));
}

MatrixXd set_weight() {
    weight.setOnes();
    return weight;
}

MatrixXd set_datamatrix(vector<string> sex1) {
    MatrixXd data_matrix(800,2);
    data_matrix.col(0).setOnes();
    
    for (int i = 0; i < 800; i++) {
        double data = stod(sex1.at(i));
        data_matrix(i, 1) = data;
    }

   // cout << data_matrix << endl;
    return data_matrix;
}


MatrixXd multiply_matrix_for_train(MatrixXd data_matrix, MatrixXd weight) {
    //MatrixXd data_matrix = set_datamatrix(sex);
    //weight = set_weight();
    MatrixXd prod = data_matrix * weight;
    MatrixXd temp = prod.unaryExpr(&signoid);
    return temp;
}

MatrixXd multiply_matrix_for_test(MatrixXd test, MatrixXd wt) {
    MatrixXd prod = test * weight;
    MatrixXd temp = prod.unaryExpr(&signoid);
    return temp;
}

MatrixXd convert_label_train_data(vector<string>survived1) {
    MatrixXd label(800,1);
    for (int i = 0; i < 800; i++) {
        double data = stod(survived1.at(i));
        label(i) = data;
    }
    cout << "Training Data rows = " << label.rows() << endl;
    return label;
}

MatrixXd convert_label_test_data(vector<string>survived1) {
    MatrixXd label(246, 1);
    int testData = 246;
    int count = 0;
    int start = 800;
    for (int i = 0; i < testData; i++) {
        double data = stod(survived1.at(start));
        label(i) = data;
        count++;
        start++;
    }
    //cout << "Test label count = " << endl;
    //cout << "Test Label rows = " << label.rows() << endl;

    return label;
}

MatrixXd set_testData() {
    int totalSize = sex.size();
    int trainingData = 800;
    int testData = totalSize - trainingData;
    int count = 0;
    MatrixXd testMatrix(246,2);
    //cout << testMatrix.rows() << endl;

    testMatrix.col(0).setOnes();
    for (int i = 0; i < testData; i++) {
        double data = stod(sex.at(trainingData));
        count++;
        testMatrix(i, 1) = data;
        trainingData++;
    }
    return testMatrix;
}

vector<int> test_data() {
    //MatrixXd wt = train_data();
    int truePositive = 0;
    int trueNegative = 0;
    int falsePositive = 0;
    int falseNegative = 0;
    vector<double> storing;
    vector<int> finalRes;
    vector<pair<double, double>> tempRes;
    MatrixXd testMatrix = set_testData();
    MatrixXd label = convert_label_test_data(survived);
    MatrixXd pred1 = multiply_matrix_for_test(testMatrix, weight);
    
    int start = 800;
    for (int i = 0; i < pred1.rows(); i++) {
        if (pred1.coeff(i) < 0.5) {
            storing.push_back(0);
        }
        else {
            storing.push_back(1);
        }
    }
    
    for (int i = 0; i < storing.size(); i++) {
        tempRes.push_back(make_pair(storing.at(i), stod(survived.at(start))));
        start++;
    }

    //cout << tempRes.size() << endl;
    for (int i = 0; i < tempRes.size(); i++) {
        //cout << tempRes.at(i).first << "-------" << tempRes.at(i).second << endl;
        if (tempRes.at(i).first == 1 && tempRes.at(i).second == 1) {
            truePositive++;
        }
        else if (tempRes.at(i).first == 0 && tempRes.at(i).second == 0) {
            trueNegative++;
        }
        else if (tempRes.at(i).first == 1 && tempRes.at(i).second == 0) {
            falsePositive++;
        }
        else if (tempRes.at(i).first == 0 && tempRes.at(i).second == 1) {
            falseNegative++;
        }
    }
    /*
    cout << "TP = " << truePositive << endl;
    cout << "TN = " << trueNegative << endl;
    cout << "FP = " << falsePositive << endl;
    cout << "FN = " << falseNegative << endl;
    
    */
    
    finalRes.push_back(truePositive);
    finalRes.push_back(trueNegative);
    finalRes.push_back(falsePositive);
    finalRes.push_back(falseNegative);
    return finalRes;
}

MatrixXd train_data() {
    weight = set_weight();
    MatrixXd label = convert_label_train_data(survived);
    for (int i = 0; i < 52; i++) {
        MatrixXd setTemp = set_datamatrix(sex);
        MatrixXd prob = multiply_matrix_for_train(setTemp,weight);
        MatrixXd error = label - prob;
        weight = weight + alpha * setTemp.transpose() * error;
    }
    return weight;
}

/*
res[0] = truePositive;
res[1] = trueNegative;
res[2] = falsePositive;
res[3] = falseNegative;
*/

double checkAccuracy(vector<int> res) {
    return double(double(res.at(0) + res.at(1)) / double(res.at(0) + res.at(1) + res.at(2) + res.at(3)) * 100);
}

double checkSensitivity(vector<int> res) {
    return double(double(res.at(0)) / double(res.at(0) + res.at(3)) * 100);
}

double checkSpecificity(vector<int> res) {
    return double(double(res.at(1)) / double(res.at(1) + res.at(2)) * 100);
}




