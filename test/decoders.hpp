#include "chromosome.hpp"

//---------------------------[ Dummy decoder ]--------------------------------//

class Sum_Decoder {
    public:
        double decode(BRKGA::Chromosome& chromosome, bool foo = true);
};

class Order_Decoder {
    public:
        double decode(BRKGA::Chromosome& chromosome, bool foo = true);
};
