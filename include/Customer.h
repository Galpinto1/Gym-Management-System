#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

typedef std:: pair<int, int> workoutPair;

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::string getType() const = 0;
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone() = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer();

private:
    const std::string name;
    const int id;

};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    std::string getType() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    ~SweatyCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::string getType() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    ~CheapCustomer();
private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::string getType() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    ~HeavyMuscleCustomer();

private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::string getType() const;
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    Customer* clone();
    ~FullBodyCustomer();
private:
};


#endif