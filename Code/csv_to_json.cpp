#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector> 
#include <string>

using namespace std;


typedef struct{
    string User_ID;
    string Screenname;
}basic_data;

void read_file(vector<basic_data> * list, string path, short int column_a, short int column_b);
void write_file(vector<basic_data> * list);

string column_selector(string line, short int n);


int main(int argc, char* argv[]){

    vector<basic_data> list;

    string path = argv[1];
    short int user_id_column = stoi(argv[2]);
    short int screenname_column = stoi(argv[3]);
    if(argc > 4){
        cerr << "Wrong Usage, Please Follow: /path/to/your/csvfile.csv USERID_COLUMN SCREENNAME_COLUMN" << endl;
        exit(1);
    }
    else
        cout << "Generating JSON..." << endl;

    read_file(&list, path, user_id_column , screenname_column);
    write_file(&list);

    cout << "Done!" << endl;

    return 0;

}



void read_file(vector<basic_data> * list, string path, short int column_a, short int column_b){
    
    string line;  
    
    ifstream csv_input;
    csv_input.open(path);
    if(!csv_input.is_open()){
        cerr << "Failed to open: " << path << endl;
        exit(1);
    }

    while(getline(csv_input, line)){
        
        basic_data element;

        element.User_ID = column_selector(line, column_a);
        element.Screenname = column_selector(line, column_b);

        list->push_back(element);
        
    }

    csv_input.close();

}


void write_file(vector<basic_data> * list){

    ofstream json_output;
    json_output.open("List.json");
    if(!json_output.is_open()){
        cerr << "Failed to create file." << endl;
        exit(1);
    }


    /* This structure is needed for every single profile :
        {
	        "user_id": "string",
	        "user": { "name": "string", "screen_name": "string" },
	        "reason": number
        } 
    */ 

    json_output << '[' << endl;
    for(int i = 0; i < list->size(); i++){

        json_output << "\t{" << endl;

        json_output << "\t\t\"user_id\": \"" << (*list)[i].User_ID << "\"," << endl;
        json_output << "\t\t\"user\": { \"name\": \"" << (*list)[i].Screenname << "\", \"screen_name\": \"" << (*list)[i].Screenname << "\" }," << endl;
        json_output << "\t\t\"reason\": " << 0 << endl;

        // Eliminate the last comma
        if(i < list->size() - 1)
            json_output << "\t}," << endl;
        else
            json_output << "\t}" << endl;
        
    }
    json_output << ']';

    json_output.close();

}



string column_selector(string line, short int n){
    
    int column_counter = 0;
    int position = line.find(',', 0);;
    short int auxiliar_position = 0;

    if(n == 0)
        return line.substr(0, position);
    else{

        while(column_counter < n){
            
            auxiliar_position = position;
            // In case of consecutive commas
            while(line[auxiliar_position] == ',')
                auxiliar_position++;
            position = line.find(',', auxiliar_position);
           

            // In case the position found is invalid (no position)
            if(position == string::npos)
                position = line.length();
            /* "auxiliar_position" can't be bigger than "position". So if it turns to be, 
               it means position > line.lenght twice, which is wrong. */ 
            if(auxiliar_position >= line.length()){
                cerr << "Column " << n << " not Found!" << endl;
                exit(1);
            }
                
            column_counter++;

        }
        return line.substr(auxiliar_position, position - auxiliar_position); // (Start, How Many Letters from Start)
    }
}
