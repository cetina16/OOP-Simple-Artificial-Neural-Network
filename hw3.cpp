/*
AYSE BETUL CETİN  ----  id: 150180730

g++ main.cpp -o main
./main input1.txt

*/
#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

class Neuron{ // abstract base class
   protected:
	double value_z;
	double activated_value_a;
	public:
   	Neuron(){value_z=0; activated_value_a=0;}
	double getter_z(){return value_z;}     // getter-setter methods 
	double getter_a(){return activated_value_a;}
	void setter_a(double in){activated_value_a = in;}
	void setter_z(double in){value_z = in;}
	virtual void activate()=0;   //pure virtual method
  virtual ~Neuron(){} //destructor

};

class SigmoidNeuron:public Neuron{
   public:
   SigmoidNeuron(){
	activated_value_a=0;  // cout << "Sigmoid constructor"<<endl;
   }
	void activate(){activated_value_a = 1 / (1+ exp (0 - value_z));}
};

class ReluNeuron:public Neuron{
   public:
   	ReluNeuron(){
		activated_value_a=0;   //cout<< "ReluNeuron constructor"<< endl;
	}
	void activate(){  
		activated_value_a = max(0.0,value_z);
	}
};

class LReluNeuron:public Neuron{
   public:
   	LReluNeuron(){
		 activated_value_a=0;    //cout<< "LReluNeuron constructor"<< endl;
	}
	void activate(){  activated_value_a = max (0.1*value_z , value_z);}
};

class Layer{   
	int number_of_neuron;
	Neuron *head;   // neuron array
	public:
	Neuron* getter_head(){return head;}
	Layer(){head = NULL;}  // default constructor
	Layer(int , int ,int *);  //constructor for layer1
	Layer(int ,int ,double *,int) ; //constructor for other layers
	void print_a(int) const;  
	~Layer(){} //destructor
};

Layer::Layer(int t_neurons,int t_neuron_type,int *arr_input){   // This constructor is for first layer
	if(t_neuron_type!=0 && t_neuron_type!=1 && t_neuron_type!=2 ){
		throw "Unidentified activation function!" ;  // error handling for wrong type
	}
	number_of_neuron = t_neurons;
	if(t_neuron_type == 0){
		head = new SigmoidNeuron[number_of_neuron];
	}else if(t_neuron_type == 1){
		head = new LReluNeuron[number_of_neuron];
	}else if(t_neuron_type == 2){
		head = new ReluNeuron[number_of_neuron];
	}
	for(int i=0;i<number_of_neuron;i++){   // create neurons
		head[i].setter_z(arr_input[i]) ;
		head[i].setter_a(arr_input[i]);
	}
//	cout << "first layer constructor"<<endl;
}
Layer::Layer(int t_neurons,int neuron_type,double *arr_input,int inputsize){
	number_of_neuron = t_neurons;
	if(neuron_type!=0 && neuron_type!=1 && neuron_type!=2 ){
		throw "Unidentified activation function!" ;  // error handling for wrong type
	}
	
	double X[inputsize];
	for(int i=0;i<inputsize;i++){   // Matrix X (input)
		X[i] = arr_input[i];
	}
	double W[number_of_neuron][inputsize] ;  // Matrix W 
	for(int i=0;i<number_of_neuron;i++){   // filling the matrix W by 0.1 
		for (int j=0;j<inputsize;j++){
			W[i][j] = 0.1;
		}
	}
	double B[number_of_neuron];    // Matrix B
	for(int i=0;i<number_of_neuron;i++){
		B[i] = 0.1;               // filling the matrix B by 0.1 
	}
	double z_matrix[number_of_neuron]; // = W*X + B;
	for(int i=0;i<number_of_neuron;i++){
		z_matrix[i] =0;
		for(int j=0;j<inputsize;j++){
			z_matrix[i] += W[i][j] * X[j] ;
		}
		z_matrix[i] += B[i];
	}
	if(neuron_type == 0){        // SigmoidNeuron
		head = new SigmoidNeuron[number_of_neuron];
	}else if(neuron_type == 1){
		head = new LReluNeuron[number_of_neuron];   // LReluNeuron
	}else if(neuron_type == 2){
		head = new ReluNeuron[number_of_neuron];   // ReluNeuron
	}
	for(int i=0;i<number_of_neuron;i++){
		head[i].setter_z(z_matrix[i]);
		head[i].activate();     // assign activated value 
	}
	//cout << "layer constructor"<<endl;
}

void Layer::print_a(int num_of_neurons)  const
{
	for(int i=0;i<num_of_neurons;i++){
		cout << head[i].getter_a() << endl;
	}
}

class Network{
	int layer_size;
	Layer* layerhead;   // layer array
    public:
	Network(){layerhead=NULL;}  // default constructor
	Network(int);  // number of layers and first layer as parameter
	void print(int*)   const;
	void AddingLayers(Layer*);
	void Addingfirstlayer(int,int,int*);
	~Network(); //destructor
};

	
Network::Network(int num_of_layers ){   // network contructor
	layer_size = num_of_layers;   // how many layers included in this network
	layerhead = NULL;
	//cout << "Network constructor"<<endl;
}
void Network::print(int *neuron_layer)  const
{
	for(int i=0;i<layer_size;i++){
		cout << "Layer "<<i<<":"<<endl;
		layerhead[i].print_a(neuron_layer[i]);   // printing  by calling print_a function
	}
}
void Network::AddingLayers(Layer* ptr){
	layerhead = ptr;
}

Network::~Network(){
	if(layerhead && layerhead->getter_head()){
		Neuron* traverse = layerhead->getter_head();
		delete [] traverse;
		traverse = NULL;
	}
	if(layerhead){
		delete [] layerhead;
		layerhead  = NULL;
	}
	//cout << "Network destructor" << endl;
}


int func(const char* file_name){
	ifstream input_file;
	input_file.open(file_name);
	if (!input_file) {      // controlling the file
		cerr << "Error...unable to open file"<<endl;
		return 0;   
	}
	int num_of_layers ;
	input_file >> num_of_layers;
	
	int x;
	int neuron_layer[num_of_layers];
	for(int i=0;i<num_of_layers;i++){
		input_file >> x;  // number of neurons these layers have.
		neuron_layer[i] = x;
	}
	
	int layer_types[num_of_layers];
	for(int i=0;i<num_of_layers;i++){   // type of neurons for layers
		input_file >> x;
		layer_types[i] = x;
	}
	
	int number_of_input_x = neuron_layer[0];  // number of inputs equal to number of neurons of first layer
	int input_values[number_of_input_x];

	for(int i=0;i<number_of_input_x;i++){  // input values
		if(!input_file >> x){       // controlling the input shape
			throw "Input shape does not match!";  
		}
		input_file >> x;
		input_values[i]=x;
	}
	if(input_file >> x){    // controlling the input shape
		throw "Input shape does not match!";
	}
	try{                 // try and catch blocks for controlling type of activation function
		Network MyNetwork(num_of_layers );   // creating network and add input layer
		
		Layer* ptr = new Layer[num_of_layers];  // dynamic array of layers
		ptr[0] =  Layer(neuron_layer[0],layer_types[0],input_values);  // input layer added 
		
		for(int i=1;i<num_of_layers;i++){
			double input_arr [neuron_layer[i-1]];
			for(int j=0;j<neuron_layer[i-1];j++){
				Neuron* traverse = ptr[i-1].getter_head();
				input_arr[j]= traverse[j].getter_a();
			}
			
			int inputsize = neuron_layer[i-1];
			
			Layer L1 = Layer(neuron_layer[i] ,layer_types[i] ,input_arr,inputsize);
			ptr[i]= L1;
		}
	
		MyNetwork.AddingLayers(ptr);    // adding other layers

		MyNetwork.print(neuron_layer);   // printing
	}
	catch(const char* result){
		cout << result << endl;
		return 0;
	}
	
	input_file.close();
	return 1;
}
int main(int argc, char *argv[]){

	const char* t_file_name =  argv[1];
	try{                                   // try and catch block for controlling input shape
		func(t_file_name);
	}
	catch(const char* in){
		cout<<in<<endl;
	}
	return 0;
}