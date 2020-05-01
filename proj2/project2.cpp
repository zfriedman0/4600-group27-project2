#include <iostream>    //cin, cout ..
#include <string>   //string
#include <fstream>  //ifstrean
#include <sstream>  //string strings
#include <regex>    //regex library
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <string.h>


using namespace std;
/* bool is process a sink */
vector<vector<int>> convert( vector<vector<int>> a, int num_processes, int num_resources, vector<int> &available_num_resources_list )
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
                    available_num_resources_list[i-num_processes]--;
                }
            }
        }
    }
    return adjList;
}

void readfile(string textfile,  vector<int> &num_resources_list, vector<vector<int>> &AdjList, int &num_processes,int &num_resources,         vector<int> &available_num_resources_list){
    vector<int> result;
    vector<vector<int>> a;
    fstream configf;
    configf.open( textfile, ios::in );


    smatch equal;
    smatch equalr;
    regex e ("num_processes=");

    regex er ("num_resources=");
    regex comment ("%[^\n]*" );
        regex blank ("[\t\n]+"  );
     regex emptyspace ("[\\ \t\b\f\r\n]+"  );
    int check_num_units=0;
    int check_adjacency_matrix=0;
    /*matching an equal sign*/
    string temp_num_processes, temp_num_resources;

    if(configf.is_open()){
            string line;
        /*continues if the file is opened*/
        while(getline(configf, line)){
            /*while were able to getline from the file*/
            
            if(regex_match (line, comment) || regex_match (line, blank) ||regex_match(line, emptyspace) ||line.empty()){
                /* there was a comment match */
               // cout <<"--line"<< line <<endl;
                line.clear();
                continue;
            }else{
        
                cout<<line<< endl;
                if(check_num_units<2){
                  //  cout <<"1--line"<< check_num_units <<":"<< line <<endl;
                    regex_search(line, equal, e);
                    regex_search(line, equalr, er);
                    line.clear();
                    
                }
                
                if(equal.size() > 0 && check_num_units <2){
                    /* converting the suffix to an int and saving num_processes */
                   // cout <<"2--line"<< (int)line[0] <<":"<< line <<endl;
                    temp_num_processes = equal.suffix().str();
                    stringstream num_process_convert(temp_num_processes);
                    num_process_convert >> num_processes;
                    check_num_units++;
                   // cout << "\nthe num_processes is:" << num_processes <<endl;
                    line.clear();
                    continue;
                }else if(equalr.size() >0 && check_num_units <2){
                   // cout <<"3--line"<< (int)line[0] <<":"<< line <<endl;
                    /* converting the suffix to an int and saving num_resources */
                    temp_num_resources = equalr.suffix().str();
                    stringstream num_resources_convert(temp_num_resources);
                    num_resources_convert >> num_resources;
                    //cout << "\nthe num_resources is:" <<num_resources<<endl;
                    check_num_units++;
                    line.clear();
                    continue;
                }else if(check_num_units == 2 && check_adjacency_matrix < num_resources){ /* ready to receive list of units */
                    /* create string stream from the string */
                    //cout <<"4--line"<< check_adjacency_matrix <<":"<< line <<endl;
                    std::string tempstring (line) ;
                    tempstring += '\0';
                    
                    stringstream so(tempstring);
                    string substr_one;
                    while(getline(so, substr_one, ',')){
     
                      // cout << substr_one << endl;
                        
                        int int_converted_one;
                        stringstream int_convert_one(substr_one);
                        int_convert_one >> int_converted_one;
                        num_resources_list.push_back(int_converted_one);
                        available_num_resources_list.push_back(int_converted_one);
                        check_adjacency_matrix++;
                        /* get first string delimited by comma */
                    }
                    line.clear();
                    
                    continue;
                }else if(check_adjacency_matrix == num_resources){ /* starting to parse for adjacency matrix */
                    /* create string stream from the string */
                    //cout <<"5--line"<< (int)line[0] <<":"<< line <<endl;
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
                    line.clear();
                }
            }

        }
                configf.close();
        /*close config file*/
    }
    /* creating adjacency list */
    AdjList = convert(a,num_processes, num_resources, available_num_resources_list);
    

}
void printAdjList(vector<vector<int>> AdjList, int num_processes, int num_resources){
    // print the adjacency list
    //int print=0;
    for (int i = 0; i < AdjList.size(); i++)
    {
        if(i < num_processes){
           cout << "P" << i;
        }else if(i>=num_processes){
            cout << "R" << i-num_processes;
        }
        for(int j = 0; j < AdjList[i].size(); j++)
        {
 
            if(j == AdjList[i].size() - 1 )
            {
                if(AdjList[i][j] < num_processes ){
                    cout << " -> P" << AdjList[i][j] << endl;
                    break;
                }else if(AdjList[i][j] >= num_processes){
                    cout << " -> R" << AdjList[i][j] -num_processes<< endl;
                    break;
                }
                
            }
            else{
                if(AdjList[i][j] < num_processes){
                    cout << " -> P" << AdjList[i][j];
                }else if(AdjList[i][j] >= num_processes){
                    cout << " -> R" << AdjList[i][j]-num_processes;
                }
            }
        }
        
    }
    cout <<endl;
}
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
void reduce(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources,vector<int> &available_num_resources_list ){
    
    for (int i = 0; i < AdjList.size(); i++)
    {
        vector<int>::iterator itr = find(AdjList[i].begin(), AdjList[i].end(), process);
        if(itr != AdjList[i].cend()){
          //  cout<< "P" <<process+1 << " present at index " << distance(AdjList[i].begin(), itr)<<endl;
            int delete_index = distance(AdjList[i].begin(), itr);
            //AdjList[i].erase (AdjList[i].begin()+delete_index);
            AdjList[i].at(delete_index) = -1;
            if(i>=num_processes ){
                available_num_resources_list[i-num_processes]++;
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
bool request_edge_check(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources, vector<int> &available_num_resources_list, vector<int> num_resources_list ){
    map<pair<int, int>, int> to_delete_map;
    int flag=false;
    int deletedcount=0;
    for(int p=0; p< num_processes;p++){ //looping through all processes
        deletedcount=0;
        if( AdjList[p][0] != -1 ){ //if the process is not requesting anything or empty
            for(int j=0; j<AdjList[p].size(); j++){ //loop through processes requests
                if(p != AdjList[p][j]){   //except itself
                        if((available_num_resources_list[AdjList[p][j] - num_processes] > 0) && (available_num_resources_list[AdjList[p][j] - num_processes] <= num_resources_list[AdjList[p][j] - num_processes]  ) ){ //means the resources are available
                            cout << "adding P"<<p+1<< " to R"<< AdjList[p][j] - num_processes +1 <<endl;
                            flag=true;
                            vector<int>::iterator itr = find(AdjList[AdjList[p][j]].begin(), AdjList[AdjList[p][j]].end(), -1); //finding a negative -1
                            if(itr != AdjList[AdjList[p][j]].cend()  ){//means a previous has left and resource is available
                              
                                
                                int checkindex =distance(AdjList[AdjList[p][j]].begin(), itr);
                                if (AdjList[AdjList[p][j]][checkindex+1] == AdjList[p][j] ){
                                    AdjList[AdjList[p][j]].at(checkindex) = p;
                                    available_num_resources_list[AdjList[p][j] - num_processes]--;
                                    to_delete_map[ { p,AdjList[p][j] }]=0;
                                    to_delete_map[ { p,p }]=-1;
                                    deletedcount++;
                                }
                                       
                            }else{
                                AdjList[AdjList[p][j]].push_back(p);
                                available_num_resources_list[AdjList[p][j] - num_processes]--;
                                to_delete_map[ { p, AdjList[p][j] }]=0;
                                to_delete_map[ { p,p }]=-1;
                            }
                        }else if(available_num_resources_list[AdjList[p][j] - num_processes] ==0){ //cannot allocate
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

bool availableResources(vector<vector<int>> &AdjList, int process, int num_processes, int num_resources,vector<int> available_num_resources_list ){
    bool flag=false;
    for(int i=0; i< AdjList[process].size(); i++){
        if(AdjList[process][i] != process){ /* if not the same process check the resource*/
            if(available_num_resources_list[AdjList[process][i] - num_processes] >0){
                return true;
            }
        }
    }
    return false;
}
int main(int argc, char *argv[]){
    vector<int> num_resources_list; //given
    vector<int> available_num_resources_list; /* availble resources*/
    vector<vector<int>> AdjList ; /* adjacency matrix */
    int num_processes, num_resources;
    string textfile;
    /* text file */
    if(argc != 2){
        cout<<"usage: " << argv[0] << " <filename>\n";
        exit(1);
    }else{
        textfile = argv[1];
    }


    /*get the text file*/
    
    readfile(textfile, num_resources_list, AdjList, num_processes, num_resources, available_num_resources_list);
    cout << "\nthe num_processes is:" << num_processes <<endl;
    cout << "\nthe num_resources is:" << num_resources<<endl;
    
    /* print list of num_resource_list */
    cout << "\nthe number of units of each resource:"<<endl;
    for(int i = 0; i<num_resources_list.size(); i++) {
        cout << "R" <<i <<":"<< num_resources_list.at(i) <<",";
    }
    cout << endl;
    /* print list available num_resource_list */
    cout << "\nthe currently available of units of each resource:";
    for(int i = 0; i<available_num_resources_list.size(); i++) {
       cout << "R" <<i <<":"<< available_num_resources_list.at(i) <<",";
    }
    cout << endl;
    cout<<endl<<"Adjacency List:"<<endl;
    printAdjList(AdjList, num_processes, num_resources);
    int checked[num_processes]={1};
   // printf("\n");
   // cout << "\nchecked:"<<endl;
    for(int i = 0; i<num_processes; i++) {
        checked[i]=1;
       //cout << "P" <<i <<":"<< checked[i] <<",";
    }

    for (int i = num_processes; i < num_resources+num_processes; i++) {
                   // cout << "here"<<AdjList[i][1]<< endl;
        for(int j=0;j<AdjList[i].size();j++){
            
            if(AdjList[i][j]<num_processes){
              
                checked[AdjList[i][j]]=0;
            }
        }

    }
    printf("\n");
   // cout << "\nchecked:"<<endl;
    for(int i = 0; i<num_processes; i++) {
       cout << "P" <<i <<":"<< checked[i] <<",";
    }
    
    for(int i=0; i < num_processes; i++){
        int request_fufilled=0;
        if(checked[i] != 1){ //have not seen this process
            
            bool flag=false;
            bool sinkflag=true;
          //  cout << "\nthe currently available of units of each resource:"<<endl;
            for(int i = 0; i<available_num_resources_list.size(); i++) {
             //  cout << "R" <<i <<":"<< available_num_resources_list.at(i) <<",";
            }
            
                       // cout << "\nrequesting:"<<endl;
            for(int j=0; j< AdjList[i].size(); j++){
                
                if(AdjList[i][j] >= num_processes){
                  //  cout << "R" <<AdjList[i][j]-num_processes ;
                    if(available_num_resources_list[AdjList[i][j] - num_processes ] > 0){
                        flag = true;
                        
                    }else{

                        flag=false;
                        break;
                    }

                }
                
            }
           // cout <<endl;
            if(flag == true){ //can be fufilled
                checked[i] = 1;
              //  cout <<"P"<<i<< " request can be fufilled" <<endl;
            //    for(int m=0; m<num_resources; m++){
                    for(int i=num_processes; i<num_resources+num_processes;i++ ){
                        vector<int>::iterator itr = find(AdjList[i].begin(), AdjList[i].end(), i);
                        if(itr != AdjList[i].cend()){
                             // cout << "pindex:"<< i+1<< endl;
                                available_num_resources_list[(i+1)-num_processes]+=1;
                                break;
                        }
                    }

                i=-1;
            }else{
                if(i == AdjList[i][0] &&  AdjList[i].size() ==1){
                   // cout <<"SINNNKKK"<<endl;
                    for(int p=num_processes; p<num_resources+num_processes;p++ ){
                        vector<int>::iterator itr = find(AdjList[p].begin(), AdjList[p].end(), i);
                        if(itr != AdjList[p].cend()){
                           //    cout << "index:"<< p-num_processes<< endl;
                                available_num_resources_list[ p-num_processes]+=1;
                            checked[i]=1;
                            
                        }
                    }

                    i=-1;
                }else{
                
                cout <<"P"<<i<< " request cannot be fufilled; trying next process" <<endl;
                }
            }
            cout <<endl;
            
        }
    }
    
    cout << "\nthe currently available of units of each resource:";
    for(int i = 0; i<available_num_resources_list.size(); i++) {
       cout << "R" <<i <<":"<< available_num_resources_list.at(i) <<",";
    }
    
    cout <<endl;
    
    int deadlock =0;
    for(int i=0; i< num_processes; i++){
        if(checked[i] != 1){
            deadlock =1;
        }
    }
    if(deadlock){
        cout <<"Deadlock detected"<<endl;
    }else{
        cout <<"No deadlock detected"<<endl;
    }
   
    
}
