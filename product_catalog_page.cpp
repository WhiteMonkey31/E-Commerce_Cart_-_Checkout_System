#include<iostream>
#include<string>
#include<math.h>

using namespace std;

#include "shared_definations.h"

const int p_MAX=100;
// int p_count=0;

struct product{
    int p_id;
    string p_name,
        p_description;
    float p_price;
};


class product_catalog{
    private:
        product p_list[p_MAX];
        int p_count;

    public:
        product_catalog(){
            p_count=0;
        }
        void add_product();
        void edit_product();
        void delete_product();
        void display_all_product();
        void search_product();
        void sort_product();
};

void product_catalog::add_product(){

    if(p_count>=p_MAX){
        cout<<"Product Catalog is Full. Cannot add more products.\n";
        return;
    }

    product &p=p_list[p_MAX];
    p.p_id = p_count;
    
    cout<<"Enter Product Details: \n"
        <<"Enter Name: ";
    cin.ignore();
    getline(cin,p.p_name);
    cout<<"Enter Description: ";
    getline(cin,p.p_description);
    cout<<"Enter Price: ";
    cin>>p.p_price;

    
    cout<<"\nProduct Added Successfully.\n"
        <<"Product ID: "<<p.p_id<<endl;
    p_count++;
    
}

void product_catalog::edit_product(){
    int c_id;
    cout<<"Enter Product ID to Edit: ";
    cin>>c_id;
    if(c_id<0 || c_id>=p_count){
        cout<<"Invalid Product ID.\n";
        return;
    }
    for(int i=0;i<p_count;i++){
        if(i==c_id){
            cout<<"Editing Product ID: "<<i<<endl
                <<"Current Name: "<<p_list[i].p_name<<endl
                <<"New Name: ";
            cin.ignore();
            getline(cin,p_list[i].p_name);
            cout<<"Current Description: "<<p_list[i].p_description<<endl
                <<"New Description: ";
            getline(cin,p_list[i].p_description);
            cout<<"Current Price: "<<p_list[i].p_price<<endl
                <<"New Price: ";
            cin>>p_list[i].p_price;
            cout<<"\nProduct Edited Successfully.\n";

        }
    }
}

void product_catalog::delete_product(){
    int d_id;
    cout<<"Enter Product ID to Delete: ";
    cin>>d_id;
    if(d_id<0 || d_id>=p_count){
        cout<<"Invalid Product ID Entered.\n";
        return;

    }

    for(int i=0;i<p_count;i++){
        if(i==d_id){
            cout<<"Deleting Product ID: "<<i<<endl
                <<"Product Name: "<<p_list[i].p_name<<endl
                <<"Product Description: "<<p_list[i].p_description<<endl
                <<"Product Price: "<<p_list[i].p_price<<endl
                <<"\nAre you sure you want to delete this product? (1/0): \n"
                <<"1. Yes\n"
                <<"0. No\n"
                <<"Enter Choice: ";
            int d_choice;
            cin>>d_choice;
            if(d_choice==1){
                for(int j=i;j<p_count-1;j++){
                    p_list[i].p_id=j+1;
                    p_list[i].p_name=p_list[j+1].p_name;
                    p_list[i].p_description=p_list[j+1].p_description;
                    p_list[i].p_price=p_list[j+1].p_price;
                    
                }
                p_count--;
                cout<<"Product Delected Successfully.\n";
                return;
                
            }else{
                cout<<"Product Deletion Cancelled.\n";
                return;
            }
        }
    }
}

void product_catalog::display_all_product(){
    if(p_count==0){
        cout<<"No Product Available.\n";
        return;
    }
    cout<<"Product Catalog:\n"<<endl;
    for(int i=0;i<p_count;i++){
        cout<<"Product ID: "<<p_list[i].p_id<<endl
            <<"Product Name: "<<p_list[i].p_name<<endl
            <<"Product Description: "<<p_list[i].p_description<<endl
            <<"Product Price: "<<p_list[i].p_price<<endl
            <<"-----------------------------\n";
    }
    cout<<"Total Products: "<<p_count<<endl;

}

void product_catalog::search_product(){
    string s_name;
    cout<<"Enter Product Name to Search: ";
    getline(cin,s_name);
    bool s_found=false;
    for(int i=0;i<p_count;i++){
        if(s_name==p_list[i].p_name){
            cout<<"Product Found: \n"
                <<"Product ID: "<<p_list[i].p_id<<endl
                <<"Product Name: "<<p_list[i].p_name<<endl
                <<"Product Description: "<<p_list[i].p_description<<endl
                <<"Product Price: "<<p_list[i].p_price<<endl;
                s_found=true;               
               break;
        }
    }
    if(!s_found){
        cout<<"Product Not Found.\n";
    }
}

void product_catalog::sort_product(){

    if(p_count==0){
        cout<<"No Product Available to sort.\n";
        return;
    }
    for(int i=0;i<p_count-1;i++){
        for(int j=i+1;j<p_count;j++){
            if(p_list[i].p_price>p_list[j].p_price){
                swap(p_list[i].p_id,p_list[j].p_id);
                swap(p_list[i].p_name,p_list[j].p_name);
                swap(p_list[i].p_description,p_list[j].p_description);
                swap(p_list[i].p_price,p_list[j].p_price);
            }
        }
    }
}