#include <iostream>
#include <vector>
#include <fstream>

class Array {
public:
    int N, M;
    int** array;

    void setN(int N) { this->N = N; }

    void setM(int M) { this->M = M; }

    void createDynamicArray() {
        array = new int*[N];
        for (int i = 0; i < N; i++) {
            array[i] = new int[M];
        }
    }

    void fillOrClearDynamicArray() const {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++)
                array[i][j] = 0;
        }
    }

    void printDynamicArray(int step) const
    {
        std::cout << "Step: " << step + 1 << std::endl;
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < M; j++)
            {
                if(array[i][j] != 0)
                    std::cout << array[i][j] << " ";
                else
                    std::cout << "* ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }

    void printDynamicArrayToStream(std::ofstream &fout)
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < M; j++)
            {
                if(array[i][j] != 0)
                    fout << array[i][j] << " ";
                else
                    fout << "* ";
            }
            fout<< std::endl;
        }
    }

    void freeArray()
    {
        for(int i = 0; i < N; i++)
        {
            delete[] array[i];
        }
        delete array;
    }
};

class Animal {
protected:
    int x, y, direction, stability, age;

    virtual void checkStability() {}
    Animal(int x, int y, int direction, int stability) {
        this->x = x;
        this->y = y;
        this->direction = direction;
        this->stability = stability;
        this->age = 0;
    }
public:
    int getX() {return this->x;}
    void setX(int x) {this->x = x;}

    int getY() {return this->y;}
    void setY(int y) {this->y = y;}

    int getDirection() {return this->direction;}
    void setDirection(int direction) {this->direction = direction;}

    int getStability() {return this->stability;}
    void setStability(int stability) {this->stability = stability;}

    int getAge() {return this->age;}
    void setAge(int age) {this->age = age;}

    void change(int N) {
        checkStability();
        switch (getDirection())
        {
            case 0:
                y += N;
                return;
            case 1:
                x += N;
                return;
            case 2:
                y -= N;
                return;
            case 3:
                x -= N;
                return;
            default:
                direction = 0;
        }
        direction++;
        change(N);
    }
};

class Rabbit : public Animal {
private:
    void checkStability() override
    {
        if (stability == 0) {
            stability = 3;
            direction++;
        } else stability--;
    }

public:
    Rabbit(int x, int y, int direction, int stability) : Animal(x, y, direction, stability) {}

    void move() {
        change(1);
    }

    int age1() {
        age++;
        if (age == 10)
        {
            return 1;
        }
        return 0;
    }
};

class Fox : public Animal {
private:
    int feed;

    void checkStability() override
    {
        if (stability == 0) {
            stability = 2;
            direction++;
        } else stability--;
    }

public:
    Fox(int x, int y, int direction, int stability) : Animal(x, y, direction, stability) {feed=0;}

    void move() {
        change(2);
    }

    int age1() {
        age++;
        if (age == 15)
        {
            return 1;
        }
        return 0;
    }

    int getFeed()
    {
        return this->feed;
    }
    void setFeed(int feed)
    {
        this->feed = feed;
    }
};

class Model {
public:
    Model(int N, int M, int K) {
        this->N = N;
        this->M = M;
        this->K = K;
    }
private:
    int N, M, K;

    int R = 0;
    int F = 0;
    int nOfStep = 0;
    std::vector<Fox> masF;
    std::vector<Rabbit> masR;
    Array mas;

    int checkCollisions(int coordinate, int max) {
        if(coordinate < 0)
            return max + coordinate;
        else if(coordinate >= max)
            return coordinate - max;
        else return coordinate;
    }

    void moveForStep() {
        for(int i = 0; i < F; i++)
        {
            masF[i].move();
            masF[i].setX(checkCollisions(masF[i].getX(), M));
            masF[i].setY(checkCollisions(masF[i].getY(), N));
        }
        for(int i = 0; i < R; i++)
        {
            masR[i].move();
            masR[i].setX(checkCollisions(masR[i].getX(), M));
            masR[i].setY(checkCollisions(masR[i].getY(), N));
        }
    }

    void eatForStep() {
        for(int i = 0; i < F; i++)
        {
            for(int j = 0; j < R; j++)
            {
                if(masF[i].getX() == masR[j].getX() && masF[i].getY() == masR[j].getY())
                {
                    masF[i].setFeed(masF[i].getFeed() + 1);
                    masR.erase(masR.begin() + j);
                    R--;
                }
            }
        }
    }

    void birthNDyingForStep()
    {
        for(int i = 0; i < F; i++) {
            int age = masF[i].age1();
            if(masF[i].getFeed() >= 2)
            {
                newFox(masF[i].getX(), masF[i].getY(), masF[i].getDirection(), masF[i].getStability());
                masF[i].setFeed(0);
            }
            if(age)
            {
                masF.erase(masF.begin() + i);
                F--;
            }

        }
        for(int i = 0; i < R; i++) {
            int age = masR[i].age1();
            if(masR[i].getAge() == 5 || masR[i].getAge() == 10)
            {
                newRabbit(masR[i].getX(), masR[i].getY(), masR[i].getDirection(), masR[i].getStability());
            }
            if(age) {
                masR.erase(masR.begin() + i);
                R--;
            }
        }
    }

    void createArray()
    {
        mas.setN(N);
        mas.setM(M);
        mas.createDynamicArray();
        mas.fillOrClearDynamicArray();
    }

    void updateArray()
    {
        mas.fillOrClearDynamicArray();
        for(int k = 0; k < (F < R ? R : F ); k++)
        {
            if(k < R) {
                mas.array[masR[k].getY()][masR[k].getX()]++;
            }
            if(k < F) {
                mas.array[masF[k].getY()][masF[k].getX()]--;
            }
        }
    }

    void step()
    {
        moveForStep();
        eatForStep();
        birthNDyingForStep();

        updateArray();
        mas.printDynamicArray(nOfStep);

    }

public:
    void newRabbit(int x, int y, int direction, int stability) {
        masR.emplace_back(x, y, direction, stability);
        R++;
    }

    void newFox(int x, int y, int direction, int stability) {
        masF.emplace_back(x, y, direction, stability);
        F++;
    }

    int startGame()
    {
        createArray();
        for(; nOfStep < K; nOfStep++)
            step();
        return 0;
    }

    Array getMas() { return mas; }

};

int main() {
    int N, M, K, R, F;
    std::ifstream fin("in.txt");
    fin >> N;
    fin >> M;
    fin >> K;
    fin >> R;
    fin >> F;
    Model m(N, M, K);
    for(int i = 0; i < R + F; i++)
    {
        int x, y, direction, stability;
        fin >> x;
        fin >> y;
        fin >> direction;
        fin >> stability;
        if(i < R)
            m.newRabbit(x, y, direction, stability);
        else
            m.newFox(x, y, direction, stability);
    }
    fin.close();

    m.startGame();
    std::ofstream fout("out.txt");
    m.getMas().printDynamicArrayToStream(fout);
    fout.close();

    m.getMas().freeArray();
}