#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    virtual ~Trainer();
    Trainer(const Trainer &other);
    Trainer& operator=(const Trainer &other);
    Trainer(Trainer&& other);
    Trainer& operator=(Trainer &&other);
    int getCapacity() const;
    int getOriginCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void removeOrders(const int customer_id);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    void setSalary(int newSalary);
    bool isOpen();
    bool isEverOpened();
    void setEverOpened();
    void setId(int trainer_id);
    void setCapacity(int trainer_capacity);


private:
    int capacity;
    int originCapacity;
    bool open;
    bool everOpened;
    int salary;
    int id;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif