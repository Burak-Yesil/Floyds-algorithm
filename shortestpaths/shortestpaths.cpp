/*******************************************************************************
 * Name    : shortestpaths.cpp
 * Author  : Burak Yesil and Matthew Wrobel
 * Version : 1.0
 * Date    : December 5, 2021
 * Description : Runs the Floyds Algorithm.
 * Pledge : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include<stdio.h>
#include<ctype.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>

using namespace std;


long** createIntArray(int size){
	//create array of rows
	long **array = new long*[size];
	//create row entries
	for (int i = 0; i < size; i++){
		array[i] = new long[size];
		fill(array[i], array[i] + size, numeric_limits<long>::max());
	}
	return array;
}

long sizeofnumber(long n){
	int counter = 0;
	long temp = n;
	while (temp != 0){
		temp = temp/10;
		counter++;
	}
	return counter;
}


void delete_matrix(long** const matrix, int size){
    for (int i = 0; i < size; i++) {
      delete [] matrix[i];
    }
    delete [] matrix;
}


//Displays Table of Long
void display_table(long** const matrix, const string &label, const bool use_letters, int num_vertices) { 
    long INF = numeric_limits<long>::max();
    cout << label << endl; 
    long max_val = 0; 

    if (!use_letters){
    for (int i = 0; i < num_vertices; i++) { 
        for (int j = 0; j < num_vertices; j++) { 
            long cell = matrix[i][j]; 
            if (cell < INF && cell > max_val) { 
                max_val = matrix[i][j]; 
            } 
        } 
    } 
    }
    int max_cell_width = use_letters ? 1 : sizeofnumber(max_val); 
    

    cout << ' ';
    

    for (int j = 0; j < num_vertices; j++) { 
        cout << " " << setw(max_cell_width) << static_cast<char>(j + 'A'); 
    } 
    cout << endl; 
    for (int i = 0; i < num_vertices; i++) { 
        cout << static_cast<char>(i + 'A'); 
        for (int j = 0; j < num_vertices; j++) { 
            cout << " " << setw(max_cell_width); 
            if (matrix[i][j] == numeric_limits<long>::max()) {  //not sure this fixes the problem
                cout << "-"; 
            
            } else if (use_letters) { 
                cout << static_cast<char>(matrix[i][j] + 'A'); 
            } else { 
                cout << matrix[i][j]; 
            } 
        } 
        cout << endl; 
    } 
    cout << endl; 
}



void print_path(int i, int j, long** matrix){
    
    if (matrix[i][j] == numeric_limits<long>::max()){
        cout << " -> " <<  static_cast<char>(j + 'A');
    }
    else{
        print_path(i, matrix[i][j], matrix);
        print_path(matrix[i][j], j, matrix);
    }
}







//Main Function
int main(int argc, const char *argv[]) {
    // Make sure the right number of command line arguments exist.
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    // Create an ifstream object.
    ifstream input_file(argv[1]);
    // If it does not exist, print an error message.
    if (!input_file) {
        cerr << "Error: Cannot open file '" << argv[1] << "'." << endl;
        return 1;
    }
    // Add read errors to the list of exceptions the ifstream will handle.
    input_file.exceptions(ifstream::badbit);
    string line;
    try {
        unsigned int line_number = 1;
        // Use getline to read in a line.
        // See http://www.cplusplus.com/reference/string/string/getline

        istringstream iss;
        int arraySize;
        int spaceCounter = 0;
        string temp;
        int argnum = 1;
        char letters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
        int firstIndex;
		int secondIndex;
		long distance;
		long** distanceMatrix;
        long** shortestArray; //
        long** Intermediate;

        while (getline(input_file, line)) {

        	//error check :D
        	if (line_number == 1){
        		iss.str(line);


        		//error <1, >26, not number
        		if (!(iss >> arraySize) || (arraySize < 1  || arraySize > 26)) {
        					cerr << "Error: Invalid number of vertices '" << line << "' on line " << line_number << "." << endl;
        					return 1;
        			}


            		distanceMatrix = createIntArray(arraySize); //original
            		shortestArray = createIntArray(arraySize);  
             		Intermediate = createIntArray(arraySize);


        	}

        	//line number not 1
        	else {
        		spaceCounter = 0;
        		for (size_t i = 0; i < line.length(); i++){
        				if (line[i] == ' '){
        						spaceCounter++;
        				};
        		};


        		//errorCheck edge data (num arguments)
        		if (spaceCounter != 2){
        			cerr << "Error: Invalid edge data '"<< line <<"' on line " << line_number << "." << endl;
                    delete_matrix(distanceMatrix, arraySize);
                    delete_matrix(shortestArray, arraySize);
                    delete_matrix(Intermediate, arraySize);
        			return 1;
        		}

        		//correct num arguments
        			argnum = 1;
        			temp = "";

        			for(auto i: line){
        				if (i == ' '){
        					//do test
        					//check if argnum = 1, argnum = 2
        					if (argnum == 1){
        						if(temp.length() != 1){
        							cerr << "Error: Starting vertex '" << temp << "' on line " << line_number << " is not among valid values A-"<<letters[arraySize-1]<<"."<< endl;
                                    delete_matrix(distanceMatrix, arraySize);
                                    delete_matrix(shortestArray, arraySize);
                                    delete_matrix(Intermediate, arraySize);
        							return 1;
        						}
        						else if(temp[0] < 'A' || temp[0] > letters[arraySize-1]){
        							cerr << "Error: Starting vertex '" << temp << "' on line " << line_number << " is not among valid values A-"<<letters[arraySize-1]<<"."<< endl;
                                    delete_matrix(distanceMatrix, arraySize);
                                    delete_matrix(shortestArray, arraySize);
                                    delete_matrix(Intermediate, arraySize);
        							return 1;
        						}

        						firstIndex = (temp[0] - 'A');
        						//store (temp - 'A') into variable for fromIndex
        					}

        					if(argnum == 2){
        						if(temp.length() != 1){
        							cerr << "Error: Ending vertex '" << temp << "' on line " << line_number << " is not among valid values A-"<<letters[arraySize-1]<<"."<< endl;
                                            delete_matrix(distanceMatrix, arraySize);
                                            delete_matrix(shortestArray, arraySize);
                                            delete_matrix(Intermediate, arraySize);
        						        	return 1;
        						        }
        						else if(temp[0] < 'A' || temp[0] > letters[arraySize-1]){
        							cerr << "Error: Ending vertex '" << temp << "' on line " << line_number << " is not among valid values A-"<<letters[arraySize-1]<<"."<< endl;
                                    delete_matrix(distanceMatrix, arraySize);
                                    delete_matrix(shortestArray, arraySize);
                                    delete_matrix(Intermediate, arraySize);
        						    return 1;
        						}
        						//store (temp - 'A') into variable for toIndex
        						secondIndex = (temp[0] - 'A');
        					}

        					temp = "";
        					argnum++;
        				}
        				else {
        					temp += i;
        				}
        			}



        			//number

        			if(temp[0] < '1' || temp[0] > '9'){
        				cerr << "Error: Invalid edge weight '" << temp << "' on line " << line_number << "." << endl;
                        delete_matrix(distanceMatrix, arraySize);
                        delete_matrix(shortestArray, arraySize);
                        delete_matrix(Intermediate, arraySize);
        				return 1;
        			}
        			for(size_t i = 1; i < temp.length(); i++){
        				if(temp.length() > 1){
        					if(temp[i] < '0' || temp[i] > '9'){
        						cerr << "Error: Invalid edge weight '" << temp << "' on line " << line_number << "." << endl;
                                delete_matrix(distanceMatrix, arraySize);
                                delete_matrix(shortestArray, arraySize);
                                delete_matrix(Intermediate, arraySize);             
        						return 1;
        					}
        				}
        			}
        			distance = stol(temp);

        			distanceMatrix[firstIndex][secondIndex] = distance;
        			shortestArray[firstIndex][secondIndex] = distance;

        			
        			//store temp into edgeWeight

        			//function to put edgeWeight into matrix[fromIndex][toIndex]


        	}

            //cout << line_number << ":\t" << line << endl;
            ++line_number;
        }

        
        //Makes diagonal of zeros for matrix 
        for (int i = 0; i < arraySize; i++){
            for (int j = 0; j < arraySize; j++){
                if (i==j){
                    distanceMatrix[i][j] = 0;
                    shortestArray[i][j] = 0;
                }
            }
        }


        // Don't forget to close the file.
        input_file.close();




    //Code to implement floyds algorithm
    for (int k = 0; k<arraySize; k++){
        for (int i = 0; i<arraySize; i++){
            for (int j = 0; j<arraySize; j++){
                if (shortestArray[i][k] + shortestArray[k][j] < shortestArray[i][j]){
                    if (shortestArray[i][k]+shortestArray[k][j]>=0){
                        shortestArray[i][j] = shortestArray[i][k] + shortestArray[k][j];
                        Intermediate[i][j] = k;
                    }
                }
            }
        }
    }




    display_table(distanceMatrix, "Distance matrix:", false, arraySize);
    display_table(shortestArray, "Path lengths:", false, arraySize);
    display_table(Intermediate, "Intermediate vertices:", true, arraySize);




    for (int i = 0; i < arraySize; i++){
    	for (int j = 0; j < arraySize; j++){



            if (shortestArray[i][j] == numeric_limits<long>::max()){
                cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: infinity" << ", path: none";
                if (!((i==arraySize-1) && (j==arraySize-1))){
                    cout << endl;
                }
            }


            else{
                cout << static_cast<char>(i + 'A') << " -> " << static_cast<char>(j + 'A') << ", distance: " << shortestArray[i][j] << ", path: ";

                //finds the path
                if (i == j){
                    cout <<  static_cast<char>(i + 'A');
                }
                else{
                    cout <<  static_cast<char>(i + 'A');
                    print_path(i,j, Intermediate);
                }

                if (!((i==arraySize-1) && (j==arraySize-1))){
                    cout << endl;
                }
            }


    	}
    }




    delete_matrix(distanceMatrix, arraySize);
    delete_matrix(shortestArray, arraySize);
    delete_matrix(Intermediate, arraySize);






    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << argv[1] << "'.";
        return 1;
    }

    return 0;
}
