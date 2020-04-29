#include <iostream>    //cin, cout ..
#include <string>   //string
#include <fstream>  //ifstrean
#include <sstream>  //string strings
#include <regex>    //regex library
#include <map>
#include <list>


using namespace std;
// converts from adjacency matrix to adjacency list
vector<vector<int>> convert( vector<vector<int>> a)
{
    vector<vector<int>> adjList(a.size());
    for (int i = 0; i < a.size(); i++)
    {
          
        for (int j = 0; j < a[i].size(); j++)
        {
            if (a[i][j] == 1)
            {
                adjList[i].push_back(j);
            }
        }
    }
    return adjList;
}

void readfile(string textfile,  vector<string> &num_resources_list, vector<vector<int>> &AdjList){
    vector<int> result;
    vector<vector<int>> a;
    ifstream configf;
    configf.open( textfile.c_str() );
    /*open configfile using c_str*/
    while(configf.fail()){
        cout << "THAT FILE DOES NOT EXIST !" << endl;
        cout << "Please enter a configuration file in the directory." << endl;
        /*print meainingful error message*/
        getline(cin, textfile);
        /*get correct file*/
        configf.open(textfile.c_str());
        /*open config file*/
    }
    
    string line;
    smatch equal;
    regex e ("=");
    regex comment ("%[^\n]*"  );
        regex blank ("[ \t\n]"  );
    int check_num_units=0;
    int check_adjacency_matrix=0;
    /*matching an equal sign*/
    string temp_num_processes, temp_num_resources;
    int num_processes, num_resources;
    if(configf.is_open()){
        /*continues if the file is opened*/
        while(getline(configf, line)){
            /*while were able to getline from the file*/
            
            if(regex_match (line, comment) || regex_match (line, blank) ){
                /* there was a comment match */
                continue;
            }else{
        
                cout<<line<< endl;
                regex_search(line, equal, e);
             
                if(equal.prefix().str() == "num_processes" ){
                    /* converting the suffix to an int and saving num_processes */
                    temp_num_processes = equal.suffix().str();
                    stringstream num_process_convert(temp_num_processes);
                    num_process_convert >> num_processes;
                    check_num_units++;
                    //cout << "\nthe num_processes is:" << num_processes <<endl;
                }else if(equal.prefix().str() == "num_resources"){
                    /* converting the suffix to an int and saving num_resources */
                    temp_num_resources = equal.suffix().str();
                    stringstream num_resources_convert(temp_num_processes);
                    num_resources_convert >> num_resources;
                    //cout << "\nthe num_resources is:" <<num_resources<<endl;
                    check_num_units++;
                }else if(check_num_units == 2 && check_adjacency_matrix != num_resources){ /* ready to receive list of units */

                    stringstream s_stream(line); //create string stream from the string
                    while(s_stream.good()) {
                       string substr;
                       getline(s_stream, substr, ','); //get first string delimited by comma
                        if(!substr.empty()){
                            num_resources_list.push_back(substr);
                            check_adjacency_matrix++;
                        }

                    }
                 /*   for(int i = 0; i<result.size(); i++) {    //print all splitted strings
                       cout <<check_adjacency_matrix<<"*"<< result.at(i) << endl;
                    } */

                }else if(check_adjacency_matrix == num_resources){
                    stringstream s_stream(line); //create string stream from the string
                    while(s_stream.good()) {
                        string substr;
                        getline(s_stream, substr, ','); //get first string delimited by comma
                        if(!substr.empty()){
                            int int_converted;
                            stringstream int_convert(substr);
                            int_convert >> int_converted;
                            result.push_back(int_converted);
                            
                        }

                    }
                    a.push_back(result);
                    result.erase (result.begin(), result.end());
                  for(int i = 0; i<result.size(); i++) {    //print all splitted strings
                        cout <<","<< result.at(i);
                    }
                    cout << endl;
                }
            }
        }
        configf.close();
        /* creating adjacency list */
        AdjList = convert(a);
        /*close config file*/
    }


}


int main(){
    vector<string> num_resources_list;
    vector<vector<int>> AdjList ;
    string textfile;
    /* text file */
    cout << "Please enter the configuration file name :";
    /*enter the text file */
    getline(cin, textfile);
    /*get the text file*/
    
    readfile(textfile, num_resources_list, AdjList);
    /* print list of num_resource_list */
    for(int i = 0; i<num_resources_list.size(); i++) {
       cout <<"*"<< num_resources_list.at(i) << endl;
    }
    cout<<endl<<"Adjacency List:"<<endl;
    // print the adjacency list
    int print=0;
    for (int i = 0; i < AdjList.size(); i++)
    {

        for(int j = 0; j < AdjList[i].size(); j++)
        {
            if(print==0){
                cout << i;
                print++;
            }
            if(j == AdjList[i].size() - 1)
            {
                cout << " -> " << AdjList[i][j] << endl;
                break;
            }
            else
                cout << " -> " << AdjList[i][j];
        }
        print=0;
    }
    cout <<endl;
}
