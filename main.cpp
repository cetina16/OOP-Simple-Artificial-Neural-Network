/*
AYSE BETUL CETİN  ----  id: 150180730

g++ main.cpp -o main
./main input1.txt

*/
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

class Neuron{                                  // abstract base class
   protected:
	double value_z;
	double activated_value_a;
	public:
		Neuron(){value_z = 0;activated_value_a = 0;}
		double getter_z(){return value_z;}      // getter-setter methods 
		double getter_a(){return activated_value_a;}
		void setter_a(double in){activated_value_a = in;}
		void setter_z(double in){value_z = in;}
		virtual void activate() = 0;           //pure virtual method
		virtual ~Neuron(){}         
};

class SigmoidNeuron:public Neuron{ 
   public:
		SigmoidNeuron(){
			activated_value_a = 0;
		}
		void activate(){
			activated_value_a = 1 / (1 + exp (0 - value_z)) ;
		}
		 ~SigmoidNeuron(){}  
};

class ReluNeuron:public Neuron{
   public:
		ReluNeuron(){
			activated_value_a = 0;   //cout<< "ReluNeuron constructor"<< endl;
		}
		void activate(){  
			activated_value_a = max(0.0 , value_z);
		}
		~ReluNeuron(){}  
};

class LReluNeuron:public Neuron{
   public:
   	LReluNeuron(){
		activated_value_a = 0;    //cout<< "LReluNeuron constructor"<< endl;
	}
	void activate(){ activated_value_a = max (0.1 * value_z , value_z );}
	~LReluNeuron(){}  
};

class Layer{   
	int number_of_neuron;
	Neuron *head;            // head of neuron array
	public:
	Layer(){head = NULL; number_of_neuron=0;}        // default constructor
	void firstLayer(int,int,int*);           // for initial layer : layer1
	void otherLayer(int,int,double*,int) ;      //for other layers
	Layer(const Layer&);      // copy constructor
	Neuron* getter_head(){return head;}
	void print_a(int) const;  
	void operator = (const Layer &);
	~Layer(){
		if(head){
			//cout << "layer destructor" << endl;
			delete [] head;
			head = NULL;
		}
	}
};

void Layer::operator=(const Layer &in){
	//cout << "operator" << endl;
	head = in.head;
	number_of_neuron = in.number_of_neuron;
}
Layer::Layer(const Layer &obj){ // copy constructor
	number_of_neuron = obj.number_of_neuron;
	head = obj.head;
	//cout << "layer copy constructor"<<endl;
}
void Layer::firstLayer(int t_neurons,int t_neuron_type,int *arr_input){     // This is for first layer
	if(t_neuron_type != 0 && t_neuron_type != 1 && t_neuron_type != 2 ){
		throw "Unidentified activation function!";         // error handling for wrong type
	}
	number_of_neuron = t_neurons;
	if(t_neuron_type == 0){
		head = new SigmoidNeuron[number_of_neuron];
	}else if(t_neuron_type == 1){
		head = new LReluNeuron[number_of_neuron];
	}else if(t_neuron_type == 2){
		head = new ReluNeuron[number_of_neuron];
	}
	for(int i = 0 ; i < number_of_neuron ; i++){   // create neurons
		head[i].setter_z(arr_input[i]);
		head[i].setter_a(arr_input[i]);
	}
}

void Layer::otherLayer(int t_neurons,int neuron_type,double *arr_input,int inputsize){
	number_of_neuron = t_neurons;
	if(neuron_type != 0 && neuron_type != 1 && neuron_type != 2 ){
		throw "Unidentified activation function!" ;  // error handling for wrong type
	}
	double X[inputsize];
	double W[number_of_neuron][inputsize] ;    // Matrix W 
	double B[number_of_neuron];               // Matrix B
	double z_matrix[number_of_neuron];        // = W*X + B;
	
	for(int i = 0; i < inputsize; i++){   // filling Matrix X (input)
		X[i] = arr_input[i];
	}
	
	for(int i = 0 ; i < number_of_neuron; i++){   // filling the matrix W by 0.1 
		for (int j=0;j<inputsize;j++){
			W[i][j] = 0.1;
		}
	}
	
	for(int i = 0; i < number_of_neuron ; i++){
		B[i] = 0.1;               // filling the matrix B by 0.1 
	}
	
	for(int i = 0; i < number_of_neuron; i++){
		z_matrix[i] = 0;
		for(int j = 0; j < inputsize; j++){
			z_matrix[i] += W[i][j] * X[j];
		}
		z_matrix[i] += B[i];
	}
	
	if(neuron_type == 0){                            // SigmoidNeuron
		head = new SigmoidNeuron[number_of_neuron];
	}else if(neuron_type == 1){
		head = new LReluNeuron[number_of_neuron];   // LReluNeuron
	}else if(neuron_type == 2){
		head = new ReluNeuron[number_of_neuron];   // ReluNeuron
	}
	for(int i = 0 ; i < number_of_neuron; i++){
		head[i].setter_z(z_matrix[i]);
		head[i].activate();             // assign activated value 
	}
	
}

void Layer::print_a(int num_of_neurons) const
{
	for(int i = 0 ; i < num_of_neurons ; i++){
		cout << head[i].getter_a() << endl ;
	}
}

class Network{
	int layer_size;
	Layer* layerhead;   // layer array
    public:
	Network(const char*);        
	void print(int*) const;
	~Network();      //destructor
};

	
Network::Network(const char* file_name){   // network contructor
	ifstream input_file;
	input_file.open(file_name);
	if (!input_file) {      // controlling the file
		cerr << "Error...unable to open file"<<endl;
		//return 0;   
	}
	int num_of_layers ;
	input_file >> num_of_layers;
	layer_size = num_of_layers;

	
	int neuron_layer[layer_size];
	int layer_types[layer_size];
	
	for(int i=0;i<layer_size;i++){
		input_file >> neuron_layer[i];  // number of neurons these layers have.
	}
	
	for(int i=0;i<layer_size;i++){   // type of neurons for layers
		input_file >> layer_types[i];
	}
	
	int input_size = neuron_layer[0];  // number of inputs equal to number of neurons of first layer
	int input_values[input_size];
	
	int x; 
	
	for(int i=0;i<input_size;i++){  // input values
		if(!input_file >> x){       // controlling the input shape
			throw "Input shape does not match!";  
		}
		input_file >> input_values[i];
	}
	if(input_file >> x){    // controlling the input shape
		throw "Input shape does not match!";
	}
  
	layerhead = new Layer[layer_size];  // dynamic array of layers	
	layerhead[0].firstLayer(neuron_layer[0],layer_types[0],input_values);

	for(int i=1;i<layer_size;i++){
		double input_arr [neuron_layer[i-1]];
		for(int j=0;j<neuron_layer[i-1];j++){
			Neuron* traverse = layerhead[i-1].getter_head();
			input_arr[j]= traverse[j].getter_a();
		}
			
		int inputsize = neuron_layer[i-1];
	
		layerhead[i].otherLayer(neuron_layer[i] ,layer_types[i] ,input_arr,inputsize);
	}
	
	input_file.close();
	print(neuron_layer);   // printing

	//cout << "Network constructor"<<endl;
}
void Network::print(int *neuron_layer)  const
{
	for(int i = 0;i < layer_size; i++){
		cout << "Layer "<< i << ":" << endl;
		layerhead[i].print_a(neuron_layer[i]);    // printing  by calling print_a function
	}
}

Network::~Network(){
	if(layerhead){
		delete [] layerhead;
		layerhead=NULL;
	}
	//cout << "Network destructor" << endl;
}

int main(int argc, char *argv[]){

	const char* t_file_name =  argv[1];
	
	try{                // try and catch blocks for controlling type of activation function and input shape
		Network MyNetwork(t_file_name);   // creating network 
	}
	catch(const char* error_string){
		cout << error_string << endl;
	}
	return 0;
}