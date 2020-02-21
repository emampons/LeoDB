#ifndef LEODB_COMMANDS_H
#define LEODB_COMMANDS_H


class commands {
public:

private:
};

typedef  enum  command_code{
    GET = 0,
    SCAN = 1,
    DELETE = 2,
    PUT = 3,
    MAX = 4,
    MIN = 5,
    SUM = 6,
    STD = 7,
    AVG = 8,
    NO_OP = 100,

} command;

#endif //LEODB_COMMANDS_H
