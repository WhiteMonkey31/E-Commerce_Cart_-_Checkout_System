
void write_user_file(user_data ud[],int u_count){
    ofstream user_outfile("users.txt");
    user_outfile.open("users.txt",std::ios_base::app);
    
    for(int i=0;i<u_count;i++){
        outfile<<ud[i].id<<"\\"<<ud[i].username<<"\\"<<ud[i].password<<endl;
    }
    
}

void read_user_file(user_data ud[], int u_count){
    
}