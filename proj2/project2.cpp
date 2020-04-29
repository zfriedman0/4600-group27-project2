#include <iostream>    //cin, cout ..
#include <string>   //string
#include <fstream>  //ifstrean
#include <sstream>  //string strings
#include <regex>    //regex library
#include <map>
#include <list>
#include <vector>
#include <algorithm>


using namespace std;
/* bool is process a sink */
bool isSink(vector<vector<int>> AdjList, int process, int num_processes, int num_resources ){
    int i = process;
    bool flag=true;
    if(AdjList[i][0] == -1){ //empty
        flag = false;
        return flag;
    }
        for(int j = 0; j < AdjList[i].size(); j++)
        {

            if(j == AdjList[i].size() - 1)
            {
                /* if it is a process */
                if(AdjList[i][j] < num_processes ){
                 //   cout << " -> P" << AdjList[i][j]  << endl;
                    break;
                }else if(AdjList[i][j] >= num_resources  ){
                    flag=false;
                 //   cout << " -> R" << (AdjList[i][j] - num_processes)  << endl;
                    break;
                }
            }
            else{
                if(AdjList[i][j]<num_processes  ){
                  //  cout << " -> P" << AdjList[i][j];
                }else if(AdjList[i][j]>=num_resources  ){
                    flag=false;
                   //  cout << " -> R" << (AdjList[i][j] -  num_processes);
                }
            }
        }
    return flag;

}
/* reduce function */
void reduce(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources,vector<int> &used_num_resources_list ){
    
    for (int i = 0; i < AdjList.size(); i++)
    {
        vector<int>::iterator itr = find(AdjList[i].begin(), AdjList[i].end(), process);
        if(itr != AdjList[i].cend()){
          //  cout<< "P" <<process+1 << " present at index " << distance(AdjList[i].begin(), itr)<<endl;
            int delete_index = distance(AdjList[i].begin(), itr);
            //AdjList[i].erase (AdjList[i].begin()+delete_index);
            AdjList[i].at(delete_index) = -1;
            if(i>=num_processes ){
                used_num_resources_list[i-num_processes]++;
            }
        }else{
           //cout << "P" <<process+1 << " not found"<<endl;
        }
    }
}
/* bool is graph fully reducable */
bool isFullyReduce(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources ){
    
    bool flag =true;

        for(int p=0; p< num_processes;p++){
            if(AdjList[p].size() >= 1 && AdjList[p][0] != -1 ){
               // cout << AdjList[p][0]<<endl;
                flag =false;
            }

        }
    return flag;
    
}
/* check if requested edges can be fufilled */
bool request_edge_check(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources, vector<int> &used_num_resources_list, vector<int> num_resources_list ){
    map<pair<int, int>, int> to_delete_map;
    int flag=false;
    int deletedcount=0;
    for(int p=0; p< num_processes;p++){ //looping through all processes
        deletedcount=0;
        if( AdjList[p][0] != -1 ){ //if the process is not requesting anything or empty
            for(int j=0; j<AdjList[p].size(); j++){ //loop through processes requests
                if(p != AdjList[p][j]){   //except itself
                        if((used_num_resources_list[AdjList[p][j] - num_processes] > 0) && (used_num_resources_list[AdjList[p][j] - num_processes] <= num_resources_list[AdjList[p][j] - num_processes]  ) ){ //means the resources are available
                            cout << "adding P"<<p+1<< " to R"<< AdjList[p][j] - num_processes +1 <<endl;
                            flag=true;
                            vector<int>::iterator itr = find(AdjList[AdjList[p][j]].begin(), AdjList[AdjList[p][j]].end(), -1); //finding a negative -1
                            if(itr != AdjList[AdjList[p][j]].cend()  ){//means a previous has left and resource is available
                              
                                
                                int checkindex =distance(AdjList[AdjList[p][j]].begin(), itr);
                                if (AdjList[AdjList[p][j]][checkindex+1] == AdjList[p][j] ){
                                    AdjList[AdjList[p][j]].at(checkindex) = p;
                                    used_num_resources_list[AdjList[p][j] - num_processes]--;
                                    to_delete_map[ { p,AdjList[p][j] }]=0;
                                    to_delete_map[ { p,p }]=-1;
                                    deletedcount++;
                                }
                                       
                            }else{
                                AdjList[AdjList[p][j]].push_back(p);
                                    used_num_resources_list[AdjList[p][j] - num_processes]--;
                                to_delete_map[ { p, AdjList[p][j] }]=0;
                                to_delete_map[ { p,p }]=-1;
                            }
                        }else if(used_num_resources_list[AdjList[p][j] - num_processes] ==0){ //cannot allocate
                            //try the next resource request
                            continue;
                        }
                }
            }
        }
                    cout<< endl;
    }
    
    /* delete excess of fufilled */

    map<pair<int, int>, int>::iterator it =to_delete_map.begin();
    while(  it != to_delete_map.end()){
        int row = it->first.first;
        int searchfor =it->first.second;
        vector<int>::iterator it_r = find(AdjList[row].begin(), AdjList[row].end(), searchfor);
        if(it_r != AdjList[row].cend()){
          //  cout << "perform delete" <<endl;
            int index = distance(AdjList[row].begin(), it_r);
            if(it->second ==0){
                
                AdjList[row].erase (AdjList[row].begin()+ index);
            }
        }
        it++;
    }
    return flag;
}
// converts from adjacency matrix to adjacency list
vector<vector<int>> convert( vector<vector<int>> a, int num_processes, int num_resources, vector<int> &used_num_resources_list )
{
    
    vector<vector<int>> adjList(a.size());
    for (int i = 0; i < a.size(); i++)
    {
          
        for (int j = 0; j < a[i].size(); j++)
        {
            if (a[i][j] == 1)
            {
                adjList[i].push_back(j);
                if(i>=num_processes && j<num_processes){
                    used_num_resources_list[i-num_processes]--;
                }
            }
        }
    }
    return adjList;
}

void readfile(string textfile,  vector<int> &num_resources_list, vector<vector<int>> &AdjList, int &num_processes,int &num_resources,         vector<int> &used_num_resources_list){
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
        smatch equalr;
    regex e ("num_processes=");
    regex er ("num_resources=");
    regex comment ("%[^\n]*"  );
        regex blank ("[ \t\n]"  );
    int check_num_units=0;
    int check_adjacency_matrix=0;
    /*matching an equal sign*/
    string temp_num_processes, temp_num_resources;

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
                regex_search(line, equalr, er);
                if(equal.size() >0){
                    /* converting the suffix to an int and saving num_processes */
                    temp_num_processes = equal.suffix().str();
                    stringstream num_process_convert(temp_num_processes);
                    num_process_convert >> num_processes;
                    check_num_units++;
                    cout << "\nthe num_processes is:" << num_processes <<endl;
                    
                }else if(equalr.size() >0){
                    /* converting the suffix to an int and saving num_resources */
                    temp_num_resources = equalr.suffix().str();
                    stringstream num_resources_convert(temp_num_resources);
                    num_resources_convert >> num_resources;
                    cout << "\nthe num_resources is:" <<num_resources<<endl;
                    check_num_units++;
                }else if(check_num_units == 2 && check_adjacency_matrix != num_resources){ /* ready to receive list of units */
                    /* create string stream from the string */
                    stringstream s_stream(line);
                    while(s_stream.good()) {
                       string substr;
                       getline(s_stream, substr, ',');
                        /* get first string delimited by comma */
                        if(!substr.empty()){
                            int int_converted_one;
                            stringstream int_convert_one(substr);
                            int_convert_one >> int_converted_one;
                            num_resources_list.push_back(int_converted_one);
                            used_num_resources_list.push_back(int_converted_one);
                            check_adjacency_matrix++;
                        }

                    }


                }else if(check_adjacency_matrix == num_resources){ /* starting to parse for adjacency matrix */
                    /* create string stream from the string */
                    
                    stringstream s_stream(line);
                    while(s_stream.good()) {
                        string substr;
                        /* get first string delimited by comma */
                        getline(s_stream, substr, ',');
                        if(!substr.empty()){
                            int int_converted_two;
                            stringstream int_convert_two(substr);
                            int_convert_two >> int_converted_two;
                            result.push_back(int_converted_two);
                        }
                    }
                   // cout << "entering"<< result.size() <<endl;
                    if(result.size() >0){
                        a.push_back(result);
                    }

                    result.erase (result.begin(), result.end());

                }
            }
        }
        configf.close();
        /* creating adjacency list */
        
        AdjList = convert(a,num_processes, num_resources, used_num_resources_list);
        /*close config file*/
    }


}
void printAdjList(vector<vector<int>> AdjList, int num_processes,int num_resources){
    // print the adjacency list
    int print=0;
    for (int i = 0; i < AdjList.size(); i++)
    {

        for(int j = 0; j < AdjList[i].size(); j++)
        {
            if(print==0){
                if(i < num_processes){
                   // cout << i <<" "<< num_processes<<endl;
                    cout <<"P"<<i+1;
                }else if(i >= num_processes){
                    cout <<"R"<< (i+1 -  num_processes);
                }
                print++;
            }
            if(j == AdjList[i].size() - 1 )
            {
                if( AdjList[i][j] == -1){
                    cout << " -> " << AdjList[i][j] << endl;
                    break;
                }else if(AdjList[i][j] < num_processes){
                    cout << " -> P" << AdjList[i][j] +1 << endl;
                    break;
                }else if(AdjList[i][j] >= num_resources){
                    cout << " -> R" << (AdjList[i][j] - num_processes) +1 << endl;
                    break;
                }
            }
            else{
                if( AdjList[i][j] == -1){
                    cout << " -> " << AdjList[i][j] ;
                }else if(AdjList[i][j]<num_processes){
                    cout << " -> P" << AdjList[i][j] +1;
                }else if(AdjList[i][j]>=num_resources){
                     cout << " -> R" << (AdjList[i][j] -  num_processes)+1;
                }
            }
        }
        print=0;
    }
    cout <<endl;
}
bool availableResources(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources,vector<int> used_num_resources_list ){
    bool flag=false;
    for(int i=0; i< AdjList[process].size(); i++){
        if(AdjList[process][i] != process){ /* if not the same process check the resource*/
            if(used_num_resources_list[AdjList[process][i] - num_processes] >0){
                return true;
            }
        }
    }
    return false;
}
int main(int argc, char *argv[]){
    vector<int> num_resources_list;
        vector<int> used_num_resources_list;
    vector<vector<int>> AdjList ;
    int num_processes, num_resources;
    string textfile;
    /* text file */
    if(argc != 2){
        cout<<"usage: " << argv[0] << " <filename>\n";
        exit(1);
    }else{
        textfile = argv[1];
    }
    /*enter the text file */

    /*get the text file*/
    
    readfile(textfile, num_resources_list, AdjList, num_processes, num_resources, used_num_resources_list);
    cout << "\nthe num_processes is:" << num_processes <<endl;
    cout << "\nthe num_resources is:" << num_resources<<endl;
    
    /* print list of num_resource_list */
    cout << "\nthe number of units of each resource:";
    for(int i = 0; i<num_resources_list.size(); i++) {
       cout << num_resources_list.at(i) <<",";
    }
    cout << endl;
    cout<<endl<<"Adjacency List:"<<endl;
    printAdjList(AdjList, num_processes, num_resources);
    int times =0;
    while(times==0){
        /* checking if a process is a sink */
        for (int i = 0; i <num_processes; i++)
        {

           bool sink= isSink(AdjList, i,  num_processes,  num_resources );
           cout << "Is process " << i+1 << " a sink?(R1+R2+R3)=0: " << boolalpha <<(sink==1)<<endl;
                printAdjList(AdjList, num_processes, num_resources);
            if(sink == 1){
                cout << "Reducing it..."<<endl;
                reduce(AdjList, i,  num_processes,  num_resources, used_num_resources_list);
                    printAdjList(AdjList, num_processes, num_resources);
                bool reducable = isFullyReduce(AdjList, i,  num_processes,  num_resources );
                cout << "Is graph fully reducible(0 processes): " << boolalpha <<(reducable==1)<<endl;
                    printAdjList(AdjList, num_processes, num_resources);
                if(reducable == true){
                    cout << "-------No Deadlock Detected" <<endl;
                    printAdjList(AdjList, num_processes, num_resources);
                    times=1;
                    break;
                }else if(reducable == false){
                    bool edge_check = request_edge_check(AdjList, i,  num_processes,  num_resources,used_num_resources_list, num_resources_list );
                     cout << "Can request edges be filled:" << boolalpha <<(edge_check==1)<<endl;
                    printAdjList(AdjList, num_processes, num_resources);
                    
                    if(edge_check==false){
                        cout << "------Deadlock Detected" <<endl;
                        printAdjList(AdjList, num_processes, num_resources);
                        times=1;
                        break;
                    }
                }
                       
            }else if(i != num_processes && sink==0){
                /* check if the resource can be used */
                bool res_check =availableResources(AdjList, i, num_processes,  num_resources,used_num_resources_list );
                
                cout << "Can Process allocate resources:" << boolalpha <<(res_check==1)<<endl;
                if(res_check == true){
                    printAdjList(AdjList, num_processes, num_resources);
                }
                else if( (res_check ==false) && (i==(num_processes-1)) ){
                    cout << "****Deadlock Detected" <<endl;
                    times=1;
                    break;
                }else if(res_check ==false && i<num_processes){
                    cout << "checking next process" <<endl;
                }
                
            }

        }
        //times++;
    }
    cout <<endl;

    
}
