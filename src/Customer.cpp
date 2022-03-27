#include "../include/Customer.h"
#include <vector>
#include <algorithm>

using namespace std ;

//Customer's implementation
Customer::Customer(string c_name, int c_id) : name(c_name), id(c_id) {}

string Customer::getName() const {
    return name; }

int Customer::getId() const {
    return id; }

Customer:: ~Customer() {}

//SweatyCustomer's implementation
SweatyCustomer:: SweatyCustomer(string name, int id) : Customer(name, id) {}

string SweatyCustomer::getType() const {
    return "swt";
}

vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> output;
    for (vector<int>:: size_type i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO) {
            output.push_back(workout_options[i].getId());
        }
    }
    return output;
}

string SweatyCustomer:: toString() const {
    return (this->getName() + ",swt");
}

Customer* SweatyCustomer:: clone(){
    Customer* my_clone = new SweatyCustomer(this->getName(), this->getId());
    return my_clone;
}

SweatyCustomer:: ~SweatyCustomer() = default;

//CheapCustomer's implementation
CheapCustomer:: CheapCustomer(std::string name, int id) : Customer(name, id) {}

string CheapCustomer::getType() const {
    return "chp";
}

vector<int> CheapCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> output;
    vector<workoutPair> to_sort;
    for(vector<int>:: size_type i = 0; i<workout_options.size(); ++i) {
        to_sort.push_back(make_pair(workout_options[i].getPrice(), workout_options[i].getId()));
    }
    sort(to_sort.begin(), to_sort.end());
    output.push_back(to_sort[0].second);
    return output;
}

string CheapCustomer:: toString() const {
    return (this->getName() + ",chp");
}


Customer* CheapCustomer:: clone(){
    Customer* my_clone = new CheapCustomer(getName(), getId());
    return my_clone;
}

CheapCustomer::~CheapCustomer() = default;

//HeavyMuscleCustomer's implementation
HeavyMuscleCustomer:: HeavyMuscleCustomer(std::string name, int id) : Customer(name, id){}

string HeavyMuscleCustomer::getType() const {
    return "mcl";
}

vector<int> HeavyMuscleCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<Workout> Anaerobic_options;
    vector<int> output;
    for (vector<int>:: size_type i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == ANAEROBIC) {
            Anaerobic_options.push_back(workout_options[i]);
        }
    }
    vector<workoutPair> to_sort;
    for(vector<int>:: size_type i = 0; i < Anaerobic_options.size(); ++i) {
        to_sort.push_back(make_pair(Anaerobic_options[i].getPrice(), Anaerobic_options[i].getId()));
    }
    sort(to_sort.begin(), to_sort.end());
    for (vector<pair<int,int>>::reverse_iterator i = to_sort.rbegin(); i != to_sort.rend(); ++i ) {
        output.push_back(i->second);
    }
        return output;
}

string HeavyMuscleCustomer::toString() const {
    return (this->getName() + ",mcl");
}


Customer* HeavyMuscleCustomer:: clone(){
    Customer* my_clone = new HeavyMuscleCustomer(this->getName(), this->getId());
    return my_clone;
}

HeavyMuscleCustomer::~HeavyMuscleCustomer() = default;

//FullBodyCustomer's implementation
FullBodyCustomer:: FullBodyCustomer(std::string name, int id) : Customer(name, id){}

string FullBodyCustomer::getType() const {
    return "fbd";
}

vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> output;
    vector<Workout> Cardio_options;
    vector<Workout> Mixed_options;
    vector<Workout> Anaerobic_options;
    for(vector<int>:: size_type i = 0; i<workout_options.size(); ++i) {
        if(workout_options[i].getType() == CARDIO)
            Cardio_options.push_back(workout_options[i]);
        else if(workout_options[i].getType() == MIXED)
            Mixed_options.push_back(workout_options[i]);
        else
            Anaerobic_options.push_back(workout_options[i]);
    }
    vector<workoutPair> cardio_sort;
    vector<workoutPair> mixed_sort;
    vector<workoutPair> anaerobic_sort;

    for(vector<int>:: size_type i = 0; i<Cardio_options.size(); ++i) {
        cardio_sort.push_back(make_pair(Cardio_options[i].getPrice(), Cardio_options[i].getId()));
    }
    for(vector<int>:: size_type i = 0; i<Mixed_options.size(); ++i) {
        mixed_sort.push_back(make_pair(Mixed_options[i].getPrice(), Mixed_options[i].getId()));
    }
    for(vector<int>:: size_type i = 0; i<Anaerobic_options.size(); ++i) {
        anaerobic_sort.push_back(make_pair(Anaerobic_options[i].getPrice(), Anaerobic_options[i].getId()));
    }

    sort(cardio_sort.begin(), cardio_sort.end());
    sort(mixed_sort.begin(), mixed_sort.end());
    sort(anaerobic_sort.begin(), anaerobic_sort.end());

    output.push_back(cardio_sort[0].second);
    output.push_back(mixed_sort[mixed_sort.size()-1].second);
    output.push_back(anaerobic_sort[0].second);

    return output;
}

string FullBodyCustomer::toString() const {
    return (this->getName() + ",fbd");
}


Customer* FullBodyCustomer:: clone(){
    Customer* my_clone = new FullBodyCustomer(this->getName(), this->getId());
    return my_clone;
}

FullBodyCustomer:: ~FullBodyCustomer() = default;