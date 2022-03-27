#include "../include/Workout.h"
using namespace std;


Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name), price(w_price), type(w_type){};

int Workout::getId() const {return id;}

string Workout::getName() const {return name;}

int Workout::getPrice() const {return price;}

WorkoutType Workout::getType() const {return type;}

string Workout::typeToString(WorkoutType type) {
    if (type == CARDIO) {
        return "Cardio";}
    if (type == MIXED) {
        return "Mixed"; }
    else {
        return "Anaerobic";
    }
}

WorkoutType Workout:: stringToType(std::string str){
    WorkoutType output;
    if (str=="Anaerobic") {
        output = ANAEROBIC;}
    if (str=="Mixed"){
        output = MIXED;}
    if (str=="Cardio"){
        output = CARDIO;}
    return output;
}





