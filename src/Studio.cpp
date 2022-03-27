#include "../include/Studio.h"
#include "../include/Workout.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

Studio::Studio() {}

//constructor
Studio::Studio(const std::string &configFilePath): open(false), numOfTrainers(0), customers_counter(0),
trainers({}), actionsLog({}), workout_options({}) {
    ifstream studioFile(configFilePath);
    if (studioFile.is_open()) {
        string line;
        vector<string> textOfFile;
        while (!studioFile.eof()) {
            getline(studioFile, line);
            if (line[0] == '#' || line.empty())
                continue;
            else
                textOfFile.push_back(line);
        }
        //update num of trainers
        string str_num = textOfFile[0];
        stringstream x(str_num);
        x>>numOfTrainers;

        //update trainers vector
        vector<int> capacities;
        string str_cap = textOfFile[1];
        int cap;
        for (int i = 0; i < numOfTrainers-1; ++i) {
            int comma_ind = str_cap.find(',');
            stringstream x(str_cap.substr(0,comma_ind));
            x>>cap;
            capacities.push_back(cap);
            str_cap = str_cap.substr(comma_ind+1,str_cap.length());
        }
        int last_cap;
        stringstream y(str_cap);
        y>>last_cap;
        capacities.push_back(last_cap);


        for(size_t i = 0; i < capacities.size(); ++i) {
            Trainer* to_insert = new Trainer(capacities[i]);
            to_insert->setId(i);
            trainers.push_back(to_insert);
        }

        //update workout_options vector
        for(size_t i = 2; i<textOfFile.size(); ++i) {
            string wo = textOfFile[i];
            int first_comma = wo.find(',');
            string name_wo = wo.substr(0, first_comma);
            wo = wo.substr(first_comma+2, wo.length());
            int sec_comma = wo.find(",");
            string type_wo = wo.substr(0, sec_comma);
            WorkoutType type = Workout::stringToType(type_wo);
            wo = wo.substr(sec_comma+2, wo.length());
            int wo_price;
            stringstream x(wo);
            x>>wo_price;
            workout_options.push_back(Workout(i-2, name_wo, wo_price, type));
        }
    }
}

//destructor
Studio:: ~Studio() {
    for(size_t i = 0; i < trainers.size(); ++i) {
        if (trainers[i]) {
            delete trainers[i];
        }
    }
    trainers.clear();

    for(size_t i = 0; i < actionsLog.size(); ++i) {
       if(actionsLog[i]) {
           delete actionsLog[i];
       }
    }
    actionsLog.clear();
    workout_options.clear();
}

//copy constructor
Studio::Studio(const Studio &other): open(other.open), numOfTrainers(other.numOfTrainers), customers_counter(other.customers_counter), trainers({}), actionsLog({}), workout_options({}) {
    //copy trainers vector
    for(size_t i = 0; i < other.trainers.size(); ++i) {
        Trainer* to_insert = new Trainer(*(other.trainers[i]));
        trainers.push_back(to_insert);
    }
    //copy actionsLog
    for(size_t i = 0; i < other.actionsLog.size(); ++i) {
        BaseAction* to_insert = other.actionsLog[i];
        BaseAction* output = to_insert->clone();
        actionsLog.push_back(output);
    }
    //copy Workout options
    for(size_t i = 0; i < other.workout_options.size(); ++i) {
        Workout to_insert = Workout(other.workout_options[i].getId(), other.workout_options[i].getName(), other.workout_options[i].getPrice(), other.workout_options[i].getType());
        workout_options.push_back(to_insert);
    }
}

//assignment operator
Studio& Studio::operator=(const Studio &other){
    if (this==&other){
        return *this;
    }
    else {
        //set this Studio's data members
        open = other.open;
        numOfTrainers = other.numOfTrainers;
        customers_counter = other.customers_counter;

        //delete this Studio's trainers vector
        for (size_t i = 0; i < trainers.size(); ++i) {
            if (trainers[i]) {
                delete trainers[i];
            }
        }
        trainers.clear();

        //clone other's trainers list and set it as this trainer's List
        for(vector<int>:: size_type i = 0; i < other.trainers.size(); ++i) {
            Trainer* to_insert = new Trainer(*(other.trainers[i]));
            trainers.push_back(to_insert);
        }

        //delete this Studio's actionsLog vector
        for (size_t i = 0; i < actionsLog.size(); ++i) {
            if (actionsLog[i]) {
                delete actionsLog[i];
            }
        }
        actionsLog.clear();

        //clone other's actionsLog list and set it as this actionsLog's List
        for(vector<int>:: size_type i = 0; i < other.actionsLog.size(); ++i) {
            BaseAction* to_insert = other.actionsLog[i]->clone();
            actionsLog.push_back(to_insert);
        }

        //copy other's Workout_option
        workout_options.clear();
        for(vector<int>:: size_type i = 0; i < other.workout_options.size(); ++i) {
            this->workout_options.push_back(Workout(other.workout_options[i].getId(),other.workout_options[i].getName(), other.workout_options[i].getPrice(), other.workout_options[i].getType()));
        }
        return *this;
    }
}

//move constructor
Studio:: Studio(Studio&& other): open(other.open), numOfTrainers(other.numOfTrainers), customers_counter(other.customers_counter), trainers({}), actionsLog({}), workout_options({}) {
    //steal other's trainers vector
    for(size_t i = 0; i < other.trainers.size(); ++i) {
        trainers.push_back(other.trainers[i]);
        other.trainers[i] = nullptr;
    }
    other.trainers.clear();

    //steal other's actionsLog vector
    for(size_t i = 0; i < other.actionsLog.size(); ++i) {
        actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i] = nullptr;
    }
    other.actionsLog.clear();

    //steal other's workoutOPtions vector
    for(size_t i = 0; i < other.workout_options.size(); ++i) {
        workout_options.push_back(other.workout_options[i]);
    }
    other.workout_options.clear();
}

Studio& Studio:: operator=(Studio &&other) {
    if(this == &other) {
        return *this;
    }
    else {
        //set this Studio's data members
        open = other.open;
        numOfTrainers = other.numOfTrainers;
        customers_counter = other.customers_counter;

        //copy other's workout_options vector to this Trainer
        workout_options.clear();
        for(size_t i = 0; i < other.workout_options.size(); ++i) {
            workout_options.push_back(other.workout_options[i]);
        }
        other.workout_options.clear();

        //delete this trainers vector
        for(vector<int>:: size_type i = 0; i < this->trainers.size(); ++i) {
            if (this->trainers[i]) {
                delete this->trainers[i];
            }
        }
        trainers.clear();

        //move other's trainers to this Studio
        for(vector<int>:: size_type i = 0; i < other.trainers.size(); ++i) {
            trainers.push_back(other.trainers[i]);
            other.trainers[i] = nullptr;
        }
        other.trainers.clear();

        //delete this actionsLog vector
        for(vector<int>:: size_type i = 0; i < this->actionsLog.size(); ++i) {
            if (this->actionsLog[i]) {
                delete this->actionsLog[i];
            }
        }
        actionsLog.clear();

        //move other's trainers to this Studio
        for(vector<int>:: size_type i = 0; i < other.actionsLog.size(); ++i) {
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
        }
        other.actionsLog.clear();

        return *this;
    }
}


void Studio::start() {

    cout << "Studio is now open!" << endl;
    open = true;
    string action;
    while (action != "closeall") {
        cout<< "please enter action" << endl;
        getline(cin, action);
        string action_name;
        if (!action.find(' ')) {
            action_name = action;
        }
        else {
            int firstspace = action.find(' ');
            action_name = action.substr(0, firstspace);
            action = action.substr(firstspace + 1, action.length());
        }
        if (action_name == "open") {
            string custumers_input = "";
            int secspace = action.find(' ');
            string trainerId = action.substr(0, secspace);
            int id;
            stringstream x(trainerId);
            x >> id;
            action = action.substr(secspace + 1, action.length());

            vector<Customer*> customersList;
            int numofCustomers = 0;
            for(size_t i = 0; i < action.length(); ++i) {
                if(action[i] == ',') {
                    numofCustomers = numofCustomers+1;
                }
            }
            for (int i = 1; i <= numofCustomers; ++i) {
                if(i < numofCustomers) {
                    int index = action.find(" ");
                    string customer_str = action.substr(0, index);
                    custumers_input = custumers_input + customer_str + " ";
                    action = action.substr(index + 1, action.length());
                    int comma = customer_str.find(",");
                    string customer_name = customer_str.substr(0, comma);
                    string customer_type = customer_str.substr(comma + 1, customer_str.length());
                    if (customer_type == "swt") {
                        SweatyCustomer *cust = new SweatyCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "chp") {
                        CheapCustomer *cust = new CheapCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "mcl") {
                        HeavyMuscleCustomer *cust = new HeavyMuscleCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "fbd") {
                        FullBodyCustomer *cust = new FullBodyCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    }
                }
                if (i == numofCustomers) {
                    string customer_str = action;
                    custumers_input = custumers_input + customer_str;
                    int comma = customer_str.find(",");
                    string customer_name = customer_str.substr(0, comma);
                    string customer_type = customer_str.substr(comma + 1, customer_str.length());
                    if (customer_type == "swt") {
                        SweatyCustomer *cust = new SweatyCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "chp") {
                        CheapCustomer *cust = new CheapCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "mcl") {
                        HeavyMuscleCustomer *cust = new HeavyMuscleCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    } else if (customer_type == "fbd") {
                        FullBodyCustomer *cust = new FullBodyCustomer(customer_name, customers_counter);
                        customersList.push_back(cust);
                        customers_counter += 1;
                    }
                }
            }
                OpenTrainer *o_t = new OpenTrainer(id, customersList);
                o_t->setCustomersInput(custumers_input);
                o_t->act(*this);
        }
        else if (action_name == "order") {
            int id;
            stringstream x(action);
            x>>id;
            Order *ord = new Order(id);
            ord->act(*this);
        }
        else if (action_name == "move") {
            int spaceInd = action.find(" ");
            string idString = action.substr(0, spaceInd);
            action = action.substr(spaceInd + 1, action.length());
            int src_id;
            stringstream x(idString);
            x >> src_id;
            spaceInd = action.find(" ");
            idString = action.substr(0, spaceInd);
            action = action.substr(spaceInd + 1, action.length());
            int dst_id;
            stringstream y(idString);
            y >> dst_id;
            int custId;
            idString = action;
            stringstream z(idString);
            z >> custId;
            MoveCustomer *move_action = new MoveCustomer(src_id, dst_id, custId);
            move_action->act(*this);
        } else if (action_name == "close") {
            int id;
            stringstream x(action);
            x >> id;
            Close *close_action = new Close(id);
            close_action->act(*this);
        } else if (action_name == "closeall") {
            CloseAll *closeAll_action = new CloseAll();
            closeAll_action->act(*this);
        } else if (action_name == "workout_options") {
            PrintWorkoutOptions *pwo = new PrintWorkoutOptions();
            pwo->act(*this);
        } else if (action_name == "status") {
            int id;
            stringstream x(action);
            x >> id;
            PrintTrainerStatus *pts = new PrintTrainerStatus(id);
            pts->act(*this);
        } else if (action_name == "log") {
            PrintActionsLog *log_action = new PrintActionsLog();
            log_action->act(*this);
        } else if (action_name == "backup") {
            BackupStudio *backup_action = new BackupStudio();
            backup_action->act(*this);
        }
        else if (action_name == "restore") {
            RestoreStudio *retsore_action = new RestoreStudio();
            retsore_action->act(*this);
        }
        else (cout << "invalid action");

    }
}


    int Studio::getCustomersCount() const {
        return customers_counter;
    }

    Trainer *Studio::getTrainer(int tid) {
        if ((tid >= 0) && (tid < numOfTrainers)) {
            return trainers[tid];
        } else {
            return nullptr;
        }
    }

    vector<Trainer *> Studio::getTrainers() {
        return trainers;
    }

    void Studio::closeStudio() {
        open = false;
    }

    const vector<BaseAction *> &Studio::getActionsLog() const {
        return actionsLog;
    }

    vector<Workout> &Studio::getWorkoutOptions() {
        return workout_options;
    }

    void Studio::insertLog(BaseAction *toInsert) {
        actionsLog.push_back(toInsert);
}





