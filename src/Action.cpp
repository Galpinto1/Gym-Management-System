#include "../include/Action.h"
#include "../include/Studio.h"


using namespace std;

//baseAction's implementation

BaseAction::BaseAction() : errorMsg(""), status() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

string BaseAction::statusToString() {
    if(status == (COMPLETED)){
        return "Completed";
    }
    else {
        return ("Error: "+getErrorMsg());
    }
}

BaseAction::~BaseAction() = default;


void BaseAction::complete(){
    status = COMPLETED;
}

void BaseAction::error(string errorMsg){
    cout <<errorMsg;
    this->errorMsg = errorMsg;
    status = ERROR;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}


//OpenTrainer's implementation
OpenTrainer::OpenTrainer(int id, std::vector<Customer*> &customersList):BaseAction(),trainerId(id),
                            customers(customersList), customers_input_backup(""){}

void OpenTrainer::act(Studio &studio){
    Trainer* my_trainer = studio.getTrainer(trainerId);
    if (my_trainer == nullptr || my_trainer->isOpen()){
        this->error("Trainer does not exist or is not open");
         }
    else {
        my_trainer->openTrainer();
        my_trainer->setEverOpened();
        int my_size = customers.size();
        for (int i = 0;  (i < my_size && i < my_trainer->getCapacity()); ++i) {
            my_trainer->addCustomer(customers[i]);
            my_trainer->setCapacity(my_trainer->getCapacity()-1);
        }
        this->complete();
    }
    studio.insertLog(this);
}
//open 2 Shalom,swt Dan,chp Alice,mcl Bob,fbd
string OpenTrainer::toString() const {
    string output = "open "+ to_string(trainerId)+" " + customers_input_backup;
    return output;
}

OpenTrainer:: ~OpenTrainer() = default;

OpenTrainer *OpenTrainer::clone() {
    return new OpenTrainer(*this);
}


void OpenTrainer::setCustomersInput(std::string backup) {
    customers_input_backup = backup;
}

Order :: Order(int id): BaseAction(),trainerId(id) {}

void Order :: act(Studio &studio) {
    Trainer* my_trainer = studio.getTrainer(trainerId);
    if (my_trainer == nullptr || !my_trainer->isOpen()){
        this->error("Trainer does not exist or is not open") ;}
    else {
        string output;
        vector<Workout> wo = studio.getWorkoutOptions();
        vector<Customer*> trainers_customers = my_trainer->getCustomers();
        for(vector<int>:: size_type i = 0; i < trainers_customers.size(); ++i) {
            vector<int> workouts_ids = trainers_customers[i]->order(wo);
            my_trainer->order(trainers_customers[i]->getId(), workouts_ids, studio.getWorkoutOptions());
            for(vector<int>:: size_type j = 0; j < workouts_ids.size(); ++j) {
                output = output + trainers_customers[i]->getName() + " is doing " + wo[workouts_ids[j]].getName()+"\n";
            }
        }
        this->complete();
        cout << output;
    }
   studio.insertLog(this);
}

string Order:: toString() const {
    return ("order " + to_string(trainerId));
}

Order:: ~Order() = default;

Order *Order::clone() {
    return new Order(*this);
}


//MoveCustomer's implementation
MoveCustomer:: MoveCustomer(int src, int dst, int customerId) : BaseAction(),srcTrainer(src), dstTrainer(dst), id(customerId){}

void MoveCustomer:: act(Studio &studio) {
    // preparation to insert  MoveCustomer action to actions log
    vector<Workout> wo_options = studio.getWorkoutOptions();

    Trainer* src_trainer = studio.getTrainer(srcTrainer);
    Trainer* dst_trainer = studio.getTrainer(dstTrainer);

    //check exceptions
    if ( src_trainer == nullptr || !src_trainer->isOpen() || dst_trainer == nullptr ||
    !dst_trainer->isOpen() || src_trainer->getCustomer(id) == nullptr || dst_trainer->getCapacity() == 0) {
        this->error("Cannot move customer");}
    //if everything is valid, do:
    else {
        //move customer from the source trainer's customers List to the destination trainer's customers list and update capacity
        Customer* to_move = src_trainer->getCustomer(id);
        src_trainer->removeCustomer(id);
        src_trainer->setCapacity(src_trainer->getCapacity()+1);
        dst_trainer->addCustomer(to_move);
        dst_trainer->setCapacity(dst_trainer->getCapacity()-1);
        //move all customer's orders from source trainer's orders list to destination trainer's orders list and update salary
        vector<int> orders_ids;
        vector<OrderPair> src_orders = src_trainer->getOrders();
        for(vector<int>::size_type i = 0; i < src_trainer->getOrders().size(); ++i) {
                if (src_orders[i].first == id) {
                    orders_ids.push_back(src_orders[i].second.getId());
                }
            }
        src_trainer->removeOrders(id);
        dst_trainer->order(id, orders_ids, wo_options);

        //if source trainer have no customers left, close his session
        if (src_trainer->getCustomers().size() == 0) {
            Close closeTrainer(srcTrainer);
            closeTrainer.act(studio);
        }
        this->complete();
    }
    studio.insertLog(this);
    }

string MoveCustomer::toString() const {
    return ("move "+ to_string(srcTrainer)+" "+ to_string(dstTrainer)+" "+ to_string(id));
}

MoveCustomer::~MoveCustomer() = default;

MoveCustomer *MoveCustomer::clone() {
    MoveCustomer* output = new MoveCustomer(srcTrainer, dstTrainer, id);
    return output;
}

//Close's implementation
Close:: Close(int id) : BaseAction(),trainerId(id){}

void Close:: act(Studio &studio) {
    Trainer* my_trainer = studio.getTrainer(trainerId);
    if (my_trainer == nullptr || !my_trainer->isOpen()){
        this->error("Trainer does not exist or is not open") ;
    }
    else {
        cout << "Trainer "+ to_string(trainerId) + " closed. Salary " + to_string(my_trainer->getSalary()) + "NIS";
        my_trainer->closeTrainer();
        this->complete();
    }
    studio.insertLog(this);
}

string Close::toString() const {
    return ("close "+ to_string(trainerId));
}

Close::~Close() = default;

Close *Close::clone() {
    return new Close(*this);
}


//CloseAll's implementation
CloseAll::CloseAll() :BaseAction() {}

void CloseAll::act(Studio &studio) {
    studio.insertLog(this);
    vector<Trainer *> Trainers_List = studio.getTrainers();
    for (vector<int>:: size_type i = 0; i < Trainers_List.size(); ++i) {
        if (Trainers_List[i]->isOpen()) {
            cout << "Trainer " + to_string(i) + " closed. Salary" + to_string(Trainers_List[i]->getSalary()) + "NIS" +"\n";
            Trainers_List[i]->closeTrainer();
        }
    }
    studio.closeStudio();
}

string CloseAll::toString() const {
    return "closeall";
}

CloseAll::~CloseAll() = default;

CloseAll *CloseAll::clone() {
    return new CloseAll(*this);
}


//PrintWorkoutOptions's implementation
PrintWorkoutOptions:: PrintWorkoutOptions() :BaseAction() {}

void PrintWorkoutOptions::act(Studio &studio) {
    vector<Workout> &opts = studio.getWorkoutOptions();
    for (auto &curr_opt: opts) {
        cout <<curr_opt.getName()+","+curr_opt.typeToString(curr_opt.getType())+","+ to_string(curr_opt.getPrice())+"\n";
    }
    this->complete();
    studio.insertLog(this);
}

string PrintWorkoutOptions::toString() const{
    return "workout_options";
}

PrintWorkoutOptions:: ~PrintWorkoutOptions() = default;

PrintWorkoutOptions *PrintWorkoutOptions:: clone() {
    return new PrintWorkoutOptions(*this);
}


//PrintTrainersStatus's implementation
PrintTrainerStatus::PrintTrainerStatus(int id): BaseAction(),trainerId(id) {}

void PrintTrainerStatus ::act(Studio &studio) {
    Trainer* my_trainer = studio.getTrainer(trainerId);
    if(my_trainer->isEverOpened()) {
        if(!my_trainer->isOpen()) {
            cout << "Trainer " + to_string(trainerId) + " status: closed";
        }
        else {
            string output = "Trainer " + to_string(trainerId) + " status: open" + "\n" + "Customers:" + "\n";
            vector<Customer*> customers_list = my_trainer->getCustomers();
            vector<OrderPair> orders_list = my_trainer->getOrders();
            for(vector<int>:: size_type i = 0; i < customers_list.size(); ++i) {
                output = output + to_string(customers_list[i]->getId()) + " " + customers_list[i]->getName() + "\n";
            }
            output = output + "Orders:" + "\n";
            for(vector<int>:: size_type i = 0; i<orders_list.size(); ++i) {
                output = output + orders_list[i].second.getName() + " " +
                        to_string(orders_list[i].second.getPrice()) + "NIS " +
                        to_string(orders_list[i].first) + "\n";
            }
            output = output + "Current Trainer's Salary: " + to_string(my_trainer->getSalary()) + "NIS" + "\n";
            this -> complete();
            studio.insertLog(this);
            cout << output;
        }
    }
}

string PrintTrainerStatus::toString() const {
    return ("status "+ to_string(trainerId));
}

PrintTrainerStatus:: ~PrintTrainerStatus() = default;

PrintTrainerStatus *PrintTrainerStatus:: clone() {
    return new PrintTrainerStatus(*this);
}


//PrintActionsLog's implementation
PrintActionsLog::PrintActionsLog(): BaseAction() {}

void PrintActionsLog::act(Studio &studio) {
    this->complete();
    for(vector<int>:: size_type i = 0; i < studio.getActionsLog().size(); ++i) {
        string output = "";
        output = output + studio.getActionsLog()[i]->toString() + " " + studio.getActionsLog()[i]->statusToString() + "\n";
        cout << output;

    }
    studio.insertLog(this);
}

string PrintActionsLog::toString() const {
    return "log";
}

PrintActionsLog::~PrintActionsLog() = default;

PrintActionsLog* PrintActionsLog:: clone() {
    return new PrintActionsLog(*this);
}


//BackupStudio's implementation
extern Studio* backup;

BackupStudio::BackupStudio():BaseAction() {}

void BackupStudio::act(Studio &studio) {
    if(backup == nullptr) {
        backup = new Studio(studio);
    }
    else {
        (*backup) = studio;
    }
    this->complete();
    studio.insertLog(this);
}

std::string BackupStudio::toString() const {
    return "backup";
}

BackupStudio::~BackupStudio() = default;

BackupStudio* BackupStudio:: clone() {
    return new BackupStudio(*this);
}

//RestoreStudio's implementation
extern Studio* backup;

RestoreStudio::RestoreStudio():BaseAction() {}

void RestoreStudio::act(Studio &studio) {
    if(backup == nullptr) {
        this->error("No backup available") ;
    }
    else {
        studio = *backup;
        this->complete();
    }
    studio.insertLog(this);
}

std::string RestoreStudio::toString() const {
    return "restore";
}

RestoreStudio::~RestoreStudio() = default;

RestoreStudio* RestoreStudio:: clone() {
    return new RestoreStudio(*this);
}

