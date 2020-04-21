#include <iostream>
#include <vector>

class chair
{
public:
    int index;
    int previousIndex;
    int next;
    int status;
    chair(/* args */);

    chair(int index, int previousIndex, int next, int status)
    {
        this->index = index;
        this->previousIndex = previousIndex;
        this->next = next;
        this->status = status;
    }
    ~chair();

    void printChair()
    {
        std::cout << "Index: " << index << std::endl;
        std::cout << "PreviousIndex: " << previousIndex << std::endl;
        std::cout << "Next: " << next << std::endl;
        std::cout << "Status: " << status << std::endl;
        std::cout << std::endl;
    }
};

chair::chair(/* args */)
{
}

chair::~chair()
{
}

bool status_isValid(chair chairActual, std::vector<chair> chairs)
{
    if ((chairActual.status == 1) || (chairActual.status == 2))
    {
        if ((chairActual.status == chairs[chairActual.previousIndex].status) || (chairActual.status == chairs[chairActual.next].status))
        {
            //std::cout << "Helytelen" << std::endl;
            return 0;
        }
        else
        {
            //std::cout << "Helyes" << std::endl;
            return 1;
        }
    }
    //std::cout << "Alapbol 0";
    return 1;
}

bool status_isValid_forGender(chair chairActual, std::vector<chair> chairs, int gender)
{

    int g = gender;

    if (chairActual.status == 0)
    {
        chairActual.status = g;
        //std::cout << "vizsgalom" << std::endl;
        if ((chairActual.status == chairs[chairActual.previousIndex].status) || (chairActual.status == chairs[chairActual.next].status))
        {
            //std::cout << "Helytelen" << std::endl;
            return 0;
        }
        else
        {
            //std::cout << "Helyes" << std::endl;
            return 1;
        }
    }
    //std::cout << "Alapbol 0";
    return 1;
}

bool elofeltetel(int chairNumber, std::vector<chair> chairs)
{
    if (chairs[chairNumber - 1].status == 0)
    {
        //std::cout << "ok";
        return true;
    }
    else
    {
        //std::cout << "false";
        return false;
    }
}

std::vector<chair> leultet(int chairNumber, int gender, std::vector<chair> chairs) // female = 1, male = 2
{
    if (elofeltetel(chairNumber, chairs))
    {
        //chairs[chairNumber - 1].printChair();
        chairs[chairNumber - 1].status = gender;
        //std::cout << "Leultetve";
        //chairs[chairNumber - 1].printChair();
        return chairs;
    }
    else
    {
        std::cout << "Nem lehet ide ultetni" << std::endl;
        return chairs;
    }
}

void printCurrentStates(std::vector<chair> chairs)
{
    for (int i = 0; i < chairs.size(); i++)
    {
        std::cout << chairs[i].status << " ";
    }
}

bool seatsAvailable(std::vector<chair> chairs, int gender)
{
    bool seatAvailable = false;
    for (int i = 0; i < chairs.size(); i++)
    {
        if (chairs[i].status == 0)
        {
            if (status_isValid_forGender(chairs[i], chairs, gender))
            {
                seatAvailable = true;
                break;
            }
        }
    }
    return seatAvailable;
}

int switchGender(int gender)
{
    if (gender == 1)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

int heurisztika(std::vector<chair> chairs, int seat, int g)
{

    int gender = g;

    if ((chairs[chairs[seat - 1].next].status == switchGender(gender)) && (chairs[chairs[seat - 1].previousIndex].status == switchGender(gender)))
    {
        return 100;
    }

    if (chairs[chairs[seat - 1].previousIndex].status == 0)
    {
        return 50;
    }
    return 25;
}

int computerChooseSeat(std::vector<chair> chairs, int g)
{

    int maxHeurisztika = 0;
    int seatIndex;
    int gender = g;

    for (int i = 0; i < chairs.size(); i++)
    {
        if (chairs[i].status == 0)
        {

            int h = 0;

            h = heurisztika(chairs, i + 1, g);

            std::cout << "Heurisztika " << i + 1 << "-hez: " << h << std::endl;

            if (h > maxHeurisztika)
            {
                maxHeurisztika = h;
                seatIndex = i;
            }
        }
    }

    return seatIndex + 1;
}

int main()
{

    std::vector<chair> chairs;

    int seat;
    int gender = 1;
    //bool gender = false;

    chairs.push_back(chair(0, 13, 1, 0));

    for (int chairCounter = 1; chairCounter < 13; chairCounter++)
    {
        chair chair(chairCounter, chairCounter - 1, chairCounter + 1, 0);
        chairs.push_back(chair);
    }

    chairs.push_back(chair(13, 12, 0, 0));

    chair actualChair = chairs[0];

    // for (int i = 0; i < chairs.size(); i++)
    // {
    //     chairs[i].printChair();
    // }

    while (seatsAvailable(chairs, gender))
    {
        printCurrentStates(chairs);
        std::cout << "Hova szeretne ultetni? 1" << std::endl;
        std::cin >> seat;

        actualChair = chairs[seat - 1];
        actualChair.status = gender;

        if (status_isValid(actualChair, chairs))
        {

            chairs = leultet(seat, gender, chairs);

            gender = switchGender(gender);
        }
        else
        {
            std::cout << "Nem lehet ide ultetni" << std::endl;
        }

        if (seatsAvailable(chairs, gender))
        {
            printCurrentStates(chairs);
            //std::cout << "Hova szeretne ultetni? 2" << std::endl;
            seat = computerChooseSeat(chairs, gender);
            std::cout << "Gep valasztott seat: " << seat << std::endl;
            actualChair = chairs[seat - 1];
            actualChair.status = gender;
            int h = heurisztika(chairs, seat, gender);
            std::cout << "Heurisztika:" << h << std::endl;

            if (status_isValid(actualChair, chairs))
            {

                chairs = leultet(seat, gender, chairs);

                gender = switchGender(gender);
            }
            else
            {
                std::cout << "Nem lehet ide ultetni" << std::endl;
            }
        }
    }

    printCurrentStates(chairs);

    std::cout << "Nyert a " << (switchGender(gender)) << ". jatekos";
}