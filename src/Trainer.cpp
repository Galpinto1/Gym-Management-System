#include "../include/Trainer.h"
#include <vector>
using namespace std;

//constructor
Trainer::Trainer(int t_capacity) :
capacity(t_capacity), originCapacity(t_capacity) ,open(false), everOpened(false), salary(0), id(-1), customersList({}) , orderList({}) {}

//destuctor
Trainer:: ~Trainer() {
    for(size_t i = 0; i < customersList.size(); ++i) {
       if(customersList[i]) {
           delete customersList[i];
       }
    }
   customersList.clear();
}

//copy constructor
Trainer::Trainer(const Trainer &other) : capacity(other.capacity), originCapacity(other.originCapacity), open(other.open), everOpened(other.everOpened), salary(other.salary), id(other.id), customersList({}), orderList({}) {
    for (vector<int>:: size_type i = 0; i < other.customersList.size(); ++i) {
        Customer* to_insert = other.customersList[i]->clone();
        this->customersList.push_back(to_insert);
    }
    for(vector<int>:: size_type i = 0; i < other.orderList.size(); ++i) {
        this->orderList.push_back(make_pair(other.orderList[i].first,
        Workout(other.orderList[i].second.getId(), other.orderList[i].second.getName(), other.orderList[i].second.getPrice(), other.orderList[i].second.getType())));
    }
}

//copy assignment operator
Trainer& Trainer:: operator=(const Trainer &other) {
    //check for "self assignment"
    if(this == &other) {
        return *this; }
    else {
        //set new Customer's data members
        capacity = other.capacity;
        originCapacity = other.originCapacity;
        open = other.open;
        everOpened = other.everOpened;
        salary = other.salary;
        id = other.id;
        orderList.clear();

        //delete this Customer's list
        for(vector<int>:: size_type i = 0; i < this->customersList.size(); ++i) {
            if (this->customersList[i]) {
                delete this->customersList[i];
            }
        }
        customersList.clear();

        //clone other's customer list and set it as this customer's List
        for(vector<int>:: size_type i = 0; i < other.customersList.size(); ++i) {
            Customer* to_insert = other.customersList[i]->clone();
            customersList.push_back(to_insert);
        }

        //copy other's OrderList
        for(vector<int>:: size_type i = 0; i < other.orderList.size(); ++i) {
            this->orderList.push_back(make_pair(other.orderList[i].first,
                        Workout(other.orderList[i].second.getId(), other.orderList[i].second.getName(), other.orderList[i].second.getPrice(), other.orderList[i].second.getType())));
        }
        return *this;
    }
}
 //move constructor
Trainer::Trainer(Trainer &&other) : capacity(other.capacity), originCapacity(other.originCapacity), open(other.open), everOpened(other.everOpened), salary(other.salary), id(other.id), customersList({}), orderList({})  {
    //steal other's customersLIst
    for(size_t i = 0; i < other.customersList.size(); ++i) {
        this->customersList.push_back(other.customersList[i]);
        other.customersList[i] = nullptr;
    }
    other.customersList.clear();

     for(size_t i = 0; i < other.orderList.size(); ++i) {
         orderList.push_back(other.orderList[i]);
     }
     other.orderList.clear();
 }

 //move assignment
 Trainer &Trainer::operator=(Trainer &&other) {
    if(this == &other) {
        return *this;
    }
    else {
        //set this Trainer's data members
        capacity = other.capacity;
        originCapacity = other.originCapacity;
        open = other.open;
        everOpened = other.everOpened;
        salary = other.salary;
        id = other.id;

        //copy other's order List to this Trainer
        orderList.clear();
        for(size_t i = 0; i < other.orderList.size(); ++i) {
            orderList.push_back(other.orderList[i]);
        }

        //delete this Trainer's customer list
        for(vector<int>:: size_type i = 0; i < this->customersList.size(); ++i) {
            if (this->customersList[i]) {
                delete this->customersList[i];
            }
        }
        customersList.clear();

        //move other's Customers to this Trainer
        for(vector<int>:: size_type i = 0; i < other.customersList.size(); ++i) {
            customersList.push_back(other.customersList[i]);
            other.customersList[i] = nullptr;
        }

        return *this;
    }
}

int Trainer::getCapacity() const {
    return capacity;}

int Trainer::getOriginCapacity() const {
    return originCapacity;
}

void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);}

void Trainer::removeCustomer(int id) {
    for(vector<int>:: size_type i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id) {
            customersList.erase(customersList.begin()+i);
        }
    }
}

Customer* Trainer::getCustomer(int id) {
    Customer* output = nullptr;
    for(vector<int>:: size_type i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id) {
            output = customersList[i];
        }
    }
    return output;
}

vector<Customer*>& Trainer::getCustomers() {
  return customersList;}

vector<OrderPair>& Trainer::getOrders() {
    return orderList;}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std::vector<Workout> &workout_options) {
    for(vector<int>:: size_type i = 0; i < workout_ids.size(); ++i) {
        orderList.push_back(make_pair(customer_id,workout_options[workout_ids[i]]));
        salary = salary + workout_options[workout_ids[i]].getPrice();
    }
}

void Trainer::removeOrders(const int customer_id) {
    //there is a problem with deleting pairs from orderList, there for we create new vector and copy all orders except the ones we wanted to delete at first
    vector<OrderPair> fixed_ordersList;
    int newSalary = 0;
    for(vector<int>:: size_type i = 0; i < orderList.size(); ++i) {
        if(orderList[i].first != customer_id) {
            newSalary += orderList[i].second.getPrice();
            Workout to_insert(orderList[i].second.getId(),orderList[i].second.getName(), orderList[i].second.getPrice(), orderList[i].second.getType());
            fixed_ordersList.push_back(make_pair((orderList[i].first), to_insert));
        }
    }
    salary = newSalary;
    orderList.clear();
    for(vector<int>:: size_type i = 0; i < fixed_ordersList.size(); ++i) {
        orderList.push_back(fixed_ordersList[i]);
    }
}

void Trainer::openTrainer() {
    open = true;
}

void Trainer::closeTrainer() {
    open = false;
    capacity = originCapacity;
    for(vector<int>:: size_type i = 0; i < customersList.size(); ++i) {
        if(customersList[i]) {
            delete customersList[i];
        }
    }
    customersList = {};
    orderList.clear();
}

int Trainer::getSalary() {
    return salary;
}

void Trainer::setSalary(int newSalary) {
    salary = newSalary;
}

bool Trainer::isOpen() {
    return open;
}

bool Trainer::isEverOpened() {
    return everOpened;
}

void Trainer::setEverOpened() {
    everOpened = true;
}


void Trainer::setId(int trainer_id) {
    id = trainer_id;
}

void Trainer::setCapacity(int trainer_capacity) {
    capacity = trainer_capacity;
}





