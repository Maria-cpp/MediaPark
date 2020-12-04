#include "Serializable.h"



Serializable::Serializable() {}

Serializable::~Serializable() {}


Serializable::Serializable(Serializable const&) {}

Serializable& Serializable::operator=(Serializable const&) {
    // TODO: insert return statement here
    return *this;
}

//void Serializable::save(std::ostream &ostream) const {
//    serialize(ostream);
//}
//
//void Serializable::load(std::istream &istream) {
//    deserialize(istream);
//}


/// -----------


void PersonInfo::serialize(std::ostream &ostream) const {
    // Serialization code
    ostream << age_ << name_ << sex_;
}

void PersonInfo::deserialize(std::istream &istream) {
    // Deserialization code
    //istream >> age_ >> name_ >> sex_;
    istream >> age_ >> name_;
}
