/************************************************
Author: Jaysen Hippensteel
Course: Matrix Algebra & Applications
Date: 11/28/25
Description: A simple program to implement the Hill Cipher. A string is
interpreted as a set of integers mod 28. The set of numbers is put into a nxn
matrix and multiplied by an invertible nxn key matrix. To decrypt, the encrypted
matrix is multiplied by the inverse of the key matrix.
File: cipher.cpp
************************************************/

//Includes
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>

void printHelp(){
	std::cout << "Usage: cipher [OPTIONS] <SIZE> <METHOD>\n"
				<< "A simple program to implement the Hill Cipher\n\n"
				<< "Options:\n"
				<< "  -h, --help	Display this help message and exit.\n\n"
				<< "Arguments:\n"
				<< "  Size	 	Specifies the size of the nxn matrix, where argument"
				<< " is n\n"
				<< "  Method\n"
				<< "    e, encrypt	Encrypts matrix using key matrix\n"
				<< "    d, decrypt	Decrypts matrix using the inverse of the key matrix\n";
}

std::queue<int> convertToNums(std::string s){
	std::queue<int> nums;
	for (char c : s) {
		nums.push(c);
	}
	return nums;
}

std::vector<std::vector<int>>
copyMatrix(std::vector<std::vector<int>> matrix, int row, int column){
	std::vector<std::vector<int>> newMatrix(matrix.size()-1,
	std::vector<int>(matrix.size()-1));
	int k = 0;
	int l;
	for (int i=0;i<matrix.size();i++){
		if (i == row) continue;
		l = 0;
		for (int j=0;j<matrix.size();j++){
			if (j == column) continue;
			newMatrix[k][l] = matrix[i][j];
			++l;
		}
		++k;
	}
	return newMatrix;
}

int det(std::vector<std::vector<int>> matrix){
	int d = 0;
	if (matrix.size() == 2){
		d = matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
		return d;
	}
	for (int i=0;i<matrix.size();i++){
		d += matrix[0][i]*pow(-1,(2+i))*det(copyMatrix(matrix, 0, i));
	}
	return d;
}

std::vector<std::vector<int>>
genKey(int n){
	std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(1, 100);
	for (int i=0;i<n;i++){
		for (int j=0;j<n;j++){
			matrix[i][j] = distrib(gen);
		}
	}
	if (det(matrix) == 0) return genKey(n);
	return matrix;
}

std::vector<std::vector<int>>
multiply(std::vector<std::vector<double>> matrix1, 
std::vector<std::vector<double>> matrix2){
	std::vector<std::vector<int>> matrix(matrix1.size(), 
	std::vector<int>(matrix2[0].size()));
	double sum;
	for (int i=0;i<matrix.size();i++){
		for (int j=0;j<matrix[0].size();j++){
			sum = 0;
			for (int k=0;k<matrix.size();k++){
				sum += matrix1[i][k]*matrix2[k][j];
			}
			matrix[i][j] = sum;
		}
	}
	return matrix;
}

void printMatrices(std::vector<std::vector<int>>matrix1, 
std::vector<std::vector<int>>matrix2, std::string name1, std::string name2){
	for (int i=0;i<(matrix1.size()-name1.length()/2+1);i++){
		std::cout << " ";
	}
	std::cout<< name1;
	for (int i=0;i<(matrix1.size()*2+6-name1.length()/2-name2.length()/2);i++){
		std::cout << " ";
	}
	std::cout << name2 << std::endl;
	std::cout << "__";
	for (int i=0;i<(matrix1.size()*2-2);i++){
		std::cout << " ";
	}
	std::cout << "__";
	for (int i=0;i<4;i++){
		std::cout << " ";
	}
	std::cout << "__";
	for (int i=0;i<(matrix2.size()*2-2);i++){
		std::cout << " ";
	}
	std::cout << "__" << std::endl;
	for (int i=0;i<matrix1.size();i++){
		std::cout << "|";
		for (int j=0;j<matrix1.size();j++){
			std::cout << std::setfill('0') << std::setw(5) << matrix1[i][j] << " ";
		}
		std::cout << "|    |";
		for (int j=0;j<matrix2.size();j++){
			std::cout << std::setfill('0') << std::setw(2) << matrix2[i][j] << " ";
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "__";
	for (int i=0;i<(matrix1.size()*2-2);i++){
		std::cout << " ";
	}
	std::cout << "__    __";
	for (int i=0;i<(matrix2.size()*2-2);i++){
		std::cout << " ";
	}
	std::cout << std::endl;
}

std::vector<std::vector<double>>
convertToDouble(std::vector<std::vector<int>> matrix){
	std::vector<std::vector<double>> dmatrix(matrix.size(), 
	std::vector<double>(matrix.size()));
	for (int i=0;i<matrix.size();i++){
		for (int j=0;j<matrix.size();j++){
			dmatrix[i][j] = matrix[i][j];
		}
	}
	return dmatrix;
}

void encrypt(unsigned n){
	std::string message;
	std::cout << "Message: ";
	std::getline(std::cin >> std::ws, message);
	std::cout << std::endl;
	std::queue<int> nums = convertToNums(message);
	
	std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
	for (unsigned i=0;i<n;i++){
		for (unsigned j=0;j<n;j++){
			if (!nums.empty()){
				matrix[i][j] = nums.front();
				nums.pop();
			}
			else {
				matrix[i][j] = 0;
			}
		}
	}
	std::vector<std::vector<int>> key = genKey(n);
	std::vector<std::vector<int>> encryptedMatrix = multiply(
	convertToDouble(matrix), convertToDouble(key));
	printMatrices(encryptedMatrix, key, "Encrypted", "Key");
	for (int i=0;i<encryptedMatrix.size();i++){
		for (int j=0;j<encryptedMatrix.size();j++){
			std::cout << encryptedMatrix[i][j] << " ";
		}
	}
	std::cout << std::endl;
	for (int i=0;i<key.size();i++){
		for (int j=0;j<key.size();j++){
			std::cout << key[i][j] << " ";
		}
	}
	std::cout << std::endl;
}

std::vector<std::vector<int>>
loadMatrix(std::string buffer, unsigned n){
	std::istringstream iss(buffer);
	std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
	int row = 0;
	int col = 0;
	int num;
	while(iss >> num){
		if (col == n){
			++row;
			col = 0;
			if (row == n){
				std::cerr << "Matrix Overflow Error" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		matrix[row][col] = num;
		++col;
	}
	return matrix;
}

void multiplyRow(std::vector<std::vector<double>>& matrix, int row, double s){
	for (int i=0;i<matrix.size();i++){
		matrix[row][i] *= s;
	}
}

void rowAdd(std::vector<std::vector<double>>& matrix, int row1, int row2, 
double s){
	for (int i=0;i<matrix.size();i++){
		matrix[row2][i] += (matrix[row1][i]*s);
	}
}

std::vector<std::vector<double>>
inverse(std::vector<std::vector<double>> matrix){
	std::vector<std::vector<double>> imatrix(matrix.size(), 
	std::vector<double>(matrix.size()));
	for (int i=0;i<matrix.size();i++){
		imatrix[i][i] = 1;
	}
	for (int i=0;i<matrix.size();i++){
		multiplyRow(matrix, i, (1/matrix[i][i]));
		multiplyRow(imatrix, i, (1/matrix[i][i]));
		for (int j=i+1;j<matrix.size();j++){
			rowAdd(matrix, i, j, -matrix[j][i]);
			rowAdd(imatrix, i, j, -matrix[j][i]);
		}
	}
	for (int i=matrix.size()-1;i>=0;i--){
		for (int j=i-1;j>=0;j--){
			rowAdd(matrix, i, j, -matrix[j][i]);
			rowAdd(imatrix, i, j, -matrix[j][i]);
		}
	}
	return imatrix;
}

std::string matrixToString(std::vector<std::vector<int>> matrix){
	std::string output = "";
	char c;

	for (int i=0;i<matrix.size();i++){
		for (int j=0;j<matrix.size();j++){
			c = matrix[i][j];
			output += c;
		}
	}
	return output;
}

void decrypt(unsigned n){
	std::string message;
	std::cout << "Enter an encrypted matrix with space seperated values: ";
	std::getline(std::cin >> std::ws, message);
	std::cout << std::endl;

	std::vector<std::vector<int>> matrix = loadMatrix(message, n);
	message = "";
	std::cout << "Enter a key matrix with space seperated values: ";
	std::getline(std::cin >> std::ws, message);
	std::cout << std::endl;
	std::vector<std::vector<int>> key = loadMatrix(message, n);
	std::vector<std::vector<double>> inverseKey = inverse(convertToDouble(key));
	for (int i=0;i<inverseKey.size();i++){
		for (int j=0;j<inverseKey.size();j++){
			std::cout << inverseKey[i][j] << "_";
		}
	}
	std::cout << std::endl;
	std::vector<std::vector<int>> decryptedMatrix = multiply(
	convertToDouble(matrix), inverseKey);
	std::cout << matrixToString(decryptedMatrix) << std::endl;
}

int main(int argc, char* argv[]){
	//Convert arguments to vector of strings
	std::vector<std::string> args(argv+1, argv+argc);

	if (args[0] == "-h" || args[0] == "--help"){
		printHelp();
		return 0;
	}
	if (argc != 3){
		std::cerr << "Usage: " << argv[0] << "<SIZE> <METHOD>" << std::endl;
		exit(EXIT_FAILURE);
	}

	unsigned n = stoul(args[0]);
	if(args[1] == "e" || args[1] == "encrypt"){
		encrypt(n);
		return 0;
	}
	if(args[1] == "d" || args[1] == "decrypt"){
		decrypt(n);
		return 0;
	}
	std::cerr << "Usage: " << argv[0] << "<SIZE> <METHOD>" << std::endl;
	exit(EXIT_FAILURE);
}

