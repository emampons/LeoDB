#ifndef LEODB_LISTENER_H
#define LEODB_LISTENER_H


class Listener {
public:
    Listener(bool tune);

    ~Listener();

    void monitor();

private:
    double read_write_ratio = 50;
    bool tune;

    void calibrate();
};


#endif //LEODB_LISTENER_H
