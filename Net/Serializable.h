#pragma once

#include <iostream>
#include <string>

class Serializable {

public:

    Serializable();
    virtual ~Serializable();

    //virtual void save(std::ostream &ostream) const;
    //virtual void load(std::istream &istream);

    virtual void serialize(std::ostream &ostream) const = 0;
    virtual void deserialize(std::istream &istream) = 0;

private:
    Serializable(Serializable const&);

    Serializable& operator=(Serializable const&);

};

inline std::ostream& operator<<(std::ostream &ostream, Serializable const &s) {
    s.serialize(ostream);
    return ostream;
}

inline std::istream& operator>>(std::istream &istream, Serializable       &s) {
    s.deserialize(istream);
    return istream;
}



struct PersonInfo :
    public Serializable {

public:

    unsigned int age_;
    std::string name_;

    enum { undef, man, woman } sex_;

    void serialize(std::ostream &ostream) const override;
    void deserialize(std::istream &istream) override;
};

